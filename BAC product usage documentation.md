无游戏项目源码时使用方法：
====
1.Release生成后需要将以下文件在VMP中进行编译：
----
* x64\Release\BAC-Helper64.exe
* x64\Release\ByteAntiCheat\BAC-Base64.dll
* x64\Release\ByteAntiCheat\BAC-Base.sys
* 编译完成后即可删除源文件然后恢复以上文件名

2.VMP编译完成后将以下文件/文件夹拷贝到游戏主进程目录下：
----
* x64\Release\BAC-Helper64.exe
* x64\Release\ByteAntiCheat

3.将游戏主进程拷贝到\ByteAntiCheat目录下(防止文件损坏)
----
4.运行BAC-Helper64.exe，等待生成注入PE后的游戏主进程，完成后将写到游戏原来的目录中
----
5.以管理员权限运行游戏即可主动加载BAC反作弊(管理员权限防止驱动加载失败，如有正规签名可另行处理)
----

有游戏项目源码时使用方法：
====
1.执行1-2步骤
----
2.游戏主进程源码中使用LoadLibrary加载ByteAntiCheat\BAC-Base64.dll(具体见ByteAntiCheat项目中的加载Demo)
----
