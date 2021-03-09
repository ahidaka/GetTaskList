#pragma once

#define TASKLIST_SIZE  (1024)

class TaskList
{
public:
	TaskList();
	~TaskList();

	DWORD GetTask(PSTR CapitalImageName, PDWORD TaskList);
	VOID Add(DWORD Pid, PTSTR Path);
	VOID Finalize();
	PDWORD Get();
	DWORD Get(INT index);
	PTSTR GetPath(INT index);
	INT Size();

private:
	INT index = 0;
	PDWORD list = NULL;
	PTSTR *paths = NULL;

};
