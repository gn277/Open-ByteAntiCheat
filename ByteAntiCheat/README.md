开发提示：
====
此项目模拟了一个正常的游戏进程（TestGame.exe），将游戏进程放在ByteAntiCheat目录下，运行BAC-Helper.exe启动游戏时会通过PE注入BAC-Base.dll，且注入完成后将游戏进程文件写回到原来的路径下，以达到让游戏主进程主动加载反作弊的目的。