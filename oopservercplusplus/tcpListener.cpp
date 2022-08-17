#include "tcplistener.h"
#include <iostream>
#include <string>


//declare CTOR
CTcpListener::CTcpListener(std::string ipAddress, int port, MessageReceivedHandler handler) :
	m_ipAddress(ipAddress), m_port(port), MessageReceived(handler) {}

// Destructor
CTcpListener::~CTcpListener() { CleanUp(); }
//privates
// Create socket
SOCKET CTcpListener::CreateSocket() {
	SOCKET listening = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listening != INVALID_SOCKET) {
		sockaddr_in hint{};
		hint.sin_family = AF_INET;
		hint.sin_port = htons(m_port);
		inet_pton(AF_INET, m_ipAddress.c_str(), &hint.sin_addr);

		int bindOk = bind(listening, (sockaddr*)&hint, sizeof(hint));
		if (bindOk != SOCKET_ERROR) {
			int listenOk = listen(listening, 250);

			if (listenOk == SOCKET_ERROR) {
				return -1;
			}
		}
		else {
			return -1;
		}
		std::cout << "Ready to receive connections on port: " << this->m_port << std::endl;
		return listening;
	}
}

// Await connections
SOCKET CTcpListener::WaitForConnection(SOCKET listening) {
	SOCKET client = accept(listening, NULL, NULL);
	std::cout << "client connected" << std::endl;
	return client;
}

//public methods
//Forwards the incoming message to any requester
void CTcpListener::ForwardMessage(int clientSocket, std::string message) {
	send(clientSocket, message.c_str(), message.size() + 1, 0);
}

// Init winsock
bool CTcpListener::Init() {
	WORD ver = MAKEWORD(2, 2);
	WSADATA data;
	return WSAStartup(ver, &data) == 0;

}

// Receive Loop
void CTcpListener::RunLoop() {

	char buffer[MAX_BUFFER_SIZE];

	while (true) {
		SOCKET listening = this->CreateSocket();
		if (listening == SOCKET_ERROR) {
			break;
		}
		SOCKET client = this->WaitForConnection(listening);
		if (client != INVALID_SOCKET) {
			closesocket(listening);
			int bytesReceived = 0;
			do {
				memset(&buffer, 0, MAX_BUFFER_SIZE);
				bytesReceived = recv(client, buffer, MAX_BUFFER_SIZE, 0);

				if (bytesReceived > 0) {
					if (MessageReceived != NULL) {
						std::string msgRecv = std::string(buffer, 0, bytesReceived);
						if(!msgRecv.empty())
							std::cout << "message received: " << msgRecv <<  std::endl;
						MessageReceived(this, client, msgRecv);
					}
				}

			} while (bytesReceived > 0);
			closesocket(client);
		}
	}

}

// Cleanup Socket
void CTcpListener::CleanUp() { WSACleanup(); }