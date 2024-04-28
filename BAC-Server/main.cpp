#include "BACServer/BACServer.h"

std::shared_ptr<BACServer> server = nullptr;


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpcmdLine, _In_ int nCmdShow)
{
	HACCEL acc_table = LoadAccelerators(hInstance, MAKEINTRESOURCE(NULL));

	//ʵ����BACServer
	server = std::make_shared<BACServer>();
	//����TCP����


	//����Ϣѭ��:
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, acc_table, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//����ҵ��
		{

		}
	}

	//�����˳��¼�
	server.~shared_ptr();

	return (int)msg.wParam;
}