#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string.h>
#pragma comment(lib,"Ws2_32.lib")
#define SIZE_MSG 1024

using namespace std;

const char* first = "*";
const char* twice = "**";
const char* third = "***";

int main(){
	setlocale(LC_ALL, "rus");
	cout << "*   - Найти сообщение." << endl;
	cout << "        Client - 0 Server - 1" << endl;
	bool flag = false;
	cin >> flag;
	if (flag == true){
		ofstream file_history("history_server.txt", ios_base::trunc);
		struct sockaddr_in addr; // структура с адресом
		char buf[SIZE_MSG];    // буфур для приема
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
				if (stricmp("start_send_history_$Q@dhasjdhagd^@&$#!@", reinterpret_cast<const char*>(buf)) == 0){
					file_history.close();
					ifstream file_read("history_server.txt", ios_base::in);
					memset(buf, 0, SIZE_MSG);
					while (!file_read.getline(buf, SIZE_MSG).eof()){
						send(sock, buf, SIZE_MSG, 0);
						Sleep(20);
						memset(buf, 0, SIZE_MSG);
					}
					send(sock, "stop_send_history_df32*2h&#@^!#$hjgh2$#", SIZE_MSG, 0);
					file_read.close();
					file_history.open("history_server.txt", ios_base::app);
					send(sock, "History send!", SIZE_MSG, 0);
					continue;
				}
				file_history << "<< " << buf << endl;
				cout << "<< " << buf << endl;
				cout << ">> ";
				file_history << ">> ";
				memset(buf, 0, SIZE_MSG);
				cin >> buf;
				if (stricmp(first, reinterpret_cast<const char*>(buf)) == 0){
					ifstream file_1("history_server.txt");
					char pCFindWord_1[SIZE_MSG];
					cout << "Input word: ";
					cin >> pCFindWord_1;
					char cBuffer_1[SIZE_MSG];
					while (!file_1.eof()){
						file_1.getline(cBuffer_1, SIZE_MSG);
						if (strstr(cBuffer_1, pCFindWord_1) != NULL){
							cout << "MSG FIND: " << cBuffer_1 << endl;
						}
						memset(cBuffer_1, 0, SIZE_MSG);
					}
					file_1.close();
				}
				file_history << buf << endl;
				send(sock, buf, SIZE_MSG, 0);
			}

			cout << "Client disconnect!" << endl;
			closesocket(sock); // закрываем сокет
			file_history.close();
		}
	}//end (if flag = server)
	else{
		ofstream file_history("history_server.txt", ios_base::trunc);
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

		send(sock, "I'm on-line", SIZE_MSG, 0); // отправка сообщения на сервер
		file_history << ">> " << "I'm on-line" << endl;
		while (1){
			recv(sock, buf, SIZE_MSG, 0);
			if (stricmp("start_send_history_$Q@dhasjdhagd^@&$#!@", reinterpret_cast<const char*>(buf)) == 0){
				file_history.close();
				ifstream file_read("history_server.txt", ios_base::in);
				memset(buf, 0, SIZE_MSG);
				while (!file_read.getline(buf, SIZE_MSG).eof()){
					send(sock, buf, SIZE_MSG, 0);
					Sleep(10);
					memset(buf, 0, SIZE_MSG);
				}
				send(sock, "stop_send_history_df32*2h&#@^!#$hjgh2$#", SIZE_MSG, 0); //отправка данных
				file_read.close();
				file_history.open("history_server.txt", ios_base::app);//открытие файла для записи в конец файла
				send(sock, "History send!", SIZE_MSG, 0); // отправка истории
				continue;
			}
			file_history << "<< " << buf << endl;
			cout << "<< " << buf << endl;
			cout << ">> ";
			file_history << ">> ";
			memset(buf, 0, SIZE_MSG);
			cin >> buf;
			if (stricmp(first, reinterpret_cast<const char*>(buf)) == 0){
				ifstream file_2("history_server.txt");
				char pCFindWord_2[SIZE_MSG];
				cout << "Input word: ";
				cin >> pCFindWord_2;
				char cBuffer_2[SIZE_MSG];
				while (!file_2.eof()){
					file_2.getline(cBuffer_2, SIZE_MSG);
					if (strstr(cBuffer_2, pCFindWord_2) != NULL){
						cout << "MSG FIND: " << cBuffer_2 << endl;
					}
					memset(cBuffer_2, 0, SIZE_MSG);
				}
				file_2.close();
			}
			file_history << buf << endl;
			send(sock, buf, SIZE_MSG, 0);
		}
		closesocket(sock); // закрываем сокет
		file_history.close();
	}//end (if flag = client)
	return 0;
}
