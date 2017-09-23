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

void find_word(void);