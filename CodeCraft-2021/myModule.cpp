#pragma warning(disable:4996)
//#pragma once
#include "myModule.h"
#include <string>
#include <vector>
#include <assert.h>
#include <iterator>
#include <cstdlib>
#include <algorithm>
#include <math.h>
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

void ServerList::read() {
    int serverNum;
    string serverType, cpuCores, memorySize, serverCost, powerCost;
    scanf("%d", &serverNum);

    for (int i = 0; i < serverNum; i++) {
        cin >> serverType >> cpuCores >> memorySize >> serverCost >> powerCost;
        this->add(serverType, cpuCores, memorySize, serverCost, powerCost);
    }
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

void VMList::read() {
    int vmNumber = 0;
    scanf("%d", &vmNumber);

    string vmType, vmCpuCores, vmMemory, vmTwoNodes;
    for (int i = 0; i < vmNumber; i++) {
        cin >> vmType >> vmCpuCores >> vmMemory >> vmTwoNodes;
        this->add(vmType, vmCpuCores, vmMemory, vmTwoNodes);
    }
}

vector<int>& VMList::operator[](const string& s) {
    return vmInfos[s];
}

vector<vector<vector<string>>>::iterator ReqList::begin() {
    return requestInfos.begin();
}

vector<vector<vector<string>>>::iterator ReqList::end() {
    return requestInfos.end();
}

vector<vector<string>>& ReqList::operator[](const int& day) {
    return requestInfos[day];
}

void ReqList::generateRequest(string& op, string& reqVmType, string& reqId, int day) {
    string _op, _reqVmType, _reqId;
    _op = op.substr(1, op.size() - 1);
    _reqVmType = reqVmType.substr(0, reqVmType.size() - 1);
    _reqId = reqId.substr(0, reqId.size() - 1);
    requestInfos[day].push_back(vector<string>{_op, _reqVmType, _reqId});
   
}

void ReqList::generateRequest(string& op, string& reqId, int day) {
    string _op, _reqId;
    _reqId = reqId.substr(0, reqId.size() - 1);
    _op = op.substr(1, op.size() - 1);
    requestInfos[day].push_back(vector<string>{_op, _reqId});

}

bool cmp1(vector<string>& a, vector<string>& b) {
    int a_cpu = vmList.vmInfos[a[1]][0];
    int a_mem = vmList.vmInfos[a[1]][1];
    double a_per = double(sqrt(a_cpu * a_cpu + a_mem * a_mem));

    int b_cpu = vmList.vmInfos[b[1]][0];
    int b_mem = vmList.vmInfos[b[1]][1];
    double b_per = double(sqrt(b_cpu * b_cpu + b_mem * b_mem));

        return a_per > b_per;
}


void ReqList::create_opInfos() {
    this->operateInfos = this->requestInfos;
    for (int day = 0; day < requestInfos.size(); ++day) {
        vector<vector<string>>::iterator iter1 = operateInfos[day].begin();
        vector<vector<string>>::iterator iter2 = operateInfos[day].begin();
        while (iter2 != operateInfos[day].end()) {
            if ((*iter2).size() != 3) {
                sort(iter1,iter2,cmp1);
                iter1 = iter2 + 1;
                }
            iter2++;
            
        }
        if (iter1!= operateInfos[day].end() && iter2 == operateInfos[day].end()) {
                sort(iter1, iter2, cmp1);
        }
    }
}

void ReqList::read() {
    int requestdays = 0, dayRequestNumber = 0;
    scanf("%d", &requestdays);
    string op, reqVmType, reqId;

    // ��ʼ��������
    for (int day = 0; day < requestdays; ++day) {
        scanf("%d", &dayRequestNumber);
        requestInfos.emplace_back();
        for (int i = 0; i < dayRequestNumber; ++i) {
            cin >> op;
            if (op[1] == 'a') {
                cin >> reqVmType >> reqId;
                this->generateRequest(op, reqVmType, reqId, day);
            }
            else {
                cin >> reqId;
                this->generateRequest(op, reqId, day);

            }
        }
    }
}

vector<vector<vector<string>>>::size_type ReqList::size() {
    return requestInfos.size();
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

VM::VM(const string& vmType) {
    this->vmType = vmType;
    cpuCores = vmList[vmType][0];
    memorySize = vmList[vmType][1];
    doubleNodes = vmList[vmType][2] == 1 ? true : false;
}


System::System() :serverCost(0), powerCost(0) {};

void System::addVM(vector<string>& createVmInfo) {
    string _reqVmType = createVmInfo[1], _reqId = createVmInfo[2];//��Ҫ�����������Ϣ ���� id
    VM* vm = new VM(_reqVmType);
    vms[_reqId] = vm;
    //addList_day.push_back(_reqId);
    //�ȼ��֮ǰ��ķ������ܷ����
    int success = -1;
    for (Server*& server:servers) {
        if (server->addVM(vm)) {
            success = 1;
            break;
        }
    }
    //������ܷ��£���鵱�칺����·������ܷ����
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
    //�����ܷ��µĻ������������
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

    string serverType = serverList.random_choose();
    Server* server = new Server(serverType);

    while(!server->addVM(vm)) {
        serverType = serverList.random_choose();
        delete server;
        server = new Server(serverType);
    }
    purchase_day[serverType].push_back(server);
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