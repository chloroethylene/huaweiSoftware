#include "myModule.h"
#include <string>
#include <vector>
#include <assert.h>
#include <iterator>
#include <cstdlib>
using namespace std;

extern ServerList serverList;
extern VMList vmList;

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

vector<int>& ServerList::operator[](const string& s) {
    return serverInfos[s];
}

string ServerList::random_choose() const {
    return next(serverInfos.begin(), rand() % serverInfos.size())->first;
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

vector<int>& VMList::operator[](const string& s) {
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

void ReqList::generateRequest(string& op, string& reqId) {
    string _op, _reqId;
    _reqId = reqId.substr(0, reqId.size() - 1);
    _op = op.substr(1, op.size() - 1);
    requestInfos.push_back(vector<string>{_op, _reqId});
}

void ReqList::clear() {
    requestInfos.clear();
}


Server::Server(const string& serverType) {
    this->serverType = serverType;
    cpuCoresA = serverList[serverType][0];
    cpuCoresB = serverList[serverType][1];
    memorySizeA = serverList[serverType][2];
    memorySizeB = serverList[serverType][3];
    purchaseCost = serverList[serverType][4];
    oneDayPowerCost = serverList[serverType][5];
}

bool Server::addVM(VM* vm) {
    int vmCores = vm->cpuCores, vmMemory = vm->memorySize;
    bool vmTwoNodes = vm->doubleNodes;
    if (vmTwoNodes) {
        int needCores = vmCores / 2, needMemory = vmMemory / 2;
        if (cpuCoresA >= needCores && cpuCoresB >= needCores && memorySizeA >= needMemory && memorySizeB >= needMemory) {
            cpuCoresA -= needCores;
            cpuCoresB -= needCores;
            memorySizeA -= needMemory;
            memorySizeB -= needMemory;
            vm->server = this;
            vmOnTwoNodes.push_back(vm);

            return true;
        }
        else {
            return false;
        }
    }
    else if (cpuCoresA >= vmCores && memorySizeA >= vmMemory) {
        cpuCoresA -= vmCores;
        memorySizeA -= vmMemory;
        vm->server = this;
        vm->node = 'A';
        vmOnANode.push_back(vm);

        return true;
    }
    else if (cpuCoresB >= vmCores && memorySizeB >= vmMemory) {
        cpuCoresB -= vmCores;
        memorySizeB -= vmMemory;
        vm->server = this;
        vm->node = 'B';
        vmOnBNode.push_back(vm);

        return true;
    }
    return false;
}

void Server::delVM(VM* vm) {
    if (vm->doubleNodes) {

        int cores = vm->cpuCores / 2, memory = vm->memorySize / 2;
        cpuCoresA += cores;
        cpuCoresB += cores;
        memorySizeA += memory;
        memorySizeB += memory;

        for (auto it = vmOnTwoNodes.begin(); it != vmOnTwoNodes.end(); ++it) {
            if (*it == vm) {
                vmOnTwoNodes.erase(it);
                break;
            }
        }
    }
    else {
        int cores = vm->cpuCores, memory = vm->memorySize;
        if (vm->node == 'A') {
            cpuCoresA += cores;
            memorySizeA += memory;

            for (auto it = vmOnANode.begin(); it != vmOnANode.end(); ++it) {
                if (*it == vm) {
                    vmOnANode.erase(it);
                    break;
                }
            }
        }
        else {
            cpuCoresB += cores;
            memorySizeB += memory;

            for (auto it = vmOnBNode.begin(); it != vmOnBNode.end(); ++it) {
                if (*it == vm) {
                    vmOnBNode.erase(it);
                    break;
                }
            }
        }
    }
}

int Server::numOfVM()const {
    return vmOnANode.size() + vmOnBNode.size() + vmOnTwoNodes.size();
}

VM::VM(const string& vmType) {
    this->vmType = vmType;
    cpuCores = vmList[vmType][0];
    memorySize = vmList[vmType][1];
    doubleNodes = vmList[vmType][2] == 1 ? true : false;
}


System::System() :serverCost(0), powerCost(0) {};

void System::addVM(vector<string>& createVmInfo) {
    string _reqVmType = createVmInfo[1], _reqId = createVmInfo[2];//需要创建虚机的信息 类型 id
    VM* vm = new VM(_reqVmType);
    vms[_reqId] = vm;
    addList_day.push_back(_reqId);
    //先检查之前买的服务器能否放下
    int success = -1;
    for (Server*& server:servers) {
        if (server->addVM(vm)) {
            success = 1;
            break;
        }
    }
    //如果不能放下，检查当天购买的新服务器能否放下
    if (success == -1) {
        for (auto& item : purchase_day){
            for (Server*& server : item.second) {
                if (server->addVM(vm)) {
                    success = 1;
                    break;
                }
            }
            if (success == 1) break;
        }
    }
    //都不能放下的话，则进行扩容
    if (success == -1) {
        expansion(vm);
    }
}

void System::delVM(vector<string>& delVmInfo) {
    string _vmId = delVmInfo[1];
    VM* vm = vms[_vmId];
    Server* server = vm->server;
    server->delVM(vm);
    delete vm;
    vms.erase(_vmId);
}

void System::expansion(VM* vm) {
    //string serverType = "hostUY41I";
    int vmCores = vm->cpuCores, vmMemory = vm->memorySize;
    bool vmTwoNodes = vm->doubleNodes;

    string serverType;
    while (1) {
        serverType = serverList.random_choose();
        int serverCoreA = serverList[serverType][0], serverCoreB = serverList[serverType][1], serverMemoryA = serverList[serverType][2], serverMemoryB = serverList[serverType][3];
        if (vmTwoNodes) {
            int needCores = vmCores / 2, needMemory = vmMemory / 2;

            if (serverCoreA >= needCores && serverCoreB >= needCores && serverMemoryA >= needMemory && serverMemoryB >= needMemory) {
                Server* server = new Server(serverType);
                server->cpuCoresA -= needCores;
                server->cpuCoresB -= needCores;
                server->memorySizeA -= needMemory;
                server->memorySizeB -= needMemory;

                purchase_day[serverType].push_back(server);
                vm->server = server;
                server->vmOnTwoNodes.push_back(vm);

                break;
            }
        }
        else if (serverCoreA >= vmCores && serverMemoryA >= vmMemory) {
            Server* server = new Server(serverType);
            server->cpuCoresA -= vmCores;
            server->memorySizeA -= vmMemory;

            purchase_day[serverType].push_back(server);
            vm->server = server;
            vm->node = 'A';
            server->vmOnANode.push_back(vm);

            break;
        }
        else if (serverCoreB >= vmCores && serverMemoryB >= vmMemory) {
            Server* server = new Server(serverType);
            server->cpuCoresB -= vmCores;
            server->memorySizeB -= vmMemory;

            purchase_day[serverType].push_back(server);
            vm->server = server;
            vm->node = 'A';
            server->vmOnANode.push_back(vm);

            break;
        }
    }
    serverCost += serverList[serverType][4];
}

void System::migrate() {
    cout << "(migration, 0)\n";
}

void System::serverPower() {
    for (Server*& server : servers) {
        if (server->numOfVM() != 0) {
            powerCost += server->oneDayPowerCost;
        }
    }
}

long long System::totalCost()const {
    return serverCost + powerCost;
}

void System::shuchu() {
    int totalnum = purchase_day.size();
    cout << "(purchase, " << totalnum << ")\n";
    for (auto& item : purchase_day) {
        string pauseInfo = "(" + item.first + ", ";
        pauseInfo += std::to_string(item.second.size()) + ")";
        cout << pauseInfo << endl;
        for (Server*& server : item.second) {
            server->id = servers.size();
            servers.push_back(server);
        }
    }

    migrate();

    for (const string& vmID : addList_day) {
        VM* vm = vms[vmID];
        if (vm->doubleNodes)
            cout << "(" << vm->server->id << ")\n";
        else
            cout << "(" << vm->server->id << ", " << vm->node << ")\n";
    }

    purchase_day.clear();
    addList_day.clear();
    //vector<vector<int>>().swap(vmOnServer_day);
}