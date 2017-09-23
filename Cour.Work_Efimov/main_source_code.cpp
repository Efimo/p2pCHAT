#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string.h>
#pragma comment(lib,"Ws2_32.lib")
#define SIZE_MSG 1024

using std::endl;
using std::cout;
using std::cin;
using std::ofstream;
using std::ifstream;

const char* first = "*";

void find_word(void);

int main(){
	setlocale(LC_ALL, "rus");
	cout << "*   - Find MSG." << endl;
	cout << "        Client - 0 Server - 1" << endl;
	bool flag = false;
	cin >> flag;
	if (flag == true){
		ofstream file_history("history_server.txt", std::ios_base::trunc);
		struct sockaddr_in addr; // структура с адресом
		char buf[SIZE_MSG];    // буфер для приема
		WSADATA wsa;            // Структура с информацией о WinSock
		SOCKET sock, listener;  // дескрипторы сокетов
		// Инициализация WinSock
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
			printf("Failed. Error Code : %d", WSAGetLastError());
			perror("socket");
			exit(1);
		}
		// создаем сокет для входных подключений
		if ((listener = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
			printf("Could not create socket : %d", WSAGetLastError());
			perror("socket");
			exit(1);
		}

		// Указываем параметры сервера
		addr.sin_family = AF_INET;
		addr.sin_port = htons(3425);
		addr.sin_addr.s_addr = htonl(INADDR_ANY);

		if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
			perror("bind");
			exit(2);
		}

		listen(listener, 1); // очередь входных подключений

		while (1){
			sock = accept(listener, NULL, NULL); // принимаем входные подключение и создаем отделный сокет для каждого нового подключившегося клиента
			if (sock < 0) {
				perror("accept");
				exit(3);
			}
			else{
				closesocket(listener);
			}
			while (1){
				recv(sock, buf, SIZE_MSG, 0);//чтение данных из сокета
				
				file_history << "<< " << buf << endl;
				cout << "<< " << buf << endl;
				cout << ">> ";
				file_history << ">> ";
				memset(buf, 0, SIZE_MSG);
				cin >> buf;
				if (stricmp(first, reinterpret_cast<const char*>(buf)) == 0){
					find_word();
				}
				file_history << buf << endl;
				send(sock, buf, SIZE_MSG, 0);
			}

			cout << "Client disconnect!" << endl;
			closesocket(sock); // закрываем сокет
			file_history.close();
			if (WSACleanup()){
				cout << "Error Cleapir\n";
			}
			else{
				cout << "Cleapir Good !!!!!\n";
			}
		}
	}//end (if flag = server)
	else{
		ofstream file_history("history_server.txt", std::ios_base::trunc);
		char buf[SIZE_MSG]; 		  // буфур для приема
		struct sockaddr_in addr; // структура с адресом

		WSADATA wsa;        // Структура с информацией о WinSock
		SOCKET sock;        // дескриптор сокета

		// Инициализация WinSock
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
			printf("Failed. Error Code : %d", WSAGetLastError());
			perror("socket");
			exit(1);
		}
		// создаем сокет для входных подключений
		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
			printf("Could not create socket : %d", WSAGetLastError());
			perror("socket");
			exit(1);
		}
		// Указываем параметры сервера
		addr.sin_family = AF_INET; // домены Internet
		addr.sin_port = htons(3425); // или любой другой порт...
		addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
		// установка соединения с сервером
		if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
			perror("connect");
			exit(2);
		}

		send(sock, "Hi!", SIZE_MSG, 0); // отправка первого сообщения на сервер
		file_history << ">> " << "I'm on-line" << endl;
		while (1){
			recv(sock, buf, SIZE_MSG, 0);

			file_history << "<< " << buf << endl;
			cout << "<< " << buf << endl;
			cout << ">> ";
			file_history << ">> ";
			memset(buf, 0, SIZE_MSG);
			cin >> buf;
			if (stricmp(first, reinterpret_cast<const char*>(buf)) == 0){
				find_word();
			}
			file_history << buf << endl;
			send(sock, buf, SIZE_MSG, 0);
		}
		closesocket(sock);
		file_history.close();
		if (WSACleanup()){
			cout << "Error Cleapir\n";
		}
		else{
			cout << "Cleapir Good !!!!!\n";
		}
	}
	return 0;
}

void find_word(void){
	ifstream file("history_server.txt");
	char pCFindWord[SIZE_MSG];
	cout << "Input word: ";
	cin >> pCFindWord;
	char cBuffer[SIZE_MSG];
	while (!file.eof()){
		file.getline(cBuffer, SIZE_MSG);
		if (strstr(cBuffer, pCFindWord) != NULL){
			cout << "MSG FIND: " << cBuffer << endl;
		}
		else{
			cout << "MSG NO FIND :-( " << endl;
		}
		memset(cBuffer, 0, SIZE_MSG);
	}
	file.close();
}