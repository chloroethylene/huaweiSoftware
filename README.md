# huaweiSoftware
2021华为软件精英挑战赛，卧龙凤雏队  
版本记录：  
v1.0 参考https://zhuanlan.zhihu.com/p/356155386 的baseline1.0  
v2.0 将部分全局变量和函数封装为类  
v2.1 去掉buyserver，改为每次add的时候expansion 结果：1366926260  
v2.2 考虑expansion时当expansion的服务器仍然容量不足时的情况，再次递归调用  
v2.3 提交有bug  
v3.0 解决了之前的bug，现在expansion时能够随机选取服务器了，一定程度上简化了代码逻辑  
v3.1 解决了上一版中的一些小bug，降低了代码的重复，将Server中对虚拟机的记录由vector改为unordered_set以节约查找时间  
v3.2 将读取数据放到程序最开始统一读取，输出放到最后统一输出；略优化了代码结构  
v3.3 引入添加虚拟机时的排序，结果：1364050524  
v4.0 加入运行中的服务器排序，对待选服务器排序，分拆代码 结果：1297457151  
v5.0 将服务器的选型改为按照价格升序选择；当A、B节点都可放置时，先放置较空的节点；增加了TTL的计算；宋公子牛逼！ 结果：1148929438  
v6.0 引入负载整理（虚拟机迁移）的过程，尽可能清空靠后的服务器；阿黛尔牛逼！ 结果：1115043380，不过暂时有点超时，运行时间：131.458s  
v6.1 修复了一点编码上的bug  
v6.2 去掉按TTL排序；迁移的代码结构改变。初赛正式集：成本1487213201，迁移次数25011，运行时间88.087s  