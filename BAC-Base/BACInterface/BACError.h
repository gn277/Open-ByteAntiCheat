#pragma once

#define BASE_ERROR											unsigned int

#define BASE_SUCCESS										1
#define BASE_UNSUCCESSFUL									10000000

#define BASE_CONNECT_TCP_SERVER_ERROR						20000000		//连接TCP服务器失败
#define BASE_CONNECT_UDP_SERVER_ERROR						20000001		//连接UDP服务器失败
#define BASE_CLIENT_LOGIN_ERROR								20000002		//BASEClient登录失败
#define BASE_CLIENT_SEND_MACHINE_INFO_ERROR					20000003		//发送机器信息失败
#define BASE_CLIENT_SEND_ABNORMAL_GAME_DATA_ERROR			20000004		//发送异常游戏数据失败
