功能介绍：
====
应用层：
----
* 窗口监视：
	* 实现：ByteAntiCheat/BAC-Base/BACInterface/AntiFunction/MoniterCreateWindow.cpp
* 内存监视：
	* 实现：ByteAntiCheat/BAC-Base/BACInterface/AntiFunction/ProtectMemory.cpp
* 线程监视：
	* 实现：待实现
* 注入监视：
	* 实现：ByteAntiCheat/BAC-Base/BACInterface/AntiFunction/MonitorInjection.cpp
* 内核消息派遣机制：
	* 心跳验证：
		* 实现：ByteAntiCheat/BAC-Base/BACInterface/AntiFunction/KernelMessageDispatch.cpp
* 循环事件：
	* CRC32校验：
		* 实现：ByteAntiCheat/BAC-Base/BACInterface/AntiFunction/CheckMemory.cpp
		* 模块代码CRC32校验：
			* 1.调用BAC::InitMemoryCRC32List函数，延时等待进程模块加载完成后枚举进程中已加载模块的.text代码节，先计算出CRC32值并将信息加入BAC::_crc32_list变量中。
			* 2.BAC::CheckMemoryCRC32函数对BAC::_crc32_list变量中的模块代码信息进行CRC32值的校验，如发现异常可dump此模块的内存，同时可根据BAC::_crc32_list.first中记录的模块路径和dump文件一起上传服务器以方便数据回溯。
		* BACHooK点CRC32校验：
			* 1.在各个监控初始化函数下hook时将hook点的内存信息和CRC32值加入到BAC::_hook_list变量中。
			* 2.BAC::CheckHookPointer函数对BAC::_hook_list变量中的hook点内存信息做CRC32的校验，发现异常时可进行上报。
* 

内核层：
----
* 应用层消息派遣机制：
	* 实现：ByteAntiCheat/BAC-Base-Kernel/IRPControl/IRPControl.cpp
	* 通讯控制码定义：ByteAntiCheat/BAC-Base-Kernel/IRPControl/IRPControlCode.h
	* 通讯结构体定义：ByteAntiCheat/BAC-Base-Kernel/IRPControl/BAC-DataStruct.h
* 内核回调保护进程
	* 实现：ByteAntiCheat/BAC-Base-Kernel/BAC-Base-Kernel/ProcessProtect/ProcessProtect.cpp
	* 回调降权保护（进程|线程）：NTSTATUS ProtectProcess(const wchar_t* process_name)
	* 清空调试端口：ProcessProtect::ClearDebugPort(IN HANDLE pid)
* 内核层循环事件：
	* 实现：/ByteAntiCheat/BAC-Base-Kernel/BAC-Base-Kernel/KernelLoopEvent/KernelLoopEvent.cpp
	* 