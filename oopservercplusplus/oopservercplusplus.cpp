// oopservercplusplus.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "tcplistener.h"

using namespace std;

//Forward declaration
void Listener_MessageReceived(CTcpListener* listener, int client, string message);

int main()
{
	CTcpListener server("0.0.0.0", 9000, Listener_MessageReceived);

	if (server.Init()) {
		server.RunLoop();
	}

	return 0;
}


void Listener_MessageReceived(CTcpListener* listener, int client, string message) {
	listener->ForwardMessage(client, message);

}

