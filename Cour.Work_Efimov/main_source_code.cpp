#include "main_header.h"

const char* first = "*";

//main function in project
int main(){
	setlocale(LC_ALL, "rus"); //for russian symbols
	cout << "*   - Find MSG." << endl;
	cout << "        Client - 0 Server - 1" << endl;
	bool flag = false; //flag: client or server
	cin >> flag;
	if (flag == true){ // server
		ofstream file_history("history_server.txt", std::ios_base::trunc);
		struct sockaddr_in addr;
		char buf[SIZE_MSG]; // box(buffer) for msg
		WSADATA wsa;
		SOCKET sock, listener;
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
			printf("Failed. Error Code : %d", WSAGetLastError());
			perror("socket");
			exit(1);
		}
		if ((listener = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
			printf("Could not create socket : %d", WSAGetLastError());
			perror("socket");
			exit(1);
		}

		addr.sin_family = AF_INET;
		addr.sin_port = htons(3425);
		addr.sin_addr.s_addr = htonl(INADDR_ANY);

		if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
			perror("bind");
			exit(2);
		}

		listen(listener, 1);

		while (1){
			sock = accept(listener, NULL, NULL);
			if (sock < 0) {
				perror("accept");
				exit(3);
			}
			else{
				closesocket(listener);
			}
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

			cout << "Client disconnect!" << endl;
			closesocket(sock);
			file_history.close();
			if (WSACleanup()){
				cout << "Error Cleapir\n";
			}
			else{
				cout << "Cleapir Good !!!!!\n";
			}
		}
	}
	else{//client
		ofstream file_history("history_server.txt", std::ios_base::trunc);
		char buf[SIZE_MSG];
		struct sockaddr_in addr;

		WSADATA wsa;
		SOCKET sock;

		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
			printf("Failed. Error Code : %d", WSAGetLastError());
			perror("socket");
			exit(1);
		}

		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
			printf("Could not create socket : %d", WSAGetLastError());
			perror("socket");
			exit(1);
		}

		addr.sin_family = AF_INET;
		addr.sin_port = htons(3425);
		addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	
		if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
			perror("connect");
			exit(2);
		}

		send(sock, "Hi!", SIZE_MSG, 0);
		file_history << ">> " << "Hi!" << endl;
		while (1){
			recv(sock, buf, SIZE_MSG, 0);
			file_history << "<< " << buf << endl << ">> " << endl;;
			cout << "<< " << buf << endl << ">> " << endl;;
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