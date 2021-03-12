#include "myModule.h"
#include <string>
#include <vector>
#include <assert.h>
using namespace std;

void ServerList::add(string& serverType, string& cpuCores, string& memorySize, string& serverCost, string& powerCost) {
    string _serverType = "";
    for (int i = 1; i < serverType.size() - 1; i++) {
        _serverType += serverType[i];
    }
    int _cpuCores = 0, _memorySize = 0, _serverCost = 0, _powerCost = 0;

    for (int i = 0; i < cpuCores.size() - 1; i++) {
        _cpuCores = 10 * _cpuCores + cpuCores[i] - '0';
    }
    for (int i = 0; i < memorySize.size() - 1; i++) {
        _memorySize = 10 * _memorySize + memorySize[i] - '0';
    }
    for (int i = 0; i < serverCost.size() - 1; i++) {
        _serverCost = 10 * _serverCost + serverCost[i] - '0';
    }
    for (int i = 0; i < powerCost.size() - 1; i++) {
        _powerCost = 10 * _powerCost + powerCost[i] - '0';
    }
    serverInfos[_serverType] = vector<int>{ _cpuCores / 2 ,_cpuCores / 2,_memorySize / 2,_memorySize / 2,_serverCost,_powerCost };
}

vector<int>& ServerList::operator[](string& s) {
    return serverInfos[s];
}

void VMList::add(string& vmType, string& vmCpuCores, string& vmMemory, string& vmTwoNodes) {
    string _vmType = "";

    for (int i = 1; i < vmType.size() - 1; i++) {
        _vmType += vmType[i];
    }

    int _vmCpuCores = 0, _vmMemory = 0, _vmTwoNodes = 0;
    for (int i = 0; i < vmCpuCores.size() - 1; i++) {
        _vmCpuCores = _vmCpuCores * 10 + vmCpuCores[i] - '0';
    }
    for (int i = 0; i < vmMemory.size() - 1; i++) {
        _vmMemory = _vmMemory * 10 + vmMemory[i] - '0';
    }
    if (vmTwoNodes[0] == '1') {
        _vmTwoNodes = 1;
    }
    else {
        _vmTwoNodes = 0;
    }
    vmInfos[_vmType] = vector<int>{ _vmCpuCores,_vmMemory,_vmTwoNodes };
}

vector<int>& VMList::operator[](string& s) {
    return vmInfos[s];
}

vector<vector<string>>::iterator ReqList::begin() {
    return requestInfos.begin();
}

vector<vector<string>>::iterator ReqList::end() {
    return requestInfos.end();
}

void ReqList::generateRequest(string& op, string& reqVmType, string& reqId) {
    string _op, _reqVmType, _reqId;
    _op = op.substr(1, op.size() - 1);
    _reqVmType = reqVmType.substr(0, reqVmType.size() - 1);
    _reqId = reqId.substr(0, reqId.size() - 1);
    requestInfos.push_back(vector<string>{_op, _reqVmType, _reqId});
}

// 解析用户删除请求
void ReqList::generateRequest(string& op, string& reqId) {
    string _op, _reqId;
    _reqId = reqId.substr(0, reqId.size() - 1);
    _op = op.substr(1, op.size() - 1);
    requestInfos.push_back(vector<string>{_op, _reqId});
}

void ReqList::clear() {
    requestInfos.clear();
}

ServersInfo::ServersInfo() :serverNumber(0) {};

bool ServersInfo::choseServer(vector<int>& server, vector<int>& vm, int serverId, string vmId) {
    int vmCores = vm[0], vmMemory = vm[1], vmTwoNodes = vm[2];
    int& serverCoreA = server[0], & serverCoreB = server[1], & serverMemoryA = server[2], & serverMemoryB = server[3];
    if (vmTwoNodes) {
        int needCores = vmCores / 2, needMemory = vmMemory / 2;
        if (serverCoreA >= needCores && serverCoreB >= needCores && serverMemoryA >= needMemory && serverMemoryB >= needMemory) {
            serverCoreA -= needCores;
            serverCoreB -= needCores;
            serverMemoryA -= needMemory;
            serverMemoryB -= needMemory;
            vmOnServer[vmId] = vector<int>{ serverId,vmCores,vmMemory,1,2 };
            cout << "(" << serverId << ")\n";
            return true;
        }
        else {
            return false;
        }
    }
    else if (serverCoreA >= vmCores && serverMemoryA >= vmMemory) {
        serverCoreA -= vmCores;
        serverMemoryA -= vmMemory;
        vmOnServer[vmId] = vector<int>{ serverId,vmCores,vmMemory,1 };
        cout << "(" << serverId << ", A)\n";
        return true;
    }
    else if (serverCoreB >= vmCores && serverMemoryB >= vmMemory) {
        serverCoreB -= vmCores;
        serverMemoryB -= vmMemory;
        vmOnServer[vmId] = vector<int>{ serverId,vmCores,vmMemory,2 };
        cout << "(" << serverId << ", B)\n";
        return true;
    }
    return false;
}
int ServersInfo::addVM(vector<string>& createVmInfo, VMList &vmList) {
    string _reqVmType = createVmInfo[1], _reqId = createVmInfo[2];
    vector<int> vm = vmList[_reqVmType];
    int success = -1;
    for (int i = 0; i < serverNumber; i++) {
        auto& server = sysServerResource[i];
        if (choseServer(server, vm, i, _reqId)) {
            //            serverRunVms[i].push_back(_reqId);
            serverRunVms[i]++;
            success = 1;
            break;
        }
        assert(server[0] >= 0 && server[1] >= 0 && server[2] >= 0 && server[3] >= 0);
    }
    return success;
}
void ServersInfo::delVM(vector<string>& delVmInfo) {
    string _vmId = delVmInfo[1];
    vector<int> _vmInfo = vmOnServer[_vmId];
    int _serverId = _vmInfo[0];

    serverRunVms[_serverId]--;

    vector<int>& server = sysServerResource[_serverId];
    if (_vmInfo.size() == 5) {
        int cores = _vmInfo[1] / 2, memory = _vmInfo[2] / 2;
        server[0] += cores;
        server[1] += cores;
        server[2] += memory;
        server[3] += memory;
    }
    else {
        int cores = _vmInfo[1], memory = _vmInfo[2];
        if (_vmInfo[3] == 1) {
            server[0] += cores;
            server[2] += memory;
        }
        else {
            server[1] += cores;
            server[3] += memory;
        }
    }
}
int ServersInfo::buyServer(ServerList& serverList) {
    int buyCost = 0;
    string serverType = "hostUY41I";
    int n = 2000;
    serverRunVms.resize(n, 0);
    cout << "(purchase, " << 1 << ")\n";
    string pauseInfo = "(" + serverType + ", ";
    pauseInfo += std::to_string(n) + ")";
    cout << pauseInfo << endl;
    for (int i = 0; i < n; i++) {
        // string random_serverType = next(serverInfos.begin(), rand()%serverInfos.size())->first; 
        sysServerResource[serverNumber++] = serverList[serverType];
        buyCost += serverList[serverType][4];
    }
    return buyCost;
    /*
    serverType = serverInfos.begin()->first;
    // serverType = "host78BMY";
    pauseInfo ="("+serverType+", ";
    pauseInfo+= std::to_string(serverNumber)+")";
    cout<<pauseInfo<<endl;
    for(int i=0;i<n/2;i++){
        sysServerResource[serverNumber++] = serverInfos[serverType];
        SERVERCOST += serverInfos[serverType][4];
    }
    */
}
void ServersInfo::expansion() {
    cout << "(purchase, 0)\n";
}
void ServersInfo::migrate() {
    cout << "(migration, 0)\n";
}
int ServersInfo::serverPower() {
    int powerCost = 0;
    for (int i = 0; i < serverNumber; i++) {
        //        if(serverRunVms[i].size() != 0){
        if (serverRunVms[i] != 0) {
            powerCost += sysServerResource[i][5];
        }
    }
    return powerCost;
}