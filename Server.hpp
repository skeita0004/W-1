#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <vector>
#include <string>

#pragma comment(lib, "ws2_32.lib")

class Server
{
public:
	Server();
	~Server();

	void Initialize();
	void Update();

	void JoinLocalClient();
	void JoinClient();

	void Send(const char* _buf);
	void Recv(const char* _buf);

private:
	inline static const uint16_t PORTNUM_{15423};
	inline static const std::string LOCAL_CLIENT_IP{"127.0.0.1"};

	SOCKET listenSock_;
	SOCKADDR_IN serverAddress_;

	std::vector<SOCKET> clients_;
};