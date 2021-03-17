//#pragma warning(disable:4996)
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

//#define TEST

// 服务器信息
ServerList serverList;
// 虚拟机信息
VMList vmList;
// 请求信息
ReqList reqList;
//定义服务器-虚拟机系统
System serversInfo;

#ifdef TEST
const string filePath = "E:/cs/2021huaweitiaozhan/training-2.txt";

#endif


// 分配策略
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
    /*for (auto req : reqList[day]) {
        // 创建虚拟机 还是 删除虚拟机
        if (req.size() == 3) {
            serversInfo.addVM(req);
        }
        else {
            serversInfo.delVM(req);
        }
        

    }*/
}

int main() {
    //    cin.tie(0);
    //    ios::sync_with_stdio(0);
    //srand((int)time(0));
    clock_t start, finish;
    start = clock();
#ifdef TEST
    freopen(filePath.c_str(), "rb", stdin);
#endif

    serverList.read();
    vmList.read();
    reqList.read();
    reqList.create_opInfos();

    // 开始处理请求
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
