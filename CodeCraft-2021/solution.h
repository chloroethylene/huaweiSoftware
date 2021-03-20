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
    bool canAdd(VM* vm)const;
    bool addVM(VM* vm);
    void delVM(VM* vm);
    //����key����server��������
    double key()const;
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

class cmpServer {
public:
    bool operator()(const Server* server1, const Server* server2)const {
        return server1->key() < server2->key();//<
    }
};

//�洢��ʱӵ�е����з�������Ϣ
class System {
    //��������ʱ�ķ��������˳�򣬰���֮ǰ�ĺ͵�������з�����
    multiset<Server*, cmpServer> serversSorted;
    //����֮ǰ�ķ�����ID=>������
    vector<Server*> servers;
    //�������ID=>�����
    unordered_map<string, VM*> vms;
    //���칺��ķ�����
    unordered_map<string, vector<Server*>>purchase_day;
    //�����״���Ӧ�ķ�������������Ӻ������ɾ��
    bool firstFit(VM* vm);
    //��ĳ�����������º����
    void removeServer(Server* server);
public:
    //������ӵ��������ID
    vector<string> addList_day;
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