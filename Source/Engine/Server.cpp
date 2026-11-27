#include "Server.hpp"

Server::Server() :
	listenSock_(INVALID_SOCKET),
	serverAddress_(),
	clients_()
{
}

Server::~Server()
{
}

void Server::Initialize()
{
}

void Server::Update()
{
}

void Server::JoinLocalClient()
{
}

void Server::JoinClient()
{
}

void Server::Send(const char* _buf)
{
}

void Server::Recv(const char* _buf)
{
}
