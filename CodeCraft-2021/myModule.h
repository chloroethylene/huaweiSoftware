//#pragma once
#include <iostream>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

//存储备选服务器列表
class ServerList {
    unordered_map<string, vector<int>> serverInfos;
public:
    vector<int>& operator[](string& s);
    void add(string& serverType, string& cpuCores, string& memorySize, string& serverCost, string& powerCost);
};

//存储备选虚拟机列表
class VMList {
    unordered_map<string, vector<int>> vmInfos;
public:
    vector<int>& operator[](string& s);
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

//存储此时拥有的所有服务器信息
class ServersInfo {
    // 已有的服务器数量
    int serverNumber;
    // 服务器的ID->服务器信息
    unordered_map<int, vector<int>> sysServerResource;
    //unordered_map<int,vector<string>> serverRunVms;
    // 服务器的ID->该服务器上运行的虚拟机数量
    vector<int> serverRunVms;
    // 记录虚拟机运行在哪个服务器上
    unordered_map<string, vector<int>> vmOnServer;
public:
    //构造函数
    ServersInfo();
    // 尝试在服务器上分配虚拟机资源
    bool choseServer(vector<int>& server, vector<int>& vm, int serverId, string vmId);
    // 处理创建虚拟机操作
    int addVM(vector<string>& createVmInfo, VMList& vmList);

    // 处理删除虚拟机操作
    void delVM(vector<string>& delVmInfo);

    // 初始化server，如何初始化购买的服务器是一个大的优化
    int buyServer(ServerList &serverList);
    // 扩容服务器策略
    void expansion();
    // 迁移虚拟机策略
    void migrate();
    int serverPower();
};