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

ServersInfo::ServersInfo() :serverNumber(0), serverCost(0), powerCost(0) {};

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
            vmOnServer_day.push_back({ serverId });
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
        vmOnServer_day.push_back({ serverId,1 });
        return true;
    }
    else if (serverCoreB >= vmCores && serverMemoryB >= vmMemory) {
        serverCoreB -= vmCores;
        serverMemoryB -= vmMemory;
        vmOnServer[vmId] = vector<int>{ serverId,vmCores,vmMemory,2 };
        vmOnServer_day.push_back({ serverId,2 });
        return true;
    }
    return false;
}

void ServersInfo::addVM(vector<string>& createVmInfo, ServerList& serverList, VMList &vmList) {
    string _reqVmType = createVmInfo[1], _reqId = createVmInfo[2];//需要创建虚机的信息 类型 id
    vector<int> vm = vmList[_reqVmType];
    int success = -1;
    if (serverNumber == 0) {
        expansion(vm, _reqId, serverList);
    }
    else {
        for (int i = 0; i < serverNumber; i++) {
            auto& server = sysServerResource[i];
            if (choseServer(server, vm, i, _reqId)) {
                serverRunVms[i]++;

                success = 1;
                break;
            }
        }
        if (success == -1) expansion(vm, _reqId, serverList);
    }
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

void ServersInfo::expansion(vector<int>&vm, string vmId, ServerList& serverList) {
    string serverType = "hostUY41I";
    int n = 1;
    // string random_serverType = next(serverInfos.begin(), rand()%serverInfos.size())->first; 
    sysServerResource[serverNumber++] = serverList[serverType];
    serverRunVms.push_back(0);
    if (purchase_day.find(serverType) == purchase_day.end())
        purchase_day[serverType] = 1;
    else
        purchase_day[serverType]++;

    int vmCores = vm[0], vmMemory = vm[1], vmTwoNodes = vm[2];
    int& serverCoreA = sysServerResource[serverNumber - 1][0], & serverCoreB = sysServerResource[serverNumber - 1][1], & serverMemoryA = sysServerResource[serverNumber - 1][2], & serverMemoryB = sysServerResource[serverNumber - 1][3];
    if (vmTwoNodes) {
        int needCores = vmCores / 2, needMemory = vmMemory / 2;

        if (serverCoreA >= needCores && serverCoreB >= needCores && serverMemoryA >= needMemory && serverMemoryB >= needMemory) {
            serverCoreA -= needCores;
            serverCoreB -= needCores;
            serverMemoryA -= needMemory;
            serverMemoryB -= needMemory;
            vmOnServer[vmId] = vector<int>{ serverNumber - 1,vmCores,vmMemory,1,2 };
            vmOnServer_day.push_back({ serverNumber - 1 });
        }
    }
    else if (serverCoreA >= vmCores && serverMemoryA >= vmMemory) {
        serverCoreA -= vmCores;
        serverMemoryA -= vmMemory;
        vmOnServer[vmId] = vector<int>{ serverNumber - 1,vmCores,vmMemory,1 };
        vmOnServer_day.push_back({ serverNumber - 1,1 });
    }
    else if (serverCoreB >= vmCores && serverMemoryB >= vmMemory) {
        serverCoreB -= vmCores;
        serverMemoryB -= vmMemory;
        vmOnServer[vmId] = vector<int>{ serverNumber - 1,vmCores,vmMemory,2 };
        vmOnServer_day.push_back({ serverNumber - 1,2 });

    }
    serverRunVms[serverNumber - 1]++;
    serverCost += serverList[serverType][4];
}
void ServersInfo::migrate() {
    cout << "(migration, 0)\n";
}
void ServersInfo::serverPower() {
    for (int i = 0; i < serverNumber; i++) {
        //        if(serverRunVms[i].size() != 0){
        if (serverRunVms[i] != 0) {
            powerCost += sysServerResource[i][5];
        }
    }
}
long long ServersInfo::totalCost() {
    return serverCost + powerCost;
}
void ServersInfo::shuchu() {
    int totalnum = purchase_day.size();
    cout << "(purchase, " << totalnum << ")\n";
    for (auto& item : purchase_day) {
        string pauseInfo = "(" + item.first + ", ";
        pauseInfo += std::to_string(item.second) + ")";
        cout << pauseInfo << endl;
    }

    migrate();

    int totalreq_day = vmOnServer_day.size();
    for (auto& item : vmOnServer_day) {
        if (item.size() == 1)
            cout << "(" << item[0] << ")\n";
        else if (item[1] == 1)
            cout << "(" << item[0] << ", A)\n";
        else
            cout << "(" << item[0] << ", B)\n";
    }

    purchase_day.clear();
    vector<vector<int>>().swap(vmOnServer_day);
}