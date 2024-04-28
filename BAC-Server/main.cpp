#include "GraphicalUserInterface/GraphicalUserInterface.h"

std::shared_ptr<GraphicalUserInterface> gui = nullptr;


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpcmdLine, _In_ int nCmdShow)
{
	//实例化GraphicalUserInterface
	gui = std::make_shared<GraphicalUserInterface>();

	SERVER_ERROR status = gui->Running(hInstance, hPrevInstance, lpcmdLine, nCmdShow);
	if (status != SERVER_SUCCESS)
		::MessageBoxA(::GetActiveWindow(), std::string("gui run error status:" + status).c_str(), "BACServer Error:", MB_OK);

	//处理退出事件
	gui.~shared_ptr();

	return 1;
}