#pragma once
#include <windows.h>

////////////////////////////////////////////////////////
/////////  NtQuerySystemInformation �����õ���ö�ٳ���
////////////////////////////////////////////////////////
////typedef enum _SYSTEM_INFORMATION_CLASS
////{
////	SystemBasicInformation,                 //  0 Y N   
////	SystemProcessorInformation,             //  1 Y N   
////	SystemPerformanceInformation,           //  2 Y N   
////	SystemTimeOfDayInformation,             //  3 Y N   
////	SystemNotImplemented1,                  //  4 Y N   
////	SystemProcessesAndThreadsInformation,   //  5 Y N   
////	SystemCallCounts,                       //  6 Y N   
////	SystemConfigurationInformation,         //  7 Y N   
////	SystemProcessorTimes,                   //  8 Y N   
////	SystemGlobalFlag,                       //  9 Y Y   
////	SystemNotImplemented2,                  // 10 Y N   
////	SystemModuleInformation,                // 11 Y N   
////	SystemLockInformation,                  // 12 Y N   
////	SystemNotImplemented3,                  // 13 Y N   
////	SystemNotImplemented4,                  // 14 Y N   
////	SystemNotImplemented5,                  // 15 Y N   
////	SystemHandleInformation,                // 16 Y N   
////	SystemObjectInformation,                // 17 Y N   
////	SystemPagefileInformation,              // 18 Y N   
////	SystemInstructionEmulationCounts,       // 19 Y N   
////	SystemInvalidInfoClass1,                // 20   
////	SystemCacheInformation,                 // 21 Y Y   
////	SystemPoolTagInformation,               // 22 Y N   
////	SystemProcessorStatistics,              // 23 Y N   
////	SystemDpcInformation,                   // 24 Y Y   
////	SystemNotImplemented6,                  // 25 Y N   
////	SystemLoadImage,                        // 26 N Y   
////	SystemUnloadImage,                      // 27 N Y   
////	SystemTimeAdjustment,                   // 28 Y Y  
////	SystemNotImplemented7,                  // 29 Y N  
////	SystemNotImplemented8,                  // 30 Y N   
////	SystemNotImplemented9,                  // 31 Y N   
////	SystemCrashDumpInformation,             // 32 Y N  
////	SystemExceptionInformation,             // 33 Y N  
////	SystemCrashDumpStateInformation,        // 34 Y Y/N   
////	SystemKernelDebuggerInformation,        // 35 Y N   
////	SystemContextSwitchInformation,         // 36 Y N  
////	SystemRegistryQuotaInformation,         // 37 Y Y   
////	SystemLoadAndCallImage,                 // 38 N Y  
////	SystemPrioritySeparation,               // 39 N Y  
////	SystemNotImplemented10,                 // 40 Y N  
////	SystemNotImplemented11,                 // 41 Y N   
////	SystemInvalidInfoClass2,                // 42   
////	SystemInvalidInfoClass3,                // 43   
////	SystemTimeZoneInformation,              // 44 Y N   
////	SystemLookasideInformation,             // 45 Y N   
////	SystemSetTimeSlipEvent,                 // 46 N Y   
////	SystemCreateSession,                    // 47 N Y   
////	SystemDeleteSession,                    // 48 N Y   
////	SystemInvalidInfoClass4,                // 49   
////	SystemRangeStartInformation,            // 50 Y N   
////	SystemVerifierInformation,              // 51 Y Y   
////	SystemAddVerifier,                      // 52 N Y   
////	SystemSessionProcessesInformation       // 53 Y N   
////} SYSTEM_INFORMATION_CLASS;
////
////#define NTAPI    __stdcall
////
///////////////////////////////////////////
/////////  �߳�״̬��ö�ٳ���
///////////////////////////////////////////
////typedef enum _THREAD_STATE {
////	StateInitialized, // ��ʼ��״̬
////	StateReady, // ׼��״̬
////	StateRunning, // ����״̬
////	StateStandby, // 
////	StateTerminated,//�ر�
////	StateWait, // �ȴ�
////	StateTransition, // �л�???
////	StateUnknown
////}THREAD_STATE;
////
///////////////////////////////////////////
///////// �̴߳��ڵȴ���ԭ���ö�ٳ��� 
///////////////////////////////////////////
////typedef enum _KWAIT_REASON {
////	Executive,
////	FreePage,
////	PageIn,
////	PoolAllocation,
////	DelayExecution,
////	Suspended,
////	UserRequest,
////	WrExecutive,
////	WrFreePage,
////	WrPageIn,
////	WrPoolAllocation,
////	WrDelayExecution,
////	WrSuspended,
////	WrUserRequest,
////	WrEventPair,
////	WrQueue,
////	WrLpcReceive,
////	WrLpcReply,
////	WrVirtualMemory,
////	WrPageOut,
////	WrRendezvous,
////	Spare2,
////	Spare3,
////	Spare4,
////	Spare5,
////	Spare6,
////	WrKernel,
////	MaximumWaitReason
////}KWAIT_REASON;
////
//////typedef LONG	NTSTATUS;
////typedef LONG    KPRIORITY;
////
///////////////////////////////////////////
///////// �ں���,��ʾ����ID���߳�ID�Ľṹ�� 
///////////////////////////////////////////
////typedef struct _CLIENT_ID {
////	HANDLE        UniqueProcess; // PID
////	HANDLE        UniqueThread;  // TID
////} CLIENT_ID, * PCLIENT_ID;
////
////typedef struct _VM_COUNTERS {
////	SIZE_T        PeakVirtualSize;
////	SIZE_T        VirtualSize;
////	ULONG         PageFaultCount;
////	SIZE_T        PeakWorkingSetSize;
////	SIZE_T        WorkingSetSize;
////	SIZE_T        QuotaPeakPagedPoolUsage;
////	SIZE_T        QuotaPagedPoolUsage;
////	SIZE_T        QuotaPeakNonPagedPoolUsage;
////	SIZE_T        QuotaNonPagedPoolUsage;
////	SIZE_T        PagefileUsage;
////	SIZE_T        PeakPagefileUsage;
////} VM_COUNTERS;
////
///////////////////////////////////////////
///////// �߳���Ϣ�ṹ�� 
///////////////////////////////////////////
////typedef struct _SYSTEM_THREAD_INFORMATION {
////	LARGE_INTEGER   KernelTime;
////	LARGE_INTEGER   UserTime;
////	LARGE_INTEGER   CreateTime;
////	ULONG           WaitTime;
////	PVOID           StartAddress;
////	CLIENT_ID       ClientId;
////	KPRIORITY       Priority;
////	KPRIORITY       BasePriority;
////	ULONG           ContextSwitchCount;
////	LONG            State;// ״̬,��THREAD_STATEö�������е�һ��ֵ
////	LONG            WaitReason;//�ȴ�ԭ��, KWAIT_REASON�е�һ��ֵ
////} SYSTEM_THREAD_INFORMATION, * PSYSTEM_THREAD_INFORMATION;
////
///////////////////////////////////////////
///////// �ں����õ���Unicode�ַ����Ľṹ�� 
///////////////////////////////////////////
////typedef struct _UNICODE_STRING {
////	USHORT Length;
////	USHORT MaximumLength;
////	WCHAR* Buffer;
////} UNICODE_STRING, * PUNICODE_STRING;
////
///////////////////////////////////////////////////////////////////
/////////  NtQuerySystemInformation ��ѯ������Ϣʱ�õ��Ľ�����Ϣ�ṹ��
///////////////////////////////////////////////////////////////////
////typedef struct _SYSTEM_PROCESS_INFORMATION {
////	ULONG            NextEntryDelta; // ƫ�Ƶ���һ��Ԫ�ص��ֽ���
////	ULONG            ThreadCount; // �����̵����߳���
////	ULONG            Reserved1[6]; // ����
////	LARGE_INTEGER    CreateTime; // ���̵Ĵ���ʱ��
////	LARGE_INTEGER    UserTime; // ���û����ʹ��ʱ��
////	LARGE_INTEGER    KernelTime; // ���ں˲��ʹ��ʱ��
////	UNICODE_STRING   ProcessName; // ������
////	KPRIORITY        BasePriority; // ���ȼ�
////	ULONG            ProcessId;
////	ULONG            InheritedFromProcessId;// ����ID
////	ULONG            HandleCount; // ���̵ľ������
////	ULONG            Reserved2[2]; // ����
////	VM_COUNTERS      VmCounters;
////	IO_COUNTERS      IoCounters;
////	SYSTEM_THREAD_INFORMATION Threads[5]; // ���߳���Ϣ����
////}SYSTEM_PROCESS_INFORMATION, * PSYSTEM_PROCESS_INFORMATION;
////
///////////////////////////////////////////
///////// ����ָ������ 
///////////////////////////////////////////
////typedef NTSTATUS(NTAPI* fnNtQuerySystemInformation)(SYSTEM_INFORMATION_CLASS SystemInformationClass,
////	PVOID               SystemInformation,
////	ULONG                SystemInformationLength,
////	PULONG              ReturnLength OPTIONAL);
////
/////*++ BUILD Version: 0001    // Increment this if a change has global effects
////
////Copyright (c) Microsoft Corporation. All rights reserved.
////
////You may only use this code if you agree to the terms of the Windows Research Kernel Source Code License agreement (see License.txt).
////If you do not agree to the terms, do not use the code.
////
////
////Module Name:
////
////pebteb.w
////
////Abstract:
////
////Declarations of PEB and TEB, and some types contained in them.
////
////Address the maintenance problem that resulted from PEB and TEB being
////defined three times, once "native" in ntpsapi.w, and twice, 32bit and 64bit
////in wow64t.w.
////
////--*/
//////
//////
////// This file deliberately lacks #pragma once or #ifndef guards.
////// It is only included by ntpsapi.h and wow64t.h, never directly.
//////
//////
//////
////// This file is #included three times.
//////
////// 1) by ntpsapi.h, with no "unusual" macros defined, to declare
//////    PEB and TEB, and some types contained in them
////// 2) by wow64t.h to declare PEB32 and TEB32, and some types contained in them
////// 3) by wow64t.h to declare PEB64 and TEB64, and some types contained in them
//////
////// wow64t.h #defines the macro PEBTEB_BITS to guide the declarations.
//////
////
////#define PEBTEB_PRIVATE_PASTE(x,y)       x##y
////#define PEBTEB_PASTE(x,y)               PEBTEB_PRIVATE_PASTE(x,y)
////
////#if defined(PEBTEB_BITS) /* This is defined by wow64t.h. */
////
////#if PEBTEB_BITS == 32
////
////#define PEBTEB_STRUCT(x)    PEBTEB_PASTE(x, 32) /* FOO32 */
////#define PEBTEB_POINTER(x)   TYPE32(x) /* ULONG, defined in wow64t.h */
////
////#elif PEBTEB_BITS == 64
////
////#define PEBTEB_STRUCT(x)    PEBTEB_PASTE(x, 64) /* FOO64 */
////#define PEBTEB_POINTER(x)   TYPE64(x) /* ULONGLONG, defined in wow64t.h */
////
////#else
////
////#error Unknown value for pebteb_bits (PEBTEB_BITS).
////
////#endif
////
////#else
////
//////
////// Declare and use regular native types.
//////
////#define PEBTEB_POINTER(x) x
////#define PEBTEB_STRUCT(x)  x
////
////#endif
////
////typedef VOID(*PPS_POST_PROCESS_INIT_ROUTINE) (VOID);
////
////typedef struct _PEB_FREE_BLOCK {
////	struct _PEB_FREE_BLOCK* Next;
////	ULONG Size;
////} PEB_FREE_BLOCK, * PPEB_FREE_BLOCK;
////
////
///////////////////////////////////////////
///////// ����ģ��������Ϣ�ṹ��
///////////////////////////////////////////
////typedef struct _PEB_LDR_DATA {
////	ULONG Length;
////	BOOLEAN Initialized;
////	HANDLE SsHandle;
////	LIST_ENTRY InLoadOrderModuleList; // ���ռ���˳����������ģ������(˫������),ÿ���ڵ㶼ָ��ṹ��:_LDR_DATA_TABLE_ENTRY
////	LIST_ENTRY InMemoryOrderModuleList;//�����ڴ��ַ˳����������ģ������(˫������),ÿ���ڵ㶼ָ��ṹ��:_LDR_DATA_TABLE_ENTRY
////	LIST_ENTRY InInitializationOrderModuleList;//���ճ�ʼ��˳�������ģ������(˫������),ÿ���ڵ㶼ָ��ṹ��:_LDR_DATA_TABLE_ENTRY
////	PVOID EntryInProgress;
////} PEB_LDR_DATA, * PPEB_LDR_DATA;
////
///////////////////////////////////////////
///////// ģ������ڵ� 
///////////////////////////////////////////
////typedef struct _LDR_DATA_TABLE_ENTRY
////{
////	LIST_ENTRY InLoadOrderLinks;// ���ռ���˳����������ģ������(˫������),ÿ���ڵ㶼ָ��ṹ��:_LDR_DATA_TABLE_ENTRY
////	LIST_ENTRY InMemoryOrderLinks;//�����ڴ��ַ˳����������ģ������(˫������),ÿ���ڵ㶼ָ��ṹ��:_LDR_DATA_TABLE_ENTRY
////	LIST_ENTRY InInitializationOrderLinks;//���ճ�ʼ��˳�������ģ������(˫������),ÿ���ڵ㶼ָ��ṹ��:_LDR_DATA_TABLE_ENTRY
////	PVOID DllBase; // dll���ػ�ַ
////	PVOID EntryPoint; // dll����ڵ�(OEP)
////	DWORD SizeOfImage; // dll���ڴ��е��ֽ���
////	UNICODE_STRING FullDllName; // dll�ļ�·��
////	UNICODE_STRING BaseDllName; // dllģ����
////	DWORD Flags;
////	WORD LoadCount;
////	WORD TlsIndex;
////	LIST_ENTRY HashLinks;
////	PVOID SectionPointer;
////	DWORD CheckSum;
////	DWORD TimeDateStamp;
////	PVOID LoadedImports;
////	PVOID EntryPointActivationContext;
////	PVOID PatchInformation;
////}LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;
//
//#define GDI_HANDLE_BUFFER_SIZE32  34
//#define GDI_HANDLE_BUFFER_SIZE64  60
//
//#if !defined(_AMD64_)
//#define GDI_HANDLE_BUFFER_SIZE      GDI_HANDLE_BUFFER_SIZE32
//#else
//#define GDI_HANDLE_BUFFER_SIZE      GDI_HANDLE_BUFFER_SIZE64
//#endif
//
//typedef ULONG GDI_HANDLE_BUFFER[GDI_HANDLE_BUFFER_SIZE];
//
///* for searching
//typedef struct _PEB
//typedef struct _PEB32
//typedef struct _PEB64
//*/
/////////////////////////////////////////
/////// PEB �ṹ�� 
/////////////////////////////////////////
//typedef struct PEBTEB_STRUCT(_PEB) {
//	BOOLEAN InheritedAddressSpace;      // These four fields cannot change unless the
//	BOOLEAN ReadImageFileExecOptions;   //
//	BOOLEAN BeingDebugged;              // �Ƿ񱻵���
//	union {
//		BOOLEAN BitField;                  //
//		struct {
//			BOOLEAN ImageUsesLargePages : 1;
//			BOOLEAN SpareBits : 7;
//		};
//	};
//	PEBTEB_POINTER(HANDLE) Mutant;      // INITIAL_PEB structure is also updated.
//
//	PEBTEB_POINTER(PVOID) ImageBaseAddress;
//	PEBTEB_POINTER(PPEB_LDR_DATA) Ldr; // ����ģ������
//	PEBTEB_POINTER(struct _RTL_USER_PROCESS_PARAMETERS*) ProcessParameters;
//	PEBTEB_POINTER(PVOID) SubSystemData;
//	PEBTEB_POINTER(PVOID) ProcessHeap;
//	PEBTEB_POINTER(struct _RTL_CRITICAL_SECTION*) FastPebLock;
//	PEBTEB_POINTER(PVOID) AtlThunkSListPtr;     // Used only for AMD64
//	PEBTEB_POINTER(PVOID) SparePtr2;
//	ULONG EnvironmentUpdateCount;
//	PEBTEB_POINTER(PVOID) KernelCallbackTable;
//	ULONG SystemReserved[1];
//	ULONG SpareUlong;
//	PEBTEB_POINTER(PPEB_FREE_BLOCK) FreeList;
//	ULONG TlsExpansionCounter;
//	PEBTEB_POINTER(PVOID) TlsBitmap;
//	ULONG TlsBitmapBits[2];         // TLS_MINIMUM_AVAILABLE bits
//	PEBTEB_POINTER(PVOID) ReadOnlySharedMemoryBase;
//	PEBTEB_POINTER(PVOID) ReadOnlySharedMemoryHeap;
//	PEBTEB_POINTER(PVOID*) ReadOnlyStaticServerData;
//	PEBTEB_POINTER(PVOID) AnsiCodePageData;
//	PEBTEB_POINTER(PVOID) OemCodePageData;
//	PEBTEB_POINTER(PVOID) UnicodeCaseTableData;
//
//	//
//	// Useful information for LdrpInitialize
//	ULONG NumberOfProcessors;
//	ULONG NtGlobalFlag;			// �ں�ȫ�ֱ��
//
//
//	//
//	// Passed up from MmCreatePeb from Session Manager registry key
//	//
//
//	LARGE_INTEGER CriticalSectionTimeout;
//	PEBTEB_POINTER(SIZE_T) HeapSegmentReserve;
//	PEBTEB_POINTER(SIZE_T) HeapSegmentCommit;
//	PEBTEB_POINTER(SIZE_T) HeapDeCommitTotalFreeThreshold;
//	PEBTEB_POINTER(SIZE_T) HeapDeCommitFreeBlockThreshold;
//
//	//
//	// Where heap manager keeps track of all heaps created for a process
//	// Fields initialized by MmCreatePeb.  ProcessHeaps is initialized
//	// to point to the first free byte after the PEB and MaximumNumberOfHeaps
//	// is computed from the page size used to hold the PEB, less the fixed
//	// size of this data structure.
//	//
//
//	ULONG NumberOfHeaps;
//	ULONG MaximumNumberOfHeaps;
//	PEBTEB_POINTER(PVOID*) ProcessHeaps;
//
//	//
//	//
//	PEBTEB_POINTER(PVOID) GdiSharedHandleTable;
//	PEBTEB_POINTER(PVOID) ProcessStarterHelper;
//	ULONG GdiDCAttributeList;
//	PEBTEB_POINTER(struct _RTL_CRITICAL_SECTION*) LoaderLock;
//
//	//
//	// Following fields filled in by MmCreatePeb from system values and/or
//	// image header.
//	//
//
//	ULONG OSMajorVersion;
//	ULONG OSMinorVersion;
//	USHORT OSBuildNumber;
//	USHORT OSCSDVersion;
//	ULONG OSPlatformId;
//	ULONG ImageSubsystem;
//	ULONG ImageSubsystemMajorVersion;
//	ULONG ImageSubsystemMinorVersion;
//	PEBTEB_POINTER(ULONG_PTR) ImageProcessAffinityMask;
//	PEBTEB_STRUCT(GDI_HANDLE_BUFFER) GdiHandleBuffer;
//	PEBTEB_POINTER(PPS_POST_PROCESS_INIT_ROUTINE) PostProcessInitRoutine;
//
//	PEBTEB_POINTER(PVOID) TlsExpansionBitmap;
//	ULONG TlsExpansionBitmapBits[32];   // TLS_EXPANSION_SLOTS bits
//
//	//
//	// Id of the Hydra session in which this process is running
//	//
//	ULONG SessionId;
//
//	//
//	// Filled in by LdrpInstallAppcompatBackend
//	//
//	ULARGE_INTEGER AppCompatFlags;
//
//	//
//	// ntuser appcompat flags
//	//
//	ULARGE_INTEGER AppCompatFlagsUser;
//
//	//
//	// Filled in by LdrpInstallAppcompatBackend
//	//
//	PEBTEB_POINTER(PVOID) pShimData;
//
//	//
//	// Filled in by LdrQueryImageFileExecutionOptions
//	//
//	PEBTEB_POINTER(PVOID) AppCompatInfo;
//
//	//
//	// Used by GetVersionExW as the szCSDVersion string
//	//
//	PEBTEB_STRUCT(UNICODE_STRING) CSDVersion;
//
//	//
//	// Fusion stuff
//	//
//	PEBTEB_POINTER(const struct _ACTIVATION_CONTEXT_DATA*) ActivationContextData;
//	PEBTEB_POINTER(struct _ASSEMBLY_STORAGE_MAP*) ProcessAssemblyStorageMap;
//	PEBTEB_POINTER(const struct _ACTIVATION_CONTEXT_DATA*) SystemDefaultActivationContextData;
//	PEBTEB_POINTER(struct _ASSEMBLY_STORAGE_MAP*) SystemAssemblyStorageMap;
//
//	//
//	// Enforced minimum initial commit stack
//	//
//	PEBTEB_POINTER(SIZE_T) MinimumStackCommit;
//
//	//
//	// Fiber local storage.
//	//
//
//	PEBTEB_POINTER(PVOID*) FlsCallback;
//	PEBTEB_STRUCT(LIST_ENTRY) FlsListHead;
//	PEBTEB_POINTER(PVOID) FlsBitmap;
//	ULONG FlsBitmapBits[FLS_MAXIMUM_AVAILABLE / (sizeof(ULONG) * 8)];
//	ULONG FlsHighIndex;
//} PEBTEB_STRUCT(PEB), * PEBTEB_STRUCT(PPEB);
//
////
////  Fusion/sxs thread state information
////
//
//#define ACTIVATION_CONTEXT_STACK_FLAG_QUERIES_DISABLED (0x00000001)
//
//typedef struct PEBTEB_STRUCT(_ACTIVATION_CONTEXT_STACK) {
//	PEBTEB_POINTER(struct _RTL_ACTIVATION_CONTEXT_STACK_FRAME*) ActiveFrame;
//	PEBTEB_STRUCT(LIST_ENTRY) FrameListCache;
//	ULONG Flags;
//	ULONG NextCookieSequenceNumber;
//	ULONG StackId;
//} PEBTEB_STRUCT(ACTIVATION_CONTEXT_STACK), * PEBTEB_STRUCT(PACTIVATION_CONTEXT_STACK);
//
//typedef const PEBTEB_STRUCT(ACTIVATION_CONTEXT_STACK)* PEBTEB_STRUCT(PCACTIVATION_CONTEXT_STACK);
//
//#define TEB_ACTIVE_FRAME_CONTEXT_FLAG_EXTENDED (0x00000001)
//
//typedef struct PEBTEB_STRUCT(_TEB_ACTIVE_FRAME_CONTEXT) {
//	ULONG Flags;
//	PEBTEB_POINTER(PCSTR) FrameName;
//} PEBTEB_STRUCT(TEB_ACTIVE_FRAME_CONTEXT), * PEBTEB_STRUCT(PTEB_ACTIVE_FRAME_CONTEXT);
//
//typedef const PEBTEB_STRUCT(TEB_ACTIVE_FRAME_CONTEXT)* PEBTEB_STRUCT(PCTEB_ACTIVE_FRAME_CONTEXT);
//
//typedef struct PEBTEB_STRUCT(_TEB_ACTIVE_FRAME_CONTEXT_EX) {
//	PEBTEB_STRUCT(TEB_ACTIVE_FRAME_CONTEXT) BasicContext;
//	PEBTEB_POINTER(PCSTR) SourceLocation; // e.g. "Z:\foo\bar\baz.c"
//} PEBTEB_STRUCT(TEB_ACTIVE_FRAME_CONTEXT_EX), * PEBTEB_STRUCT(PTEB_ACTIVE_FRAME_CONTEXT_EX);
//
//typedef const PEBTEB_STRUCT(TEB_ACTIVE_FRAME_CONTEXT_EX)* PEBTEB_STRUCT(PCTEB_ACTIVE_FRAME_CONTEXT_EX);
//
//#define TEB_ACTIVE_FRAME_FLAG_EXTENDED (0x00000001)
//
//typedef struct PEBTEB_STRUCT(_TEB_ACTIVE_FRAME) {
//	ULONG Flags;
//	PEBTEB_POINTER(struct _TEB_ACTIVE_FRAME*) Previous;
//	PEBTEB_POINTER(PCTEB_ACTIVE_FRAME_CONTEXT) Context;
//} PEBTEB_STRUCT(TEB_ACTIVE_FRAME), * PEBTEB_STRUCT(PTEB_ACTIVE_FRAME);
//
//typedef const PEBTEB_STRUCT(TEB_ACTIVE_FRAME)* PEBTEB_STRUCT(PCTEB_ACTIVE_FRAME);
//
//typedef struct PEBTEB_STRUCT(_TEB_ACTIVE_FRAME_EX) {
//	PEBTEB_STRUCT(TEB_ACTIVE_FRAME) BasicFrame;
//	PEBTEB_POINTER(PVOID) ExtensionIdentifier; // use address of your DLL Main or something unique to your mapping in the address space
//} PEBTEB_STRUCT(TEB_ACTIVE_FRAME_EX), * PEBTEB_STRUCT(PTEB_ACTIVE_FRAME_EX);
//
//typedef const PEBTEB_STRUCT(TEB_ACTIVE_FRAME_EX)* PEBTEB_STRUCT(PCTEB_ACTIVE_FRAME_EX);
//
///* for searching
//typedef struct _TEB
//typedef struct _TEB32
//typedef struct _TEB64
//*/
//#define WIN32_CLIENT_INFO_LENGTH 62
//#define STATIC_UNICODE_BUFFER_LENGTH 261
//
////typedef PVOID* PPVOID;
//
////
//// Gdi command batching
////
//
//#define GDI_BATCH_BUFFER_SIZE 310
//
//typedef struct _GDI_TEB_BATCH {
//	ULONG    Offset;
//	ULONG_PTR HDC;
//	ULONG    Buffer[GDI_BATCH_BUFFER_SIZE];
//} GDI_TEB_BATCH, * PGDI_TEB_BATCH;
//
//typedef struct PEBTEB_STRUCT(_TEB) {
//	PEBTEB_STRUCT(NT_TIB) NtTib;
//	PEBTEB_POINTER(PVOID) EnvironmentPointer;
//	PEBTEB_STRUCT(CLIENT_ID) ClientId;
//	PEBTEB_POINTER(PVOID) ActiveRpcHandle;
//	PEBTEB_POINTER(PVOID) ThreadLocalStoragePointer;
//	PEBTEB_POINTER(PPEB) ProcessEnvironmentBlock;
//	ULONG LastErrorValue;
//	ULONG CountOfOwnedCriticalSections;
//	PEBTEB_POINTER(PVOID) CsrClientThread;
//	PEBTEB_POINTER(PVOID) Win32ThreadInfo;          // PtiCurrent
//	ULONG User32Reserved[26];       // user32.dll items
//	ULONG UserReserved[5];          // Winsrv SwitchStack
//	PEBTEB_POINTER(PVOID) WOW32Reserved;            // used by WOW
//	LCID CurrentLocale;
//	ULONG FpSoftwareStatusRegister; // offset known by outsiders!
//	PEBTEB_POINTER(PVOID) SystemReserved1[54];      // Used by FP emulator
//	NTSTATUS ExceptionCode;         // for RaiseUserException
//	// 4 bytes of padding here on native 64bit TEB and TEB64
//	PEBTEB_POINTER(PACTIVATION_CONTEXT_STACK) ActivationContextStackPointer; // Fusion activation stack
//#if (defined(PEBTEB_BITS) && (PEBTEB_BITS == 64)) || (!defined(PEBTEB_BITS) && defined(_WIN64))
//	UCHAR SpareBytes1[28]; // native 64bit TEB and TEB64
//#else
//	UCHAR SpareBytes1[40]; // native 32bit TEB and TEB32
//#endif
//	PEBTEB_STRUCT(GDI_TEB_BATCH) GdiTebBatch;      // Gdi batching
//	PEBTEB_STRUCT(CLIENT_ID) RealClientId;
//	PEBTEB_POINTER(HANDLE) GdiCachedProcessHandle;
//	ULONG GdiClientPID;
//	ULONG GdiClientTID;
//	PEBTEB_POINTER(PVOID) GdiThreadLocalInfo;
//	PEBTEB_POINTER(ULONG_PTR) Win32ClientInfo[WIN32_CLIENT_INFO_LENGTH]; // User32 Client Info
//	PEBTEB_POINTER(PVOID) glDispatchTable[233];     // OpenGL
//	PEBTEB_POINTER(ULONG_PTR) glReserved1[29];      // OpenGL
//	PEBTEB_POINTER(PVOID) glReserved2;              // OpenGL
//	PEBTEB_POINTER(PVOID) glSectionInfo;            // OpenGL
//	PEBTEB_POINTER(PVOID) glSection;                // OpenGL
//	PEBTEB_POINTER(PVOID) glTable;                  // OpenGL
//	PEBTEB_POINTER(PVOID) glCurrentRC;              // OpenGL
//	PEBTEB_POINTER(PVOID) glContext;                // OpenGL
//	ULONG LastStatusValue;
//	PEBTEB_STRUCT(UNICODE_STRING) StaticUnicodeString;
//	WCHAR StaticUnicodeBuffer[STATIC_UNICODE_BUFFER_LENGTH];
//	PEBTEB_POINTER(PVOID) DeallocationStack;
//	PEBTEB_POINTER(PVOID) TlsSlots[TLS_MINIMUM_AVAILABLE];
//	PEBTEB_STRUCT(LIST_ENTRY) TlsLinks;
//	PEBTEB_POINTER(PVOID) Vdm;
//	PEBTEB_POINTER(PVOID) ReservedForNtRpc;
//	PEBTEB_POINTER(PVOID) DbgSsReserved[2];
//	ULONG HardErrorMode;
//	PEBTEB_POINTER(PVOID) Instrumentation[14];
//	PEBTEB_POINTER(PVOID) SubProcessTag;
//	PEBTEB_POINTER(PVOID) EtwTraceData;
//	PEBTEB_POINTER(PVOID) WinSockData;              // WinSock
//	ULONG GdiBatchCount;
//	BOOLEAN InDbgPrint;
//	BOOLEAN FreeStackOnTermination;
//	BOOLEAN HasFiberData;
//	BOOLEAN IdealProcessor;
//	ULONG GuaranteedStackBytes;
//	PEBTEB_POINTER(PVOID) ReservedForPerf;
//	PEBTEB_POINTER(PVOID) ReservedForOle;
//	ULONG WaitingOnLoaderLock;
//	PEBTEB_POINTER(ULONG_PTR) SparePointer1;
//	PEBTEB_POINTER(ULONG_PTR) SoftPatchPtr1;
//	PEBTEB_POINTER(ULONG_PTR) SoftPatchPtr2;
//	PEBTEB_POINTER(PVOID*) TlsExpansionSlots;
//#if (defined(_WIN64) && !defined(PEBTEB_BITS)) \
//    || ((defined(_WIN64) || defined(_X86_)) && defined(PEBTEB_BITS) && PEBTEB_BITS == 64)
//	//
//	// These are in native Win64 TEB, Win64 TEB64, and x86 TEB64.
//	//
//	PEBTEB_POINTER(PVOID) DeallocationBStore;
//	PEBTEB_POINTER(PVOID) BStoreLimit;
//#endif    
//	LCID ImpersonationLocale;       // Current locale of impersonated user
//	ULONG IsImpersonating;          // Thread impersonation status
//	PEBTEB_POINTER(PVOID) NlsCache;                 // NLS thread cache
//	PEBTEB_POINTER(PVOID) pShimData;                // Per thread data used in the shim
//	ULONG HeapVirtualAffinity;
//	PEBTEB_POINTER(HANDLE) CurrentTransactionHandle;// reserved for TxF transaction context
//	PEBTEB_POINTER(PTEB_ACTIVE_FRAME) ActiveFrame;
//	PEBTEB_POINTER(PVOID) FlsData;
//	BOOLEAN SafeThunkCall;
//	BOOLEAN BooleanSpare[3];
//
//} PEBTEB_STRUCT(TEB), * PEBTEB_STRUCT(PTEB);
//
//////���̻�����Ϣ
////typedef struct _PROCESS_BASIC_INFORMATION
////{
////	NTSTATUS ExitStatus;
////	PVOID PebBaseAddress;
////	ULONG_PTR AffinityMask;
////	LONG BasePriority;
////	ULONG_PTR UniqueProcessId;
////	ULONG_PTR InheritedFromUniqueProcessId;
////} PROCESS_BASIC_INFORMATION;
////typedef PROCESS_BASIC_INFORMATION* PPROCESS_BASIC_INFORMATION;
//
//#define DUPLICATE_SAME_ATTRIBUTES   0x00000004
//#define SystemHandleInformation 16
//#define ObjectNameInformation 1
//
//extern "C"
//{
//	typedef struct _SYSTEM_HANDLE_TABLE_ENTRY_INFO {
//		USHORT UniqueProcessId;
//		USHORT CreatorBackTraceIndex;
//		UCHAR ObjectTypeIndex;
//		UCHAR HandleAttributes;
//		USHORT HandleValue;
//		PVOID Object;
//		ULONG GrantedAccess;
//	} SYSTEM_HANDLE_TABLE_ENTRY_INFO, * PSYSTEM_HANDLE_TABLE_ENTRY_INFO;
//
//	typedef struct _SYSTEM_HANDLE_INFORMATION {
//		ULONG NumberOfHandles;
//		SYSTEM_HANDLE_TABLE_ENTRY_INFO Handles[1];
//	} SYSTEM_HANDLE_INFORMATION, * PSYSTEM_HANDLE_INFORMATION;
//
//
//	typedef struct _OBJECT_NAME_INFORMATION {
//		UNICODE_STRING Name;
//	} OBJECT_NAME_INFORMATION, * POBJECT_NAME_INFORMATION;
//
//	typedef struct _OBJECT_TYPE_INFORMATION {
//		UNICODE_STRING TypeName;
//		ULONG TotalNumberOfObjects;
//		ULONG TotalNumberOfHandles;
//		ULONG TotalPagedPoolUsage;
//		ULONG TotalNonPagedPoolUsage;
//		ULONG TotalNamePoolUsage;
//		ULONG TotalHandleTableUsage;
//		ULONG HighWaterNumberOfObjects;
//		ULONG HighWaterNumberOfHandles;
//		ULONG HighWaterPagedPoolUsage;
//		ULONG HighWaterNonPagedPoolUsage;
//		ULONG HighWaterNamePoolUsage;
//		ULONG HighWaterHandleTableUsage;
//		ULONG InvalidAttributes;
//		GENERIC_MAPPING GenericMapping;
//		ULONG ValidAccessMask;
//		BOOLEAN SecurityRequired;
//		BOOLEAN MaintainHandleCount;
//		ULONG PoolType;
//		ULONG DefaultPagedPoolCharge;
//		ULONG DefaultNonPagedPoolCharge;
//	} OBJECT_TYPE_INFORMATION, * POBJECT_TYPE_INFORMATION;
//
//	typedef struct _OBJECT_ATTRIBUTES {
//		ULONG Length;
//		HANDLE RootDirectory;
//		PUNICODE_STRING ObjectName;
//		ULONG Attributes;
//		PVOID SecurityDescriptor;
//		PVOID SecurityQualityOfService;
//	} OBJECT_ATTRIBUTES;
//	typedef OBJECT_ATTRIBUTES* POBJECT_ATTRIBUTES;
//
//	NTSTATUS
//		NTAPI
//		ZwQuerySystemInformation(
//			__in LONG SystemInformationClass,
//			__out_bcount_opt(SystemInformationLength) PVOID SystemInformation,
//			__in ULONG SystemInformationLength,
//			__out_opt PULONG ReturnLength
//		);
//
//	NTSTATUS
//		NTAPI
//		RtlAdjustPrivilege(
//			ULONG Privilege,
//			BOOLEAN Enable,
//			BOOLEAN Client,
//			PBOOLEAN WasEnabled
//		);
//
//	NTSTATUS
//		NTAPI
//		ZwOpenProcess(
//			__out PHANDLE ProcessHandle,
//			__in ACCESS_MASK DesiredAccess,
//			__in POBJECT_ATTRIBUTES ObjectAttributes,
//			__in_opt PCLIENT_ID ClientId
//		);
//
//	NTSTATUS
//		NTAPI
//		ZwDuplicateObject(
//			__in HANDLE SourceProcessHandle,
//			__in HANDLE SourceHandle,
//			__in_opt HANDLE TargetProcessHandle,
//			__out_opt PHANDLE TargetHandle,
//			__in ACCESS_MASK DesiredAccess,
//			__in ULONG HandleAttributes,
//			__in ULONG Options
//		);
//
//	NTSTATUS
//		NTAPI
//		ZwQueryObject(
//			__in HANDLE Handle,
//			__in LONG ObjectInformationClass,
//			__out_bcount_opt(ObjectInformationLength) PVOID ObjectInformation,
//			__in ULONG ObjectInformationLength,
//			__out_opt PULONG ReturnLength
//		);
//};
//
//typedef enum _OBJECT_INFORMATION_CLASS {
//	ObjectBasicInformation = 0,
//	ObjectTypeInformation = 2
//} OBJECT_INFORMATION_CLASS;
//
//#ifndef InitializeObjectAttributes
//#define InitializeObjectAttributes( p, n, a, r, s ) { \
//	(p)->Length = sizeof(OBJECT_ATTRIBUTES);          \
//	(p)->RootDirectory = r;                             \
//	(p)->Attributes = a;                                \
//	(p)->ObjectName = n;                                \
//	(p)->SecurityDescriptor = s;                        \
//	(p)->SecurityQualityOfService = NULL;               \
//}
//#endif
//
//typedef struct _LDR_DATA_TABLE_ENTRY_EX_
//{
//	LIST_ENTRY InLoadOrderLinks;
//	LIST_ENTRY InMemoryOrderModuleList;
//	LIST_ENTRY InInitializationOrderModuleList;
//	PVOID DllBase;
//	PVOID EntryPoint;
//	ULONG SizeOfImage;
//	UNICODE_STRING FullDllName;
//	UNICODE_STRING BaseDllName;
//	ULONG Flags;
//	USHORT LoadCount;
//	USHORT TlsIndex;
//	union
//	{
//		LIST_ENTRY HashLinks;
//		PVOID SectionPointer;
//	};
//	ULONG CheckSum;
//	union
//	{
//		ULONG TimeDateStamp;
//		PVOID LoadedImports;
//	};
//	PVOID EntryPointActivationContext;
//	PVOID PatchInformation;
//} LDR_DATA_TABLE_ENTRYEX, * PLDR_DATA_TABLE_ENTRYEX;
//
//typedef struct _PEB_LDR_DATA_EX_
//{
//	ULONG Length;
//	BOOLEAN Initialized;
//	PVOID SsHandle;
//	LIST_ENTRY InLoadOrderModuleList;
//	LIST_ENTRY InMemoryOrderModuleList;
//	LIST_ENTRY InInitializationOrderModuleList;
//} PEB_LDR_DATAEX, * PPEB_LDR_DATAEX;
//
//typedef struct _PEB_EX_
//{
//	UCHAR InheritedAddressSpace;                     // 00h
//	UCHAR ReadImageFileExecOptions;                  // 01h
//	UCHAR BeingDebugged;                             // 02h
//	UCHAR Spare;                                     // 03h
//	PVOID Mutant;                                    // 04h
//	PVOID ImageBaseAddress;                          // 08h
//	PPEB_LDR_DATAEX Ldr;                             // 0Ch
//} PEBEX, * PPEBEX;



#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)
#define STATUS_SUCCESS     ((NTSTATUS) 0x00000000)

typedef NTSTATUS(WINAPI* PfnZwQueryInformationProcess)(HANDLE ProcessHandle,
	ULONG ProcessInformationClass,
	PVOID ProcessInformation,
	ULONG ProcessInformationLength,
	PULONG ReturnLength);