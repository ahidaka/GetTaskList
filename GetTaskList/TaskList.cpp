#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <windows.h>
#include <tlhelp32.h>
#include <shlwapi.h>
#include "TaskList.h"

TaskList::TaskList()
{
	if (list == NULL) {
		list = new DWORD[TASKLIST_SIZE];
		paths = new PTSTR[TASKLIST_SIZE];

		list[0] = 0; // counter
		paths[0] = NULL;
		index = 1;
	}
}

TaskList::~TaskList()
{
	delete list;
}

DWORD TaskList::GetTask(PSTR TaskName, PDWORD TaskList)
{
	DWORD count = 0;
	CHAR capitalBuffer[MAX_PATH];
	TCHAR wcsBuffer[MAX_PATH];
	size_t numConverted;
	BOOL allTasks = TaskName == NULL || TaskName[0] == '\0';

	if (!allTasks) {
		DWORD taskNameLength = (DWORD)strlen(TaskName);

		if (taskNameLength >= MAX_PATH) {
			fprintf(stderr, "taskNameLength is too long=%lu\n", taskNameLength);
			return 0;
		}

		for (INT i = 0; i < MAX_PATH; i++) {
			capitalBuffer[i] = toupper(TaskName[i]);
		}

		mbstowcs_s(&numConverted, wcsBuffer, (char*)capitalBuffer, MAX_PATH);
	}

	do {
		PROCESSENTRY32W entry{};
		entry.dwSize = sizeof PROCESSENTRY32W;
		HANDLE snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		if (!Process32FirstW(snapShot, &entry)) {
			fprintf(stderr, "Process32FirstW error\n");
			return 0;
		}

		do {
			if (allTasks || !_wcsicmp(entry.szExeFile, wcsBuffer)) {
				Add(entry.th32ProcessID, entry.szExeFile);
				count++;
			}
		} while (Process32NextW(snapShot, &entry));

		Finalize();

	} while (FALSE);

	return count;
}

VOID TaskList::Add(DWORD Pid, PTSTR Path)
{
	paths[index] = StrDupW(Path);
	list[index++] = Pid;
	list[0] = index;
}

INT TaskList::Size()
{
	return index;
}

VOID TaskList::Finalize()
{
	list[index] = 0;
	list[0] = index;
}

PDWORD TaskList::Get()
{
	return list;
}

DWORD TaskList::Get(INT i)
{
	return list[i];
}

PTSTR TaskList::GetPath(INT i)
{
	return paths[i];
}
