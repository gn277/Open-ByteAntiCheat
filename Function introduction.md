���ܽ��ܣ�
====
Ӧ�ò㣺
----
* ���ڼ��ӣ�
	* ʵ�֣�ByteAntiCheat/BAC-Base/BACInterface/AntiFunction/MoniterCreateWindow.cpp
* �ڴ���ӣ�
	* ʵ�֣�ByteAntiCheat/BAC-Base/BACInterface/AntiFunction/ProtectMemory.cpp
* �̼߳��ӣ�
	* ʵ�֣���ʵ��
* ע����ӣ�
	* ʵ�֣�ByteAntiCheat/BAC-Base/BACInterface/AntiFunction/MonitorInjection.cpp
* �ں���Ϣ��ǲ���ƣ�
	* ������֤��
		* ʵ�֣�ByteAntiCheat/BAC-Base/BACInterface/AntiFunction/KernelMessageDispatch.cpp
* ѭ���¼���
	* CRC32У�飺
		* ʵ�֣�ByteAntiCheat/BAC-Base/BACInterface/AntiFunction/CheckMemory.cpp
		* ģ�����CRC32У�飺
			* 1.����BAC::InitMemoryCRC32List��������ʱ�ȴ�����ģ�������ɺ�ö�ٽ������Ѽ���ģ���.text����ڣ��ȼ����CRC32ֵ������Ϣ����BAC::_crc32_list�����С�
			* 2.BAC::CheckMemoryCRC32������BAC::_crc32_list�����е�ģ�������Ϣ����CRC32ֵ��У�飬�緢���쳣��dump��ģ����ڴ棬ͬʱ�ɸ���BAC::_crc32_list.first�м�¼��ģ��·����dump�ļ�һ���ϴ��������Է������ݻ��ݡ�
		* BACHooK��CRC32У�飺
			* 1.�ڸ�����س�ʼ��������hookʱ��hook����ڴ���Ϣ��CRC32ֵ���뵽BAC::_hook_list�����С�
			* 2.BAC::CheckHookPointer������BAC::_hook_list�����е�hook���ڴ���Ϣ��CRC32��У�飬�����쳣ʱ�ɽ����ϱ���
* 

�ں˲㣺
----
* Ӧ�ò���Ϣ��ǲ���ƣ�
	* ʵ�֣�ByteAntiCheat/BAC-Base-Kernel/IRPControl/IRPControl.cpp
	* ͨѶ�����붨�壺ByteAntiCheat/BAC-Base-Kernel/IRPControl/IRPControlCode.h
	* ͨѶ�ṹ�嶨�壺ByteAntiCheat/BAC-Base-Kernel/IRPControl/BAC-DataStruct.h
* �ں˻ص���������
	* ʵ�֣�ByteAntiCheat/BAC-Base-Kernel/BAC-Base-Kernel/ProcessProtect/ProcessProtect.cpp
	* �ص���Ȩ����������|�̣߳���NTSTATUS ProtectProcess(const wchar_t* process_name)
	* ��յ��Զ˿ڣ�ProcessProtect::ClearDebugPort(IN HANDLE pid)
* �ں˲�ѭ���¼���
	* ʵ�֣�/ByteAntiCheat/BAC-Base-Kernel/BAC-Base-Kernel/KernelLoopEvent/KernelLoopEvent.cpp
	* 