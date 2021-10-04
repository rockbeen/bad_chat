#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>

#pragma warning(disable: 4996)

SOCKET Connections[100];//100 соединений
int Number_Connections = 0;
void ClientHandler(int index) {
	char msg[256];
	while (true) {
		recv(Connections[index], msg, sizeof(msg), NULL);
		for (int i = 0; i < Number_Connections; i++) {
			if (i == index) {//чтобы не отправлять самому себе
				continue;
			}

			send(Connections[i], msg, sizeof(msg), NULL);
		}
	}
}
int main(int argc, char* argv[]) {
	//загружаем библиотеку
	//WSAStartup
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Error" << std::endl;
		exit(1);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(5006);
	addr.sin_family = AF_INET;//TCP/IP
	//слушаем
	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);//TCP/IP
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));//привязать адрес
	listen(sListen, SOMAXCONN);
	//удерживам соединения
	SOCKET newConnection;
	for (int i = 0; i < 100; i++)
	{
		newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

		if (newConnection == 0) {
			std::cout << "Error #2\n";
		}
		else {
			std::cout << "Client Connected!\n";
			char msg[256] = "Hello. It`s my first network program!";
			send(newConnection, msg, sizeof(msg), NULL);//передает строчку
			Connections[i] = newConnection;
			Number_Connections++;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL);//создание потока для кажлого соединения
			
		}
	}

	system("pause");
	return 0;
}