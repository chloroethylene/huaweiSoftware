//#pragma once
#include <iostream>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

//�洢��ѡ�������б�
class ServerList {
    unordered_map<string, vector<int>> serverInfos;
public:
    vector<int>& operator[](string& s);
    void add(string& serverType, string& cpuCores, string& memorySize, string& serverCost, string& powerCost);
};

//�洢��ѡ������б�
class VMList {
    unordered_map<string, vector<int>> vmInfos;
public:
    vector<int>& operator[](string& s);
	void add(string& vmType, string& vmCpuCores, string& vmMemory, string& vmTwoNodes);
};

//�洢�û��������б�
class ReqList {
    vector<vector<string>> requestInfos;
public:
    vector<vector<string>>::iterator begin();
    vector<vector<string>>::iterator end();
    // �����û��������
    void generateRequest(string& op, string& reqVmType, string& reqId);
    // �����û�ɾ������
    void generateRequest(string& op, string& reqId);
    void clear();
};

//�洢��ʱӵ�е����з�������Ϣ
class ServersInfo {
    // ���еķ���������
    int serverNumber;
    // ��������ID->��������Ϣ
    unordered_map<int, vector<int>> sysServerResource;
    //unordered_map<int,vector<string>> serverRunVms;
    // ��������ID->�÷����������е����������
    vector<int> serverRunVms;
    // ��¼������������ĸ���������
    unordered_map<string, vector<int>> vmOnServer;
public:
    //���캯��
    ServersInfo();
    // �����ڷ������Ϸ����������Դ
    bool choseServer(vector<int>& server, vector<int>& vm, int serverId, string vmId);
    // ���������������
    int addVM(vector<string>& createVmInfo, VMList& vmList);

    // ����ɾ�����������
    void delVM(vector<string>& delVmInfo);

    // ��ʼ��server����γ�ʼ������ķ�������һ������Ż�
    int buyServer(ServerList &serverList);
    // ���ݷ���������
    void expansion();
    // Ǩ�����������
    void migrate();
    int serverPower();
};