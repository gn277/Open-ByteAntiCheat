#include "BACServer/BACServer.h"

std::shared_ptr<BACServer> server = nullptr;


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpcmdLine, _In_ int nCmdShow)
{
	HACCEL acc_table = LoadAccelerators(hInstance, MAKEINTRESOURCE(NULL));

	//实例化BACServer
	server = std::make_shared<BACServer>();
	//开启TCP服务
	SERVER_ERROR status = server->StartTcpServer(L"0.0.0.0", 5999);
	if (status != SERVER_SUCCESS)
	{
		::MessageBoxA(::GetActiveWindow(), "Start tcp server error", "BACServer error", MB_OK);
		return 0;
	}

	//主消息循环:
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, acc_table, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//处理业务
		{

		}
	}

	//处理退出事件
	server.~shared_ptr();

	return (int)msg.wParam;
}