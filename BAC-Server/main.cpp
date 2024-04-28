#include "GraphicalUserInterface/GraphicalUserInterface.h"

std::shared_ptr<GraphicalUserInterface> gui = nullptr;


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpcmdLine, _In_ int nCmdShow)
{
	//ʵ����GraphicalUserInterface
	gui = std::make_shared<GraphicalUserInterface>();

	SERVER_ERROR status = gui->Running(hInstance, hPrevInstance, lpcmdLine, nCmdShow);
	if (status != SERVER_SUCCESS)
		::MessageBoxA(::GetActiveWindow(), std::string("gui run error status:" + status).c_str(), "BACServer Error:", MB_OK);

	//�����˳��¼�
	gui.~shared_ptr();

	return 1;
}