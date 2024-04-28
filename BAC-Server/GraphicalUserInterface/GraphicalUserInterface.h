#pragma once
#include <stdio.h>
#include <iostream>
#include <string>

#include "../BACServer/BACServer.h"


class GraphicalUserInterfaceError :public std::exception
{
private:
	std::string _error_str;

public:
	GraphicalUserInterfaceError(const char* error);
	GraphicalUserInterfaceError(std::string error);
	~GraphicalUserInterfaceError();

public:
	virtual char const* what() const throw();

};


class GraphicalUserInterface
{
private:
	HWND _self_window_handle = NULL;

public:
	GraphicalUserInterface();
	~GraphicalUserInterface();

public:
	SERVER_ERROR Running(_In_ HINSTANCE instance, _In_opt_ HINSTANCE prev_instance, _In_ LPSTR cmd_line, _In_ int cmd_show);
};

