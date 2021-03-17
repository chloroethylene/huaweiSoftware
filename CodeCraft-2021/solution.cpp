#include <string>
#include <vector>
#include <cassert>
#include <iterator>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include "solution.h"

using namespace std;

extern ServerList serverList;
extern VMList vmList;

Server::Server(const string& serverType) {
    this->serverType = serverType;
    cpuCoresA = serverList[serverType][0];
    cpuCoresB = serverList[serverType][1];
    memorySizeA = serverList[serverType][2];
    memorySizeB = serverList[serverType][3];
    purchaseCost = serverList[serverType][4];
    oneDayPowerCost = serverList[serverType][5];
}

bool Server::canAdd(VM* vm)const {
    int vmCores = vm->cpuCores, vmMemory = vm->memorySize;
    bool vmTwoNodes = vm->doubleNodes;
    if (vmTwoNodes) {
        int needCores = vmCores / 2, needMemory = vmMemory / 2;
        if (cpuCoresA >= needCores && cpuCoresB >= needCores && memorySizeA >= needMemory && memorySizeB >= needMemory) return true;
        else return false;
    }
    else if (cpuCoresA >= vmCores && memorySizeA >= vmMemory) return true;
    else if (cpuCoresB >= vmCores && memorySizeB >= vmMemory) return true;
    return false;
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
            vmOnTwoNodes.insert(vm);

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
        vmOnANode.insert(vm);

        return true;
    }
    else if (cpuCoresB >= vmCores && memorySizeB >= vmMemory) {
        cpuCoresB -= vmCores;
        memorySizeB -= vmMemory;
        vm->server = this;
        vm->node = 'B';
        vmOnBNode.insert(vm);

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

        vmOnTwoNodes.erase(vm);
    }
    else {
        int cores = vm->cpuCores, memory = vm->memorySize;
        if (vm->node == 'A') {
            cpuCoresA += cores;
            memorySizeA += memory;

            vmOnANode.erase(vm);
        }
        else {
            cpuCoresB += cores;
            memorySizeB += memory;

            vmOnBNode.erase(vm);
        }
    }
}

int Server::numOfVM()const {
    return vmOnANode.size() + vmOnBNode.size() + vmOnTwoNodes.size();
}

double Server::key()const {
    int cpuCores = cpuCoresA + cpuCoresB;
    int memorySize = memorySizeA + memorySizeB;
    double free = double(sqrt(cpuCores * cpuCores + memorySize * memorySize));
    return free;
}

VM::VM(const string& vmType) {
    this->vmType = vmType;
    cpuCores = vmList[vmType][0];
    memorySize = vmList[vmType][1];
    doubleNodes = vmList[vmType][2] == 1 ? true : false;
}


bool System::firstFit(VM* vm) {
    int success = -1;
    auto it = serversSorted.begin();
    for (; it != serversSorted.end(); ++it) {
        if ((*it)->canAdd(vm)) {
            success = 1;
            break;
        }
    }
    if (success == 1) {
        Server* server = *it;
        serversSorted.erase(it);
        server->addVM(vm);
        serversSorted.insert(server);
        return true;
    }
    return false;
}

void System::removeServer(Server* server) {
    auto it_pair = serversSorted.equal_range(server);
    auto it = it_pair.first;
    for (; it != it_pair.second; ++it) {
        if (*it == server) break;
    }
    serversSorted.erase(it);
}

System::System() :serverCost(0), powerCost(0) {};

void System::addVM(vector<string>& createVmInfo) {
    string _reqVmType = createVmInfo[1], _reqId = createVmInfo[2];//需要创建虚机的信息 类型 id
    VM* vm = new VM(_reqVmType);
    vms[_reqId] = vm;
    /*
    //先检查之前买的服务器能否放下
    int success = -1;
    for (Server*& server : serversSorted) {
        if (server->addVM(vm)) {
            success = 1;
            break;
        }
    }
    */
    if (firstFit(vm)) return;

    expansion(vm);
}

void System::delVM(vector<string>& delVmInfo) {
    string _vmId = delVmInfo[1];
    VM* vm = vms[_vmId];
    Server* server = vm->server;
    removeServer(server);
    server->delVM(vm);
    delete vm;
    vms.erase(_vmId);
    serversSorted.insert(server);
}

void System::expansion(VM* vm) {
    /*
    //string serverType = "hostUY41I";
    int vmCores = vm->cpuCores, vmMemory = vm->memorySize;
    bool vmTwoNodes = vm->doubleNodes;
    string serverType = serverList.random_choose();
    Server* server = new Server(serverType);
    while(!server->addVM(vm)) {
        serverType = serverList.random_choose();
        delete server;
        server = new Server(serverType);
    }
    purchase_day[serverType].push_back(server);
    serversSorted.insert(server);
    serverCost += serverList[serverType][4];
    */
    
    string serverType = serverList.wise_choose(vm);
    Server* server = new Server(serverType);
    server->addVM(vm);

    purchase_day[serverType].push_back(server);
    serversSorted.insert(server);
    serverCost += serverList[serverType][4];
    
}

void System::migrate() {
    output.push_back("(migration, 0)\n");
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
    output.push_back("(purchase, " + to_string(totalnum) + ")\n");
    for (auto& item : purchase_day) {
        output.push_back("(" + item.first + ", " + to_string(item.second.size()) + ")\n");
        for (Server*& server : item.second) {
            server->id = servers.size();
            servers.push_back(server);
        }
    }

    migrate();

    for (const string& vmID : addList_day) {
        VM* vm = vms[vmID];
        if (vm->doubleNodes)
            output.push_back("(" + to_string(vm->server->id) + ")\n");
        else
            output.push_back("(" + to_string(vm->server->id) + ", " + vm->node + ")\n");
    }

    purchase_day.clear();
    addList_day.clear();
    //vector<vector<int>>().swap(vmOnServer_day);
}