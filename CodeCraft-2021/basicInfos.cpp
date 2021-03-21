//#pragma warning(disable:4996)
#include <string>
#include <vector>
#include <cassert>
#include <iterator>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include "basicInfos.h"
#include "solution.h"
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

void ServerList::self_sort() {
    auto cmp = [](const pair<string, vector<int>>& a, const pair<string, vector<int>>& b)->bool {
        /*int a_cpu = a.second[0] + a.second[1];
        int a_mem = a.second[2] + a.second[3];
        int a_price = a.second[4];
        double a_per = double(a_cpu  + a_mem );*/
        //double a_per = double(sqrt(a_cpu * a_cpu + a_mem * a_mem));

        /*int b_cpu = b.second[0] + b.second[1];
        int b_mem = b.second[2] + b.second[3];
        int b_price = b.second[4];*/
        //double b_per = double(sqrt(b_cpu * b_cpu + b_mem * b_mem));
        //double b_per = double(b_cpu  + b_mem );
        int a_price = a.second[4];
        int b_price = b.second[4];

        return a_price < b_price;
    };
    for (auto& item : serverInfos) {
        servernum_str.push_back(make_pair(item.first, item.second));
    }
    sort(servernum_str.begin(), servernum_str.end(), cmp);
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

string ServerList::wise_choose(VM* vm) const {

    int vmCores = vm->cpuCores, vmMemory = vm->memorySize;
    bool vmTwoNodes = vm->doubleNodes;

    for (auto& item : servernum_str) {

        int cpuCoresA = item.second[0];
        int cpuCoresB = item.second[1];
        int memorySizeA = item.second[2];
        int memorySizeB = item.second[3];

        if (vmTwoNodes) {
            int needCores = vmCores / 2, needMemory = vmMemory / 2;
            if (cpuCoresA >= needCores && cpuCoresB >= needCores && memorySizeA >= needMemory && memorySizeB >= needMemory) {
                return item.first;
            }
        }
        else if (cpuCoresA >= vmCores && memorySizeA >= vmMemory) {
            return item.first;
        }
        else if (cpuCoresB >= vmCores && memorySizeB >= vmMemory) {
            return item.first;
        }
    }
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
    vm_ttl[_reqId].push_back(day);
}

void ReqList::generateRequest(string& op, string& reqId, int day) {
    string _op, _reqId;
    _reqId = reqId.substr(0, reqId.size() - 1);
    _op = op.substr(1, op.size() - 1);
    requestInfos[day].push_back(vector<string>{_op, _reqId});
    vm_ttl[_reqId].push_back(day);
}


void ReqList::create_opInfos() {
    auto cmp1 = [this](vector<string>& a, vector<string>& b)->bool {
        int a_ttl = vm_ttl[a[2]][1]- vm_ttl[a[2]][0];
        /*int a_cpu = vmList.vmInfos[a[1]][0];
        int a_mem = vmList.vmInfos[a[1]][1];
        double a_per = double(sqrt(a_cpu* a_cpu + a_mem* a_mem));//double(sqrt(a_cpu * a_cpu + a_mem * a_mem));*/

        int b_ttl = vm_ttl[b[2]][1]- vm_ttl[b[2]][0];
        /*int b_cpu = vmList.vmInfos[b[1]][0];
        int b_mem = vmList.vmInfos[b[1]][1];
        double b_per = double(sqrt(b_cpu* b_cpu + b_mem* b_mem));//double(sqrt(b_cpu * b_cpu + b_mem * b_mem));*/

        return a_ttl > b_ttl;
    };

    this->operateInfos = this->requestInfos;
    for (int day = 0; day < requestInfos.size(); ++day) {
        vector<vector<string>>::iterator iter1 = operateInfos[day].begin();
        vector<vector<string>>::iterator iter2 = operateInfos[day].begin();
        while (iter2 != operateInfos[day].end()) {
            if ((*iter2).size() != 3) {
                sort(iter1, iter2, cmp1);
                iter1 = iter2 + 1;
            }
            iter2++;

        }
        if (iter1 != operateInfos[day].end() && iter2 == operateInfos[day].end()) {
            sort(iter1, iter2, cmp1);
        }
    }
}

void ReqList::read() {
    int requestdays = 0, dayRequestNumber = 0;
    scanf("%d", &requestdays);
    string op, reqVmType, reqId;

    // 开始处理请求
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
    for (auto& item : vm_ttl) {
        if (item.second.size() == 1) {
            item.second.push_back(requestdays-1);
        }
    }
}

vector<vector<vector<string>>>::size_type ReqList::size() {
    return requestInfos.size();
}
