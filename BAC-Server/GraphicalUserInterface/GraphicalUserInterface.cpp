#include "GraphicalUserInterface.h"

std::shared_ptr<BACServer> server = nullptr;


template<typename T1, typename T2>
void OutPutLog(T1 title, T2 data)
{
#if _PRINT_INFORMATION
	std::cout << "[BACServer]-> " << title << " : " << data << std::endl;
#else
	return;
#endif
}


GraphicalUserInterfaceError::GraphicalUserInterfaceError(const char* error) :_error_str(error)
{
}

GraphicalUserInterfaceError::GraphicalUserInterfaceError(std::string error) :_error_str(error)
{
}

GraphicalUserInterfaceError::~GraphicalUserInterfaceError()
{
}

char const* GraphicalUserInterfaceError::what() const throw()
{
	return this->_error_str.c_str();
}


GraphicalUserInterface::GraphicalUserInterface()
{
}

GraphicalUserInterface::~GraphicalUserInterface()
{
}

SERVER_ERROR GraphicalUserInterface::Running(_In_ HINSTANCE instance, _In_opt_ HINSTANCE prev_instance, _In_ LPSTR cmd_line, _In_ int cmd_show)
{
	SERVER_ERROR status = SERVER_SUCCESS;
	HACCEL acc_table = LoadAccelerators(instance, MAKEINTRESOURCE(NULL));

	//实例化BACServer
	server = std::make_shared<BACServer>();
	//开启TCP服务
	status = server->StartTcpServer(L"0.0.0.0", 5999);
	if (status != SERVER_SUCCESS)
	{
		OutPutLog(__FUNCTION__, "Start tcp server error");
		return SERVER_START_TCP_ERROR;
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

	return status;
}

