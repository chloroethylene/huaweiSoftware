#pragma once
#include <iostream>
#include <cstdlib>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

class ServerList;
class VMList;
class ReqList;
class VM;

//存储备选服务器列表
class ServerList {
    unordered_map<string, vector<int>> serverInfos;
    vector<pair<string, vector<int>>> servernum_str;
public:
    vector<int>& operator[](const string& s);
    void add(string& serverType, string& cpuCores, string& memorySize, string& serverCost, string& powerCost);
    void self_sort();
    void read();
    string random_choose() const;
    string wise_choose(VM* vm) const;
};

//存储备选虚拟机列表
class VMList {
public:
    unordered_map<string, vector<int>> vmInfos;
    vector<int>& operator[](const string& s);
    void add(string& vmType, string& vmCpuCores, string& vmMemory, string& vmTwoNodes);
    void read();
};

//存储用户的请求列表
class ReqList {
    // 解析用户添加请求
    void generateRequest(string& op, string& reqVmType, string& reqId, int day);
    // 解析用户删除请求
    void generateRequest(string& op, string& reqId, int day);
public:
    unordered_map<string, vector<int>> vm_ttl;
    //[第几天][第几条]=>vector<string>
    vector<vector<vector<string>>> requestInfos;
    vector<vector<vector<string>>> operateInfos;
    vector<vector<vector<string>>>::iterator begin();
    vector<vector<vector<string>>>::iterator end();
    vector<vector<string>>& operator[](const int& ind);
    vector<vector<vector<string>>>::size_type size();
    void create_opInfos();
    void read();
};
