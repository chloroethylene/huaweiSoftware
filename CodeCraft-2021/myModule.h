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

//�洢��ѡ�������б�
class ServerList {
    unordered_map<string, vector<int>> serverInfos;
public:
    vector<int>& operator[](const string& s);
    void add(string& serverType, string& cpuCores, string& memorySize, string& serverCost, string& powerCost);
    void read();
    string random_choose() const;
};

//�洢��ѡ������б�
class VMList {
    unordered_map<string, vector<int>> vmInfos;
public:
    vector<int>& operator[](const string& s);
    void add(string& vmType, string& vmCpuCores, string& vmMemory, string& vmTwoNodes);
    void read();
};

//�洢�û��������б�
class ReqList {
    //[�ڼ���][�ڼ���]=>vector<string>
    vector<vector<vector<string>>> requestInfos;
    // �����û��������
    void generateRequest(string& op, string& reqVmType, string& reqId, int day);
    // �����û�ɾ������
    void generateRequest(string& op, string& reqId, int day);
public:
    vector<vector<vector<string>>>::iterator begin();
    vector<vector<vector<string>>>::iterator end();
    vector<vector<string>>& operator[](const int& ind);
    vector<vector<vector<string>>>::size_type size();
    void read();
};

class Server {
public:
    //������Ϣ
    string serverType;
    int cpuCoresA;
    int cpuCoresB;
    int memorySizeA;
    int memorySizeB;
    int purchaseCost;
    int oneDayPowerCost;
    //��¼���з����ڸ÷������ϵ������
    unordered_set<VM*> vmOnANode;
    unordered_set<VM*> vmOnBNode;
    unordered_set<VM*> vmOnTwoNodes;
    //��¼�����ID�������������Բ��ҵ�������ID
    int id;

    Server(const string& serverType);
    bool addVM(VM* vm);
    void delVM(VM* vm);
    int numOfVM()const;
};

class VM {
public:
    //������Ϣ
    string vmType;
    int cpuCores;
    int memorySize;
    bool doubleNodes;
    //��¼����������õķ�����
    Server* server;
    //��¼�������ĸ��ڵ�
    char node;

    VM(const string& vmType);
};

//�洢��ʱӵ�е����з�������Ϣ
class System {
    // ��������ID=>������
    vector<Server*> servers;
    // �������ID=>�����
    unordered_map<string, VM*> vms;

    unordered_map<string, vector<Server*>>purchase_day;
    vector<string> addList_day;

public:
    long long serverCost, powerCost;
    vector<string> output;
public:
    //���캯��
    System();
    // ���������������
    void addVM(vector<string>& createVmInfo);
    // ����ɾ�����������
    void delVM(vector<string>& delVmInfo);
    // ���ݷ���������
    void expansion(VM* vm);
    // Ǩ�����������
    void migrate();
    void serverPower();
    long long totalCost()const;
    void shuchu();
};