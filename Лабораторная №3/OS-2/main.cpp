#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <ctime>
#include <string>
#include <system_error>
#include "WriteInFile.h"
using namespace std;

/*#define CREATE_PROCESS_ERROR 1
#define MAP_VIEW_OF_FILE_ERROR 2*/

//HANDLE hFileLog = CreateFile(TEXT("log.txt"), GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

typedef struct
{
	STARTUPINFO startupinfo;//���������, ������������ � ����-� CreateProcess, ����� ���������� ������� ��������, ���. ����, 
							//��������. ����������, ������� ��� ���. ���� ��� ������ ��������
	PROCESS_INFORMATION process_information;
	//��������� ����������� �������� CreateProcess � ����������� � ������� ��������� �������� � ��� ��������� ������
} processDate;

processDate initializationProcDate()
{
	processDate pd;
	ZeroMemory(&pd.startupinfo, sizeof(pd.startupinfo));
	pd.startupinfo.cb = sizeof(pd.startupinfo);
	ZeroMemory(&pd.process_information, sizeof(pd.process_information));
	return pd;
}

void close(processDate pd)
{
	CloseHandle(pd.process_information.hProcess);
	CloseHandle(pd.process_information.hThread);
}

BOOL createProcess(TCHAR commandLine[], processDate  pd)
{
	return CreateProcess(NULL,//��� ����������; ������ ��������� ������ ���� ��� NULL
		commandLine,//��������� ������. ������� ������������
		NULL,//�������� ��������
		NULL,//�������� ������
		FALSE,//����������� ������������
		0,//�������� ������
		NULL,//�����
		NULL,//������� ����������
		&pd.startupinfo,
		&pd.process_information);
}

/*void log(char* currentProcess, char* number)
{
	SYSTEMTIME systemtime;
	GetLocalTime(&systemtime);
	string s_time = to_string(systemtime.wHour) + ':' + to_string(systemtime.wMinute) + ':' + to_string(systemtime.wSecond) + ':' + to_string(systemtime.wMilliseconds);
	const char* ch_time = s_time.c_str();
	WriteFile(hFileLog, ch_time, sizeof(ch_time) * 3, NULL, NULL);
	WriteFile(hFileLog, " ", sizeof(" "), NULL, NULL);
	WriteFile(hFileLog, currentProcess, sizeof(char) * 2, NULL, NULL);
	WriteFile(hFileLog, " ", sizeof(" "), NULL, NULL);
	WriteFile(hFileLog, number, sizeof(number), NULL, NULL);
	WriteFile(hFileLog, "\n", 1, NULL, NULL);
}*/

int _tmain(int argc, TCHAR *argv[])
{
	WriteInFile writeInFile;
	writeInFile.clearFile();
	DWORD lastError;

	//HANDLE semaphore = CreateSemaphore(NULL, 1, 1, TEXT("MainProcessSemaphore"));
	HANDLE mutex = CreateMutex(NULL, TRUE, TEXT("MainProcessMutex"));
	lastError = GetLastError();
	if (lastError != 0)
	{
		writeInFile.writeErrorInLog("CreateSemaphore's in main Error", lastError);
		return lastError;
	}
	HANDLE fileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(int), TEXT("FileMapping"));;
	lastError = GetLastError();
	if (lastError != 0)
	{
		writeInFile.writeErrorInLog("CreateFileMapping's in main Error", lastError);
		CloseHandle(mutex);
		return lastError;
	}

	TCHAR consoleProcessCL[] = TEXT("ConsoleProcess");
	TCHAR fileProcessCL[] = TEXT("FileProcess");

	int buffer[20];

	srand(time(0));
	for (int i = 0; i < 20; i++)
		buffer[i] = rand() % 1000 + 1;

	processDate consoleData = initializationProcDate();
	processDate fileData = initializationProcDate();

	BOOL cpSuccess = createProcess(consoleProcessCL, consoleData);
	if (!cpSuccess)
	{
		lastError = GetLastError();
		writeInFile.writeErrorInLog("ConsoleProcess's creation's Error", lastError);
		CloseHandle(mutex);
		CloseHandle(fileMapping);
		return lastError;
	}
	BOOL fpSuccess = createProcess(fileProcessCL, fileData);
	if (!fpSuccess)
	{
		lastError = GetLastError();
		writeInFile.writeErrorInLog("FileProcess's creation's Error", lastError);
		CloseHandle(mutex);
		CloseHandle(fileMapping);
		return lastError;
	}

	//if (createProcess(consoleProcessCL, consoleData) && createProcess(fileProcessCL, fileData))//?
	if (cpSuccess && fpSuccess)
	{
		char* ch = (char*)MapViewOfFile(fileMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
		if (ch == NULL)
		{
			lastError = GetLastError();
			writeInFile.writeErrorInLog("MapViewOfFile's in main Error", lastError);
			close(consoleData);
			close(fileData);
			CloseHandle(mutex);
			CloseHandle(fileMapping);
			return lastError;
		}
		for (int i = 0; i < (sizeof(buffer) / sizeof(int)) * 2; i++)
		{
			WaitForSingleObject(mutex, INFINITE);
			//WaitForSingleObject(semaphore, INFINITE);
			int index = i;
			if (i % 2 == 1)
				index--;
			string s = to_string(buffer[index]);
			char const *str = s.c_str();
			CopyMemory(ch, str, sizeof(int));
			ReleaseMutex(mutex);
			//ReleaseSemaphore(semaphore, 1, NULL);
			/*if (i % 2 == 1)
				log("FP", ch);
			else
				log("CP", ch);*/
			//ReleaseSemaphore(semaphore, 1, NULL);
			//Sleep(400);
		}
		UnmapViewOfFile(ch);
	}
	/*else
	{
		if (!cpSuccess)
		{
			lastError = GetLastError();
			writeInFile.writeErrorInLog("ConsoleProcess's creation's Error", lastError);
			//writeInFile.writeErrorInLog("Error of ConsoleProcess's creation", CREATE_PROCESS_ERROR);
		}
		if (!fpSuccess)
		{
			lastError = GetLastError();
			writeInFile.writeErrorInLog("FileProcess's creation's ", lastError);
		}
			//writeInFile.writeErrorInLog("Error of FileProcess's creation", CREATE_PROCESS_ERROR);
	}*/
	close(consoleData);
	close(fileData);
	CloseHandle(mutex);
	CloseHandle(fileMapping);
}