//#pragma once
#include <iostream>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

class ServerList;
class VMList;
class ReqList;
class Server;
class VM;
class System;

//存储备选服务器列表
class ServerList {
    unordered_map<string, vector<int>> serverInfos;
public:
    vector<int>& operator[](const string& s);
    void add(string& serverType, string& cpuCores, string& memorySize, string& serverCost, string& powerCost);
    string random_choose() const;
};

//存储备选虚拟机列表
class VMList {
    unordered_map<string, vector<int>> vmInfos;
public:
    vector<int>& operator[](const string& s);
    void add(string& vmType, string& vmCpuCores, string& vmMemory, string& vmTwoNodes);
};

//存储用户的请求列表
class ReqList {
    vector<vector<string>> requestInfos;
public:
    vector<vector<string>>::iterator begin();
    vector<vector<string>>::iterator end();
    // 解析用户添加请求
    void generateRequest(string& op, string& reqVmType, string& reqId);
    // 解析用户删除请求
    void generateRequest(string& op, string& reqId);
    void clear();
};
/*
class AllProcessInfos {
    vector<string> res;
public:
    void addInfo();
};
*/

class Server {
public:
    //基本信息
    string serverType;
    int cpuCoresA;
    int cpuCoresB;
    int memorySizeA;
    int memorySizeB;
    int purchaseCost;
    int oneDayPowerCost;
    //记录所有放置在该服务器上的虚拟机
    unordered_set<VM*> vmOnANode;
    unordered_set<VM*> vmOnBNode;
    unordered_set<VM*> vmOnTwoNodes;
    //记录本身的ID方便从虚拟机可以查找到服务器ID
    int id;

    Server(const string& serverType);
    bool addVM(VM* vm);
    void delVM(VM* vm);
    int numOfVM()const;
};

class VM {
public:
    //基本信息
    string vmType;
    int cpuCores;
    int memorySize;
    bool doubleNodes;
    //记录该虚拟机放置的服务器
    Server* server;
    //记录放置在哪个节点
    char node;

    VM(const string& vmType);
};

//存储此时拥有的所有服务器信息
class System {
    // 服务器的ID->服务器
    vector<Server*> servers;
    // 虚拟机的ID->虚拟机
    unordered_map<string, VM*> vms;

    unordered_map<string, vector<Server*>>purchase_day;
    vector<string> addList_day;
public:
    long long serverCost, powerCost;
public:
    //构造函数
    System();
    // 处理创建虚拟机操作
    void addVM(vector<string>& createVmInfo);
    // 处理删除虚拟机操作
    void delVM(vector<string>& delVmInfo);
    // 扩容服务器策略
    void expansion(VM* vm);
    // 迁移虚拟机策略
    void migrate();
    void serverPower();
    long long totalCost()const;
    void shuchu();
};