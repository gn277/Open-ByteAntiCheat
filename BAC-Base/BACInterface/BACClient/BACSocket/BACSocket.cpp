#include "BACSocket.h"


BACTcpSocket::BACTcpSocket(int af, int type, int protocol)
{
	WSADATA wsa_data;

	//��ʼ��wsa
	auto result = ::WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (result != 0)
	{
		std::cout << "wsa startup error" << std::endl;
		return;
	}

	//����socket
	this->_socket = ::socket(af, type, protocol);

}

BACTcpSocket::~BACTcpSocket()
{	
	//�ر�socket
	if (this->_socket)
		closesocket(this->_socket);

	::WSACleanup();

}

int BACTcpSocket::Connect(int af, const char* host, int port)
{
	//���жϹ��캯����socket�Ƿ񴴽��ɹ���ʧ�ܷ��� 2
	if (!this->_socket)
		return 2;

	//���ӷ�������ʧ�ܷ��� 3
	SOCKADDR_IN address = { NULL };
	address.sin_family = af;
	address.sin_port = htons(port);
	address.sin_addr.S_un.S_addr = inet_addr(host);

	auto result = ::connect(this->_socket, (sockaddr*)&address, sizeof(address));
	if (result == -1)
	{
		std::cout << "connect server error:" << std::hex << ::GetLastError() << std::endl;
		return 3;
	}

	//�ɹ����� 1
	return true;
}

bool BACTcpSocket::Listen()
{
	//�����˿ڣ�ʧ�ܷ���2
	auto result = ::listen(this->_socket, 10);
	if (result == -1)
	{
		std::cout << "listen port error: " << ::GetLastError() << std::endl;

		closesocket(this->_socket);
		::WSACleanup();
		return false;
	}

	return true;
}

SOCKET BACTcpSocket::Accept(sockaddr FAR* addr, int FAR* addrlen)
{
	return ::accept(this->_socket, addr, addrlen);
}

int BACTcpSocket::Recv(char FAR* buf, int len)
{
	auto readed = ::recv(this->_socket, buf, len, 0);
	return readed;
}

int BACTcpSocket::Send(const char FAR* buf, int len)
{
	auto sended = ::send(this->_socket, buf, len, 0);
	return sended;
}


TcpServer::TcpServer(int af, int type, int protocol) :BACTcpSocket(af, type, protocol)
{
}

TcpServer::~TcpServer()
{
}

auto TcpServer::Send(BACCommand cmd, std::string data, BACStatus status) -> int
{
	//��Ӱ�ͷ��Ϣ
	BACPacketHeader header = { NULL };
	header.data_len = data.size();
	header.status = status;
	header.cmd = cmd;

	//��header������׷�ӵ�buffer��
	std::string buffer((const char*)&header, sizeof(BACPacketHeader));
	buffer.append(data);

	return this->BACTcpSocket::Send(buffer.data(), buffer.size());
}

auto TcpServer::Recv(std::string* buffer, int recv_len) -> int
{
	buffer->clear();
	char* temp = new char[recv_len];

	auto recved = this->BACTcpSocket::Recv(temp, recv_len);

	buffer->append(temp, recved);
	delete[] temp;

	return recved;
}

auto TcpServer::Recv(int recv_len) -> std::string
{
	std::string temp_data;
	char* temp = new char[recv_len];

	auto recved = this->BACTcpSocket::Recv(temp, recv_len);

	temp_data.append(temp, recved);
	delete[] temp;

	return temp_data;
}

auto TcpServer::GetResponseStatus(std::string buffer) -> BACStatus
{
	PBACPacketHeader p_header = (PBACPacketHeader)buffer.data();

	return p_header->status;
}

auto TcpServer::GetCommand(std::string buffer) -> BACCommand
{
	PBACPacketHeader p_header = (PBACPacketHeader)buffer.data();

	return p_header->cmd;
}

auto TcpServer::GetDataLength(std::string buffer) -> unsigned long long
{
	PBACPacketHeader p_header = (PBACPacketHeader)buffer.data();

	return p_header->data_len;
}

