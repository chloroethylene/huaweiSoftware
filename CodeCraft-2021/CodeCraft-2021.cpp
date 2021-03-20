//#pragma warning(disable:4996)
#include <iostream>
#include <cstdlib>
#include <string>
#include <unordered_map>
#include <vector>
#include <ctime>
#include <algorithm>
#include <cassert>
#include "basicInfos.h"
#include "solution.h"
using namespace std;

//#define TEST

// ��������Ϣ
ServerList serverList;
// �������Ϣ
VMList vmList;
// ������Ϣ
ReqList reqList;
//���������-�����ϵͳ
System serversInfo;

#ifdef TEST
const string filePath = "E:/cs/2021huaweitiaozhan/training-2.txt";
#endif

// �������
void match(int day) {
    for (int i = 0; i < reqList[day].size(); i++) {
        if (reqList[day][i].size() == 3) {
            serversInfo.addVM(reqList.operateInfos[day][i]);
            serversInfo.addList_day.push_back(reqList[day][i][2]);
        }
        else {
            serversInfo.delVM(reqList.operateInfos[day][i]);
        }
    }
}

int main() {
    clock_t start, finish;
    start = clock();
    //srand((int)time(0));

#ifdef TEST
    freopen(filePath.c_str(), "rb", stdin);
#endif

    serverList.read();
    serverList.self_sort();
    vmList.read();
    reqList.read();
    reqList.create_opInfos();

    // ��ʼ��������
    for (int day = 0; day < reqList.size(); ++day) {
        match(day);
        serversInfo.shuchu();
        serversInfo.serverPower();
    }

    for (auto& s : serversInfo.output) cout << s;

    fclose(stdin);
    finish = clock();
#ifdef TEST
    printf("\nusr time: %f s \n", double(finish - start) / CLOCKS_PER_SEC);
    printf("server cost: %lld \npower cost: %lld \ntotal cost: %lld \n", serversInfo.serverCost, serversInfo.powerCost, serversInfo.totalCost());
#endif
    return 0;
}
