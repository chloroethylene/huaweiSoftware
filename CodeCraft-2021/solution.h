#pragma once
#include <iostream>
#include <cstdlib>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <set>
#include "basicInfos.h"

using namespace std;

class Server;
class VM;
class System;

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
    bool canAdd(VM* vm)const;
    bool addVM(VM* vm);
    void delVM(VM* vm);
    //利用key来对server进行排序
    double key()const;
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

class cmpServer {
public:
    bool operator()(const Server* server1, const Server* server2)const {
        return server1->key() < server2->key();//<
    }
};

//存储此时拥有的所有服务器信息
class System {
    //添加虚拟机时的服务器检查顺序，包括之前的和当天的所有服务器
    multiset<Server*, cmpServer> serversSorted;
    //当天之前的服务器ID=>服务器
    vector<Server*> servers;
    //虚拟机的ID=>虚拟机
    unordered_map<string, VM*> vms;
    //当天购买的服务器
    unordered_map<string, vector<Server*>>purchase_day;
    //查找首次适应的服务器，并将其从红黑树中删除
    bool firstFit(VM* vm);
    //对某个服务器更新红黑树
    void removeServer(Server* server);
public:
    //当天添加的虚拟机的ID
    vector<string> addList_day;
    long long serverCost, powerCost;
    vector<string> output;
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