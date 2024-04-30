#pragma once
#include <stdio.h>
#include <iostream>
#include <string>

#include <HPSocket/HPSocket.h>
#include <cJSON.h>

#include "BACServerError.h"
#include "BACProtocol/BACProtocol.h"
#include "../../BAC-Base/BACTools/BACTools.h"

typedef struct _ClientNode
{
	ITcpServer* server;
	CONNID connect_id;

	bool login = false;							//是否登录成功
	std::string user_id;						//用户id

	//systime login_time;
	//systime online_time;
}ClientNode, * PClientNode;


class BACServerError :public std::exception
{
private:
	std::string _error_str;

public:
	BACServerError(const char* error);
	BACServerError(std::string error);
	~BACServerError();

public:
	virtual char const* what() const throw();

};


class BACServer :public CTcpServerListener, public CUdpNodeListener, public BACTools
{
private:
	CTcpServerPtr _tcp;
	CUdpNodePtr _udp;

	//已连接客户端表
	std::map<CONNID, PClientNode> _clients;

public:
	BACServer();
	~BACServer();

public:
	//TCP服务处理回调函数
	virtual EnHandleResult OnPrepareListen(ITcpServer* pSender, SOCKET soListen);
	virtual EnHandleResult OnAccept(ITcpServer* pSender, CONNID dwConnID, UINT_PTR soClient);
	virtual EnHandleResult OnHandShake(ITcpServer* pSender, CONNID dwConnID);
	virtual EnHandleResult OnReceive(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength);
	virtual EnHandleResult OnSend(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength);
	virtual EnHandleResult OnClose(ITcpServer* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode);
	virtual EnHandleResult OnShutdown(ITcpServer* pSender);

public:
	CTcpServerPtr* GetTcpServerPtr();

public:
	//UDP服务处理回调函数
	virtual EnHandleResult OnPrepareListen(IUdpNode* pSender, SOCKET soListen);
	virtual EnHandleResult OnSend(IUdpNode* pSender, LPCTSTR lpszRemoteAddress, USHORT usRemotePort, const BYTE* pData, int iLength);
	virtual EnHandleResult OnReceive(IUdpNode* pSender, LPCTSTR lpszRemoteAddress, USHORT usRemotePort, const BYTE* pData, int iLength);
	virtual EnHandleResult OnError(IUdpNode* pSender, EnSocketOperation enOperation, int iErrorCode, LPCTSTR lpszRemoteAddress, USHORT usRemotePort, const BYTE* pBuffer, int iLength);
	virtual EnHandleResult OnShutdown(IUdpNode* pSender);

public:
	CUdpNodePtr* GetUdpNodePtr();

	//TCP业务处理函数：
public:
	auto GetClients() -> std::map<CONNID, PClientNode>;

private:
	//处理错误命令
	auto OnCommandError(PClientNode p_client, std::string buffer) -> void;
	//处理状态错误
	auto OnStatusError(PClientNode p_client, std::string buffer) -> void;
	//返回响应状态
	auto ResponseStatus(PClientNode p_client, BACCommand command, BACStatus status = BACStatus::Success) -> bool;
	//处理客户端登录
	auto ClientLogin(PClientNode p_client, std::string buffer) -> void;
	//获取客户端机器信息
	auto GetClientMachineInfo(PClientNode p_client, std::string buffer) -> void;
	//获取客户端异常游戏数据
	auto GetClientAbnormalGameData(PClientNode p_client, std::string buffer) -> void;
	//TCP消息派遣函数
	auto DispatchTcpMessage(PClientNode p_client, std::string buffer) -> void;

	//UDP业务处理函数：
private:

public:

public:
	SERVER_ERROR StartTcpServer(const wchar_t* bind_address, unsigned short port);
	SERVER_ERROR StartUdpServer();

};

