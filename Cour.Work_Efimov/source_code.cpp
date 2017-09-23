#include "main_header.h"

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