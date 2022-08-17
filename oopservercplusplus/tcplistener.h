#pragma once
#include <WS2tcpip.h>
#include <string>
#pragma comment(lib, "ws2_32.lib")

#define MAX_BUFFER_SIZE (16384)

//forward declare the class
class CTcpListener;

//callback function
typedef void (*MessageReceivedHandler)(CTcpListener* listener, int socketId, std::string message);

class CTcpListener {
	//privates
	private:
		std::string				m_ipAddress;
		int						m_port;
		MessageReceivedHandler	MessageReceived;
		
		// Create socket
		SOCKET CreateSocket();
		// Await connections
		SOCKET WaitForConnection(SOCKET listening);

	public:
		//declare CTOR
		CTcpListener(std::string ipAddress, int port, MessageReceivedHandler handler);
		// Destructor
		~CTcpListener();

		//public methods
		//Forwards the incoming message to any requester
		void ForwardMessage(int clientSocket, std::string message);

		// Init winsock
		bool Init();
	
		// Receive Loop
		void RunLoop();

		// Cleanup Socket
		void CleanUp();
};
