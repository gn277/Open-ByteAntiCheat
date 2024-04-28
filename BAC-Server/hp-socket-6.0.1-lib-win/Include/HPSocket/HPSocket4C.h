﻿/*
 * Copyright: JessMA Open Source (ldcsaa@gmail.com)
 *
 * Author	: Bruce Liang
 * Website	: https://github.com/ldcsaa
 * Project	: https://github.com/ldcsaa/HP-Socket
 * Blog		: http://www.cnblogs.com/ldcsaa
 * Wiki		: http://www.oschina.net/p/hp-socket
 * QQ Group	: 44636872, 75375912
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/******************************************************************************
Module:  HPSocket for C

Desc: 导出纯 C 函数，让其它语言（如：C / C# / Delphi 等）能方便地使用 HPSocket

Usage:
		方法一：
		--------------------------------------------------------------------------------------
		0. （C/C++ 程序）包含 HPTypeDef.h / HPSocket4C.h 头文件
		1. 调用 ::Create_HP_XxxListener() 函数创建监听器对象
		2. 调用 ::Create_HP_Xxx(pListener) 函数创建 HPSocket 对象
		3. 调用 ::HP_Set_FN_Xxx_OnYyy(pListener, ...) 函数设置监听器的回调函数
		4. 调用相关导出函数操作 HPSocket 对象
		5. ...... ......
		6. 调用 ::Destroy_HP_Xxx(pSocket) 函数销毁 HPSocket 对象
		7. 调用 ::Destroy_HP_XxxListener(pListener) 函数销毁监听器对象

		方法二：
		--------------------------------------------------------------------------------------
		1. 应用程序把需要用到的导出函数封装到特定语言的包装类中
		2. 通过包装类封装后，以面向对象的方式使用 HPSocket

Release:
		<-- 动态链接库 -->
		1. x86/HPSocket4C.dll			- (32位/MBCS/Release)
		2. x86/HPSocket4C_D.dll			- (32位/MBCS/DeBug)
		3. x86/HPSocket4C_U.dll			- (32位/UNICODE/Release)
		4. x86/HPSocket4C_UD.dll		- (32位/UNICODE/DeBug)
		5. x64/HPSocket4C.dll			- (64位/MBCS/Release)
		6. x64/HPSocket4C_D.dll			- (64位/MBCS/DeBug)
		7. x64/HPSocket4C_U.dll			- (64位/UNICODE/Release)
		8. x64/HPSocket4C_UD.dll		- (64位/UNICODE/DeBug)

		<-- 静态链接库 -->
		!!注意!!：使用 HPSocket 静态库时，需要在工程属性中定义预处理宏 -> HPSOCKET_STATIC_LIB
		1. x86/static/HPSocket4C.lib	- (32位/MBCS/Release)
		2. x86/static/HPSocket4C_D.lib	- (32位/MBCS/DeBug)
		3. x86/static/HPSocket4C_U.lib	- (32位/UNICODE/Release)
		4. x86/static/HPSocket4C_UD.lib	- (32位/UNICODE/DeBug)
		5. x64/static/HPSocket4C.lib	- (64位/MBCS/Release)
		6. x64/static/HPSocket4C_D.lib	- (64位/MBCS/DeBug)
		7. x64/static/HPSocket4C_U.lib	- (64位/UNICODE/Release)
		8. x64/static/HPSocket4C_UD.lib	- (64位/UNICODE/DeBug)

******************************************************************************/

#pragma once

#include <winsock2.h>

#include "HPTypeDef.h"

/************************************************************************
名称：定义 Socket 对象指针类型别名
描述：把 Socket 对象指针定义为更直观的别名
************************************************************************/

typedef PVOID		HP_Object;

typedef HP_Object	HP_Server;
typedef HP_Object	HP_Agent;
typedef HP_Object	HP_Client;
typedef HP_Object	HP_Node;
typedef HP_Object	HP_TcpServer;
typedef HP_Object	HP_TcpAgent;
typedef HP_Object	HP_TcpClient;
typedef HP_Object	HP_PullSocket;
typedef HP_Object	HP_PullClient;
typedef HP_Object	HP_TcpPullServer;
typedef HP_Object	HP_TcpPullAgent;
typedef HP_Object	HP_TcpPullClient;
typedef HP_Object	HP_PackSocket;
typedef HP_Object	HP_PackClient;
typedef HP_Object	HP_TcpPackServer;
typedef HP_Object	HP_TcpPackAgent;
typedef HP_Object	HP_TcpPackClient;
typedef HP_Object	HP_ThreadPool;
typedef HP_Object	HP_Compressor;
typedef HP_Object	HP_Decompressor;

typedef HP_Object	HP_Listener;
typedef HP_Object	HP_ServerListener;
typedef HP_Object	HP_AgentListener;
typedef HP_Object	HP_ClientListener;
typedef HP_Object	HP_TcpServerListener;
typedef HP_Object	HP_TcpAgentListener;
typedef HP_Object	HP_TcpClientListener;
typedef HP_Object	HP_PullSocketListener;
typedef HP_Object	HP_PullClientListener;
typedef HP_Object	HP_TcpPullServerListener;
typedef HP_Object	HP_TcpPullAgentListener;
typedef HP_Object	HP_TcpPullClientListener;
typedef HP_Object	HP_PackSocketListener;
typedef HP_Object	HP_PackClientListener;
typedef HP_Object	HP_TcpPackServerListener;
typedef HP_Object	HP_TcpPackAgentListener;
typedef HP_Object	HP_TcpPackClientListener;
typedef HP_Object	HP_ThreadPoolListener;

#ifdef _UDP_SUPPORT

typedef HP_Object	HP_UdpServer;
typedef HP_Object	HP_UdpClient;
typedef HP_Object	HP_UdpCast;
typedef HP_Object	HP_UdpNode;
typedef HP_Object	HP_UdpArqServer;
typedef HP_Object	HP_UdpArqClient;

typedef HP_Object	HP_UdpServerListener;
typedef HP_Object	HP_UdpClientListener;
typedef HP_Object	HP_UdpCastListener;
typedef HP_Object	HP_UdpNodeListener;
typedef HP_Object	HP_UdpArqServerListener;
typedef HP_Object	HP_UdpArqClientListener;

#endif

#ifdef _HTTP_SUPPORT

typedef HP_Object	HP_Http;
typedef HP_Object	HP_HttpServer;
typedef HP_Object	HP_HttpAgent;
typedef HP_Object	HP_HttpClient;
typedef HP_Object	HP_HttpSyncClient;

typedef HP_Object	HP_HttpServerListener;
typedef HP_Object	HP_HttpAgentListener;
typedef HP_Object	HP_HttpClientListener;

#endif

/*****************************************************************************************************************************************************/
/****************************************************************** TCP/UDP Exports ******************************************************************/
/*****************************************************************************************************************************************************/

/****************************************************/
/***************** TCP/UDP 回调函数 ******************/

/* Server 回调函数 */
typedef En_HP_HandleResult (__HP_CALL *HP_FN_Server_OnPrepareListen)	(HP_Server pSender, UINT_PTR soListen);
// 如果为 TCP 连接，pClient为 SOCKET 句柄；如果为 UDP 连接，pClient为 SOCKADDR 指针；
typedef En_HP_HandleResult (__HP_CALL *HP_FN_Server_OnAccept)			(HP_Server pSender, HP_CONNID dwConnID, UINT_PTR pClient);
typedef En_HP_HandleResult (__HP_CALL *HP_FN_Server_OnHandShake)		(HP_Server pSender, HP_CONNID dwConnID);
typedef En_HP_HandleResult (__HP_CALL *HP_FN_Server_OnSend)				(HP_Server pSender, HP_CONNID dwConnID, const BYTE* pData, int iLength);
typedef En_HP_HandleResult (__HP_CALL *HP_FN_Server_OnReceive)			(HP_Server pSender, HP_CONNID dwConnID, const BYTE* pData, int iLength);
typedef En_HP_HandleResult (__HP_CALL *HP_FN_Server_OnPullReceive)		(HP_Server pSender, HP_CONNID dwConnID, int iLength);
typedef En_HP_HandleResult (__HP_CALL *HP_FN_Server_OnClose)			(HP_Server pSender, HP_CONNID dwConnID, En_HP_SocketOperation enOperation, int iErrorCode);
typedef En_HP_HandleResult (__HP_CALL *HP_FN_Server_OnShutdown)			(HP_Server pSender);

/* Agent 回调函数 */
typedef En_HP_HandleResult (__HP_CALL *HP_FN_Agent_OnPrepareConnect)	(HP_Agent pSender, HP_CONNID dwConnID, UINT_PTR socket);
typedef En_HP_HandleResult (__HP_CALL *HP_FN_Agent_OnConnect)			(HP_Agent pSender, HP_CONNID dwConnID);
typedef En_HP_HandleResult (__HP_CALL *HP_FN_Agent_OnHandShake)			(HP_Agent pSender, HP_CONNID dwConnID);
typedef En_HP_HandleResult (__HP_CALL *HP_FN_Agent_OnSend)				(HP_Agent pSender, HP_CONNID dwConnID, const BYTE* pData, int iLength);
typedef En_HP_HandleResult (__HP_CALL *HP_FN_Agent_OnReceive)			(HP_Agent pSender, HP_CONNID dwConnID, const BYTE* pData, int iLength);
typedef En_HP_HandleResult (__HP_CALL *HP_FN_Agent_OnPullReceive)		(HP_Agent pSender, HP_CONNID dwConnID, int iLength);
typedef En_HP_HandleResult (__HP_CALL *HP_FN_Agent_OnClose)				(HP_Agent pSender, HP_CONNID dwConnID, En_HP_SocketOperation enOperation, int iErrorCode);
typedef En_HP_HandleResult (__HP_CALL *HP_FN_Agent_OnShutdown)			(HP_Agent pSender);

/* Client 回调函数 */
typedef En_HP_HandleResult (__HP_CALL *HP_FN_Client_OnPrepareConnect)	(HP_Client pSender, HP_CONNID dwConnID, UINT_PTR socket);
typedef En_HP_HandleResult (__HP_CALL *HP_FN_Client_OnConnect)			(HP_Client pSender, HP_CONNID dwConnID);
typedef En_HP_HandleResult (__HP_CALL *HP_FN_Client_OnHandShake)		(HP_Client pSender, HP_CONNID dwConnID);
typedef En_HP_HandleResult (__HP_CALL *HP_FN_Client_OnSend)				(HP_Client pSender, HP_CONNID dwConnID, const BYTE* pData, int iLength);
typedef En_HP_HandleResult (__HP_CALL *HP_FN_Client_OnReceive)			(HP_Client pSender, HP_CONNID dwConnID, const BYTE* pData, int iLength);
typedef En_HP_HandleResult (__HP_CALL *HP_FN_Client_OnPullReceive)		(HP_Client pSender, HP_CONNID dwConnID, int iLength);
typedef En_HP_HandleResult (__HP_CALL *HP_FN_Client_OnClose)			(HP_Client pSender, HP_CONNID dwConnID, En_HP_SocketOperation enOperation, int iErrorCode);

#ifdef _UDP_SUPPORT

/* UdpNode 回调函数 */
typedef En_HP_HandleResult (__HP_CALL *HP_FN_UdpNode_OnPrepareListen)	(HP_UdpNode pSender, UINT_PTR soListen);
typedef En_HP_HandleResult (__HP_CALL *HP_FN_UdpNode_OnSend)			(HP_UdpNode pSender, LPCTSTR lpszRemoteAddress, USHORT usRemotePort, const BYTE* pData, int iLength);
typedef En_HP_HandleResult (__HP_CALL *HP_FN_UdpNode_OnReceive)			(HP_UdpNode pSender, LPCTSTR lpszRemoteAddress, USHORT usRemotePort, const BYTE* pData, int iLength);
typedef En_HP_HandleResult (__HP_CALL *HP_FN_UdpNode_OnError)			(HP_UdpNode pSender, En_HP_SocketOperation enOperation, int iErrorCode, LPCTSTR lpszRemoteAddress, USHORT usRemotePort, const BYTE* pData, int iLength);
typedef En_HP_HandleResult (__HP_CALL *HP_FN_UdpNode_OnShutdown)		(HP_UdpNode pSender);

#endif

/****************************************************/
/*************** TCP/UDP 对象创建函数 ****************/

// 创建 HP_TcpServer 对象
HPSOCKET_API HP_TcpServer __HP_CALL Create_HP_TcpServer(HP_TcpServerListener pListener);
// 创建 HP_TcpAgent 对象
HPSOCKET_API HP_TcpAgent __HP_CALL Create_HP_TcpAgent(HP_TcpAgentListener pListener);
// 创建 HP_TcpClient 对象
HPSOCKET_API HP_TcpClient __HP_CALL Create_HP_TcpClient(HP_TcpClientListener pListener);
// 创建 HP_TcpPullServer 对象
HPSOCKET_API HP_TcpPullServer __HP_CALL Create_HP_TcpPullServer(HP_TcpPullServerListener pListener);
// 创建 HP_TcpPullAgent 对象
HPSOCKET_API HP_TcpPullAgent __HP_CALL Create_HP_TcpPullAgent(HP_TcpPullAgentListener pListener);
// 创建 HP_TcpPullClient 对象
HPSOCKET_API HP_TcpPullClient __HP_CALL Create_HP_TcpPullClient(HP_TcpPullClientListener pListener);
// 创建 HP_TcpPackServer 对象
HPSOCKET_API HP_TcpPackServer __HP_CALL Create_HP_TcpPackServer(HP_TcpServerListener pListener);
// 创建 HP_TcpPackAgent 对象
HPSOCKET_API HP_TcpPackAgent __HP_CALL Create_HP_TcpPackAgent(HP_TcpAgentListener pListener);
// 创建 HP_TcpPackClient 对象
HPSOCKET_API HP_TcpPackClient __HP_CALL Create_HP_TcpPackClient(HP_TcpClientListener pListener);

// 销毁 HP_TcpServer 对象
HPSOCKET_API void __HP_CALL Destroy_HP_TcpServer(HP_TcpServer pServer);
// 销毁 HP_TcpAgent 对象
HPSOCKET_API void __HP_CALL Destroy_HP_TcpAgent(HP_TcpAgent pAgent);
// 销毁 HP_TcpClient 对象
HPSOCKET_API void __HP_CALL Destroy_HP_TcpClient(HP_TcpClient pClient);
// 销毁 HP_TcpPullServer 对象
HPSOCKET_API void __HP_CALL Destroy_HP_TcpPullServer(HP_TcpPullServer pServer);
// 销毁 HP_TcpPullAgent 对象
HPSOCKET_API void __HP_CALL Destroy_HP_TcpPullAgent(HP_TcpPullAgent pAgent);
// 销毁 HP_TcpPullClient 对象
HPSOCKET_API void __HP_CALL Destroy_HP_TcpPullClient(HP_TcpPullClient pClient);
// 销毁 HP_TcpPackServer 对象
HPSOCKET_API void __HP_CALL Destroy_HP_TcpPackServer(HP_TcpPackServer pServer);
// 销毁 HP_TcpPackAgent 对象
HPSOCKET_API void __HP_CALL Destroy_HP_TcpPackAgent(HP_TcpPackAgent pAgent);
// 销毁 HP_TcpPackClient 对象
HPSOCKET_API void __HP_CALL Destroy_HP_TcpPackClient(HP_TcpPackClient pClient);

// 创建 HP_TcpServerListener 对象
HPSOCKET_API HP_TcpServerListener __HP_CALL Create_HP_TcpServerListener();
// 创建 HP_TcpAgentListener 对象
HPSOCKET_API HP_TcpAgentListener __HP_CALL Create_HP_TcpAgentListener();
// 创建 HP_TcpClientListener 对象
HPSOCKET_API HP_TcpClientListener __HP_CALL Create_HP_TcpClientListener();
// 创建 HP_TcpPullServerListener 对象
HPSOCKET_API HP_TcpPullServerListener __HP_CALL Create_HP_TcpPullServerListener();
// 创建 HP_TcpPullAgentListener 对象
HPSOCKET_API HP_TcpPullAgentListener __HP_CALL Create_HP_TcpPullAgentListener();
// 创建 HP_TcpPullClientListener 对象
HPSOCKET_API HP_TcpPullClientListener __HP_CALL Create_HP_TcpPullClientListener();
// 创建 HP_TcpPackServerListener 对象
HPSOCKET_API HP_TcpPackServerListener __HP_CALL Create_HP_TcpPackServerListener();
// 创建 HP_TcpPackAgentListener 对象
HPSOCKET_API HP_TcpPackAgentListener __HP_CALL Create_HP_TcpPackAgentListener();
// 创建 HP_TcpPackClientListener 对象
HPSOCKET_API HP_TcpPackClientListener __HP_CALL Create_HP_TcpPackClientListener();

// 销毁 HP_TcpServerListener 对象
HPSOCKET_API void __HP_CALL Destroy_HP_TcpServerListener(HP_TcpServerListener pListener);
// 销毁 HP_TcpAgentListener 对象
HPSOCKET_API void __HP_CALL Destroy_HP_TcpAgentListener(HP_TcpAgentListener pListener);
// 销毁 HP_TcpClientListener 对象
HPSOCKET_API void __HP_CALL Destroy_HP_TcpClientListener(HP_TcpClientListener pListener);
// 销毁 HP_TcpPullServerListener 对象
HPSOCKET_API void __HP_CALL Destroy_HP_TcpPullServerListener(HP_TcpPullServerListener pListener);
// 销毁 HP_TcpPullAgentListener 对象
HPSOCKET_API void __HP_CALL Destroy_HP_TcpPullAgentListener(HP_TcpPullAgentListener pListener);
// 销毁 HP_TcpPullClientListener 对象
HPSOCKET_API void __HP_CALL Destroy_HP_TcpPullClientListener(HP_TcpPullClientListener pListener);
// 销毁 HP_TcpPackServerListener 对象
HPSOCKET_API void __HP_CALL Destroy_HP_TcpPackServerListener(HP_TcpPackServerListener pListener);
// 销毁 HP_TcpPackAgentListener 对象
HPSOCKET_API void __HP_CALL Destroy_HP_TcpPackAgentListener(HP_TcpPackAgentListener pListener);
// 销毁 HP_TcpPackClientListener 对象
HPSOCKET_API void __HP_CALL Destroy_HP_TcpPackClientListener(HP_TcpPackClientListener pListener);

#ifdef _UDP_SUPPORT

// 创建 HP_UdpServer 对象
HPSOCKET_API HP_UdpServer __HP_CALL Create_HP_UdpServer(HP_UdpServerListener pListener);
// 创建 HP_UdpClient 对象
HPSOCKET_API HP_UdpClient __HP_CALL Create_HP_UdpClient(HP_UdpClientListener pListener);
// 创建 HP_UdpCast 对象
HPSOCKET_API HP_UdpCast __HP_CALL Create_HP_UdpCast(HP_UdpCastListener pListener);
// 创建 HP_UdpNode 对象
HPSOCKET_API HP_UdpNode __HP_CALL Create_HP_UdpNode(HP_UdpNodeListener pListener);
// 创建 HP_UdpArqServer 对象
HPSOCKET_API HP_UdpArqServer __HP_CALL Create_HP_UdpArqServer(HP_UdpServerListener pListener);
// 创建 HP_UdpArqClient 对象
HPSOCKET_API HP_UdpArqClient __HP_CALL Create_HP_UdpArqClient(HP_UdpClientListener pListener);

// 销毁 HP_UdpServer 对象
HPSOCKET_API void __HP_CALL Destroy_HP_UdpServer(HP_UdpServer pServer);
// 销毁 HP_UdpClient 对象
HPSOCKET_API void __HP_CALL Destroy_HP_UdpClient(HP_UdpClient pClient);
// 销毁 HP_UdpCast 对象
HPSOCKET_API void __HP_CALL Destroy_HP_UdpCast(HP_UdpCast pCast);
// 销毁 HP_UdpNode 对象
HPSOCKET_API void __HP_CALL Destroy_HP_UdpNode(HP_UdpNode pNode);
// 销毁 HP_UdpArqServer 对象
HPSOCKET_API void __HP_CALL Destroy_HP_UdpArqServer(HP_UdpArqServer pServer);
// 销毁 HP_UdpArqClient 对象
HPSOCKET_API void __HP_CALL Destroy_HP_UdpArqClient(HP_UdpArqClient pClient);

// 创建 HP_UdpServerListener 对象
HPSOCKET_API HP_UdpServerListener __HP_CALL Create_HP_UdpServerListener();
// 创建 HP_UdpClientListener 对象
HPSOCKET_API HP_UdpClientListener __HP_CALL Create_HP_UdpClientListener();
// 创建 HP_UdpCastListener 对象
HPSOCKET_API HP_UdpCastListener __HP_CALL Create_HP_UdpCastListener();
// 创建 HP_UdpNodeListener 对象
HPSOCKET_API HP_UdpNodeListener __HP_CALL Create_HP_UdpNodeListener();
// 创建 HP_UdpArqServerListener 对象
HPSOCKET_API HP_UdpArqServerListener __HP_CALL Create_HP_UdpArqServerListener();
// 创建 HP_UdpArqClientListener 对象
HPSOCKET_API HP_UdpArqClientListener __HP_CALL Create_HP_UdpArqClientListener();

// 销毁 HP_UdpServerListener 对象
HPSOCKET_API void __HP_CALL Destroy_HP_UdpServerListener(HP_UdpServerListener pListener);
// 销毁 HP_UdpClientListener 对象
HPSOCKET_API void __HP_CALL Destroy_HP_UdpClientListener(HP_UdpClientListener pListener);
// 销毁 HP_UdpCastListener 对象
HPSOCKET_API void __HP_CALL Destroy_HP_UdpCastListener(HP_UdpCastListener pListener);
// 销毁 HP_UdpNodeListener 对象
HPSOCKET_API void __HP_CALL Destroy_HP_UdpNodeListener(HP_UdpNodeListener pListener);
// 销毁 HP_UdpArqServerListener 对象
HPSOCKET_API void __HP_CALL Destroy_HP_UdpArqServerListener(HP_UdpArqServerListener pListener);
// 销毁 HP_UdpArqClientListener 对象
HPSOCKET_API void __HP_CALL Destroy_HP_UdpArqClientListener(HP_UdpArqClientListener pListener);

#endif

/**********************************************************************************/
/***************************** Server 回调函数设置方法 *****************************/

HPSOCKET_API void __HP_CALL HP_Set_FN_Server_OnPrepareListen(HP_ServerListener pListener	, HP_FN_Server_OnPrepareListen fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_Server_OnAccept(HP_ServerListener pListener			, HP_FN_Server_OnAccept fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_Server_OnHandShake(HP_ServerListener pListener		, HP_FN_Server_OnHandShake fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_Server_OnSend(HP_ServerListener pListener				, HP_FN_Server_OnSend fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_Server_OnReceive(HP_ServerListener pListener			, HP_FN_Server_OnReceive fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_Server_OnPullReceive(HP_ServerListener pListener		, HP_FN_Server_OnPullReceive fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_Server_OnClose(HP_ServerListener pListener			, HP_FN_Server_OnClose fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_Server_OnShutdown(HP_ServerListener pListener			, HP_FN_Server_OnShutdown fn);

/**********************************************************************************/
/****************************** Agent 回调函数设置方法 *****************************/

HPSOCKET_API void __HP_CALL HP_Set_FN_Agent_OnPrepareConnect(HP_AgentListener pListener		, HP_FN_Agent_OnPrepareConnect fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_Agent_OnConnect(HP_AgentListener pListener			, HP_FN_Agent_OnConnect fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_Agent_OnHandShake(HP_AgentListener pListener			, HP_FN_Agent_OnHandShake fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_Agent_OnSend(HP_AgentListener pListener				, HP_FN_Agent_OnSend fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_Agent_OnReceive(HP_AgentListener pListener			, HP_FN_Agent_OnReceive fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_Agent_OnPullReceive(HP_AgentListener pListener		, HP_FN_Agent_OnPullReceive fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_Agent_OnClose(HP_AgentListener pListener				, HP_FN_Agent_OnClose fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_Agent_OnShutdown(HP_AgentListener pListener			, HP_FN_Agent_OnShutdown fn);

/**********************************************************************************/
/***************************** Client 回调函数设置方法 *****************************/

HPSOCKET_API void __HP_CALL HP_Set_FN_Client_OnPrepareConnect(HP_ClientListener pListener	, HP_FN_Client_OnPrepareConnect fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_Client_OnConnect(HP_ClientListener pListener			, HP_FN_Client_OnConnect fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_Client_OnHandShake(HP_ClientListener pListener		, HP_FN_Client_OnHandShake fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_Client_OnSend(HP_ClientListener pListener				, HP_FN_Client_OnSend fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_Client_OnReceive(HP_ClientListener pListener			, HP_FN_Client_OnReceive fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_Client_OnPullReceive(HP_ClientListener pListener		, HP_FN_Client_OnPullReceive fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_Client_OnClose(HP_ClientListener pListener			, HP_FN_Client_OnClose fn);

#ifdef _UDP_SUPPORT

/**********************************************************************************/
/***************************** UdpNode 回调函数设置方法 *****************************/

HPSOCKET_API void __HP_CALL HP_Set_FN_UdpNode_OnPrepareListen(HP_UdpNodeListener pListener	, HP_FN_UdpNode_OnPrepareListen fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_UdpNode_OnSend(HP_UdpNodeListener pListener			, HP_FN_UdpNode_OnSend fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_UdpNode_OnReceive(HP_UdpNodeListener pListener		, HP_FN_UdpNode_OnReceive fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_UdpNode_OnError(HP_UdpNodeListener pListener			, HP_FN_UdpNode_OnError fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_UdpNode_OnShutdown(HP_UdpNodeListener pListener		, HP_FN_UdpNode_OnShutdown fn);

#endif

/**************************************************************************/
/***************************** Server 操作方法 *****************************/

/*
* 名称：启动通信组件
* 描述：启动服务端通信组件，启动完成后可开始接收客户端连接并收发数据
*		
* 参数：		lpszBindAddress	-- 监听地址
*			usPort			-- 监听端口
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 HP_Server_GetLastError() 获取错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_Server_Start(HP_Server pServer, LPCTSTR lpszBindAddress, USHORT usPort);

/*
* 名称：关闭通信组件
* 描述：关闭服务端通信组件，关闭完成后断开所有客户端连接并释放所有资源
*		
* 参数：	
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 HP_Server_GetLastError() 获取错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_Server_Stop(HP_Server pServer);

/*
* 名称：等待
* 描述：等待通信组件停止运行
*
* 参数：		dwMilliseconds	-- 超时时间（毫秒，默认：-1，永不超时）
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 SYS_GetLastError() 获取错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_Server_Wait(HP_Server pServer, DWORD dwMilliseconds);

/*
* 名称：发送数据
* 描述：向指定连接发送数据
*		
* 参数：		dwConnID	-- 连接 ID
*			pBuffer		-- 发送缓冲区
*			iLength		-- 发送缓冲区长度
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 SYS_GetLastError() 获取 Windows 错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_Server_Send(HP_Server pServer, HP_CONNID dwConnID, const BYTE* pBuffer, int iLength);

/*
* 名称：发送数据
* 描述：向指定连接发送数据
*		
* 参数：		dwConnID	-- 连接 ID
*			pBuffer		-- 发送缓冲区
*			iLength		-- 发送缓冲区长度
*			iOffset		-- 发送缓冲区指针偏移量
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 SYS_GetLastError() 获取 Windows 错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_Server_SendPart(HP_Server pServer, HP_CONNID dwConnID, const BYTE* pBuffer, int iLength, int iOffset);

/*
* 名称：发送多组数据
* 描述：向指定连接发送多组数据
*		TCP - 顺序发送所有数据包 
*		UDP - 把所有数据包组合成一个数据包发送（数据包的总长度不能大于设置的 UDP 包最大长度） 
*		
* 参数：		dwConnID	-- 连接 ID
*			pBuffers	-- 发送缓冲区数组
*			iCount		-- 发送缓冲区数目
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 SYS_GetLastError() 获取 Windows 错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_Server_SendPackets(HP_Server pServer, HP_CONNID dwConnID, const WSABUF pBuffers[], int iCount);

/*
* 名称：暂停/恢复接收
* 描述：暂停/恢复某个连接的数据接收工作
*		
* 参数：		dwConnID	-- 连接 ID
*			bPause		-- TRUE - 暂停, FALSE - 恢复
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败
*/
HPSOCKET_API BOOL __HP_CALL HP_Server_PauseReceive(HP_Server pServer, HP_CONNID dwConnID, BOOL bPause);

/*
* 名称：断开连接
* 描述：断开与某个客户端的连接
*		
* 参数：		dwConnID	-- 连接 ID
*			bForce		-- 是否强制断开连接
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败
*/
HPSOCKET_API BOOL __HP_CALL HP_Server_Disconnect(HP_Server pServer, HP_CONNID dwConnID, BOOL bForce);

/*
* 名称：断开超时连接
* 描述：断开超过指定时长的连接
*		
* 参数：		dwPeriod	-- 时长（毫秒）
*			bForce		-- 是否强制断开连接
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败
*/
HPSOCKET_API BOOL __HP_CALL HP_Server_DisconnectLongConnections(HP_Server pServer, DWORD dwPeriod, BOOL bForce);

/*
* 名称：断开静默连接
* 描述：断开超过指定时长的静默连接
*		
* 参数：		dwPeriod	-- 时长（毫秒）
*			bForce		-- 是否强制断开连接
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败
*/
HPSOCKET_API BOOL __HP_CALL HP_Server_DisconnectSilenceConnections(HP_Server pServer, DWORD dwPeriod, BOOL bForce);

/******************************************************************************/
/***************************** Server 属性访问方法 *****************************/

/*
* 名称：设置连接的附加数据
* 描述：是否为连接绑定附加数据或者绑定什么样的数据，均由应用程序只身决定
*		
* 参数：		dwConnID	-- 连接 ID
*			pv			-- 数据
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败（无效的连接 ID）
*/
HPSOCKET_API BOOL __HP_CALL HP_Server_SetConnectionExtra(HP_Server pServer, HP_CONNID dwConnID, PVOID pExtra);

/*
* 名称：获取连接的附加数据
* 描述：是否为连接绑定附加数据或者绑定什么样的数据，均由应用程序只身决定
*		
* 参数：		dwConnID	-- 连接 ID
*			ppv			-- 数据指针
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败（无效的连接 ID）
*/
HPSOCKET_API BOOL __HP_CALL HP_Server_GetConnectionExtra(HP_Server pServer, HP_CONNID dwConnID, PVOID* ppExtra);

/* 检测是否为安全连接（SSL/HTTPS） */
HPSOCKET_API BOOL __HP_CALL HP_Server_IsSecure(HP_Server pServer);
/* 检查通信组件是否已启动 */
HPSOCKET_API BOOL __HP_CALL HP_Server_HasStarted(HP_Server pServer);
/* 查看通信组件当前状态 */
HPSOCKET_API En_HP_ServiceState __HP_CALL HP_Server_GetState(HP_Server pServer);
/* 获取最近一次失败操作的错误代码 */
HPSOCKET_API En_HP_SocketError __HP_CALL HP_Server_GetLastError(HP_Server pServer);
/* 获取最近一次失败操作的错误描述 */
HPSOCKET_API LPCTSTR __HP_CALL HP_Server_GetLastErrorDesc(HP_Server pServer);
/* 获取连接中未发出数据的长度 */
HPSOCKET_API BOOL __HP_CALL HP_Server_GetPendingDataLength(HP_Server pServer, HP_CONNID dwConnID, int* piPending);
/* 获取连接的数据接收状态 */
HPSOCKET_API BOOL __HP_CALL HP_Server_IsPauseReceive(HP_Server pServer, HP_CONNID dwConnID, BOOL* pbPaused);
/* 检测是否有效连接 */
HPSOCKET_API BOOL __HP_CALL HP_Server_IsConnected(HP_Server pServer, HP_CONNID dwConnID);
/* 获取客户端连接数 */
HPSOCKET_API DWORD __HP_CALL HP_Server_GetConnectionCount(HP_Server pServer);
/* 获取所有连接的 HP_CONNID */
HPSOCKET_API BOOL __HP_CALL HP_Server_GetAllConnectionIDs(HP_Server pServer, HP_CONNID pIDs[], DWORD* pdwCount);
/* 获取某个客户端连接时长（毫秒） */
HPSOCKET_API BOOL __HP_CALL HP_Server_GetConnectPeriod(HP_Server pServer, HP_CONNID dwConnID, DWORD* pdwPeriod);
/* 获取某个连接静默时间（毫秒） */
HPSOCKET_API BOOL __HP_CALL HP_Server_GetSilencePeriod(HP_Server pServer, HP_CONNID dwConnID, DWORD* pdwPeriod);
/* 获取监听 Socket 的地址信息 */
HPSOCKET_API BOOL __HP_CALL HP_Server_GetListenAddress(HP_Server pServer, TCHAR lpszAddress[], int* piAddressLen, USHORT* pusPort);
/* 获取某个连接的本地地址信息 */
HPSOCKET_API BOOL __HP_CALL HP_Server_GetLocalAddress(HP_Server pServer, HP_CONNID dwConnID, TCHAR lpszAddress[], int* piAddressLen, USHORT* pusPort);
/* 获取某个连接的远程地址信息 */
HPSOCKET_API BOOL __HP_CALL HP_Server_GetRemoteAddress(HP_Server pServer, HP_CONNID dwConnID, TCHAR lpszAddress[], int* piAddressLen, USHORT* pusPort);

/* 设置地址重用选项 */
HPSOCKET_API void __HP_CALL HP_Server_SetReuseAddressPolicy(HP_Server pServer, En_HP_ReuseAddressPolicy enReusePolicy);
/* 设置数据发送策略 */
HPSOCKET_API void __HP_CALL HP_Server_SetSendPolicy(HP_Server pServer, En_HP_SendPolicy enSendPolicy);
/* 设置 OnSend 事件同步策略（默认：OSSP_NONE，不同步） */
HPSOCKET_API void __HP_CALL HP_Server_SetOnSendSyncPolicy(HP_Server pServer, En_HP_OnSendSyncPolicy enSyncPolicy);
/* 设置 Socket 缓存对象锁定时间（毫秒，在锁定期间该 Socket 缓存对象不能被获取使用） */
HPSOCKET_API void __HP_CALL HP_Server_SetFreeSocketObjLockTime(HP_Server pServer, DWORD dwFreeSocketObjLockTime);
/* 设置 Socket 缓存池大小（通常设置为平均并发连接数量的 1/3 - 1/2） */
HPSOCKET_API void __HP_CALL HP_Server_SetFreeSocketObjPool(HP_Server pServer, DWORD dwFreeSocketObjPool);
/* 设置内存块缓存池大小（通常设置为 Socket 缓存池大小的 2 - 3 倍） */
HPSOCKET_API void __HP_CALL HP_Server_SetFreeBufferObjPool(HP_Server pServer, DWORD dwFreeBufferObjPool);
/* 设置 Socket 缓存池回收阀值（通常设置为 Socket 缓存池大小的 3 倍） */
HPSOCKET_API void __HP_CALL HP_Server_SetFreeSocketObjHold(HP_Server pServer, DWORD dwFreeSocketObjHold);
/* 设置内存块缓存池回收阀值（通常设置为内存块缓存池大小的 3 倍） */
HPSOCKET_API void __HP_CALL HP_Server_SetFreeBufferObjHold(HP_Server pServer, DWORD dwFreeBufferObjHold);
/* 设置最大连接数（组件会根据设置值预分配内存，因此需要根据实际情况设置，不宜过大）*/
HPSOCKET_API void __HP_CALL HP_Server_SetMaxConnectionCount(HP_Server pServer, DWORD dwMaxConnectionCount);
/* 设置工作线程数量（通常设置为 2 * CPU + 2） */
HPSOCKET_API void __HP_CALL HP_Server_SetWorkerThreadCount(HP_Server pServer, DWORD dwWorkerThreadCount);
/* 设置是否标记静默时间（设置为 TRUE 时 DisconnectSilenceConnections() 和 GetSilencePeriod() 才有效，默认：TRUE） */
HPSOCKET_API void __HP_CALL HP_Server_SetMarkSilence(HP_Server pServer, BOOL bMarkSilence);

/* 获取地址重用选项 */
HPSOCKET_API En_HP_ReuseAddressPolicy __HP_CALL HP_Server_GetReuseAddressPolicy(HP_Server pServer);
/* 获取数据发送策略 */
HPSOCKET_API En_HP_SendPolicy __HP_CALL HP_Server_GetSendPolicy(HP_Server pServer);
/* 获取 OnSend 事件同步策略 */
HPSOCKET_API En_HP_OnSendSyncPolicy __HP_CALL HP_Server_GetOnSendSyncPolicy(HP_Server pServer);
/* 获取 Socket 缓存对象锁定时间 */
HPSOCKET_API DWORD __HP_CALL HP_Server_GetFreeSocketObjLockTime(HP_Server pServer);
/* 获取 Socket 缓存池大小 */
HPSOCKET_API DWORD __HP_CALL HP_Server_GetFreeSocketObjPool(HP_Server pServer);
/* 获取内存块缓存池大小 */
HPSOCKET_API DWORD __HP_CALL HP_Server_GetFreeBufferObjPool(HP_Server pServer);
/* 获取 Socket 缓存池回收阀值 */
HPSOCKET_API DWORD __HP_CALL HP_Server_GetFreeSocketObjHold(HP_Server pServer);
/* 获取内存块缓存池回收阀值 */
HPSOCKET_API DWORD __HP_CALL HP_Server_GetFreeBufferObjHold(HP_Server pServer);
/* 获取最大连接数 */
HPSOCKET_API DWORD __HP_CALL HP_Server_GetMaxConnectionCount(HP_Server pServer);
/* 获取工作线程数量 */
HPSOCKET_API DWORD __HP_CALL HP_Server_GetWorkerThreadCount(HP_Server pServer);
/* 检测是否标记静默时间 */
HPSOCKET_API BOOL __HP_CALL HP_Server_IsMarkSilence(HP_Server pServer);

/**********************************************************************************/
/******************************* TCP Server 操作方法 *******************************/

/*
* 名称：发送小文件
* 描述：向指定连接发送 4096 KB 以下的小文件
*		
* 参数：		dwConnID		-- 连接 ID
*			lpszFileName	-- 文件路径
*			pHead			-- 头部附加数据
*			pTail			-- 尾部附加数据
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 SYS_GetLastError() 获取 Windows 错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_TcpServer_SendSmallFile(HP_Server pServer, HP_CONNID dwConnID, LPCTSTR lpszFileName, const LPWSABUF pHead, const LPWSABUF pTail);

/**********************************************************************************/
/***************************** TCP Server 属性访问方法 *****************************/

/* 设置监听 Socket 的等候队列大小（根据并发连接数量调整设置） */
HPSOCKET_API void __HP_CALL HP_TcpServer_SetSocketListenQueue(HP_TcpServer pServer, DWORD dwSocketListenQueue);
/* 设置 Accept 预投递数量（根据负载调整设置，Accept 预投递数量越大则支持的并发连接请求越多） */
HPSOCKET_API void __HP_CALL HP_TcpServer_SetAcceptSocketCount(HP_TcpServer pServer, DWORD dwAcceptSocketCount);
/* 设置通信数据缓冲区大小（根据平均通信数据包大小调整设置，通常设置为 1024 的倍数） */
HPSOCKET_API void __HP_CALL HP_TcpServer_SetSocketBufferSize(HP_TcpServer pServer, DWORD dwSocketBufferSize);
/* 设置正常心跳包间隔（毫秒，0 则不发送心跳包，默认：60 * 1000） */
HPSOCKET_API void __HP_CALL HP_TcpServer_SetKeepAliveTime(HP_TcpServer pServer, DWORD dwKeepAliveTime);
/* 设置异常心跳包间隔（毫秒，0 不发送心跳包，，默认：20 * 1000，如果超过若干次 [默认：WinXP 5 次, Win7 10 次] 检测不到心跳确认包则认为已断线） */
HPSOCKET_API void __HP_CALL HP_TcpServer_SetKeepAliveInterval(HP_TcpServer pServer, DWORD dwKeepAliveInterval);
/* 设置是否开启 nodelay 模式（默认：FALSE，不开启） */
HPSOCKET_API void __HP_CALL HP_TcpServer_SetNoDelay(HP_TcpServer pServer, BOOL bNoDelay);

/* 获取 Accept 预投递数量 */
HPSOCKET_API DWORD __HP_CALL HP_TcpServer_GetAcceptSocketCount(HP_TcpServer pServer);
/* 获取通信数据缓冲区大小 */
HPSOCKET_API DWORD __HP_CALL HP_TcpServer_GetSocketBufferSize(HP_TcpServer pServer);
/* 获取监听 Socket 的等候队列大小 */
HPSOCKET_API DWORD __HP_CALL HP_TcpServer_GetSocketListenQueue(HP_TcpServer pServer);
/* 获取正常心跳包间隔 */
HPSOCKET_API DWORD __HP_CALL HP_TcpServer_GetKeepAliveTime(HP_TcpServer pServer);
/* 获取异常心跳包间隔 */
HPSOCKET_API DWORD __HP_CALL HP_TcpServer_GetKeepAliveInterval(HP_TcpServer pServer);
/* 检查是否开启 nodelay 模式 */
HPSOCKET_API BOOL __HP_CALL HP_TcpServer_IsNoDelay(HP_TcpServer pServer);

#ifdef _UDP_SUPPORT

/**********************************************************************************/
/***************************** UDP Server 属性访问方法 *****************************/

/* 设置数据报文最大长度（建议在局域网环境下不超过 1432 字节，在广域网环境下不超过 548 字节） */
HPSOCKET_API void __HP_CALL HP_UdpServer_SetMaxDatagramSize(HP_UdpServer pServer, DWORD dwMaxDatagramSize);
/* 获取数据报文最大长度 */
HPSOCKET_API DWORD __HP_CALL HP_UdpServer_GetMaxDatagramSize(HP_UdpServer pServer);

/* 设置 Receive 预投递数量（根据负载调整设置，Receive 预投递数量越大则丢包概率越小） */
HPSOCKET_API void __HP_CALL HP_UdpServer_SetPostReceiveCount(HP_UdpServer pServer, DWORD dwPostReceiveCount);
/* 获取 Receive 预投递数量 */
HPSOCKET_API DWORD __HP_CALL HP_UdpServer_GetPostReceiveCount(HP_UdpServer pServer);

/* 设置监测包尝试次数（0 则不发送监测跳包，如果超过最大尝试次数则认为已断线） */
HPSOCKET_API void __HP_CALL HP_UdpServer_SetDetectAttempts(HP_UdpServer pServer, DWORD dwDetectAttempts);
/* 设置监测包发送间隔（毫秒，0 不发送监测包） */
HPSOCKET_API void __HP_CALL HP_UdpServer_SetDetectInterval(HP_UdpServer pServer, DWORD dwDetectInterval);
/* 获取心跳检查次数 */
HPSOCKET_API DWORD __HP_CALL HP_UdpServer_GetDetectAttempts(HP_UdpServer pServer);
/* 获取心跳检查间隔 */
HPSOCKET_API DWORD __HP_CALL HP_UdpServer_GetDetectInterval(HP_UdpServer pServer);

/**********************************************************************************/
/*************************** UDP ARQ Server 属性访问方法 ***************************/

/* 设置是否开启 nodelay 模式（默认：FALSE，不开启） */
HPSOCKET_API void __HP_CALL HP_UdpArqServer_SetNoDelay(HP_UdpArqServer pServer, BOOL bNoDelay);
/* 设置是否关闭拥塞控制（默认：FALSE，不关闭） */
HPSOCKET_API void __HP_CALL HP_UdpArqServer_SetTurnoffCongestCtrl(HP_UdpArqServer pServer, BOOL bTurnOff);
/* 设置数据刷新间隔（毫秒，默认：60） */
HPSOCKET_API void __HP_CALL HP_UdpArqServer_SetFlushInterval(HP_UdpArqServer pServer, DWORD dwFlushInterval);
/* 设置快速重传 ACK 跨越次数（默认：0，关闭快速重传） */
HPSOCKET_API void __HP_CALL HP_UdpArqServer_SetResendByAcks(HP_UdpArqServer pServer, DWORD dwResendByAcks);
/* 设置发送窗口大小（数据包数量，默认：128） */
HPSOCKET_API void __HP_CALL HP_UdpArqServer_SetSendWndSize(HP_UdpArqServer pServer, DWORD dwSendWndSize);
/* 设置接收窗口大小（数据包数量，默认：512） */
HPSOCKET_API void __HP_CALL HP_UdpArqServer_SetRecvWndSize(HP_UdpArqServer pServer, DWORD dwRecvWndSize);
/* 设置最小重传超时时间（毫秒，默认：30） */
HPSOCKET_API void __HP_CALL HP_UdpArqServer_SetMinRto(HP_UdpArqServer pServer, DWORD dwMinRto);
/* 设置快速握手次数限制（默认：5，如果为 0 则不限制） */
HPSOCKET_API void __HP_CALL HP_UdpArqServer_SetFastLimit(HP_UdpArqServer pServer, DWORD dwFastLimit);
/* 设置最大传输单元（默认：0，与 SetMaxDatagramSize() 一致） */
HPSOCKET_API void __HP_CALL HP_UdpArqServer_SetMaxTransUnit(HP_UdpArqServer pServer, DWORD dwMaxTransUnit);
/* 设置最大数据包大小（默认：4096） */
HPSOCKET_API void __HP_CALL HP_UdpArqServer_SetMaxMessageSize(HP_UdpArqServer pServer, DWORD dwMaxMessageSize);
/* 设置握手超时时间（毫秒，默认：5000） */
HPSOCKET_API void __HP_CALL HP_UdpArqServer_SetHandShakeTimeout(HP_UdpArqServer pServer, DWORD dwHandShakeTimeout);

/* 检测是否开启 nodelay 模式 */
HPSOCKET_API BOOL __HP_CALL HP_UdpArqServer_IsNoDelay(HP_UdpArqServer pServer);
/* 检测是否关闭拥塞控制 */
HPSOCKET_API BOOL __HP_CALL HP_UdpArqServer_IsTurnoffCongestCtrl(HP_UdpArqServer pServer);
/* 获取数据刷新间隔 */
HPSOCKET_API DWORD __HP_CALL HP_UdpArqServer_GetFlushInterval(HP_UdpArqServer pServer);
/* 获取快速重传 ACK 跨越次数 */
HPSOCKET_API DWORD __HP_CALL HP_UdpArqServer_GetResendByAcks(HP_UdpArqServer pServer);
/* 获取发送窗口大小 */
HPSOCKET_API DWORD __HP_CALL HP_UdpArqServer_GetSendWndSize(HP_UdpArqServer pServer);
/* 获取接收窗口大小 */
HPSOCKET_API DWORD __HP_CALL HP_UdpArqServer_GetRecvWndSize(HP_UdpArqServer pServer);
/* 获取最小重传超时时间 */
HPSOCKET_API DWORD __HP_CALL HP_UdpArqServer_GetMinRto(HP_UdpArqServer pServer);
/* 获取快速握手次数限制 */
HPSOCKET_API DWORD __HP_CALL HP_UdpArqServer_GetFastLimit(HP_UdpArqServer pServer);
/* 获取最大传输单元 */
HPSOCKET_API DWORD __HP_CALL HP_UdpArqServer_GetMaxTransUnit(HP_UdpArqServer pServer);
/* 获取最大数据包大小 */
HPSOCKET_API DWORD __HP_CALL HP_UdpArqServer_GetMaxMessageSize(HP_UdpArqServer pServer);
/* 获取握手超时时间 */
HPSOCKET_API DWORD __HP_CALL HP_UdpArqServer_GetHandShakeTimeout(HP_UdpArqServer pServer);

/* 获取等待发送包数量 */
HPSOCKET_API BOOL __HP_CALL HP_UdpArqServer_GetWaitingSendMessageCount(HP_UdpArqServer pServer, HP_CONNID dwConnID, int* piCount);

#endif

/**************************************************************************/
/***************************** Agent 操作方法 *****************************/

/*
* 名称：启动通信组件
* 描述：启动通信代理组件，启动完成后可开始连接远程服务器
*		
* 参数：		lpszBindAddress	-- 绑定地址（默认：nullptr，绑定任意地址）
*			bAsyncConnect	-- 是否采用异步 Connect
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 HP_Agent_GetLastError() 获取错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_Agent_Start(HP_Agent pAgent, LPCTSTR lpszBindAddress, BOOL bAsyncConnect);

/*
* 名称：关闭通信组件
* 描述：关闭通信组件，关闭完成后断开所有连接并释放所有资源
*		
* 参数：	
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 HP_Agent_GetLastError() 获取错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_Agent_Stop(HP_Agent pAgent);

/*
* 名称：等待
* 描述：等待通信组件停止运行
*
* 参数：		dwMilliseconds	-- 超时时间（毫秒，默认：-1，永不超时）
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 SYS_GetLastError() 获取错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_Agent_Wait(HP_Agent pAgent, DWORD dwMilliseconds);

/*
* 名称：连接服务器
* 描述：连接服务器，连接成功后 IAgentListener 会接收到 OnConnect() / OnHandShake() 事件
*		
* 参数：		lpszRemoteAddress	-- 服务端地址
*			usPort				-- 服务端端口
*			pdwConnID			-- 连接 ID（默认：nullptr，不获取连接 ID）
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过函数 SYS_GetLastError() 获取 Windows 错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_Agent_Connect(HP_Agent pAgent, LPCTSTR lpszRemoteAddress, USHORT usPort, HP_CONNID* pdwConnID);

/*
* 名称：连接服务器
* 描述：连接服务器，连接成功后 IAgentListener 会接收到 OnConnect() / OnHandShake() 事件
*		
* 参数：		lpszRemoteAddress	-- 服务端地址
*			usPort				-- 服务端端口
*			pdwConnID			-- 连接 ID（默认：nullptr，不获取连接 ID）
*			pExtra				-- 连接附加数据（默认：nullptr）
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过函数 SYS_GetLastError() 获取 Windows 错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_Agent_ConnectWithExtra(HP_Agent pAgent, LPCTSTR lpszRemoteAddress, USHORT usPort, HP_CONNID* pdwConnID, PVOID pExtra);

/*
* 名称：连接服务器
* 描述：连接服务器，连接成功后 IAgentListener 会接收到 OnConnect() / OnHandShake() 事件
*		
* 参数：		lpszRemoteAddress	-- 服务端地址
*			usPort				-- 服务端端口
*			pdwConnID			-- 连接 ID（默认：nullptr，不获取连接 ID）
*			usLocalPort			-- 本地端口（默认：0）
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过函数 SYS_GetLastError() 获取 Windows 错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_Agent_ConnectWithLocalPort(HP_Agent pAgent, LPCTSTR lpszRemoteAddress, USHORT usPort, HP_CONNID* pdwConnID, USHORT usLocalPort);

/*
* 名称：连接服务器
* 描述：连接服务器，连接成功后 IAgentListener 会接收到 OnConnect() / OnHandShake() 事件
*		
* 参数：		lpszRemoteAddress	-- 服务端地址
*			usPort				-- 服务端端口
*			pdwConnID			-- 连接 ID（默认：nullptr，不获取连接 ID）
*			lpszLocalAddress	-- 本地地址（默认：nullptr，使用 Start() 方法中绑定的地址）
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过函数 SYS_GetLastError() 获取 Windows 错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_Agent_ConnectWithLocalAddress(HP_Agent pAgent, LPCTSTR lpszRemoteAddress, USHORT usPort, HP_CONNID* pdwConnID, LPCTSTR lpszLocalAddress);

/* 
* 名称：连接服务器
* 描述：连接服务器，连接成功后 IAgentListener 会接收到 OnConnect() / OnHandShake() 事件
*		
* 参数：		lpszRemoteAddress	-- 服务端地址
*			usPort				-- 服务端端口
*			pdwConnID			-- 连接 ID（默认：nullptr，不获取连接 ID）
*			pExtra				-- 连接附加数据（默认：nullptr）
*			usLocalPort			-- 本地端口（默认：0）
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过函数 SYS_GetLastError() 获取 Windows 错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_Agent_ConnectWithExtraAndLocalPort(HP_Agent pAgent, LPCTSTR lpszRemoteAddress, USHORT usPort, HP_CONNID* pdwConnID, PVOID pExtra, USHORT usLocalPort);

/* 
* 名称：连接服务器
* 描述：连接服务器，连接成功后 IAgentListener 会接收到 OnConnect() / OnHandShake() 事件
*		
* 参数：		lpszRemoteAddress	-- 服务端地址
*			usPort				-- 服务端端口
*			pdwConnID			-- 连接 ID（默认：nullptr，不获取连接 ID）
*			pExtra				-- 连接附加数据（默认：nullptr）
*			usLocalPort			-- 本地端口（默认：0）
*			lpszLocalAddress	-- 本地地址（默认：nullptr，使用 Start() 方法中绑定的地址）
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过函数 SYS_GetLastError() 获取 Windows 错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_Agent_ConnectWithExtraAndLocalAddressPort(HP_Agent pAgent, LPCTSTR lpszRemoteAddress, USHORT usPort, HP_CONNID* pdwConnID, PVOID pExtra, USHORT usLocalPort, LPCTSTR lpszLocalAddress);

/*
* 名称：发送数据
* 描述：向指定连接发送数据
*		
* 参数：		dwConnID	-- 连接 ID
*			pBuffer		-- 发送缓冲区
*			iLength		-- 发送缓冲区长度
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 SYS_GetLastError() 获取 Windows 错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_Agent_Send(HP_Agent pAgent, HP_CONNID dwConnID, const BYTE* pBuffer, int iLength);

/*
* 名称：发送数据
* 描述：向指定连接发送数据
*		
* 参数：		dwConnID	-- 连接 ID
*			pBuffer		-- 发送缓冲区
*			iLength		-- 发送缓冲区长度
*			iOffset		-- 发送缓冲区指针偏移量
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 SYS_GetLastError() 获取 Windows 错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_Agent_SendPart(HP_Agent pAgent, HP_CONNID dwConnID, const BYTE* pBuffer, int iLength, int iOffset);

/*
* 名称：发送多组数据
* 描述：向指定连接发送多组数据
*		TCP - 顺序发送所有数据包 
*		UDP - 把所有数据包组合成一个数据包发送（数据包的总长度不能大于设置的 UDP 包最大长度） 
*		
* 参数：		dwConnID	-- 连接 ID
*			pBuffers	-- 发送缓冲区数组
*			iCount		-- 发送缓冲区数目
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 SYS_GetLastError() 获取 Windows 错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_Agent_SendPackets(HP_Agent pAgent, HP_CONNID dwConnID, const WSABUF pBuffers[], int iCount);

/*
* 名称：暂停/恢复接收
* 描述：暂停/恢复某个连接的数据接收工作
*		
* 参数：		dwConnID	-- 连接 ID
*			bPause		-- TRUE - 暂停, FALSE - 恢复
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败
*/
HPSOCKET_API BOOL __HP_CALL HP_Agent_PauseReceive(HP_Agent pAgent, HP_CONNID dwConnID, BOOL bPause);

/*
* 名称：断开连接
* 描述：断开某个连接
*		
* 参数：		dwConnID	-- 连接 ID
*			bForce		-- 是否强制断开连接
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败
*/
HPSOCKET_API BOOL __HP_CALL HP_Agent_Disconnect(HP_Agent pAgent, HP_CONNID dwConnID, BOOL bForce);

/*
* 名称：断开超时连接
* 描述：断开超过指定时长的连接
*		
* 参数：		dwPeriod	-- 时长（毫秒）
*			bForce		-- 是否强制断开连接
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败
*/
HPSOCKET_API BOOL __HP_CALL HP_Agent_DisconnectLongConnections(HP_Agent pAgent, DWORD dwPeriod, BOOL bForce);

/*
* 名称：断开静默连接
* 描述：断开超过指定时长的静默连接
*		
* 参数：		dwPeriod	-- 时长（毫秒）
*			bForce		-- 是否强制断开连接
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败
*/
HPSOCKET_API BOOL __HP_CALL HP_Agent_DisconnectSilenceConnections(HP_Agent pAgent, DWORD dwPeriod, BOOL bForce);

/******************************************************************************/
/***************************** Agent 属性访问方法 *****************************/

/*
* 名称：设置连接的附加数据
* 描述：是否为连接绑定附加数据或者绑定什么样的数据，均由应用程序只身决定
*		
* 参数：		dwConnID	-- 连接 ID
*			pv			-- 数据
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败（无效的连接 ID）
*/
HPSOCKET_API BOOL __HP_CALL HP_Agent_SetConnectionExtra(HP_Agent pAgent, HP_CONNID dwConnID, PVOID pExtra);

/*
* 名称：获取连接的附加数据
* 描述：是否为连接绑定附加数据或者绑定什么样的数据，均由应用程序只身决定
*		
* 参数：		dwConnID	-- 连接 ID
*			ppv			-- 数据指针
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败（无效的连接 ID）
*/
HPSOCKET_API BOOL __HP_CALL HP_Agent_GetConnectionExtra(HP_Agent pAgent, HP_CONNID dwConnID, PVOID* ppExtra);

/* 检测是否为安全连接（SSL/HTTPS） */
HPSOCKET_API BOOL __HP_CALL HP_Agent_IsSecure(HP_Agent pAgent);
/* 检查通信组件是否已启动 */
HPSOCKET_API BOOL __HP_CALL HP_Agent_HasStarted(HP_Agent pAgent);
/* 查看通信组件当前状态 */
HPSOCKET_API En_HP_ServiceState __HP_CALL HP_Agent_GetState(HP_Agent pAgent);
/* 获取连接数 */
HPSOCKET_API DWORD __HP_CALL HP_Agent_GetConnectionCount(HP_Agent pAgent);
/* 获取所有连接的 HP_CONNID */
HPSOCKET_API BOOL __HP_CALL HP_Agent_GetAllConnectionIDs(HP_Agent pAgent, HP_CONNID pIDs[], DWORD* pdwCount);
/* 获取某个连接时长（毫秒） */
HPSOCKET_API BOOL __HP_CALL HP_Agent_GetConnectPeriod(HP_Agent pAgent, HP_CONNID dwConnID, DWORD* pdwPeriod);
/* 获取某个连接静默时间（毫秒） */
HPSOCKET_API BOOL __HP_CALL HP_Agent_GetSilencePeriod(HP_Agent pAgent, HP_CONNID dwConnID, DWORD* pdwPeriod);
/* 获取某个连接的本地地址信息 */
HPSOCKET_API BOOL __HP_CALL HP_Agent_GetLocalAddress(HP_Agent pAgent, HP_CONNID dwConnID, TCHAR lpszAddress[], int* piAddressLen, USHORT* pusPort);
/* 获取某个连接的远程地址信息 */
HPSOCKET_API BOOL __HP_CALL HP_Agent_GetRemoteAddress(HP_Agent pAgent, HP_CONNID dwConnID, TCHAR lpszAddress[], int* piAddressLen, USHORT* pusPort);
/* 获取某个连接的远程主机信息 */
HPSOCKET_API BOOL __HP_CALL HP_Agent_GetRemoteHost(HP_Agent pAgent, HP_CONNID dwConnID, TCHAR lpszHost[], int* piHostLen, USHORT* pusPort);
/* 获取最近一次失败操作的错误代码 */
HPSOCKET_API En_HP_SocketError __HP_CALL HP_Agent_GetLastError(HP_Agent pAgent);
/* 获取最近一次失败操作的错误描述 */
HPSOCKET_API LPCTSTR __HP_CALL HP_Agent_GetLastErrorDesc(HP_Agent pAgent);
/* 获取连接中未发出数据的长度 */
HPSOCKET_API BOOL __HP_CALL HP_Agent_GetPendingDataLength(HP_Agent pAgent, HP_CONNID dwConnID, int* piPending);
/* 获取连接的数据接收状态 */
HPSOCKET_API BOOL __HP_CALL HP_Agent_IsPauseReceive(HP_Agent pAgent, HP_CONNID dwConnID, BOOL* pbPaused);
/* 检测是否有效连接 */
HPSOCKET_API BOOL __HP_CALL HP_Agent_IsConnected(HP_Agent pAgent, HP_CONNID dwConnID);

/* 设置地址重用选项 */
HPSOCKET_API void __HP_CALL HP_Agent_SetReuseAddressPolicy(HP_Agent pAgent, En_HP_ReuseAddressPolicy enReusePolicy);
/* 设置数据发送策略 */
HPSOCKET_API void __HP_CALL HP_Agent_SetSendPolicy(HP_Agent pAgent, En_HP_SendPolicy enSendPolicy);
/* 设置 OnSend 事件同步策略（默认：OSSP_NONE，不同步） */
HPSOCKET_API void __HP_CALL HP_Agent_SetOnSendSyncPolicy(HP_Agent pAgent, En_HP_OnSendSyncPolicy enSyncPolicy);
/* 设置 Socket 缓存对象锁定时间（毫秒，在锁定期间该 Socket 缓存对象不能被获取使用） */
HPSOCKET_API void __HP_CALL HP_Agent_SetFreeSocketObjLockTime(HP_Agent pAgent, DWORD dwFreeSocketObjLockTime);
/* 设置 Socket 缓存池大小（通常设置为平均并发连接数量的 1/3 - 1/2） */
HPSOCKET_API void __HP_CALL HP_Agent_SetFreeSocketObjPool(HP_Agent pAgent, DWORD dwFreeSocketObjPool);
/* 设置内存块缓存池大小（通常设置为 Socket 缓存池大小的 2 - 3 倍） */
HPSOCKET_API void __HP_CALL HP_Agent_SetFreeBufferObjPool(HP_Agent pAgent, DWORD dwFreeBufferObjPool);
/* 设置 Socket 缓存池回收阀值（通常设置为 Socket 缓存池大小的 3 倍） */
HPSOCKET_API void __HP_CALL HP_Agent_SetFreeSocketObjHold(HP_Agent pAgent, DWORD dwFreeSocketObjHold);
/* 设置内存块缓存池回收阀值（通常设置为内存块缓存池大小的 3 倍） */
HPSOCKET_API void __HP_CALL HP_Agent_SetFreeBufferObjHold(HP_Agent pAgent, DWORD dwFreeBufferObjHold);
/* 设置最大连接数（组件会根据设置值预分配内存，因此需要根据实际情况设置，不宜过大）*/
HPSOCKET_API void __HP_CALL HP_Agent_SetMaxConnectionCount(HP_Agent pAgent, DWORD dwMaxConnectionCount);
/* 设置工作线程数量（通常设置为 2 * CPU + 2） */
HPSOCKET_API void __HP_CALL HP_Agent_SetWorkerThreadCount(HP_Agent pAgent, DWORD dwWorkerThreadCount);
/* 设置是否标记静默时间（设置为 TRUE 时 DisconnectSilenceConnections() 和 GetSilencePeriod() 才有效，默认：TRUE） */
HPSOCKET_API void __HP_CALL HP_Agent_SetMarkSilence(HP_Agent pAgent, BOOL bMarkSilence);

/* 获取地址重用选项 */
HPSOCKET_API En_HP_ReuseAddressPolicy __HP_CALL HP_Agent_GetReuseAddressPolicy(HP_Agent pAgent);
/* 获取数据发送策略 */
HPSOCKET_API En_HP_SendPolicy __HP_CALL HP_Agent_GetSendPolicy(HP_Agent pAgent);
/* 获取 OnSend 事件同步策略 */
HPSOCKET_API En_HP_OnSendSyncPolicy __HP_CALL HP_Agent_GetOnSendSyncPolicy(HP_Agent pAgent);
/* 获取 Socket 缓存对象锁定时间 */
HPSOCKET_API DWORD __HP_CALL HP_Agent_GetFreeSocketObjLockTime(HP_Agent pAgent);
/* 获取 Socket 缓存池大小 */
HPSOCKET_API DWORD __HP_CALL HP_Agent_GetFreeSocketObjPool(HP_Agent pAgent);
/* 获取内存块缓存池大小 */
HPSOCKET_API DWORD __HP_CALL HP_Agent_GetFreeBufferObjPool(HP_Agent pAgent);
/* 获取 Socket 缓存池回收阀值 */
HPSOCKET_API DWORD __HP_CALL HP_Agent_GetFreeSocketObjHold(HP_Agent pAgent);
/* 获取内存块缓存池回收阀值 */
HPSOCKET_API DWORD __HP_CALL HP_Agent_GetFreeBufferObjHold(HP_Agent pAgent);
/* 获取最大连接数 */
HPSOCKET_API DWORD __HP_CALL HP_Agent_GetMaxConnectionCount(HP_Agent pAgent);
/* 获取工作线程数量 */
HPSOCKET_API DWORD __HP_CALL HP_Agent_GetWorkerThreadCount(HP_Agent pAgent);
/* 检测是否标记静默时间 */
HPSOCKET_API BOOL __HP_CALL HP_Agent_IsMarkSilence(HP_Agent pAgent);

/**********************************************************************************/
/******************************* TCP Agent 操作方法 *******************************/

/*
* 名称：发送小文件
* 描述：向指定连接发送 4096 KB 以下的小文件
*		
* 参数：		dwConnID		-- 连接 ID
*			lpszFileName	-- 文件路径
*			pHead			-- 头部附加数据
*			pTail			-- 尾部附加数据
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 SYS_GetLastError() 获取 Windows 错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_TcpAgent_SendSmallFile(HP_Agent pAgent, HP_CONNID dwConnID, LPCTSTR lpszFileName, const LPWSABUF pHead, const LPWSABUF pTail);

/**********************************************************************************/
/***************************** TCP Agent 属性访问方法 *****************************/

/* 设置同步连接超时时间（毫秒） */
HPSOCKET_API void __HP_CALL HP_TcpAgent_SetSyncConnectTimeout(HP_TcpAgent pAgent, DWORD dwSyncConnectTimeout);
/* 设置通信数据缓冲区大小（根据平均通信数据包大小调整设置，通常设置为 1024 的倍数） */
HPSOCKET_API void __HP_CALL HP_TcpAgent_SetSocketBufferSize(HP_TcpAgent pAgent, DWORD dwSocketBufferSize);
/* 设置正常心跳包间隔（毫秒，0 则不发送心跳包，默认：60 * 1000） */
HPSOCKET_API void __HP_CALL HP_TcpAgent_SetKeepAliveTime(HP_TcpAgent pAgent, DWORD dwKeepAliveTime);
/* 设置异常心跳包间隔（毫秒，0 不发送心跳包，，默认：20 * 1000，如果超过若干次 [默认：WinXP 5 次, Win7 10 次] 检测不到心跳确认包则认为已断线） */
HPSOCKET_API void __HP_CALL HP_TcpAgent_SetKeepAliveInterval(HP_TcpAgent pAgent, DWORD dwKeepAliveInterval);
/* 设置是否开启 nodelay 模式（默认：FALSE，不开启） */
HPSOCKET_API void __HP_CALL HP_TcpAgent_SetNoDelay(HP_TcpAgent pAgent, BOOL bNoDelay);

/* 获取同步连接超时时间 */
HPSOCKET_API DWORD __HP_CALL HP_TcpAgent_GetSyncConnectTimeout(HP_TcpAgent pAgent);
/* 获取通信数据缓冲区大小 */
HPSOCKET_API DWORD __HP_CALL HP_TcpAgent_GetSocketBufferSize(HP_TcpAgent pAgent);
/* 获取正常心跳包间隔 */
HPSOCKET_API DWORD __HP_CALL HP_TcpAgent_GetKeepAliveTime(HP_TcpAgent pAgent);
/* 获取异常心跳包间隔 */
HPSOCKET_API DWORD __HP_CALL HP_TcpAgent_GetKeepAliveInterval(HP_TcpAgent pAgent);
/* 检查是否开启 nodelay 模式 */
HPSOCKET_API BOOL __HP_CALL HP_TcpAgent_IsNoDelay(HP_TcpAgent pAgent);

/******************************************************************************/
/***************************** Client 组件操作方法 *****************************/

/*
* 名称：启动通信组件
* 描述：启动客户端通信组件并连接服务端，启动完成后可开始收发数据
*		
* 参数：		lpszRemoteAddress	-- 服务端地址
*			usPort				-- 服务端端口
*			bAsyncConnect		-- 是否采用异步 Connect
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 HP_Client_GetLastError() 获取错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_Client_Start(HP_Client pClient, LPCTSTR lpszRemoteAddress, USHORT usPort, BOOL bAsyncConnect);

/*
* 名称：启动通信组件（并指定绑定地址）
* 描述：启动客户端通信组件并连接服务端，启动完成后可开始收发数据
*		
* 参数：		lpszRemoteAddress	-- 服务端地址
*			usPort				-- 服务端端口
*			bAsyncConnect		-- 是否采用异步 Connect
*			lpszBindAddress		-- 绑定地址（默认：nullptr，TcpClient/UdpClient -> 不执行绑定操作，UdpCast 绑定 -> 任意地址）
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 HP_Client_GetLastError() 获取错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_Client_StartWithBindAddress(HP_Client pClient, LPCTSTR lpszRemoteAddress, USHORT usPort, BOOL bAsyncConnect, LPCTSTR lpszBindAddress);

/*
* 名称：启动通信组件（并指定绑定地址）
* 描述：启动客户端通信组件并连接服务端，启动完成后可开始收发数据
*		
* 参数：		lpszRemoteAddress	-- 服务端地址
*			usPort				-- 服务端端口
*			bAsyncConnect		-- 是否采用异步 Connect
*			lpszBindAddress		-- 绑定地址（默认：nullptr，TcpClient/UdpClient -> 不执行绑定操作，UdpCast 绑定 -> 任意地址）
*			usLocalPort			-- 本地端口（默认：0）
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 HP_Client_GetLastError() 获取错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_Client_StartWithBindAddressAndLocalPort(HP_Client pClient, LPCTSTR lpszRemoteAddress, USHORT usPort, BOOL bAsyncConnect, LPCTSTR lpszBindAddress, USHORT usLocalPort);

/*
* 名称：关闭通信组件
* 描述：关闭客户端通信组件，关闭完成后断开与服务端的连接并释放所有资源
*		
* 参数：	
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 HP_Client_GetLastError() 获取错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_Client_Stop(HP_Client pClient);

/*
* 名称：发送数据
* 描述：向服务端发送数据
*		
* 参数：		pBuffer		-- 发送缓冲区
*			iLength		-- 发送缓冲区长度
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 SYS_GetLastError() 获取 Windows 错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_Client_Send(HP_Client pClient, const BYTE* pBuffer, int iLength);

/*
* 名称：发送数据
* 描述：向服务端发送数据
*		
* 参数：		pBuffer		-- 发送缓冲区
*			iLength		-- 发送缓冲区长度
*			iOffset		-- 发送缓冲区指针偏移量
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 SYS_GetLastError() 获取 Windows 错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_Client_SendPart(HP_Client pClient, const BYTE* pBuffer, int iLength, int iOffset);

/*
* 名称：发送多组数据
* 描述：向服务端发送多组数据
*		TCP - 顺序发送所有数据包 
*		UDP - 把所有数据包组合成一个数据包发送（数据包的总长度不能大于设置的 UDP 包最大长度） 
*		
* 参数：		pBuffers	-- 发送缓冲区数组
*			iCount		-- 发送缓冲区数目
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 SYS_GetLastError() 获取 Windows 错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_Client_SendPackets(HP_Client pClient, const WSABUF pBuffers[], int iCount);

/*
* 名称：暂停/恢复接收
* 描述：暂停/恢复某个连接的数据接收工作
*		
*			bPause	-- TRUE - 暂停, FALSE - 恢复
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败
*/
HPSOCKET_API BOOL __HP_CALL HP_Client_PauseReceive(HP_Client pClient, BOOL bPause);

/*
* 名称：等待
* 描述：等待通信组件停止运行
*		
* 参数：		dwMilliseconds	-- 超时时间（毫秒，默认：-1，永不超时）
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 SYS_GetLastError() 获取错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_Client_Wait(HP_Client pClient, DWORD dwMilliseconds);

/******************************************************************************/
/***************************** Client 属性访问方法 *****************************/

/* 设置连接的附加数据 */
HPSOCKET_API void __HP_CALL HP_Client_SetExtra(HP_Client pClient, PVOID pExtra);
/* 获取连接的附加数据 */
HPSOCKET_API PVOID __HP_CALL HP_Client_GetExtra(HP_Client pClient);

/* 检测是否为安全连接（SSL/HTTPS） */
HPSOCKET_API BOOL __HP_CALL HP_Client_IsSecure(HP_Client pClient);
/* 检查通信组件是否已启动 */
HPSOCKET_API BOOL __HP_CALL HP_Client_HasStarted(HP_Client pClient);
/* 查看通信组件当前状态 */
HPSOCKET_API En_HP_ServiceState	__HP_CALL HP_Client_GetState(HP_Client pClient);
/* 获取最近一次失败操作的错误代码 */
HPSOCKET_API En_HP_SocketError	__HP_CALL HP_Client_GetLastError(HP_Client pClient);
/* 获取最近一次失败操作的错误描述 */
HPSOCKET_API LPCTSTR __HP_CALL HP_Client_GetLastErrorDesc(HP_Client pClient);
/* 获取该组件对象的连接 ID */
HPSOCKET_API HP_CONNID __HP_CALL HP_Client_GetConnectionID(HP_Client pClient);
/* 获取 Client Socket 的地址信息 */
HPSOCKET_API BOOL __HP_CALL HP_Client_GetLocalAddress(HP_Client pClient, TCHAR lpszAddress[], int* piAddressLen, USHORT* pusPort);
/* 获取连接的远程主机信息 */
HPSOCKET_API BOOL __HP_CALL HP_Client_GetRemoteHost(HP_Client pClient, TCHAR lpszHost[], int* piHostLen, USHORT* pusPort);
/* 获取连接中未发出数据的长度 */
HPSOCKET_API BOOL __HP_CALL HP_Client_GetPendingDataLength(HP_Client pClient, int* piPending);
/* 获取连接的数据接收状态 */
HPSOCKET_API BOOL __HP_CALL HP_Client_IsPauseReceive(HP_Client pClient, BOOL* pbPaused);
/* 检测是否有效连接 */
HPSOCKET_API BOOL __HP_CALL HP_Client_IsConnected(HP_Client pClient);

/* 设置地址重用选项 */
HPSOCKET_API void __HP_CALL HP_Client_SetReuseAddressPolicy(HP_Client pClient, En_HP_ReuseAddressPolicy enReusePolicy);
/* 设置内存块缓存池大小（通常设置为 -> PUSH 模型：5 - 10；PULL 模型：10 - 20 ） */
HPSOCKET_API void __HP_CALL HP_Client_SetFreeBufferPoolSize(HP_Client pClient, DWORD dwFreeBufferPoolSize);
/* 设置内存块缓存池回收阀值（通常设置为内存块缓存池大小的 3 倍） */
HPSOCKET_API void __HP_CALL HP_Client_SetFreeBufferPoolHold(HP_Client pClient, DWORD dwFreeBufferPoolHold);

/* 获取地址重用选项 */
HPSOCKET_API En_HP_ReuseAddressPolicy __HP_CALL HP_Client_GetReuseAddressPolicy(HP_Client pClient);
/* 获取内存块缓存池大小 */
HPSOCKET_API DWORD __HP_CALL HP_Client_GetFreeBufferPoolSize(HP_Client pClient);
/* 获取内存块缓存池回收阀值 */
HPSOCKET_API DWORD __HP_CALL HP_Client_GetFreeBufferPoolHold(HP_Client pClient);

/**********************************************************************************/
/******************************* TCP Client 操作方法 *******************************/

/*
* 名称：发送小文件
* 描述：向服务端发送 4096 KB 以下的小文件
*		
* 参数：		lpszFileName	-- 文件路径
*			pHead			-- 头部附加数据
*			pTail			-- 尾部附加数据
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 SYS_GetLastError() 获取 Windows 错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_TcpClient_SendSmallFile(HP_Client pClient, LPCTSTR lpszFileName, const LPWSABUF pHead, const LPWSABUF pTail);

/**********************************************************************************/
/***************************** TCP Client 属性访问方法 *****************************/

/* 设置同步连接超时时间（毫秒） */
HPSOCKET_API void __HP_CALL HP_TcpClient_SetSyncConnectTimeout(HP_TcpClient pClient, DWORD dwSyncConnectTimeout);
/* 设置通信数据缓冲区大小（根据平均通信数据包大小调整设置，通常设置为：(N * 1024) - sizeof(TBufferObj)） */
HPSOCKET_API void __HP_CALL HP_TcpClient_SetSocketBufferSize(HP_TcpClient pClient, DWORD dwSocketBufferSize);
/* 设置正常心跳包间隔（毫秒，0 则不发送心跳包，默认：60 * 1000） */
HPSOCKET_API void __HP_CALL HP_TcpClient_SetKeepAliveTime(HP_TcpClient pClient, DWORD dwKeepAliveTime);
/* 设置异常心跳包间隔（毫秒，0 不发送心跳包，，默认：20 * 1000，如果超过若干次 [默认：WinXP 5 次, Win7 10 次] 检测不到心跳确认包则认为已断线） */
HPSOCKET_API void __HP_CALL HP_TcpClient_SetKeepAliveInterval(HP_TcpClient pClient, DWORD dwKeepAliveInterval);
/* 设置是否开启 nodelay 模式（默认：FALSE，不开启） */
HPSOCKET_API void __HP_CALL HP_TcpClient_SetNoDelay(HP_TcpClient pClient, BOOL bNoDelay);

/* 获取同步连接超时时间 */
HPSOCKET_API DWORD __HP_CALL HP_TcpClient_GetSyncConnectTimeout(HP_TcpClient pClient);
/* 获取通信数据缓冲区大小 */
HPSOCKET_API DWORD __HP_CALL HP_TcpClient_GetSocketBufferSize(HP_TcpClient pClient);
/* 获取正常心跳包间隔 */
HPSOCKET_API DWORD __HP_CALL HP_TcpClient_GetKeepAliveTime(HP_TcpClient pClient);
/* 获取异常心跳包间隔 */
HPSOCKET_API DWORD __HP_CALL HP_TcpClient_GetKeepAliveInterval(HP_TcpClient pClient);
/* 检查是否开启 nodelay 模式 */
HPSOCKET_API BOOL __HP_CALL HP_TcpClient_IsNoDelay(HP_TcpClient pClient);

#ifdef _UDP_SUPPORT

/**********************************************************************************/
/***************************** UDP Client 属性访问方法 *****************************/

/* 设置数据报文最大长度（建议在局域网环境下不超过 1432 字节，在广域网环境下不超过 548 字节） */
HPSOCKET_API void __HP_CALL HP_UdpClient_SetMaxDatagramSize(HP_UdpClient pClient, DWORD dwMaxDatagramSize);
/* 获取数据报文最大长度 */
HPSOCKET_API DWORD __HP_CALL HP_UdpClient_GetMaxDatagramSize(HP_UdpClient pClient);

/* 设置监测包尝试次数（0 则不发送监测跳包，如果超过最大尝试次数则认为已断线） */
HPSOCKET_API void __HP_CALL HP_UdpClient_SetDetectAttempts(HP_UdpClient pClient, DWORD dwDetectAttempts);
/* 设置监测包发送间隔（毫秒，0 不发送监测包） */
HPSOCKET_API void __HP_CALL HP_UdpClient_SetDetectInterval(HP_UdpClient pClient, DWORD dwDetectInterval);
/* 获取心跳检查次数 */
HPSOCKET_API DWORD __HP_CALL HP_UdpClient_GetDetectAttempts(HP_UdpClient pClient);
/* 获取心跳检查间隔 */
HPSOCKET_API DWORD __HP_CALL HP_UdpClient_GetDetectInterval(HP_UdpClient pClient);

/**********************************************************************************/
/*************************** UDP ARQ Client 属性访问方法 ***************************/

/* 设置是否开启 nodelay 模式（默认：FALSE，不开启） */
HPSOCKET_API void __HP_CALL HP_UdpArqClient_SetNoDelay(HP_UdpArqClient pClient, BOOL bNoDelay);
/* 设置是否关闭拥塞控制（默认：FALSE，不关闭） */
HPSOCKET_API void __HP_CALL HP_UdpArqClient_SetTurnoffCongestCtrl(HP_UdpArqClient pClient, BOOL bTurnOff);
/* 设置数据刷新间隔（毫秒，默认：60） */
HPSOCKET_API void __HP_CALL HP_UdpArqClient_SetFlushInterval(HP_UdpArqClient pClient, DWORD dwFlushInterval);
/* 设置快速重传 ACK 跨越次数（默认：0，关闭快速重传） */
HPSOCKET_API void __HP_CALL HP_UdpArqClient_SetResendByAcks(HP_UdpArqClient pClient, DWORD dwResendByAcks);
/* 设置发送窗口大小（数据包数量，默认：128） */
HPSOCKET_API void __HP_CALL HP_UdpArqClient_SetSendWndSize(HP_UdpArqClient pClient, DWORD dwSendWndSize);
/* 设置接收窗口大小（数据包数量，默认：512） */
HPSOCKET_API void __HP_CALL HP_UdpArqClient_SetRecvWndSize(HP_UdpArqClient pClient, DWORD dwRecvWndSize);
/* 设置最小重传超时时间（毫秒，默认：30） */
HPSOCKET_API void __HP_CALL HP_UdpArqClient_SetMinRto(HP_UdpArqClient pClient, DWORD dwMinRto);
/* 设置快速握手次数限制（默认：5，如果为 0 则不限制） */
HPSOCKET_API void __HP_CALL HP_UdpArqClient_SetFastLimit(HP_UdpArqClient pClient, DWORD dwFastLimit);
/* 设置最大传输单元（默认：0，与 SetMaxDatagramSize() 一致） */
HPSOCKET_API void __HP_CALL HP_UdpArqClient_SetMaxTransUnit(HP_UdpArqClient pClient, DWORD dwMaxTransUnit);
/* 设置最大数据包大小（默认：4096） */
HPSOCKET_API void __HP_CALL HP_UdpArqClient_SetMaxMessageSize(HP_UdpArqClient pClient, DWORD dwMaxMessageSize);
/* 设置握手超时时间（毫秒，默认：5000） */
HPSOCKET_API void __HP_CALL HP_UdpArqClient_SetHandShakeTimeout(HP_UdpArqClient pClient, DWORD dwHandShakeTimeout);

/* 检测是否开启 nodelay 模式 */
HPSOCKET_API BOOL __HP_CALL HP_UdpArqClient_IsNoDelay(HP_UdpArqClient pClient);
/* 检测是否关闭拥塞控制 */
HPSOCKET_API BOOL __HP_CALL HP_UdpArqClient_IsTurnoffCongestCtrl(HP_UdpArqClient pClient);
/* 获取数据刷新间隔 */
HPSOCKET_API DWORD __HP_CALL HP_UdpArqClient_GetFlushInterval(HP_UdpArqClient pClient);
/* 获取快速重传 ACK 跨越次数 */
HPSOCKET_API DWORD __HP_CALL HP_UdpArqClient_GetResendByAcks(HP_UdpArqClient pClient);
/* 获取发送窗口大小 */
HPSOCKET_API DWORD __HP_CALL HP_UdpArqClient_GetSendWndSize(HP_UdpArqClient pClient);
/* 获取接收窗口大小 */
HPSOCKET_API DWORD __HP_CALL HP_UdpArqClient_GetRecvWndSize(HP_UdpArqClient pClient);
/* 获取最小重传超时时间 */
HPSOCKET_API DWORD __HP_CALL HP_UdpArqClient_GetMinRto(HP_UdpArqClient pClient);
/* 获取快速握手次数限制 */
HPSOCKET_API DWORD __HP_CALL HP_UdpArqClient_GetFastLimit(HP_UdpArqClient pClient);
/* 获取最大传输单元 */
HPSOCKET_API DWORD __HP_CALL HP_UdpArqClient_GetMaxTransUnit(HP_UdpArqClient pClient);
/* 获取最大数据包大小 */
HPSOCKET_API DWORD __HP_CALL HP_UdpArqClient_GetMaxMessageSize(HP_UdpArqClient pClient);
/* 获取握手超时时间 */
HPSOCKET_API DWORD __HP_CALL HP_UdpArqClient_GetHandShakeTimeout(HP_UdpArqClient pClient);

/* 获取等待发送包数量 */
HPSOCKET_API BOOL __HP_CALL HP_UdpArqClient_GetWaitingSendMessageCount(HP_UdpArqClient pClient, int* piCount);

/**********************************************************************************/
/****************************** UDP Cast 属性访问方法 ******************************/

/* 设置数据报文最大长度（建议在局域网环境下不超过 1432 字节，在广域网环境下不超过 548 字节） */
HPSOCKET_API void __HP_CALL HP_UdpCast_SetMaxDatagramSize(HP_UdpCast pCast, DWORD dwMaxDatagramSize);
/* 获取数据报文最大长度 */
HPSOCKET_API DWORD __HP_CALL HP_UdpCast_GetMaxDatagramSize(HP_UdpCast pCast);
/* 获取当前数据报的远程地址信息（通常在 OnReceive 事件中调用） */
HPSOCKET_API BOOL __HP_CALL HP_UdpCast_GetRemoteAddress(HP_UdpCast pCast, TCHAR lpszAddress[], int* piAddressLen, USHORT* pusPort);
/* 设置传播模式（组播或广播） */
HPSOCKET_API void __HP_CALL HP_UdpCast_SetCastMode(HP_UdpCast pCast, En_HP_CastMode enCastMode);
/* 获取传播模式 */
HPSOCKET_API En_HP_CastMode __HP_CALL HP_UdpCast_GetCastMode(HP_UdpCast pCast);
/* 设置组播报文的 TTL（0 - 255） */
HPSOCKET_API void __HP_CALL HP_UdpCast_SetMultiCastTtl(HP_UdpCast pCast, int iMCTtl);
/* 获取组播报文的 TTL */
HPSOCKET_API int __HP_CALL HP_UdpCast_GetMultiCastTtl(HP_UdpCast pCast);
/* 设置是否启用组播环路（TRUE or FALSE） */
HPSOCKET_API void __HP_CALL HP_UdpCast_SetMultiCastLoop(HP_UdpCast pCast, BOOL bMCLoop);
/* 检测是否启用组播环路 */
HPSOCKET_API BOOL __HP_CALL HP_UdpCast_IsMultiCastLoop(HP_UdpCast pCast);

/**********************************************************************************/
/****************************** UDP Node 组件操作方法 ******************************/

/*
* 名称：启动通信组件
* 描述：启动 UDP 节点通信组件，启动完成后可开始收发数据
*		
* 参数：		lpszBindAddress		-- 绑定地址（默认：nullptr，绑定任意地址）
*			usPort				-- 本地端口（默认：0）
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 GetLastError() 获取错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_UdpNode_Start(HP_UdpNode pNode, LPCTSTR lpszBindAddress, USHORT usPort);

/*
* 名称：启动通信组件
* 描述：启动 UDP 节点通信组件，启动完成后可开始收发数据
*		
* 参数：		lpszBindAddress		-- 绑定地址（默认：nullptr，绑定任意地址）
*			usPort				-- 本地端口（默认：0）
*			enCastMode			-- 传播模式（默认：CM_UNICAST）
*			lpszCastAddress		-- 传播地址（默认：nullptr，当 enCaseMode 为 CM_MULTICAST 或 CM_BROADCAST 时有效）
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 GetLastError() 获取错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_UdpNode_StartWithCast(HP_UdpNode pNode, LPCTSTR lpszBindAddress, USHORT usPort, En_HP_CastMode enCastMode, LPCTSTR lpszCastAddress);

/*
* 名称：关闭通信组件
* 描述：关闭 UDP 节点通信组件，关闭完成后释放所有资源
*		
* 参数：	
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 GetLastError() 获取错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_UdpNode_Stop(HP_UdpNode pNode);

/*
* 名称：等待
* 描述：等待通信组件停止运行
*
* 参数：		dwMilliseconds	-- 超时时间（毫秒，默认：-1，永不超时）
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 SYS_GetLastError() 获取错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_UdpNode_Wait(HP_UdpNode pNode, DWORD dwMilliseconds);

/*
* 名称：发送数据
* 描述：向指定地址发送数据
*		
* 参数：		lpszRemoteAddress	-- 远程地址
*			usRemotePort		-- 远程端口
*			pBuffer				-- 发送缓冲区
*			iLength				-- 发送缓冲区长度
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 SYS_GetLastError() 获取错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_UdpNode_Send(HP_UdpNode pNode, LPCTSTR lpszRemoteAddress, USHORT usRemotePort, const BYTE* pBuffer, int iLength);

/*
* 名称：发送数据
* 描述：向指定地址发送数据
*		
* 参数：		lpszRemoteAddress	-- 远程地址
*			usRemotePort		-- 远程端口
*			pBuffer				-- 发送缓冲区
*			iLength				-- 发送缓冲区长度
*			iOffset				-- 发送缓冲区指针偏移量
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 SYS_GetLastError() 获取错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_UdpNode_SendPart(HP_UdpNode pNode, LPCTSTR lpszRemoteAddress, USHORT usRemotePort, const BYTE* pBuffer, int iLength, int iOffset);

/*
* 名称：发送多组数据
* 描述：向指定地址发送多组数据，把所有数据包组合成一个数据包发送（数据包的总长度不能大于设置的 UDP 包最大长度） 
*		
* 参数：		lpszRemoteAddress	-- 远程地址
*			usRemotePort		-- 远程端口
*			pBuffers			-- 发送缓冲区数组
*			iCount				-- 发送缓冲区数目
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 SYS_GetLastError() 获取错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_UdpNode_SendPackets(HP_UdpNode pNode, LPCTSTR lpszRemoteAddress, USHORT usRemotePort, const WSABUF pBuffers[], int iCount);

/*
* 名称：发送数据
* 描述：向传播地址发送数据
*		
* 参数：		pBuffer		-- 发送缓冲区
*			iLength		-- 发送缓冲区长度
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 SYS_GetLastError() 获取错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_UdpNode_SendCast(HP_UdpNode pNode, const BYTE* pBuffer, int iLength);

/*
* 名称：发送数据
* 描述：向传播地址发送数据
*		
* 参数：		pBuffer		-- 发送缓冲区
*			iLength		-- 发送缓冲区长度
*			iOffset		-- 发送缓冲区指针偏移量
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 SYS_GetLastError() 获取错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_UdpNode_SendCastPart(HP_UdpNode pNode, const BYTE* pBuffer, int iLength, int iOffset);

/*
* 名称：发送多组数据
* 描述：向传播地址发送多组数据，把所有数据包组合成一个数据包发送（数据包的总长度不能大于设置的 UDP 包最大长度） 
*		
* 参数：		pBuffers	-- 发送缓冲区数组
*			iCount		-- 发送缓冲区数目
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 SYS_GetLastError() 获取错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_UdpNode_SendCastPackets(HP_UdpNode pNode, const WSABUF pBuffers[], int iCount);

/**********************************************************************************/
/****************************** UDP Node 属性访问方法 ******************************/

/* 设置附加数据 */
HPSOCKET_API void __HP_CALL HP_UdpNode_SetExtra(HP_UdpNode pNode, PVOID pExtra);

/* 获取附加数据 */
HPSOCKET_API PVOID __HP_CALL HP_UdpNode_GetExtra(HP_UdpNode pNode);

/* 检查通信组件是否已启动 */
HPSOCKET_API BOOL __HP_CALL HP_UdpNode_HasStarted(HP_UdpNode pNode);
/* 查看通信组件当前状态 */
HPSOCKET_API En_HP_ServiceState __HP_CALL HP_UdpNode_GetState(HP_UdpNode pNode);
/* 获取最近一次失败操作的错误代码 */
HPSOCKET_API En_HP_SocketError __HP_CALL HP_UdpNode_GetLastError(HP_UdpNode pNode);
/* 获取最近一次失败操作的错误描述 */
HPSOCKET_API LPCTSTR __HP_CALL HP_UdpNode_GetLastErrorDesc(HP_UdpNode pNode);
/* 获取本节点地址 */
HPSOCKET_API BOOL __HP_CALL HP_UdpNode_GetLocalAddress(HP_UdpNode pNode, TCHAR lpszAddress[], int* piAddressLen, USHORT* pusPort);
/* 获取本节点传播地址 */
HPSOCKET_API BOOL __HP_CALL HP_UdpNode_GetCastAddress(HP_UdpNode pNode, TCHAR lpszAddress[], int* piAddressLen, USHORT* pusPort);
/* 获取传播模式 */
HPSOCKET_API En_HP_CastMode __HP_CALL HP_UdpNode_GetCastMode(HP_UdpNode pNode);
/* 获取未发出数据的长度 */
HPSOCKET_API BOOL __HP_CALL HP_UdpNode_GetPendingDataLength(HP_UdpNode pNode, int* piPending);

/* 设置数据报文最大长度（建议在局域网环境下不超过 1432 字节，在广域网环境下不超过 548 字节） */
HPSOCKET_API void __HP_CALL HP_UdpNode_SetMaxDatagramSize(HP_UdpNode pNode, DWORD dwMaxDatagramSize);
/* 获取数据报文最大长度 */
HPSOCKET_API DWORD __HP_CALL HP_UdpNode_GetMaxDatagramSize(HP_UdpNode pNode);

/* 设置组播报文的 TTL（0 - 255） */
HPSOCKET_API void __HP_CALL HP_UdpNode_SetMultiCastTtl(HP_UdpNode pNode, int iMCTtl);
/* 获取组播报文的 TTL */
HPSOCKET_API int __HP_CALL HP_UdpNode_GetMultiCastTtl(HP_UdpNode pNode);

/* 设置是否启用组播环路（TRUE or FALSE） */
HPSOCKET_API void __HP_CALL HP_UdpNode_SetMultiCastLoop(HP_UdpNode pNode, BOOL bMCLoop);
/* 检测是否启用组播环路 */
HPSOCKET_API BOOL __HP_CALL HP_UdpNode_IsMultiCastLoop(HP_UdpNode pNode);

/* 设置地址重用选项 */
HPSOCKET_API void __HP_CALL HP_UdpNode_SetReuseAddressPolicy(HP_UdpNode pNode, En_HP_ReuseAddressPolicy enReusePolicy);
/* 设置工作线程数量（通常设置为 2 * CPU + 2） */
HPSOCKET_API void __HP_CALL HP_UdpNode_SetWorkerThreadCount(HP_UdpNode pNode, DWORD dwWorkerThreadCount);
/* 设置 Receive 预投递数量（根据负载调整设置，Receive 预投递数量越大则丢包概率越小） */
HPSOCKET_API void __HP_CALL HP_UdpNode_SetPostReceiveCount(HP_UdpNode pNode, DWORD dwPostReceiveCount);
/* 设置内存块缓存池大小 */
HPSOCKET_API void __HP_CALL HP_UdpNode_SetFreeBufferPoolSize(HP_UdpNode pNode, DWORD dwFreeBufferPoolSize);
/* 设置内存块缓存池回收阀值 */
HPSOCKET_API void __HP_CALL HP_UdpNode_SetFreeBufferPoolHold(HP_UdpNode pNode, DWORD dwFreeBufferPoolHold);

/* 获取地址重用选项 */
HPSOCKET_API En_HP_ReuseAddressPolicy __HP_CALL HP_UdpNode_GetReuseAddressPolicy(HP_UdpNode pNode);
/* 获取工作线程数量 */
HPSOCKET_API DWORD __HP_CALL HP_UdpNode_GetWorkerThreadCount(HP_UdpNode pNode);
/* 获取 Receive 预投递数量 */
HPSOCKET_API DWORD __HP_CALL HP_UdpNode_GetPostReceiveCount(HP_UdpNode pNode);
/* 获取内存块缓存池大小 */
HPSOCKET_API DWORD __HP_CALL HP_UdpNode_GetFreeBufferPoolSize(HP_UdpNode pNode);
/* 获取内存块缓存池回收阀值 */
HPSOCKET_API DWORD __HP_CALL HP_UdpNode_GetFreeBufferPoolHold(HP_UdpNode pNode);

#endif

/***************************************************************************************/
/***************************** TCP Pull Server 组件操作方法 *****************************/

/*
* 名称：抓取数据
* 描述：用户通过该方法从 Socket 组件中抓取数据
*		
* 参数：		dwConnID	-- 连接 ID
*			pData		-- 抓取缓冲区
*			iLength		-- 抓取数据长度
* 返回值：	En_HP_FetchResult
*/
HPSOCKET_API En_HP_FetchResult __HP_CALL HP_TcpPullServer_Fetch(HP_TcpPullServer pServer, HP_CONNID dwConnID, BYTE* pData, int iLength);

/*
* 名称：窥探数据（不会移除缓冲区数据）
* 描述：用户通过该方法从 Socket 组件中窥探数据
*		
* 参数：		dwConnID	-- 连接 ID
*			pData		-- 窥探缓冲区
*			iLength		-- 窥探数据长度
* 返回值：	En_HP_FetchResult
*/
HPSOCKET_API En_HP_FetchResult __HP_CALL HP_TcpPullServer_Peek(HP_TcpPullServer pServer, HP_CONNID dwConnID, BYTE* pData, int iLength);

/***************************************************************************************/
/***************************** TCP Pull Server 属性访问方法 *****************************/

/***************************************************************************************/
/***************************** TCP Pull Agent 组件操作方法 *****************************/

/*
* 名称：抓取数据
* 描述：用户通过该方法从 Socket 组件中抓取数据
*		
* 参数：		dwConnID	-- 连接 ID
*			pData		-- 抓取缓冲区
*			iLength		-- 抓取数据长度
* 返回值：	En_HP_FetchResult
*/
HPSOCKET_API En_HP_FetchResult __HP_CALL HP_TcpPullAgent_Fetch(HP_TcpPullAgent pAgent, HP_CONNID dwConnID, BYTE* pData, int iLength);

/*
* 名称：窥探数据（不会移除缓冲区数据）
* 描述：用户通过该方法从 Socket 组件中窥探数据
*		
* 参数：		dwConnID	-- 连接 ID
*			pData		-- 窥探缓冲区
*			iLength		-- 窥探数据长度
* 返回值：	En_HP_FetchResult
*/
HPSOCKET_API En_HP_FetchResult __HP_CALL HP_TcpPullAgent_Peek(HP_TcpPullAgent pAgent, HP_CONNID dwConnID, BYTE* pData, int iLength);

/***************************************************************************************/
/***************************** TCP Pull Agent 属性访问方法 *****************************/

/***************************************************************************************/
/***************************** TCP Pull Client 组件操作方法 *****************************/

/*
* 名称：抓取数据
* 描述：用户通过该方法从 Socket 组件中抓取数据
*		
* 参数：		dwConnID	-- 连接 ID
*			pData		-- 抓取缓冲区
*			iLength		-- 抓取数据长度
* 返回值：	En_HP_FetchResult
*/
HPSOCKET_API En_HP_FetchResult __HP_CALL HP_TcpPullClient_Fetch(HP_TcpPullClient pClient, BYTE* pData, int iLength);

/*
* 名称：窥探数据（不会移除缓冲区数据）
* 描述：用户通过该方法从 Socket 组件中窥探数据
*		
* 参数：		dwConnID	-- 连接 ID
*			pData		-- 窥探缓冲区
*			iLength		-- 窥探数据长度
* 返回值：	En_HP_FetchResult
*/
HPSOCKET_API En_HP_FetchResult __HP_CALL HP_TcpPullClient_Peek(HP_TcpPullClient pClient, BYTE* pData, int iLength);

/***************************************************************************************/
/***************************** TCP Pull Client 属性访问方法 *****************************/

/***************************************************************************************/
/***************************** TCP Pack Server 组件操作方法 *****************************/

/***************************************************************************************/
/***************************** TCP Pack Server 属性访问方法 *****************************/

/* 设置数据包最大长度（有效数据包最大长度不能超过 4194303/0x3FFFFF 字节，默认：262144/0x40000） */
HPSOCKET_API void __HP_CALL HP_TcpPackServer_SetMaxPackSize(HP_TcpPackServer pServer, DWORD dwMaxPackSize);
/* 设置包头标识（有效包头标识取值范围 0 ~ 1023/0x3FF，当包头标识为 0 时不校验包头，默认：0） */
HPSOCKET_API void __HP_CALL HP_TcpPackServer_SetPackHeaderFlag(HP_TcpPackServer pServer, USHORT usPackHeaderFlag);

/* 获取数据包最大长度 */
HPSOCKET_API DWORD __HP_CALL HP_TcpPackServer_GetMaxPackSize(HP_TcpPackServer pServer);
/* 获取包头标识 */
HPSOCKET_API USHORT __HP_CALL HP_TcpPackServer_GetPackHeaderFlag(HP_TcpPackServer pServer);

/***************************************************************************************/
/***************************** TCP Pack Agent 组件操作方法 *****************************/

/***************************************************************************************/
/***************************** TCP Pack Agent 属性访问方法 *****************************/

/* 设置数据包最大长度（有效数据包最大长度不能超过 4194303/0x3FFFFF 字节，默认：262144/0x40000） */
HPSOCKET_API void __HP_CALL HP_TcpPackAgent_SetMaxPackSize(HP_TcpPackAgent pAgent, DWORD dwMaxPackSize);
/* 设置包头标识（有效包头标识取值范围 0 ~ 1023/0x3FF，当包头标识为 0 时不校验包头，默认：0） */
HPSOCKET_API void __HP_CALL HP_TcpPackAgent_SetPackHeaderFlag(HP_TcpPackAgent pAgent, USHORT usPackHeaderFlag);

/* 获取数据包最大长度 */
HPSOCKET_API DWORD __HP_CALL HP_TcpPackAgent_GetMaxPackSize(HP_TcpPackAgent pAgent);
/* 获取包头标识 */
HPSOCKET_API USHORT __HP_CALL HP_TcpPackAgent_GetPackHeaderFlag(HP_TcpPackAgent pAgent);

/***************************************************************************************/
/***************************** TCP Pack Client 组件操作方法 *****************************/

/***************************************************************************************/
/***************************** TCP Pack Client 属性访问方法 *****************************/

/* 设置数据包最大长度（有效数据包最大长度不能超过 4194303/0x3FFFFF 字节，默认：262144/0x40000） */
HPSOCKET_API void __HP_CALL HP_TcpPackClient_SetMaxPackSize(HP_TcpPackClient pClient, DWORD dwMaxPackSize);
/* 设置包头标识（有效包头标识取值范围 0 ~ 1023/0x3FF，当包头标识为 0 时不校验包头，默认：0） */
HPSOCKET_API void __HP_CALL HP_TcpPackClient_SetPackHeaderFlag(HP_TcpPackClient pClient, USHORT usPackHeaderFlag);

/* 获取数据包最大长度 */
HPSOCKET_API DWORD __HP_CALL HP_TcpPackClient_GetMaxPackSize(HP_TcpPackClient pClient);
/* 获取包头标识 */
HPSOCKET_API USHORT __HP_CALL HP_TcpPackClient_GetPackHeaderFlag(HP_TcpPackClient pClient);

/*****************************************************************************************************************************************************/
/*************************************************************** Global Function Exports *************************************************************/
/*****************************************************************************************************************************************************/

// 获取 HPSocket 版本号（4 个字节分别为：主版本号，子版本号，修正版本号，构建编号）
HPSOCKET_API DWORD __HP_CALL HP_GetHPSocketVersion();

/* 获取错误描述文本 */
HPSOCKET_API LPCTSTR __HP_CALL HP_GetSocketErrorDesc(En_HP_SocketError enCode);
/* 调用系统的 GetLastError() 方法获取系统错误代码 */
HPSOCKET_API DWORD __HP_CALL SYS_GetLastError();
// 调用系统的 WSAGetLastError() 方法获取系统错误代码
HPSOCKET_API int __HP_CALL SYS_WSAGetLastError();
// 调用系统的 setsockopt()
HPSOCKET_API int __HP_CALL SYS_SetSocketOption(SOCKET sock, int level, int name, LPVOID val, int len);
// 调用系统的 getsockopt()
HPSOCKET_API int __HP_CALL SYS_GetSocketOption(SOCKET sock, int level, int name, LPVOID val, int* len);
// 调用系统的 ioctlsocket()
HPSOCKET_API int __HP_CALL SYS_IoctlSocket(SOCKET sock, long cmd, ULONG* arg);
// 调用系统的 WSAIoctl()
HPSOCKET_API int __HP_CALL SYS_WSAIoctl(SOCKET sock, DWORD dwIoControlCode, LPVOID lpvInBuffer, DWORD cbInBuffer, LPVOID lpvOutBuffer, DWORD cbOutBuffer, LPDWORD lpcbBytesReturned);

// 设置 socket 选项：IPPROTO_TCP -> TCP_NODELAY
HPSOCKET_API int __HP_CALL SYS_SSO_NoDelay(SOCKET sock, BOOL bNoDelay);
// 设置 socket 选项：SOL_SOCKET -> SO_DONTLINGER
HPSOCKET_API int __HP_CALL SYS_SSO_DontLinger(SOCKET sock, BOOL bDont);
// 设置 socket 选项：SOL_SOCKET -> SO_LINGER
HPSOCKET_API int __HP_CALL SYS_SSO_Linger(SOCKET sock, USHORT l_onoff, USHORT l_linger);
// 设置 socket 选项：SOL_SOCKET -> SO_RCVBUF
HPSOCKET_API int __HP_CALL SYS_SSO_RecvBuffSize(SOCKET sock, int size);
// 设置 socket 选项：SOL_SOCKET -> SO_SNDBUF
HPSOCKET_API int __HP_CALL SYS_SSO_SendBuffSize(SOCKET sock, int size);
// 设置 socket 选项：SOL_SOCKET -> SO_RCVTIMEO
HPSOCKET_API int __HP_CALL SYS_SSO_RecvTimeOut(SOCKET sock, int ms);
// 设置 socket 选项：SOL_SOCKET -> SO_SNDTIMEO
HPSOCKET_API int __HP_CALL SYS_SSO_SendTimeOut(SOCKET sock, int ms);
// 设置 socket 选项：SOL_SOCKET -> SO_EXCLUSIVEADDRUSE / SO_REUSEADDR
HPSOCKET_API int __HP_CALL SYS_SSO_ReuseAddress(SOCKET sock, En_HP_ReuseAddressPolicy opt);
// 设置 socket 选项：SOL_SOCKET -> SO_EXCLUSIVEADDRUSE
HPSOCKET_API int __HP_CALL SYS_SSO_ExclusiveAddressUse(SOCKET sock, BOOL bExclusive);

// 获取 SOCKET 本地地址信息
HPSOCKET_API BOOL __HP_CALL SYS_GetSocketLocalAddress(SOCKET socket, TCHAR lpszAddress[], int* piAddressLen, USHORT* pusPort);
// 获取 SOCKET 远程地址信息
HPSOCKET_API BOOL __HP_CALL SYS_GetSocketRemoteAddress(SOCKET socket, TCHAR lpszAddress[], int* piAddressLen, USHORT* pusPort);

/* 枚举主机 IP 地址 */
HPSOCKET_API BOOL __HP_CALL SYS_EnumHostIPAddresses(LPCTSTR lpszHost, En_HP_IPAddrType enType, HP_LPTIPAddr** lpppIPAddr, int* piIPAddrCount);
/* 释放 HP_LPTIPAddr* */
HPSOCKET_API BOOL __HP_CALL SYS_FreeHostIPAddresses(HP_LPTIPAddr* lppIPAddr);
/* 检查字符串是否符合 IP 地址格式 */
HPSOCKET_API BOOL __HP_CALL SYS_IsIPAddress(LPCTSTR lpszAddress, En_HP_IPAddrType* penType);
/* 通过主机名获取 IP 地址 */
HPSOCKET_API BOOL __HP_CALL SYS_GetIPAddress(LPCTSTR lpszHost, TCHAR lpszIP[], int* piIPLenth, En_HP_IPAddrType* penType);

/* 64 位网络字节序转主机字节序 */
HPSOCKET_API ULONGLONG __HP_CALL SYS_NToH64(ULONGLONG value);
/* 64 位主机字节序转网络字节序 */
HPSOCKET_API ULONGLONG __HP_CALL SYS_HToN64(ULONGLONG value);
/* 短整型高低字节交换 */
HPSOCKET_API USHORT __HP_CALL SYS_SwapEndian16(USHORT value);
/* 长整型高低字节交换 */
HPSOCKET_API DWORD __HP_CALL SYS_SwapEndian32(DWORD value);
/* 检查是否小端字节序 */
HPSOCKET_API BOOL __HP_CALL SYS_IsLittleEndian();

/* 分配内存 */
HPSOCKET_API LPBYTE __HP_CALL SYS_Malloc(int size);
/* 重新分配内存 */
HPSOCKET_API LPBYTE __HP_CALL SYS_Realloc(LPBYTE p, int size);
/* 释放内存 */
HPSOCKET_API VOID __HP_CALL SYS_Free(LPBYTE p);
/* 分配内存块 */
HPSOCKET_API LPVOID __HP_CALL SYS_Calloc(int number, int size);

// CP_XXX -> UNICODE
HPSOCKET_API BOOL __HP_CALL SYS_CodePageToUnicodeEx(int iCodePage, const char szSrc[], int iSrcLength, WCHAR szDest[], int* piDestLength);
// UNICODE -> CP_XXX
HPSOCKET_API BOOL __HP_CALL SYS_UnicodeToCodePageEx(int iCodePage, const WCHAR szSrc[], int iSrcLength, char szDest[], int* piDestLength);
// GBK -> UNICODE
HPSOCKET_API BOOL __HP_CALL SYS_GbkToUnicodeEx(const char szSrc[], int iSrcLength, WCHAR szDest[], int* piDestLength);
// UNICODE -> GBK
HPSOCKET_API BOOL __HP_CALL SYS_UnicodeToGbkEx(const WCHAR szSrc[], int iSrcLength, char szDest[], int* piDestLength);
// UTF8 -> UNICODE
HPSOCKET_API BOOL __HP_CALL SYS_Utf8ToUnicodeEx(const char szSrc[], int iSrcLength, WCHAR szDest[], int* piDestLength);
// UNICODE -> UTF8
HPSOCKET_API BOOL __HP_CALL SYS_UnicodeToUtf8Ex(const WCHAR szSrc[], int iSrcLength, char szDest[], int* piDestLength);
// GBK -> UTF8
HPSOCKET_API BOOL __HP_CALL SYS_GbkToUtf8Ex(const char szSrc[], int iSrcLength, char szDest[], int* piDestLength);
// UTF8 -> GBK
HPSOCKET_API BOOL __HP_CALL SYS_Utf8ToGbkEx(const char szSrc[], int iSrcLength, char szDest[], int* piDestLength);

// CP_XXX -> UNICODE
HPSOCKET_API BOOL __HP_CALL SYS_CodePageToUnicode(int iCodePage, const char szSrc[], WCHAR szDest[], int* piDestLength);
// UNICODE -> CP_XXX
HPSOCKET_API BOOL __HP_CALL SYS_UnicodeToCodePage(int iCodePage, const WCHAR szSrc[], char szDest[], int* piDestLength);
// GBK -> UNICODE
HPSOCKET_API BOOL __HP_CALL SYS_GbkToUnicode(const char szSrc[], WCHAR szDest[], int* piDestLength);
// UNICODE -> GBK
HPSOCKET_API BOOL __HP_CALL SYS_UnicodeToGbk(const WCHAR szSrc[], char szDest[], int* piDestLength);
// UTF8 -> UNICODE
HPSOCKET_API BOOL __HP_CALL SYS_Utf8ToUnicode(const char szSrc[], WCHAR szDest[], int* piDestLength);
// UNICODE -> UTF8
HPSOCKET_API BOOL __HP_CALL SYS_UnicodeToUtf8(const WCHAR szSrc[], char szDest[], int* piDestLength);
// GBK -> UTF8
HPSOCKET_API BOOL __HP_CALL SYS_GbkToUtf8(const char szSrc[], char szDest[], int* piDestLength);
// UTF8 -> GBK
HPSOCKET_API BOOL __HP_CALL SYS_Utf8ToGbk(const char szSrc[], char szDest[], int* piDestLength);

// 计算 Base64 编码后长度
HPSOCKET_API DWORD __HP_CALL SYS_GuessBase64EncodeBound(DWORD dwSrcLen);
// 计算 Base64 解码后长度
HPSOCKET_API DWORD __HP_CALL SYS_GuessBase64DecodeBound(const BYTE* lpszSrc, DWORD dwSrcLen);
// Base64 编码（返回值：0 -> 成功，-3 -> 输入数据不正确，-5 -> 输出缓冲区不足）
HPSOCKET_API int __HP_CALL SYS_Base64Encode(const BYTE* lpszSrc, DWORD dwSrcLen, BYTE* lpszDest, DWORD* pdwDestLen);
// Base64 解码（返回值：0 -> 成功，-3 -> 输入数据不正确，-5 -> 输出缓冲区不足）
HPSOCKET_API int __HP_CALL SYS_Base64Decode(const BYTE* lpszSrc, DWORD dwSrcLen, BYTE* lpszDest, DWORD* pdwDestLen);

// 计算 URL 编码后长度
HPSOCKET_API DWORD __HP_CALL SYS_GuessUrlEncodeBound(const BYTE* lpszSrc, DWORD dwSrcLen);
// 计算 URL 解码后长度
HPSOCKET_API DWORD __HP_CALL SYS_GuessUrlDecodeBound(const BYTE* lpszSrc, DWORD dwSrcLen);
// URL 编码（返回值：0 -> 成功，-3 -> 输入数据不正确，-5 -> 输出缓冲区不足）
HPSOCKET_API int __HP_CALL SYS_UrlEncode(BYTE* lpszSrc, DWORD dwSrcLen, BYTE* lpszDest, DWORD* pdwDestLen);
// URL 解码（返回值：0 -> 成功，-3 -> 输入数据不正确，-5 -> 输出缓冲区不足）
HPSOCKET_API int __HP_CALL SYS_UrlDecode(BYTE* lpszSrc, DWORD dwSrcLen, BYTE* lpszDest, DWORD* pdwDestLen);

#ifdef _ZLIB_SUPPORT

// 普通压缩（返回值：0 -> 成功，-3 -> 输入数据不正确，-5 -> 输出缓冲区不足）
//（默认参数：iLevel -> -1，iMethod -> 8，iWindowBits -> 15，iMemLevel -> 8，iStrategy -> 0）
HPSOCKET_API int __HP_CALL SYS_Compress(const BYTE* lpszSrc, DWORD dwSrcLen, BYTE* lpszDest, DWORD* pdwDestLen);
// 高级压缩（返回值：0 -> 成功，-3 -> 输入数据不正确，-5 -> 输出缓冲区不足）
HPSOCKET_API int __HP_CALL SYS_CompressEx(const BYTE* lpszSrc, DWORD dwSrcLen, BYTE* lpszDest, DWORD* pdwDestLen, int iLevel /*= -1*/, int iMethod /*= 8*/, int iWindowBits /*= 15*/, int iMemLevel /*= 8*/, int iStrategy /*= 0*/);
// 普通解压（返回值：0 -> 成功，-3 -> 输入数据不正确，-5 -> 输出缓冲区不足）
// （默认参数：iWindowBits -> 15）
HPSOCKET_API int __HP_CALL SYS_Uncompress(const BYTE* lpszSrc, DWORD dwSrcLen, BYTE* lpszDest, DWORD* pdwDestLen);
// 高级解压（返回值：0 -> 成功，-3 -> 输入数据不正确，-5 -> 输出缓冲区不足）
HPSOCKET_API int __HP_CALL SYS_UncompressEx(const BYTE* lpszSrc, DWORD dwSrcLen, BYTE* lpszDest, DWORD* pdwDestLen, int iWindowBits /*= 15*/);
// 推测压缩结果长度
HPSOCKET_API DWORD __HP_CALL SYS_GuessCompressBound(DWORD dwSrcLen, BOOL bGZip);

// Gzip 压缩（返回值：0 -> 成功，-3 -> 输入数据不正确，-5 -> 输出缓冲区不足）
HPSOCKET_API int __HP_CALL SYS_GZipCompress(const BYTE* lpszSrc, DWORD dwSrcLen, BYTE* lpszDest, DWORD* pdwDestLen);
// Gzip 解压（返回值：0 -> 成功，-3 -> 输入数据不正确，-5 -> 输出缓冲区不足）
HPSOCKET_API int __HP_CALL SYS_GZipUncompress(const BYTE* lpszSrc, DWORD dwSrcLen, BYTE* lpszDest, DWORD* pdwDestLen);
// 推测 Gzip 解压结果长度（如果返回 0 或不合理值则说明输入内容并非有效的 Gzip 格式）
HPSOCKET_API DWORD __HP_CALL SYS_GZipGuessUncompressBound(const BYTE* lpszSrc, DWORD dwSrcLen);

#endif

#ifdef _BROTLI_SUPPORT

// Brotli 压缩（返回值：0 -> 成功，-3 -> 输入数据不正确，-5 -> 输出缓冲区不足）
//（默认参数：iQuality -> 11，iWindow -> 22，iMode -> 0）
HPSOCKET_API int __HP_CALL SYS_BrotliCompress(const BYTE* lpszSrc, DWORD dwSrcLen, BYTE* lpszDest, DWORD* pdwDestLen);
// Brotli 高级压缩（返回值：0 -> 成功，-3 -> 输入数据不正确，-5 -> 输出缓冲区不足）
HPSOCKET_API int __HP_CALL SYS_BrotliCompressEx(const BYTE* lpszSrc, DWORD dwSrcLen, BYTE* lpszDest, DWORD* pdwDestLen, int iQuality /*= 11*/, int iWindow /*= 22*/, int iMode /*= 0*/);
// Brotli 解压（返回值：0 -> 成功，-3 -> 输入数据不正确，-5 -> 输出缓冲区不足）
HPSOCKET_API int __HP_CALL SYS_BrotliUncompress(const BYTE* lpszSrc, DWORD dwSrcLen, BYTE* lpszDest, DWORD* pdwDestLen);
// Brotli 推测压缩结果长度
HPSOCKET_API DWORD __HP_CALL SYS_BrotliGuessCompressBound(DWORD dwSrcLen);

#endif

/*****************************************************************************************************************************************************/
/******************************************************************** HTTP Exports *******************************************************************/
/*****************************************************************************************************************************************************/

#ifdef _HTTP_SUPPORT

/****************************************************/
/******************* HTTP 回调函数 *******************/

/* HTTP 回调函数 */
typedef En_HP_HttpParseResult (__HP_CALL *HP_FN_Http_OnMessageBegin)	(HP_Http pSender, HP_CONNID dwConnID);
typedef En_HP_HttpParseResult (__HP_CALL *HP_FN_Http_OnRequestLine)		(HP_Http pSender, HP_CONNID dwConnID, LPCSTR lpszMethod, LPCSTR lpszUrl);
typedef En_HP_HttpParseResult (__HP_CALL *HP_FN_Http_OnStatusLine)		(HP_Http pSender, HP_CONNID dwConnID, USHORT usStatusCode, LPCSTR lpszDesc);
typedef En_HP_HttpParseResult (__HP_CALL *HP_FN_Http_OnHeader)			(HP_Http pSender, HP_CONNID dwConnID, LPCSTR lpszName, LPCSTR lpszValue);
typedef En_HP_HttpParseResult (__HP_CALL *HP_FN_Http_OnHeadersComplete)	(HP_Http pSender, HP_CONNID dwConnID);
typedef En_HP_HttpParseResult (__HP_CALL *HP_FN_Http_OnBody)			(HP_Http pSender, HP_CONNID dwConnID, const BYTE* pData, int iLength);
typedef En_HP_HttpParseResult (__HP_CALL *HP_FN_Http_OnChunkHeader)		(HP_Http pSender, HP_CONNID dwConnID, int iLength);
typedef En_HP_HttpParseResult (__HP_CALL *HP_FN_Http_OnChunkComplete)	(HP_Http pSender, HP_CONNID dwConnID);
typedef En_HP_HttpParseResult (__HP_CALL *HP_FN_Http_OnMessageComplete)	(HP_Http pSender, HP_CONNID dwConnID);
typedef En_HP_HttpParseResult (__HP_CALL *HP_FN_Http_OnUpgrade)			(HP_Http pSender, HP_CONNID dwConnID, En_HP_HttpUpgradeType enUpgradeType);
typedef En_HP_HttpParseResult (__HP_CALL *HP_FN_Http_OnParseError)		(HP_Http pSender, HP_CONNID dwConnID, int iErrorCode, LPCSTR lpszErrorDesc);

typedef En_HP_HandleResult	 (__HP_CALL *HP_FN_Http_OnWSMessageHeader)	(HP_Http pSender, HP_CONNID dwConnID, BOOL bFinal, BYTE iReserved, BYTE iOperationCode, const BYTE lpszMask[4], ULONGLONG ullBodyLen);
typedef En_HP_HandleResult	 (__HP_CALL *HP_FN_Http_OnWSMessageBody)	(HP_Http pSender, HP_CONNID dwConnID, const BYTE* pData, int iLength);
typedef En_HP_HandleResult	 (__HP_CALL *HP_FN_Http_OnWSMessageComplete)(HP_Http pSender, HP_CONNID dwConnID);

/* HTTP Server 回调函数 */
typedef HP_FN_Http_OnMessageBegin			HP_FN_HttpServer_OnMessageBegin;
typedef HP_FN_Http_OnRequestLine			HP_FN_HttpServer_OnRequestLine;
typedef HP_FN_Http_OnHeader					HP_FN_HttpServer_OnHeader;
typedef HP_FN_Http_OnHeadersComplete		HP_FN_HttpServer_OnHeadersComplete;
typedef HP_FN_Http_OnBody					HP_FN_HttpServer_OnBody;
typedef HP_FN_Http_OnChunkHeader			HP_FN_HttpServer_OnChunkHeader;
typedef HP_FN_Http_OnChunkComplete			HP_FN_HttpServer_OnChunkComplete;
typedef HP_FN_Http_OnMessageComplete		HP_FN_HttpServer_OnMessageComplete;
typedef HP_FN_Http_OnUpgrade				HP_FN_HttpServer_OnUpgrade;
typedef HP_FN_Http_OnParseError				HP_FN_HttpServer_OnParseError;

typedef HP_FN_Http_OnWSMessageHeader		HP_FN_HttpServer_OnWSMessageHeader;
typedef HP_FN_Http_OnWSMessageBody			HP_FN_HttpServer_OnWSMessageBody;
typedef HP_FN_Http_OnWSMessageComplete		HP_FN_HttpServer_OnWSMessageComplete;

typedef HP_FN_Server_OnPrepareListen		HP_FN_HttpServer_OnPrepareListen;
typedef HP_FN_Server_OnAccept				HP_FN_HttpServer_OnAccept;
typedef HP_FN_Server_OnHandShake			HP_FN_HttpServer_OnHandShake;
typedef HP_FN_Server_OnReceive				HP_FN_HttpServer_OnReceive;
typedef HP_FN_Server_OnSend					HP_FN_HttpServer_OnSend;
typedef HP_FN_Server_OnClose				HP_FN_HttpServer_OnClose;
typedef HP_FN_Server_OnShutdown				HP_FN_HttpServer_OnShutdown;

/* HTTP Agent 回调函数 */
typedef HP_FN_Http_OnMessageBegin			HP_FN_HttpAgent_OnMessageBegin;
typedef HP_FN_Http_OnStatusLine				HP_FN_HttpAgent_OnStatusLine;
typedef HP_FN_Http_OnHeader					HP_FN_HttpAgent_OnHeader;
typedef HP_FN_Http_OnHeadersComplete		HP_FN_HttpAgent_OnHeadersComplete;
typedef HP_FN_Http_OnBody					HP_FN_HttpAgent_OnBody;
typedef HP_FN_Http_OnChunkHeader			HP_FN_HttpAgent_OnChunkHeader;
typedef HP_FN_Http_OnChunkComplete			HP_FN_HttpAgent_OnChunkComplete;
typedef HP_FN_Http_OnMessageComplete		HP_FN_HttpAgent_OnMessageComplete;
typedef HP_FN_Http_OnUpgrade				HP_FN_HttpAgent_OnUpgrade;
typedef HP_FN_Http_OnParseError				HP_FN_HttpAgent_OnParseError;

typedef HP_FN_Http_OnWSMessageHeader		HP_FN_HttpAgent_OnWSMessageHeader;
typedef HP_FN_Http_OnWSMessageBody			HP_FN_HttpAgent_OnWSMessageBody;
typedef HP_FN_Http_OnWSMessageComplete		HP_FN_HttpAgent_OnWSMessageComplete;

typedef HP_FN_Agent_OnPrepareConnect		HP_FN_HttpAgent_OnPrepareConnect;
typedef HP_FN_Agent_OnConnect				HP_FN_HttpAgent_OnConnect;
typedef HP_FN_Agent_OnHandShake				HP_FN_HttpAgent_OnHandShake;
typedef HP_FN_Agent_OnReceive				HP_FN_HttpAgent_OnReceive;
typedef HP_FN_Agent_OnSend					HP_FN_HttpAgent_OnSend;
typedef HP_FN_Agent_OnClose					HP_FN_HttpAgent_OnClose;
typedef HP_FN_Agent_OnShutdown				HP_FN_HttpAgent_OnShutdown;

/* HTTP Client 回调函数 */
typedef HP_FN_Http_OnMessageBegin			HP_FN_HttpClient_OnMessageBegin;
typedef HP_FN_Http_OnStatusLine				HP_FN_HttpClient_OnStatusLine;
typedef HP_FN_Http_OnHeader					HP_FN_HttpClient_OnHeader;
typedef HP_FN_Http_OnHeadersComplete		HP_FN_HttpClient_OnHeadersComplete;
typedef HP_FN_Http_OnBody					HP_FN_HttpClient_OnBody;
typedef HP_FN_Http_OnChunkHeader			HP_FN_HttpClient_OnChunkHeader;
typedef HP_FN_Http_OnChunkComplete			HP_FN_HttpClient_OnChunkComplete;
typedef HP_FN_Http_OnMessageComplete		HP_FN_HttpClient_OnMessageComplete;
typedef HP_FN_Http_OnUpgrade				HP_FN_HttpClient_OnUpgrade;
typedef HP_FN_Http_OnParseError				HP_FN_HttpClient_OnParseError;

typedef HP_FN_Http_OnWSMessageHeader		HP_FN_HttpClient_OnWSMessageHeader;
typedef HP_FN_Http_OnWSMessageBody			HP_FN_HttpClient_OnWSMessageBody;
typedef HP_FN_Http_OnWSMessageComplete		HP_FN_HttpClient_OnWSMessageComplete;

typedef HP_FN_Client_OnPrepareConnect		HP_FN_HttpClient_OnPrepareConnect;
typedef HP_FN_Client_OnConnect				HP_FN_HttpClient_OnConnect;
typedef HP_FN_Client_OnHandShake			HP_FN_HttpClient_OnHandShake;
typedef HP_FN_Client_OnReceive				HP_FN_HttpClient_OnReceive;
typedef HP_FN_Client_OnSend					HP_FN_HttpClient_OnSend;
typedef HP_FN_Client_OnClose				HP_FN_HttpClient_OnClose;

/****************************************************/
/***************** HTTP 对象创建函数 *****************/

// 创建 HP_HttpServer 对象
HPSOCKET_API HP_HttpServer __HP_CALL Create_HP_HttpServer(HP_HttpServerListener pListener);
// 创建 HP_HttpAgent 对象
HPSOCKET_API HP_HttpAgent __HP_CALL Create_HP_HttpAgent(HP_HttpAgentListener pListener);
// 创建 HP_HttpClient 对象
HPSOCKET_API HP_HttpClient __HP_CALL Create_HP_HttpClient(HP_HttpClientListener pListener);
// 创建 HP_HttpSyncClient 对象（pListener 参数可传入 nullptr）
HPSOCKET_API HP_HttpSyncClient __HP_CALL Create_HP_HttpSyncClient(HP_HttpClientListener pListener /*= nullptr*/);

// 销毁 HP_HttpServer 对象
HPSOCKET_API void __HP_CALL Destroy_HP_HttpServer(HP_HttpServer pServer);
// 销毁 HP_HttpAgent 对象
HPSOCKET_API void __HP_CALL Destroy_HP_HttpAgent(HP_HttpAgent pAgent);
// 销毁 HP_HttpClient 对象
HPSOCKET_API void __HP_CALL Destroy_HP_HttpClient(HP_HttpClient pClient);
// 销毁 HP_HttpSyncClient 对象
HPSOCKET_API void __HP_CALL Destroy_HP_HttpSyncClient(HP_HttpSyncClient pClient);

// 创建 HP_HttpServerListener 对象
HPSOCKET_API HP_HttpServerListener __HP_CALL Create_HP_HttpServerListener();
// 创建 HP_HttpAgentListener 对象
HPSOCKET_API HP_HttpAgentListener __HP_CALL Create_HP_HttpAgentListener();
// 创建 HP_HttpClientListener 对象
HPSOCKET_API HP_HttpClientListener __HP_CALL Create_HP_HttpClientListener();

// 销毁 HP_HttpServerListener 对象
HPSOCKET_API void __HP_CALL Destroy_HP_HttpServerListener(HP_HttpServerListener pListener);
// 销毁 HP_HttpAgentListener 对象
HPSOCKET_API void __HP_CALL Destroy_HP_HttpAgentListener(HP_HttpAgentListener pListener);
// 销毁 HP_HttpClientListener 对象
HPSOCKET_API void __HP_CALL Destroy_HP_HttpClientListener(HP_HttpClientListener pListener);

/**********************************************************************************/
/*************************** HTTP Server 回调函数设置方法 **************************/

HPSOCKET_API void __HP_CALL HP_Set_FN_HttpServer_OnMessageBegin(HP_HttpServerListener pListener		, HP_FN_HttpServer_OnMessageBegin fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpServer_OnRequestLine(HP_HttpServerListener pListener		, HP_FN_HttpServer_OnRequestLine fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpServer_OnHeader(HP_HttpServerListener pListener			, HP_FN_HttpServer_OnHeader fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpServer_OnHeadersComplete(HP_HttpServerListener pListener	, HP_FN_HttpServer_OnHeadersComplete fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpServer_OnBody(HP_HttpServerListener pListener				, HP_FN_HttpServer_OnBody fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpServer_OnChunkHeader(HP_HttpServerListener pListener		, HP_FN_HttpServer_OnChunkHeader fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpServer_OnChunkComplete(HP_HttpServerListener pListener	, HP_FN_HttpServer_OnChunkComplete fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpServer_OnMessageComplete(HP_HttpServerListener pListener	, HP_FN_HttpServer_OnMessageComplete fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpServer_OnUpgrade(HP_HttpServerListener pListener			, HP_FN_HttpServer_OnUpgrade fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpServer_OnParseError(HP_HttpServerListener pListener		, HP_FN_HttpServer_OnParseError fn);

HPSOCKET_API void __HP_CALL HP_Set_FN_HttpServer_OnWSMessageHeader(HP_HttpServerListener pListener	, HP_FN_HttpServer_OnWSMessageHeader fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpServer_OnWSMessageBody(HP_HttpServerListener pListener	, HP_FN_HttpServer_OnWSMessageBody fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpServer_OnWSMessageComplete(HP_HttpServerListener pListener, HP_FN_HttpServer_OnWSMessageComplete fn);

HPSOCKET_API void __HP_CALL HP_Set_FN_HttpServer_OnPrepareListen(HP_HttpServerListener pListener	, HP_FN_HttpServer_OnPrepareListen fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpServer_OnAccept(HP_HttpServerListener pListener			, HP_FN_HttpServer_OnAccept fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpServer_OnHandShake(HP_HttpServerListener pListener		, HP_FN_HttpServer_OnHandShake fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpServer_OnReceive(HP_HttpServerListener pListener			, HP_FN_HttpServer_OnReceive fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpServer_OnSend(HP_HttpServerListener pListener				, HP_FN_HttpServer_OnSend fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpServer_OnClose(HP_HttpServerListener pListener			, HP_FN_HttpServer_OnClose fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpServer_OnShutdown(HP_HttpServerListener pListener			, HP_FN_HttpServer_OnShutdown fn);

/**********************************************************************************/
/**************************** HTTP Agent 回调函数设置方法 **************************/

HPSOCKET_API void __HP_CALL HP_Set_FN_HttpAgent_OnMessageBegin(HP_HttpAgentListener pListener		, HP_FN_HttpAgent_OnMessageBegin fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpAgent_OnStatusLine(HP_HttpAgentListener pListener			, HP_FN_HttpAgent_OnStatusLine fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpAgent_OnHeader(HP_HttpAgentListener pListener				, HP_FN_HttpAgent_OnHeader fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpAgent_OnHeadersComplete(HP_HttpAgentListener pListener	, HP_FN_HttpAgent_OnHeadersComplete fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpAgent_OnBody(HP_HttpAgentListener pListener				, HP_FN_HttpAgent_OnBody fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpAgent_OnChunkHeader(HP_HttpAgentListener pListener		, HP_FN_HttpAgent_OnChunkHeader fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpAgent_OnChunkComplete(HP_HttpAgentListener pListener		, HP_FN_HttpAgent_OnChunkComplete fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpAgent_OnMessageComplete(HP_HttpAgentListener pListener	, HP_FN_HttpAgent_OnMessageComplete fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpAgent_OnUpgrade(HP_HttpAgentListener pListener			, HP_FN_HttpAgent_OnUpgrade fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpAgent_OnParseError(HP_HttpAgentListener pListener			, HP_FN_HttpAgent_OnParseError fn);

HPSOCKET_API void __HP_CALL HP_Set_FN_HttpAgent_OnWSMessageHeader(HP_HttpAgentListener pListener	, HP_FN_HttpAgent_OnWSMessageHeader fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpAgent_OnWSMessageBody(HP_HttpAgentListener pListener		, HP_FN_HttpAgent_OnWSMessageBody fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpAgent_OnWSMessageComplete(HP_HttpAgentListener pListener	, HP_FN_HttpAgent_OnWSMessageComplete fn);

HPSOCKET_API void __HP_CALL HP_Set_FN_HttpAgent_OnPrepareConnect(HP_HttpAgentListener pListener		, HP_FN_HttpAgent_OnPrepareConnect fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpAgent_OnConnect(HP_HttpAgentListener pListener			, HP_FN_HttpAgent_OnConnect fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpAgent_OnHandShake(HP_HttpAgentListener pListener			, HP_FN_HttpAgent_OnHandShake fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpAgent_OnReceive(HP_HttpAgentListener pListener			, HP_FN_HttpAgent_OnReceive fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpAgent_OnSend(HP_HttpAgentListener pListener				, HP_FN_HttpAgent_OnSend fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpAgent_OnClose(HP_HttpAgentListener pListener				, HP_FN_HttpAgent_OnClose fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpAgent_OnShutdown(HP_HttpAgentListener pListener			, HP_FN_HttpAgent_OnShutdown fn);

/**********************************************************************************/
/*************************** HTTP Client 回调函数设置方法 **************************/

HPSOCKET_API void __HP_CALL HP_Set_FN_HttpClient_OnMessageBegin(HP_HttpClientListener pListener		, HP_FN_HttpClient_OnMessageBegin fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpClient_OnStatusLine(HP_HttpClientListener pListener		, HP_FN_HttpClient_OnStatusLine fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpClient_OnHeader(HP_HttpClientListener pListener			, HP_FN_HttpClient_OnHeader fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpClient_OnHeadersComplete(HP_HttpClientListener pListener	, HP_FN_HttpClient_OnHeadersComplete fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpClient_OnBody(HP_HttpClientListener pListener				, HP_FN_HttpClient_OnBody fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpClient_OnChunkHeader(HP_HttpClientListener pListener		, HP_FN_HttpClient_OnChunkHeader fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpClient_OnChunkComplete(HP_HttpClientListener pListener	, HP_FN_HttpClient_OnChunkComplete fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpClient_OnMessageComplete(HP_HttpClientListener pListener	, HP_FN_HttpClient_OnMessageComplete fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpClient_OnUpgrade(HP_HttpClientListener pListener			, HP_FN_HttpClient_OnUpgrade fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpClient_OnParseError(HP_HttpClientListener pListener		, HP_FN_HttpClient_OnParseError fn);

HPSOCKET_API void __HP_CALL HP_Set_FN_HttpClient_OnWSMessageHeader(HP_HttpClientListener pListener	, HP_FN_HttpClient_OnWSMessageHeader fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpClient_OnWSMessageBody(HP_HttpClientListener pListener	, HP_FN_HttpClient_OnWSMessageBody fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpClient_OnWSMessageComplete(HP_HttpClientListener pListener, HP_FN_HttpClient_OnWSMessageComplete fn);

HPSOCKET_API void __HP_CALL HP_Set_FN_HttpClient_OnPrepareConnect(HP_HttpClientListener pListener	, HP_FN_HttpClient_OnPrepareConnect fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpClient_OnConnect(HP_HttpClientListener pListener			, HP_FN_HttpClient_OnConnect fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpClient_OnHandShake(HP_HttpClientListener pListener		, HP_FN_HttpClient_OnHandShake fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpClient_OnReceive(HP_HttpClientListener pListener			, HP_FN_HttpClient_OnReceive fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpClient_OnSend(HP_HttpClientListener pListener				, HP_FN_HttpClient_OnSend fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_HttpClient_OnClose(HP_HttpClientListener pListener			, HP_FN_HttpClient_OnClose fn);

/**************************************************************************/
/*************************** HTTP Server 操作方法 **************************/

/*
* 名称：回复请求
* 描述：向客户端回复 HTTP 请求
*		
* 参数：		dwConnID		-- 连接 ID
*			usStatusCode	-- HTTP 状态码
*			lpszDesc		-- HTTP 状态描述
*			lpHeaders		-- 回复请求头
*			iHeaderCount	-- 回复请求头数量
*			pData			-- 回复请求体
*			iLength			-- 回复请求体长度
* 返回值：	TRUE			-- 成功
*			FALSE			-- 失败
*/
HPSOCKET_API BOOL __HP_CALL HP_HttpServer_SendResponse(HP_HttpServer pServer, HP_CONNID dwConnID, USHORT usStatusCode, LPCSTR lpszDesc, const HP_THeader lpHeaders[], int iHeaderCount, const BYTE* pData, int iLength);

/*
* 名称：发送本地文件
* 描述：向指定连接发送 4096 KB 以下的小文件
*		
* 参数：		dwConnID		-- 连接 ID
*			lpszFileName	-- 文件路径
*			usStatusCode	-- HTTP 状态码
*			lpszDesc		-- HTTP 状态描述
*			lpHeaders		-- 回复请求头
*			iHeaderCount	-- 回复请求头数量
* 返回值：	TRUE			-- 成功
*			FALSE			-- 失败
*/
HPSOCKET_API BOOL __HP_CALL HP_HttpServer_SendLocalFile(HP_HttpServer pServer, HP_CONNID dwConnID, LPCSTR lpszFileName, USHORT usStatusCode, LPCSTR lpszDesc, const HP_THeader lpHeaders[], int iHeaderCount);

/*
* 名称：发送 Chunked 数据分片
* 描述：向对端发送 Chunked 数据分片
*
* 参数：		dwConnID		-- 连接 ID
*			pData			-- Chunked 数据分片
*			iLength			-- 数据分片长度（为 0 表示结束分片）
*			lpszExtensions	-- 扩展属性（默认：nullptr）
* 返回值：	TRUE			-- 成功
*			FALSE			-- 失败
*/
HPSOCKET_API BOOL __HP_CALL HP_HttpServer_SendChunkData(HP_HttpServer pServer, HP_CONNID dwConnID, const BYTE* pData /*= nullptr*/, int iLength /*= 0*/, LPCSTR lpszExtensions /*= nullptr*/);

/*
* 名称：发送 WebSocket 消息
* 描述：向对端端发送 WebSocket 消息
*		
* 参数：		dwConnID		-- 连接 ID
*			bFinal			-- 是否结束帧
*			iReserved		-- RSV1/RSV2/RSV3 各 1 位
*			iOperationCode	-- 操作码：0x0 - 0xF
*			pData			-- 消息体数据缓冲区
*			iLength			-- 消息体数据长度
*			ullBodyLen		-- 消息总长度
* 								ullBodyLen = 0		 -> 消息总长度为 iLength
* 								ullBodyLen = iLength -> 消息总长度为 ullBodyLen
* 								ullBodyLen > iLength -> 消息总长度为 ullBodyLen，后续消息体长度为 ullBOdyLen - iLength，后续消息体通过底层方法 Send() / SendPackets() 发送
* 								ullBodyLen < iLength -> 错误参数，发送失败
* 返回值：	TRUE			-- 成功
*			FALSE			-- 失败
*/
HPSOCKET_API BOOL __HP_CALL HP_HttpServer_SendWSMessage(HP_HttpServer pServer, HP_CONNID dwConnID, BOOL bFinal, BYTE iReserved, BYTE iOperationCode, const BYTE* pData, int iLength, ULONGLONG ullBodyLen);

/*
* 名称：释放连接
* 描述：把连接放入释放队列，等待某个时间（通过 SetReleaseDelay() 设置）关闭连接
*		
* 参数：		dwConnID		-- 连接 ID
* 返回值：	TRUE			-- 成功
*			FALSE			-- 失败
*/
HPSOCKET_API BOOL __HP_CALL HP_HttpServer_Release(HP_HttpServer pServer, HP_CONNID dwConnID);

/*
* 名称：启动 HTTP 通信
* 描述：当通信组件设置为非自动启动 HTTP 通信时，需要调用本方法启动 HTTP 通信
*
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 SYS_GetLastError() 获取失败原因
*/
HPSOCKET_API BOOL __HP_CALL HP_HttpServer_StartHttp(HP_HttpServer pServer, HP_CONNID dwConnID);

/******************************************************************************/
/*************************** HTTP Server 属性访问方法 **************************/

/* 设置连接释放延时（默认：3000 毫秒） */
HPSOCKET_API void __HP_CALL HP_HttpServer_SetReleaseDelay(HP_HttpServer pServer, DWORD dwReleaseDelay);
/* 获取连接释放延时 */
HPSOCKET_API DWORD __HP_CALL HP_HttpServer_GetReleaseDelay(HP_HttpServer pServer);
/* 获取请求行 URL 域掩码（URL 域参考：EnHttpUrlField） */
HPSOCKET_API USHORT __HP_CALL HP_HttpServer_GetUrlFieldSet(HP_HttpServer pServer, HP_CONNID dwConnID);
/* 获取某个 URL 域值 */
HPSOCKET_API LPCSTR __HP_CALL HP_HttpServer_GetUrlField(HP_HttpServer pServer, HP_CONNID dwConnID, En_HP_HttpUrlField enField);
/* 获取请求方法 */
HPSOCKET_API LPCSTR __HP_CALL HP_HttpServer_GetMethod(HP_HttpServer pServer, HP_CONNID dwConnID);

/* 设置本地协议版本 */
HPSOCKET_API void __HP_CALL HP_HttpServer_SetLocalVersion(HP_HttpServer pServer, En_HP_HttpVersion usVersion);
/* 获取本地协议版本 */
HPSOCKET_API En_HP_HttpVersion __HP_CALL HP_HttpServer_GetLocalVersion(HP_HttpServer pServer);

/* 检查是否升级协议 */
HPSOCKET_API BOOL __HP_CALL HP_HttpServer_IsUpgrade(HP_HttpServer pServer, HP_CONNID dwConnID);
/* 检查是否有 Keep-Alive 标识 */
HPSOCKET_API BOOL __HP_CALL HP_HttpServer_IsKeepAlive(HP_HttpServer pServer, HP_CONNID dwConnID);
/* 获取协议版本 */
HPSOCKET_API USHORT __HP_CALL HP_HttpServer_GetVersion(HP_HttpServer pServer, HP_CONNID dwConnID);
/* 获取主机 */
HPSOCKET_API LPCSTR __HP_CALL HP_HttpServer_GetHost(HP_HttpServer pServer, HP_CONNID dwConnID);
/* 获取内容长度 */
HPSOCKET_API ULONGLONG __HP_CALL HP_HttpServer_GetContentLength(HP_HttpServer pServer, HP_CONNID dwConnID);
/* 获取内容类型 */
HPSOCKET_API LPCSTR __HP_CALL HP_HttpServer_GetContentType(HP_HttpServer pServer, HP_CONNID dwConnID);
/* 获取内容编码 */
HPSOCKET_API LPCSTR __HP_CALL HP_HttpServer_GetContentEncoding(HP_HttpServer pServer, HP_CONNID dwConnID);
/* 获取传输编码 */
HPSOCKET_API LPCSTR __HP_CALL HP_HttpServer_GetTransferEncoding(HP_HttpServer pServer, HP_CONNID dwConnID);
/* 获取协议升级类型 */
HPSOCKET_API En_HP_HttpUpgradeType __HP_CALL HP_HttpServer_GetUpgradeType(HP_HttpServer pServer, HP_CONNID dwConnID);
/* 获取解析错误代码 */
HPSOCKET_API USHORT __HP_CALL HP_HttpServer_GetParseErrorCode(HP_HttpServer pServer, HP_CONNID dwConnID, LPCSTR* lpszErrorDesc);

/* 获取某个请求头（单值） */
HPSOCKET_API BOOL __HP_CALL HP_HttpServer_GetHeader(HP_HttpServer pServer, HP_CONNID dwConnID, LPCSTR lpszName, LPCSTR* lpszValue);
/* 获取某个请求头（多值） */
HPSOCKET_API BOOL __HP_CALL HP_HttpServer_GetHeaders(HP_HttpServer pServer, HP_CONNID dwConnID, LPCSTR lpszName, LPCSTR lpszValue[], DWORD* pdwCount);
/* 获取所有请求头 */
HPSOCKET_API BOOL __HP_CALL HP_HttpServer_GetAllHeaders(HP_HttpServer pServer, HP_CONNID dwConnID, HP_THeader lpHeaders[], DWORD* pdwCount);
/* 获取所有请求头名称 */
HPSOCKET_API BOOL __HP_CALL HP_HttpServer_GetAllHeaderNames(HP_HttpServer pServer, HP_CONNID dwConnID, LPCSTR lpszName[], DWORD* pdwCount);

/* 获取 Cookie */
HPSOCKET_API BOOL __HP_CALL HP_HttpServer_GetCookie(HP_HttpServer pServer, HP_CONNID dwConnID, LPCSTR lpszName, LPCSTR* lpszValue);
/* 获取所有 Cookie */
HPSOCKET_API BOOL __HP_CALL HP_HttpServer_GetAllCookies(HP_HttpServer pServer, HP_CONNID dwConnID, HP_TCookie lpCookies[], DWORD* pdwCount);

/* 获取当前 WebSocket 消息状态，传入 nullptr 则不获取相应字段 */
HPSOCKET_API BOOL __HP_CALL HP_HttpServer_GetWSMessageState(HP_HttpServer pServer, HP_CONNID dwConnID, BOOL* lpbFinal, BYTE* lpiReserved, BYTE* lpiOperationCode, LPCBYTE* lpszMask, ULONGLONG* lpullBodyLen, ULONGLONG* lpullBodyRemain);

/* 设置 HTTP 启动方式（默认：TRUE，自动启动） */
HPSOCKET_API void __HP_CALL HP_HttpServer_SetHttpAutoStart(HP_HttpServer pServer, BOOL bAutoStart);
/* 获取 HTTP 启动方式 */
HPSOCKET_API BOOL __HP_CALL HP_HttpServer_IsHttpAutoStart(HP_HttpServer pServer);

/**************************************************************************/
/*************************** HTTP Agent 操作方法 ***************************/

/*
* 名称：发送请求
* 描述：向服务端发送 HTTP 请求
*		
* 参数：		dwConnID		-- 连接 ID
*			lpszMethod		-- 请求方法
*			lpszPath		-- 请求路径
*			lpHeaders		-- 请求头
*			iHeaderCount	-- 请求头数量
*			pBody			-- 请求体
*			iLength			-- 请求体长度
* 返回值：	TRUE			-- 成功
*			FALSE			-- 失败
*/
HPSOCKET_API BOOL __HP_CALL HP_HttpAgent_SendRequest(HP_HttpAgent pAgent, HP_CONNID dwConnID, LPCSTR lpszMethod, LPCSTR lpszPath, const HP_THeader lpHeaders[], int iHeaderCount, const BYTE* pData, int iLength);

/*
* 名称：发送本地文件
* 描述：向指定连接发送 4096 KB 以下的小文件
*		
* 参数：		dwConnID		-- 连接 ID
*			lpszFileName	-- 文件路径
*			lpszMethod		-- 请求方法
*			lpszPath		-- 请求路径
*			lpHeaders		-- 请求头
*			iHeaderCount	-- 请求头数量
* 返回值：	TRUE			-- 成功
*			FALSE			-- 失败
*/
HPSOCKET_API BOOL __HP_CALL HP_HttpAgent_SendLocalFile(HP_HttpAgent pAgent, HP_CONNID dwConnID, LPCSTR lpszFileName, LPCSTR lpszMethod, LPCSTR lpszPath, const HP_THeader lpHeaders[], int iHeaderCount);

/*
* 名称：发送 Chunked 数据分片
* 描述：向对端发送 Chunked 数据分片
*
* 参数：		dwConnID		-- 连接 ID
*			pData			-- Chunked 数据分片
*			iLength			-- 数据分片长度（为 0 表示结束分片）
*			lpszExtensions	-- 扩展属性（默认：nullptr）
* 返回值：	TRUE			-- 成功
*			FALSE			-- 失败
*/
HPSOCKET_API BOOL __HP_CALL HP_HttpAgent_SendChunkData(HP_HttpAgent pAgent, HP_CONNID dwConnID, const BYTE* pData /*= nullptr*/, int iLength /*= 0*/, LPCSTR lpszExtensions /*= nullptr*/);

/* 发送 POST 请求 */
HPSOCKET_API BOOL __HP_CALL HP_HttpAgent_SendPost(HP_HttpAgent pAgent, HP_CONNID dwConnID, LPCSTR lpszPath, const HP_THeader lpHeaders[], int iHeaderCount, const BYTE* pBody, int iLength);
/* 发送 PUT 请求 */
HPSOCKET_API BOOL __HP_CALL HP_HttpAgent_SendPut(HP_HttpAgent pAgent, HP_CONNID dwConnID, LPCSTR lpszPath, const HP_THeader lpHeaders[], int iHeaderCount, const BYTE* pBody, int iLength);
/* 发送 PATCH 请求 */
HPSOCKET_API BOOL __HP_CALL HP_HttpAgent_SendPatch(HP_HttpAgent pAgent, HP_CONNID dwConnID, LPCSTR lpszPath, const HP_THeader lpHeaders[], int iHeaderCount, const BYTE* pBody, int iLength);
/* 发送 GET 请求 */
HPSOCKET_API BOOL __HP_CALL HP_HttpAgent_SendGet(HP_HttpAgent pAgent, HP_CONNID dwConnID, LPCSTR lpszPath, const HP_THeader lpHeaders[], int iHeaderCount);
/* 发送 DELETE 请求 */
HPSOCKET_API BOOL __HP_CALL HP_HttpAgent_SendDelete(HP_HttpAgent pAgent, HP_CONNID dwConnID, LPCSTR lpszPath, const HP_THeader lpHeaders[], int iHeaderCount);
/* 发送 HEAD 请求 */
HPSOCKET_API BOOL __HP_CALL HP_HttpAgent_SendHead(HP_HttpAgent pAgent, HP_CONNID dwConnID, LPCSTR lpszPath, const HP_THeader lpHeaders[], int iHeaderCount);
/* 发送 TRACE 请求 */
HPSOCKET_API BOOL __HP_CALL HP_HttpAgent_SendTrace(HP_HttpAgent pAgent, HP_CONNID dwConnID, LPCSTR lpszPath, const HP_THeader lpHeaders[], int iHeaderCount);
/* 发送 OPTIONS 请求 */
HPSOCKET_API BOOL __HP_CALL HP_HttpAgent_SendOptions(HP_HttpAgent pAgent, HP_CONNID dwConnID, LPCSTR lpszPath, const HP_THeader lpHeaders[], int iHeaderCount);
/* 发送 CONNECT 请求 */
HPSOCKET_API BOOL __HP_CALL HP_HttpAgent_SendConnect(HP_HttpAgent pAgent, HP_CONNID dwConnID, LPCSTR lpszHost, const HP_THeader lpHeaders[], int iHeaderCount);

/*
* 名称：发送 WebSocket 消息
* 描述：向对端端发送 WebSocket 消息
*		
* 参数：		dwConnID		-- 连接 ID
*			bFinal			-- 是否结束帧
*			iReserved		-- RSV1/RSV2/RSV3 各 1 位
*			iOperationCode	-- 操作码：0x0 - 0xF
*			lpszMask		-- 掩码（nullptr 或 4 字节掩码，如果为 nullptr 则没有掩码）
*			pData			-- 消息体数据缓冲区
*			iLength			-- 消息体数据长度
*			ullBodyLen		-- 消息总长度
* 								ullBodyLen = 0		 -> 消息总长度为 iLength
* 								ullBodyLen = iLength -> 消息总长度为 ullBodyLen
* 								ullBodyLen > iLength -> 消息总长度为 ullBodyLen，后续消息体长度为 ullBOdyLen - iLength，后续消息体通过底层方法 Send() / SendPackets() 发送
* 								ullBodyLen < iLength -> 错误参数，发送失败
* 返回值：	TRUE			-- 成功
*			FALSE			-- 失败
*/
HPSOCKET_API BOOL __HP_CALL HP_HttpAgent_SendWSMessage(HP_HttpAgent pAgent, HP_CONNID dwConnID, BOOL bFinal, BYTE iReserved, BYTE iOperationCode, const BYTE lpszMask[4], const BYTE* pData, int iLength, ULONGLONG ullBodyLen);

/*
* 名称：启动 HTTP 通信
* 描述：当通信组件设置为非自动启动 HTTP 通信时，需要调用本方法启动 HTTP 通信
*
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 SYS_GetLastError() 获取失败原因
*/
HPSOCKET_API BOOL __HP_CALL HP_HttpAgent_StartHttp(HP_HttpAgent pAgent, HP_CONNID dwConnID);

/******************************************************************************/
/*************************** HTTP Agent 属性访问方法 ***************************/

/* 获取 HTTP 状态码 */
HPSOCKET_API USHORT __HP_CALL HP_HttpAgent_GetStatusCode(HP_HttpAgent pAgent, HP_CONNID dwConnID);

/* 设置本地协议版本 */
HPSOCKET_API void __HP_CALL HP_HttpAgent_SetLocalVersion(HP_HttpAgent pAgent, En_HP_HttpVersion usVersion);
/* 获取本地协议版本 */
HPSOCKET_API En_HP_HttpVersion __HP_CALL HP_HttpAgent_GetLocalVersion(HP_HttpAgent pAgent);

/* 检查是否升级协议 */
HPSOCKET_API BOOL __HP_CALL HP_HttpAgent_IsUpgrade(HP_HttpAgent pAgent, HP_CONNID dwConnID);
/* 检查是否有 Keep-Alive 标识 */
HPSOCKET_API BOOL __HP_CALL HP_HttpAgent_IsKeepAlive(HP_HttpAgent pAgent, HP_CONNID dwConnID);
/* 获取协议版本 */
HPSOCKET_API USHORT __HP_CALL HP_HttpAgent_GetVersion(HP_HttpAgent pAgent, HP_CONNID dwConnID);
/* 获取内容长度 */
HPSOCKET_API ULONGLONG __HP_CALL HP_HttpAgent_GetContentLength(HP_HttpAgent pAgent, HP_CONNID dwConnID);
/* 获取内容类型 */
HPSOCKET_API LPCSTR __HP_CALL HP_HttpAgent_GetContentType(HP_HttpAgent pAgent, HP_CONNID dwConnID);
/* 获取内容编码 */
HPSOCKET_API LPCSTR __HP_CALL HP_HttpAgent_GetContentEncoding(HP_HttpAgent pAgent, HP_CONNID dwConnID);
/* 获取传输编码 */
HPSOCKET_API LPCSTR __HP_CALL HP_HttpAgent_GetTransferEncoding(HP_HttpAgent pAgent, HP_CONNID dwConnID);
/* 获取协议升级类型 */
HPSOCKET_API En_HP_HttpUpgradeType __HP_CALL HP_HttpAgent_GetUpgradeType(HP_HttpAgent pAgent, HP_CONNID dwConnID);
/* 获取解析错误代码 */
HPSOCKET_API USHORT __HP_CALL HP_HttpAgent_GetParseErrorCode(HP_HttpAgent pAgent, HP_CONNID dwConnID, LPCSTR* lpszErrorDesc);

/* 获取某个请求头（单值） */
HPSOCKET_API BOOL __HP_CALL HP_HttpAgent_GetHeader(HP_HttpAgent pAgent, HP_CONNID dwConnID, LPCSTR lpszName, LPCSTR* lpszValue);
/* 获取某个请求头（多值） */
HPSOCKET_API BOOL __HP_CALL HP_HttpAgent_GetHeaders(HP_HttpAgent pAgent, HP_CONNID dwConnID, LPCSTR lpszName, LPCSTR lpszValue[], DWORD* pdwCount);
/* 获取所有请求头 */
HPSOCKET_API BOOL __HP_CALL HP_HttpAgent_GetAllHeaders(HP_HttpAgent pAgent, HP_CONNID dwConnID, HP_THeader lpHeaders[], DWORD* pdwCount);
/* 获取所有请求头名称 */
HPSOCKET_API BOOL __HP_CALL HP_HttpAgent_GetAllHeaderNames(HP_HttpAgent pAgent, HP_CONNID dwConnID, LPCSTR lpszName[], DWORD* pdwCount);

/* 设置是否使用 Cookie */
HPSOCKET_API void __HP_CALL HP_HttpAgent_SetUseCookie(HP_HttpAgent pAgent, BOOL bUseCookie);
/* 检查是否使用 Cookie */
HPSOCKET_API BOOL __HP_CALL HP_HttpAgent_IsUseCookie(HP_HttpAgent pAgent);
/* 获取 Cookie */
HPSOCKET_API BOOL __HP_CALL HP_HttpAgent_GetCookie(HP_HttpAgent pAgent, HP_CONNID dwConnID, LPCSTR lpszName, LPCSTR* lpszValue);
/* 获取所有 Cookie */
HPSOCKET_API BOOL __HP_CALL HP_HttpAgent_GetAllCookies(HP_HttpAgent pAgent, HP_CONNID dwConnID, HP_TCookie lpCookies[], DWORD* pdwCount);

/* 获取当前 WebSocket 消息状态，传入 nullptr 则不获取相应字段 */
HPSOCKET_API BOOL __HP_CALL HP_HttpAgent_GetWSMessageState(HP_HttpAgent pAgent, HP_CONNID dwConnID, BOOL* lpbFinal, BYTE* lpiReserved, BYTE* lpiOperationCode, LPCBYTE* lpszMask, ULONGLONG* lpullBodyLen, ULONGLONG* lpullBodyRemain);

/* 设置 HTTP 启动方式（默认：TRUE，自动启动） */
HPSOCKET_API void __HP_CALL HP_HttpAgent_SetHttpAutoStart(HP_HttpAgent pAgent, BOOL bAutoStart);
/* 获取 HTTP 启动方式 */
HPSOCKET_API BOOL __HP_CALL HP_HttpAgent_IsHttpAutoStart(HP_HttpAgent pAgent);

/**************************************************************************/
/*************************** HTTP Client 操作方法 **************************/

/*
* 名称：发送请求
* 描述：向服务端发送 HTTP 请求
*		
* 参数：		lpszMethod		-- 请求方法
*			lpszPath		-- 请求路径
*			lpHeaders		-- 请求头
*			iHeaderCount	-- 请求头数量
*			pBody			-- 请求体
*			iLength			-- 请求体长度
* 返回值：	TRUE			-- 成功
*			FALSE			-- 失败
*/
HPSOCKET_API BOOL __HP_CALL HP_HttpClient_SendRequest(HP_HttpClient pClient, LPCSTR lpszMethod, LPCSTR lpszPath, const HP_THeader lpHeaders[], int iHeaderCount, const BYTE* pBody, int iLength);

/*
* 名称：发送本地文件
* 描述：向指定连接发送 4096 KB 以下的小文件
*		
* 参数：		dwConnID		-- 连接 ID
*			lpszFileName	-- 文件路径
*			lpszMethod		-- 请求方法
*			lpszPath		-- 请求路径
*			lpHeaders		-- 请求头
*			iHeaderCount	-- 请求头数量
* 返回值：	TRUE			-- 成功
*			FALSE			-- 失败
*/
HPSOCKET_API BOOL __HP_CALL HP_HttpClient_SendLocalFile(HP_HttpClient pClient, LPCSTR lpszFileName, LPCSTR lpszMethod, LPCSTR lpszPath, const HP_THeader lpHeaders[], int iHeaderCount);

/*
* 名称：发送 Chunked 数据分片
* 描述：向对端发送 Chunked 数据分片
*
* 参数：		pData			-- Chunked 数据分片
*			iLength			-- 数据分片长度（为 0 表示结束分片）
*			lpszExtensions	-- 扩展属性（默认：nullptr）
* 返回值：	TRUE			-- 成功
*			FALSE			-- 失败
*/
HPSOCKET_API BOOL __HP_CALL HP_HttpClient_SendChunkData(HP_HttpClient pClient, const BYTE* pData /*= nullptr*/, int iLength /*= 0*/, LPCSTR lpszExtensions /*= nullptr*/);

/* 发送 POST 请求 */
HPSOCKET_API BOOL __HP_CALL HP_HttpClient_SendPost(HP_HttpClient pClient, LPCSTR lpszPath, const HP_THeader lpHeaders[], int iHeaderCount, const BYTE* pBody, int iLength);
/* 发送 PUT 请求 */
HPSOCKET_API BOOL __HP_CALL HP_HttpClient_SendPut(HP_HttpClient pClient, LPCSTR lpszPath, const HP_THeader lpHeaders[], int iHeaderCount, const BYTE* pBody, int iLength);
/* 发送 PATCH 请求 */
HPSOCKET_API BOOL __HP_CALL HP_HttpClient_SendPatch(HP_HttpClient pClient, LPCSTR lpszPath, const HP_THeader lpHeaders[], int iHeaderCount, const BYTE* pBody, int iLength);
/* 发送 GET 请求 */
HPSOCKET_API BOOL __HP_CALL HP_HttpClient_SendGet(HP_HttpClient pClient, LPCSTR lpszPath, const HP_THeader lpHeaders[], int iHeaderCount);
/* 发送 DELETE 请求 */
HPSOCKET_API BOOL __HP_CALL HP_HttpClient_SendDelete(HP_HttpClient pClient, LPCSTR lpszPath, const HP_THeader lpHeaders[], int iHeaderCount);
/* 发送 HEAD 请求 */
HPSOCKET_API BOOL __HP_CALL HP_HttpClient_SendHead(HP_HttpClient pClient, LPCSTR lpszPath, const HP_THeader lpHeaders[], int iHeaderCount);
/* 发送 TRACE 请求 */
HPSOCKET_API BOOL __HP_CALL HP_HttpClient_SendTrace(HP_HttpClient pClient, LPCSTR lpszPath, const HP_THeader lpHeaders[], int iHeaderCount);
/* 发送 OPTIONS 请求 */
HPSOCKET_API BOOL __HP_CALL HP_HttpClient_SendOptions(HP_HttpClient pClient, LPCSTR lpszPath, const HP_THeader lpHeaders[], int iHeaderCount);
/* 发送 CONNECT 请求 */
HPSOCKET_API BOOL __HP_CALL HP_HttpClient_SendConnect(HP_HttpClient pClient, LPCSTR lpszHost, const HP_THeader lpHeaders[], int iHeaderCount);

/*
* 名称：发送 WebSocket 消息
* 描述：向对端端发送 WebSocket 消息
*		
* 参数：		bFinal			-- 是否结束帧
*			iReserved		-- RSV1/RSV2/RSV3 各 1 位
*			iOperationCode	-- 操作码：0x0 - 0xF
*			lpszMask		-- 掩码（nullptr 或 4 字节掩码，如果为 nullptr 则没有掩码）
*			pData			-- 消息体数据缓冲区
*			iLength			-- 消息体数据长度
*			ullBodyLen		-- 消息总长度
* 								ullBodyLen = 0		 -> 消息总长度为 iLength
* 								ullBodyLen = iLength -> 消息总长度为 ullBodyLen
* 								ullBodyLen > iLength -> 消息总长度为 ullBodyLen，后续消息体长度为 ullBOdyLen - iLength，后续消息体通过底层方法 Send() / SendPackets() 发送
* 								ullBodyLen < iLength -> 错误参数，发送失败
* 返回值：	TRUE			-- 成功
*			FALSE			-- 失败
*/
HPSOCKET_API BOOL __HP_CALL HP_HttpClient_SendWSMessage(HP_HttpClient pClient, BOOL bFinal, BYTE iReserved, BYTE iOperationCode, const BYTE lpszMask[4], const BYTE* pData, int iLength, ULONGLONG ullBodyLen);

/*
* 名称：启动 HTTP 通信
* 描述：当通信组件设置为非自动启动 HTTP 通信时，需要调用本方法启动 HTTP 通信
*
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 SYS_GetLastError() 获取失败原因
*/
HPSOCKET_API BOOL __HP_CALL HP_HttpClient_StartHttp(HP_HttpClient pClient);

/******************************************************************************/
/*************************** HTTP Client 属性访问方法 **************************/

/* 获取 HTTP 状态码 */
HPSOCKET_API USHORT __HP_CALL HP_HttpClient_GetStatusCode(HP_HttpClient pClient);

/* 设置本地协议版本 */
HPSOCKET_API void __HP_CALL HP_HttpClient_SetLocalVersion(HP_HttpClient pClient, En_HP_HttpVersion usVersion);
/* 获取本地协议版本 */
HPSOCKET_API En_HP_HttpVersion __HP_CALL HP_HttpClient_GetLocalVersion(HP_HttpClient pClient);

/* 检查是否升级协议 */
HPSOCKET_API BOOL __HP_CALL HP_HttpClient_IsUpgrade(HP_HttpClient pClient);
/* 检查是否有 Keep-Alive 标识 */
HPSOCKET_API BOOL __HP_CALL HP_HttpClient_IsKeepAlive(HP_HttpClient pClient);
/* 获取协议版本 */
HPSOCKET_API USHORT __HP_CALL HP_HttpClient_GetVersion(HP_HttpClient pClient);
/* 获取内容长度 */
HPSOCKET_API ULONGLONG __HP_CALL HP_HttpClient_GetContentLength(HP_HttpClient pClient);
/* 获取内容类型 */
HPSOCKET_API LPCSTR __HP_CALL HP_HttpClient_GetContentType(HP_HttpClient pClient);
/* 获取内容编码 */
HPSOCKET_API LPCSTR __HP_CALL HP_HttpClient_GetContentEncoding(HP_HttpClient pClient);
/* 获取传输编码 */
HPSOCKET_API LPCSTR __HP_CALL HP_HttpClient_GetTransferEncoding(HP_HttpClient pClient);
/* 获取协议升级类型 */
HPSOCKET_API En_HP_HttpUpgradeType __HP_CALL HP_HttpClient_GetUpgradeType(HP_HttpClient pClient);
/* 获取解析错误代码 */
HPSOCKET_API USHORT __HP_CALL HP_HttpClient_GetParseErrorCode(HP_HttpClient pClient, LPCSTR* lpszErrorDesc);

/* 获取某个请求头（单值） */
HPSOCKET_API BOOL __HP_CALL HP_HttpClient_GetHeader(HP_HttpClient pClient, LPCSTR lpszName, LPCSTR* lpszValue);
/* 获取某个请求头（多值） */
HPSOCKET_API BOOL __HP_CALL HP_HttpClient_GetHeaders(HP_HttpClient pClient, LPCSTR lpszName, LPCSTR lpszValue[], DWORD* pdwCount);
/* 获取所有请求头 */
HPSOCKET_API BOOL __HP_CALL HP_HttpClient_GetAllHeaders(HP_HttpClient pClient, HP_THeader lpHeaders[], DWORD* pdwCount);
/* 获取所有请求头名称 */
HPSOCKET_API BOOL __HP_CALL HP_HttpClient_GetAllHeaderNames(HP_HttpClient pClient, LPCSTR lpszName[], DWORD* pdwCount);

/* 设置是否使用 Cookie */
HPSOCKET_API void __HP_CALL HP_HttpClient_SetUseCookie(HP_HttpClient pClient, BOOL bUseCookie);
/* 检查是否使用 Cookie */
HPSOCKET_API BOOL __HP_CALL HP_HttpClient_IsUseCookie(HP_HttpClient pClient);
/* 获取 Cookie */
HPSOCKET_API BOOL __HP_CALL HP_HttpClient_GetCookie(HP_HttpClient pClient, LPCSTR lpszName, LPCSTR* lpszValue);
/* 获取所有 Cookie */
HPSOCKET_API BOOL __HP_CALL HP_HttpClient_GetAllCookies(HP_HttpClient pClient, HP_TCookie lpCookies[], DWORD* pdwCount);

/* 获取当前 WebSocket 消息状态，传入 nullptr 则不获取相应字段 */
HPSOCKET_API BOOL __HP_CALL HP_HttpClient_GetWSMessageState(HP_HttpClient pClient, BOOL* lpbFinal, BYTE* lpiReserved, BYTE* lpiOperationCode, LPCBYTE* lpszMask, ULONGLONG* lpullBodyLen, ULONGLONG* lpullBodyRemain);

/* 设置 HTTP 启动方式（默认：TRUE，自动启动） */
HPSOCKET_API void __HP_CALL HP_HttpClient_SetHttpAutoStart(HP_HttpClient pClient, BOOL bAutoStart);
/* 获取 HTTP 启动方式 */
HPSOCKET_API BOOL __HP_CALL HP_HttpClient_IsHttpAutoStart(HP_HttpClient pClient);

/**************************************************************************/
/************************ HTTP Sync Client 操作方法 ************************/

/*
* 名称：发送 URL 请求
* 描述：向服务端发送 HTTP URL 请求
*		
* 参数：		lpszMethod		-- 请求方法
*			lpszUrl			-- 请求 URL
*			lpHeaders		-- 请求头
*			iHeaderCount	-- 请求头数量
*			pBody			-- 请求体
*			iLength			-- 请求体长度
*			bForceReconnect	-- 强制重新连接（默认：FALSE，当请求 URL 的主机和端口与现有连接一致时，重用现有连接）
* 返回值：	TRUE			-- 成功
*			FALSE			-- 失败
*/
HPSOCKET_API BOOL __HP_CALL HP_HttpSyncClient_OpenUrl(HP_HttpSyncClient pClient, LPCSTR lpszMethod, LPCSTR lpszUrl, const HP_THeader lpHeaders[], int iHeaderCount, const BYTE* pBody, int iLength, BOOL bForceReconnect);

/*
* 名称：清除请求结果
* 描述：清除上一次请求的响应头和响应体等结果信息（该方法会在每次发送请求前自动调用）
*
* 参数：		
* 返回值：	TRUE			-- 成功
*			FALSE			-- 失败
*/
HPSOCKET_API BOOL __HP_CALL HP_HttpSyncClient_CleanupRequestResult(HP_HttpSyncClient pClient);

/******************************************************************************/
/************************ HTTP Sync Client 属性访问方法 ************************/

/* 设置连接超时（毫秒，0：系统默认超时，默认：5000） */
HPSOCKET_API void __HP_CALL HP_HttpSyncClient_SetConnectTimeout(HP_HttpSyncClient pClient, DWORD dwConnectTimeout);
/* 设置请求超时（毫秒，0：无限等待，默认：10000） */
HPSOCKET_API void __HP_CALL HP_HttpSyncClient_SetRequestTimeout(HP_HttpSyncClient pClient, DWORD dwRequestTimeout);

/* 获取连接超时 */
HPSOCKET_API DWORD __HP_CALL HP_HttpSyncClient_GetConnectTimeout(HP_HttpSyncClient pClient);
/* 获取请求超时 */
HPSOCKET_API DWORD __HP_CALL HP_HttpSyncClient_GetRequestTimeout(HP_HttpSyncClient pClient);

/* 获取响应体 */
HPSOCKET_API BOOL __HP_CALL HP_HttpSyncClient_GetResponseBody(HP_HttpSyncClient pClient, LPCBYTE* lpszBody, int* piLength);

/**************************************************************************/
/*************************** HTTP Cookie 管理方法 **************************/

/* 从文件加载 Cookie */
HPSOCKET_API BOOL __HP_CALL HP_HttpCookie_MGR_LoadFromFile(LPCSTR lpszFile, BOOL bKeepExists /*= TRUE*/);
/* 保存 Cookie 到文件 */
HPSOCKET_API BOOL __HP_CALL HP_HttpCookie_MGR_SaveToFile(LPCSTR lpszFile, BOOL bKeepExists /*= TRUE*/);
/* 清理 Cookie */
HPSOCKET_API BOOL __HP_CALL HP_HttpCookie_MGR_ClearCookies(LPCSTR lpszDomain /*= nullptr*/, LPCSTR lpszPath /*= nullptr*/);
/* 清理过期 Cookie */
HPSOCKET_API BOOL __HP_CALL HP_HttpCookie_MGR_RemoveExpiredCookies(LPCSTR lpszDomain /*= nullptr*/, LPCSTR lpszPath /*= nullptr*/);
/* 设置 Cookie */
HPSOCKET_API BOOL __HP_CALL HP_HttpCookie_MGR_SetCookie(LPCSTR lpszName, LPCSTR lpszValue, LPCSTR lpszDomain, LPCSTR lpszPath, int iMaxAge /*= -1*/, BOOL bHttpOnly /*= FALSE*/, BOOL bSecure /*= FALSE*/, int enSameSite /*= 0*/, BOOL bOnlyUpdateValueIfExists /*= TRUE*/);
/* 删除 Cookie */
HPSOCKET_API BOOL __HP_CALL HP_HttpCookie_MGR_DeleteCookie(LPCSTR lpszDomain, LPCSTR lpszPath, LPCSTR lpszName);
/* 设置是否允许第三方 Cookie */
HPSOCKET_API void __HP_CALL HP_HttpCookie_MGR_SetEnableThirdPartyCookie(BOOL bEnableThirdPartyCookie /*= TRUE*/);
/* 检查是否允许第三方 Cookie */
HPSOCKET_API BOOL __HP_CALL HP_HttpCookie_MGR_IsEnableThirdPartyCookie();

/* Cookie expires 字符串转换为整数 */
HPSOCKET_API BOOL __HP_CALL HP_HttpCookie_HLP_ParseExpires(LPCSTR lpszExpires, __time64_t* ptmExpires);
/* 整数转换为 Cookie expires 字符串 */
HPSOCKET_API BOOL __HP_CALL HP_HttpCookie_HLP_MakeExpiresStr(char lpszBuff[], int* piBuffLen, __time64_t tmExpires);
/* 生成 Cookie 字符串 */
HPSOCKET_API BOOL __HP_CALL HP_HttpCookie_HLP_ToString(char lpszBuff[], int* piBuffLen, LPCSTR lpszName, LPCSTR lpszValue, LPCSTR lpszDomain, LPCSTR lpszPath, int iMaxAge /*= -1*/, BOOL bHttpOnly /*= FALSE*/, BOOL bSecure /*= FALSE*/, int enSameSite /*= 0*/);
/* 获取当前 UTC 时间 */
HPSOCKET_API __time64_t __HP_CALL HP_HttpCookie_HLP_CurrentUTCTime();
/* Max-Age -> expires */
HPSOCKET_API __time64_t __HP_CALL HP_HttpCookie_HLP_MaxAgeToExpires(int iMaxAge);
/* expires -> Max-Age */
HPSOCKET_API int __HP_CALL HP_HttpCookie_HLP_ExpiresToMaxAge(__time64_t tmExpires);

/*****************************************************************************************************************************************************/
/************************************************************* HTTP Global Function Exports **********************************************************/
/*****************************************************************************************************************************************************/

#endif

/*****************************************************************************************************************************************************/
/**************************************************************** Thread Pool Exports ****************************************************************/
/*****************************************************************************************************************************************************/

/* Thread Pool 回调函数 */
typedef void (__HP_CALL *HP_FN_ThreadPool_OnStartup)			(HP_ThreadPool pThreadPool);
typedef void (__HP_CALL *HP_FN_ThreadPool_OnShutdown)			(HP_ThreadPool pThreadPool);
typedef void (__HP_CALL *HP_FN_ThreadPool_OnWorkerThreadStart)	(HP_ThreadPool pThreadPool, THR_ID dwThreadID);
typedef void (__HP_CALL *HP_FN_ThreadPool_OnWorkerThreadEnd)	(HP_ThreadPool pThreadPool, THR_ID dwThreadID);

/**********************************************************************************/
/************************** Thread Pool 回调函数设置方法 ***************************/

HPSOCKET_API void __HP_CALL HP_Set_FN_ThreadPool_OnStartup(HP_ThreadPoolListener pListener				, HP_FN_ThreadPool_OnStartup fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_ThreadPool_OnShutdown(HP_ThreadPoolListener pListener				, HP_FN_ThreadPool_OnShutdown fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_ThreadPool_OnWorkerThreadStart(HP_ThreadPoolListener pListener	, HP_FN_ThreadPool_OnWorkerThreadStart fn);
HPSOCKET_API void __HP_CALL HP_Set_FN_ThreadPool_OnWorkerThreadEnd(HP_ThreadPoolListener pListener		, HP_FN_ThreadPool_OnWorkerThreadEnd fn);

/****************************************************/
/******************* 对象创建函数 ********************/

// 创建 IHPThreadPoolListener 对象
HPSOCKET_API HP_ThreadPoolListener __HP_CALL Create_HP_ThreadPoolListener();
// 销毁 IHPThreadPoolListener 对象
HPSOCKET_API void __HP_CALL Destroy_HP_ThreadPoolListener(HP_ThreadPoolListener pListener);

// 创建 IHPThreadPool 对象（pListener 参数可传入 nullptr）
HPSOCKET_API HP_ThreadPool __HP_CALL Create_HP_ThreadPool(HP_ThreadPoolListener pListener /*= nullptr*/);
// 销毁 IHPThreadPool 对象
HPSOCKET_API void __HP_CALL Destroy_HP_ThreadPool(HP_ThreadPool pThreadPool);

/*
* 名称：创建 TSocketTask 对象
* 描述：创建任务对象，该对象最终需由 HP_Destroy_SocketTaskObj() 销毁
*		
* 参数：		fnTaskProc	-- 任务处理函数
*			pSender		-- 发起对象
*			dwConnID	-- 连接 ID
*			pBuffer		-- 数据缓冲区
*			iBuffLen	-- 数据缓冲区长度
*			enBuffType	-- 数据缓冲区类型（默认：TBT_COPY）
*							TBT_COPY	：（深拷贝）pBuffer 复制到 TSocketTask 对象。此后 TSocketTask 对象与 pBuffer 不再有任何关联
*											-> 适用于 pBuffer 不大或 pBuffer 生命周期不受控的场景
*							TBT_REFER	：（浅拷贝）pBuffer 不复制到 TSocketTask 对象，需确保 TSocketTask 对象生命周期内 pBuffer 必须有效
*											-> 适用于 pBuffer 较大或 pBuffer 可重用，并且 pBuffer 生命周期受控的场景
*							TBT_ATTACH	：（附属）执行浅拷贝，但 TSocketTask 对象会获得 pBuffer 的所有权，并负责释放 pBuffer，避免多次缓冲区拷贝
*											-> 注意：pBuffer 必须由 SYS_Malloc()/SYS_Calloc() 函数分配才能使用本类型，否则可能会发生内存访问错误
*			wParam		-- 自定义参数
*			lParam		-- 自定义参数
* 返回值：	HP_LPTSocketTask
*/
HPSOCKET_API HP_LPTSocketTask __HP_CALL Create_HP_SocketTaskObj(Fn_SocketTaskProc fnTaskProc, PVOID pSender, HP_CONNID dwConnID, LPCBYTE pBuffer, INT iBuffLen, En_HP_TaskBufferType enBuffType /*= TBT_COPY*/, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/);

// 销毁 TSocketTask 对象
HPSOCKET_API void __HP_CALL Destroy_HP_SocketTaskObj(HP_LPTSocketTask pTask);

/***********************************************************************/
/***************************** 组件操作方法 *****************************/

/*
* 名称：启动线程池组件
* 描述：
*		
* 参数：		dwThreadCount		-- 线程数量，（默认：0）
*									>0 -> dwThreadCount
*									=0 -> (CPU核数 * 2 + 2)
*									<0 -> (CPU核数 * (-dwThreadCount))
*			dwMaxQueueSize		-- 任务队列最大容量（默认：0，不限制）
*			enRejectedPolicy	-- 任务拒绝处理策略
*									TRP_CALL_FAIL（默认）	：立刻返回失败
*									TRP_WAIT_FOR			：等待（直到成功、超时或线程池关闭等原因导致失败）
*									TRP_CALLER_RUN			：调用者线程直接执行
*			dwStackSize			-- 线程堆栈空间大小（默认：0 -> 操作系统默认）
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 SYS_GetLastError() 获取系统错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_ThreadPool_Start(HP_ThreadPool pThreadPool, DWORD dwThreadCount /*= 0*/, DWORD dwMaxQueueSize /*= 0*/, En_HP_RejectedPolicy enRejectedPolicy /*= TRP_CALL_FAIL*/, DWORD dwStackSize /*= 0*/);

/*
* 名称：关闭线程池组件
* 描述：在规定时间内关闭线程池组件，如果工作线程在最大等待时间内未能正常关闭，会尝试强制关闭，这种情况下很可能会造成系统资源泄漏
*		
* 参数：		dwMaxWait	-- 最大等待时间（毫秒，默认：INFINITE，一直等待）
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 SYS_GetLastError() 获取系统错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_ThreadPool_Stop(HP_ThreadPool pThreadPool, DWORD dwMaxWait /*= INFINITE*/);

/*
* 名称：提交任务
* 描述：向线程池提交异步任务
*		
* 参数：		fnTaskProc	-- 任务处理函数
*			pvArg		-- 任务参数
*			dwMaxWait	-- 任务提交最大等待时间（仅对 TRP_WAIT_FOR 类型线程池生效，默认：INFINITE，一直等待）
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 SYS_GetLastError() 获取系统错误代码
*							其中，错误码 ERROR_DESTINATION_ELEMENT_FULL 表示任务队列已满
*/
HPSOCKET_API BOOL __HP_CALL HP_ThreadPool_Submit(HP_ThreadPool pThreadPool, HP_Fn_TaskProc fnTaskProc, PVOID pvArg, DWORD dwMaxWait /*= INFINITE*/);

/*
* 名称：提交 Socket 任务
* 描述：向线程池提交异步 Socket 任务
*		
* 参数：		pTask		-- 任务参数
*			dwMaxWait	-- 任务提交最大等待时间（仅对 TRP_WAIT_FOR 类型线程池生效，默认：INFINITE，一直等待）
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 SYS_GetLastError() 获取系统错误代码
*							其中，错误码 ERROR_DESTINATION_ELEMENT_FULL 表示任务队列已满
*							注意：如果提交失败，需要手工调用 Destroy_HP_SocketTaskObj() 销毁 TSocketTask 对象
*/
HPSOCKET_API BOOL __HP_CALL HP_ThreadPool_Submit_Task(HP_ThreadPool pThreadPool, HP_LPTSocketTask pTask, DWORD dwMaxWait /*= INFINITE*/);

/*
* 名称：调整线程池大小
* 描述：增加或减少线程池的工作线程数量
*		
* 参数：		dwNewThreadCount	-- 线程数量
*									>0 -> dwNewThreadCount
*									=0 -> (CPU核数 * 2 + 2)
*									<0 -> (CPU核数 * (-dwNewThreadCount))
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 SYS_GetLastError() 获取系统错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_ThreadPool_AdjustThreadCount(HP_ThreadPool pThreadPool, DWORD dwNewThreadCount);

/*
* 名称：等待
* 描述：等待线程池组件停止运行
*
* 参数：		dwMilliseconds	-- 超时时间（毫秒，默认：-1，永不超时）
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 SYS_GetLastError() 获取错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_ThreadPool_Wait(HP_ThreadPool pThreadPool, DWORD dwMilliseconds);

/***********************************************************************/
/***************************** 属性访问方法 *****************************/

/* 检查线程池组件是否已启动 */
HPSOCKET_API BOOL __HP_CALL HP_ThreadPool_HasStarted(HP_ThreadPool pThreadPool);
/* 查看线程池组件当前状态 */
HPSOCKET_API En_HP_ServiceState	__HP_CALL HP_ThreadPool_GetState(HP_ThreadPool pThreadPool);
/* 获取当前任务等待队列大小 */
HPSOCKET_API DWORD __HP_CALL HP_ThreadPool_GetQueueSize(HP_ThreadPool pThreadPool);
/* 获取当前正在执行的任务数量 */
HPSOCKET_API DWORD __HP_CALL HP_ThreadPool_GetTaskCount(HP_ThreadPool pThreadPool);
/* 获取工作线程数量 */
HPSOCKET_API DWORD __HP_CALL HP_ThreadPool_GetThreadCount(HP_ThreadPool pThreadPool);
/* 获取任务队列最大容量 */
HPSOCKET_API DWORD __HP_CALL HP_ThreadPool_GetMaxQueueSize(HP_ThreadPool pThreadPool);
/* 获取任务拒绝处理策略 */
HPSOCKET_API En_HP_RejectedPolicy __HP_CALL HP_ThreadPool_GetRejectedPolicy(HP_ThreadPool pThreadPool);

/*****************************************************************************************************************************************************/
/********************************************************* Compressor / Decompressor Exports *********************************************************/
/*****************************************************************************************************************************************************/

/****************************************************/
/******************* 对象创建函数 ********************/

/* 销毁压缩器对象 */
HPSOCKET_API void __HP_CALL Destroy_HP_Compressor(HP_Compressor pCompressor);
/* 销毁解压器对象 */
HPSOCKET_API void __HP_CALL Destroy_HP_Decompressor(HP_Decompressor pDecompressor);

#ifdef _ZLIB_SUPPORT

/* 创建 ZLib 压缩器对象 */
// （默认参数：iWindowBits = 15, iLevel = -1, iMethod = 8, iMemLevel = 8, iStrategy = 0, dwBuffSize = 16 * 1024）
HPSOCKET_API HP_Compressor __HP_CALL Create_HP_ZLibCompressor(HP_Fn_CompressDataCallback fnCallback);
/* 创建 ZLib 压缩器对象 */
HPSOCKET_API HP_Compressor __HP_CALL Create_HP_ZLibCompressorEx(HP_Fn_CompressDataCallback fnCallback, int iWindowBits /*= 15*/, int iLevel /*= -1*/, int iMethod /*= 8*/, int iMemLevel /*= 8*/, int iStrategy /*= 0*/, DWORD dwBuffSize /*= 16 * 1024*/);
/* 创建 GZip 压缩器对象 */
// （默认参数：iLevel = -1, iMethod = 8, iMemLevel = 8, iStrategy = 0, dwBuffSize = 16 * 1024）
HPSOCKET_API HP_Compressor __HP_CALL Create_HP_GZipCompressor(HP_Fn_CompressDataCallback fnCallback);
/* 创建 GZip 压缩器对象 */
HPSOCKET_API HP_Compressor __HP_CALL Create_HP_GZipCompressorEx(HP_Fn_CompressDataCallback fnCallback, int iLevel /*= -1*/, int iMethod /*= 8*/, int iMemLevel /*= 8*/, int iStrategy /*= 0*/, DWORD dwBuffSize /*= 16 * 1024*/);
/* 创建 ZLib 解压器对象 */
// （默认参数：iWindowBits = 15, dwBuffSize = 16 * 1024）
HPSOCKET_API HP_Decompressor __HP_CALL Create_HP_ZLibDecompressor(HP_Fn_DecompressDataCallback fnCallback);
/* 创建 ZLib 解压器对象 */
HPSOCKET_API HP_Decompressor __HP_CALL Create_HP_ZLibDecompressorEx(HP_Fn_DecompressDataCallback fnCallback, int iWindowBits /*= 15*/, DWORD dwBuffSize /*= 16 * 1024*/);
/* 创建 GZip 解压器对象 */
// （默认参数：dwBuffSize = 16 * 1024）
HPSOCKET_API HP_Decompressor __HP_CALL Create_HP_GZipDecompressor(HP_Fn_DecompressDataCallback fnCallback);
/* 创建 GZip 解压器对象 */
HPSOCKET_API HP_Decompressor __HP_CALL Create_HP_GZipDecompressorEx(HP_Fn_DecompressDataCallback fnCallback, DWORD dwBuffSize /*= 16 * 1024*/);

#endif

#ifdef _BROTLI_SUPPORT

/* 创建 Brotli 压缩器对象 */
//（默认参数：iQuality = 11，iWindow = 22，iMode = 0, dwBuffSize = 16 * 1024）
HPSOCKET_API HP_Compressor __HP_CALL Create_HP_BrotliCompressor(HP_Fn_CompressDataCallback fnCallback);
/* 创建 Brotli 压缩器对象 */
HPSOCKET_API HP_Compressor __HP_CALL Create_HP_BrotliCompressorEx(HP_Fn_CompressDataCallback fnCallback, int iQuality /*= 11*/, int iWindow /*= 22*/, int iMode /*= 0*/, DWORD dwBuffSize /*= 16 * 1024*/);
/* 创建 Brotli 解压器对象 */
// （默认参数：dwBuffSize = 16 * 1024）
HPSOCKET_API HP_Decompressor __HP_CALL Create_HP_BrotliDecompressor(HP_Fn_DecompressDataCallback fnCallback);
/* 创建 Brotli 解压器对象 */
HPSOCKET_API HP_Decompressor __HP_CALL Create_HP_BrotliDecompressorEx(HP_Fn_DecompressDataCallback fnCallback, DWORD dwBuffSize /*= 16 * 1024*/);

#endif

/***********************************************************************/
/***************************** 组件操作方法 *****************************/

/*
* 名称：执行压缩
* 描述：可循环调用以压缩流式或分段数据
*		
* 参数：		pData		-- 待压缩数据缓冲区
*			iLength		-- 待压缩数据长度
*			bLast		-- 是否最后一段待压缩数据
*			pContext	-- 压缩回调函数 Fn_CompressDataCallback 的上下文参数
*
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 SYS_GetLastError() 获取错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_Compressor_Process(HP_Compressor pCompressor, const BYTE* pData, int iLength, BOOL bLast, PVOID pContext /*= nullptr*/);

/*
* 名称：执行压缩
* 描述：可循环调用以压缩流式或分段数据
*		
* 参数：		pData		-- 待压缩数据缓冲区
*			iLength		-- 待压缩数据长度
*			bLast		-- 是否最后一段待压缩数据
*			bFlush		-- 是否强制刷新（强制刷新会降低压缩效率，但可对数据进行分段压缩）
*			pContext	-- 压缩回调函数 Fn_CompressDataCallback 的上下文参数
*
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 SYS_GetLastError() 获取错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_Compressor_ProcessEx(HP_Compressor pCompressor, const BYTE* pData, int iLength, BOOL bLast, BOOL bFlush /*= FALSE*/, PVOID pContext /*= nullptr*/);

/* 重置压缩器 */
HPSOCKET_API BOOL __HP_CALL HP_Compressor_Reset(HP_Compressor pCompressor);

/*
* 名称：执行解压
* 描述：可循环调用以解压流式或分段数据
*		
* 参数：		pData		-- 待解压数据缓冲区
*			iLength		-- 待解压数据长度
*			pContext	-- 解压回调函数 Fn_DecompressDataCallback 的上下文参数
*
* 返回值：	TRUE	-- 成功
*			FALSE	-- 失败，可通过 SYS_GetLastError() 获取错误代码
*/
HPSOCKET_API BOOL __HP_CALL HP_Decompressor_Process(HP_Decompressor pDecompressor, const BYTE* pData, int iLength, PVOID pContext /*= nullptr*/);

/* 重置解压器 */
HPSOCKET_API BOOL __HP_CALL HP_Decompressor_Reset(HP_Decompressor pDecompressor);

/***********************************************************************/
/***************************** 属性访问方法 *****************************/

/* 检测压缩器是否可用 */
HPSOCKET_API BOOL __HP_CALL HP_Compressor_IsValid(HP_Compressor pCompressor);

/* 检测解压器是否可用 */
HPSOCKET_API BOOL __HP_CALL HP_Decompressor_IsValid(HP_Decompressor pDecompressor);
