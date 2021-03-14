#pragma warning(disable:4996)
#include <iostream>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <ctime>
#include <algorithm>
#include <cassert>
#include "myModule.h"

using namespace std;

#define TEST

// 服务器信息
ServerList serverList;
// 虚拟机信息
VMList vmList;
//定义服务器-虚拟机系统
System serversInfo;
// 请求信息
ReqList reqList;

#ifdef TEST
const string filePath = "C:/Users/jjy/Desktop/huaweiSoftware/vsProject/training-data/training-2.txt";

#endif

//unordered_map<int,vector<string>> requestAddInfos;
//vector<int> requestDelInfos;

void read() {
    int serverNum;
    string serverType, cpuCores, memorySize, serverCost, powerCost;
    scanf("%d", &serverNum);

    for (int i = 0; i < serverNum; i++) {
        cin >> serverType >> cpuCores >> memorySize >> serverCost >> powerCost;
        serverList.add(serverType, cpuCores, memorySize, serverCost, powerCost);
    }


    int vmNumber = 0;
    scanf("%d", &vmNumber);

    string vmType, vmCpuCores, vmMemory, vmTwoNodes;
    for (int i = 0; i < vmNumber; i++) {
        cin >> vmType >> vmCpuCores >> vmMemory >> vmTwoNodes;
        vmList.add(vmType, vmCpuCores, vmMemory, vmTwoNodes);
    }
}
// 分配策略
void match(int day) {
    for (auto req : reqList) {
        // 创建虚拟机 还是 删除虚拟机
        int opType = req.size() == 3 ? 1 : 0;
        if (opType) {
            serversInfo.addVM(req);
        }
        else {
            serversInfo.delVM(req);
        }
    }
}

int main() {
    //    cin.tie(0);
    //    ios::sync_with_stdio(0);
    srand((int)time(0));
    clock_t start, finish;
    start = clock();
#ifdef TEST
    std::freopen(filePath.c_str(), "rb", stdin);
#endif

    read();

    int requestdays = 0, dayRequestNumber = 0;
    scanf("%d", &requestdays);
    string op, reqVmType, reqId;

    // 开始处理请求
    for (int day = 0; day < requestdays; day++) {
        scanf("%d", &dayRequestNumber);
        reqList.clear();
        for (int i = 0; i < dayRequestNumber; i++) {
            cin >> op;
            if (op[1] == 'a') {
                cin >> reqVmType >> reqId;
                reqList.generateRequest(op, reqVmType, reqId);
            }
            else {
                cin >> reqId;
                reqList.generateRequest(op, reqId);
            }
        }
        match(day);
        serversInfo.shuchu();
        serversInfo.serverPower();
    }

    fclose(stdin);
    finish = clock();
#ifdef TEST
    printf("\nusr time: %f s \n", double(finish - start) / CLOCKS_PER_SEC);
    printf("server cost: %lld \npower cost: %lld \ntotal cost: %lld \n", serversInfo.serverCost, serversInfo.powerCost, serversInfo.totalCost());
#endif
    return 0;
}
