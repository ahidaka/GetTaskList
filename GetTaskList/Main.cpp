#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <windows.h>
#include "TaskList.h"

HRESULT GetTasks(PSTR Name);

INT main(INT ac, CHAR **av)
{
	CHAR* taskName;

	taskName = NULL;

	if (ac > 1) {
		taskName = av[1];
	}

	printf("Start GetTaskList=%s\n", taskName);

	(VOID)GetTasks((PSTR) taskName);

	return 0;
}

HRESULT GetTasks(PSTR Name)
{
	DWORD count;
	DWORD lists[128];
	TaskList* pidList = new TaskList();

	count = pidList->GetTask(Name, lists);

	printf("ImageNamePID: count=%d\n", count);

	for (INT i = 1; i < pidList->Size(); i++) {
		DWORD pid = pidList->Get(i);
		printf("%d: Pid(%ws): %0X (%lu)\n",
			i, pidList->GetPath(i), pid, pid);
	}

	return S_OK;
}
