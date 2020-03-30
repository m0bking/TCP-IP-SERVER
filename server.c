#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include<winsock2.h>
#include<WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

void main() {

	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int ws0k = WSAStartup(ver, &wsData);

	if (ws0k != 0) {
		printf("Can't start winsock! \n");
		return;
	}

	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);

	if (listening == INVALID_SOCKET) {

		printf("Unable to create socket!\n");
		return;
	}

	SOCKADDR_IN hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(60000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY; // vsichki vuzmojni localni addresi ?

	bind(listening, (SOCKADDR*)&hint, sizeof(hint));

	listen(listening, SOMAXCONN);

	fd_set master;
	FD_ZERO(&master);
	FD_SET(listening, &master);



	while (TRUE) {
		fd_set copy = master;

		int socketCount = select(0, &copy, NULL, NULL, NULL);

		for (int i = 0; i < socketCount; i++) {
			SOCKET sock = copy.fd_array[i];

			if (sock == listening) {

				SOCKET client = accept(listening, NULL, NULL); // kak da zapametq info za connceted client

				FD_SET(client, &master);

				char message[] = "Connected to Server!\n";

				send(client, message, sizeof(message), 0);
			}
			else {
				char  buf[4000];
		
				ZeroMemory(buf, 4000);
				int bytesReceived = recv(sock, buf, 4000, 0);

				if (bytesReceived <= 0) {
					closesocket(sock);
					FD_CLR(sock, &master);
				}
				else {
					for (int i = 0; i < master.fd_count; i++) {
						SOCKET outSock = master.fd_array[i];
						if (outSock != listening && outSock != sock) {
							

							send(outSock, buf, bytesReceived, 0);
						}
					}
				}

			}
		}
	}
	WSACleanup();
	system("pause");
}














