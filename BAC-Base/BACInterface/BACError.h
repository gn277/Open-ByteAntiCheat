#pragma once

#define BASE_ERROR											unsigned int

#define BASE_SUCCESS										1
#define BASE_UNSUCCESSFUL									10000000

#define BASE_CONNECT_TCP_SERVER_ERROR						20000000		//����TCP������ʧ��
#define BASE_CONNECT_UDP_SERVER_ERROR						20000001		//����UDP������ʧ��
#define BASE_CLIENT_LOGIN_ERROR								20000002		//BASEClient��¼ʧ��
#define BASE_CLIENT_SEND_MACHINE_INFO_ERROR					20000003		//���ͻ�����Ϣʧ��
#define BASE_CLIENT_SEND_ABNORMAL_GAME_DATA_ERROR			20000004		//�����쳣��Ϸ����ʧ��
