开发提示：
====
此项目为BAC客户端重要的模块
BAC-Helper通过PE注入达到让游戏主进程主动加载此dll，并在此模块中进行BAC反作弊系统的初始化

模块入口函数定义在：BAC-Base.cpp

有两种调用反作弊方法：
1.有游戏项目源码时可以通过LoadLibrary将动态库加载到项目中以达到反作弊系统的初始化
2.没有游戏项目源码时可使用BAC-Helper项目中的BACHelper::ImportTableInject函数以达到主动加载反作弊系统的目的

