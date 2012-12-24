/*
 *  ProcessManager.cpp
 *  Kore-Engine
 *
 *  Created by Sean Chapel on 11/15/05.
 *  Copyright 2005 Seoushi Games. All rights reserved.
 *
 */

#include "cProcessManager.h"
#include "ePurpose.h"

cProcessManager::cProcessManager()
{
	//set sane defaults
	m_FirstProcess = NULL;
	m_LastProcess = NULL;
}

cProcessManager::~cProcessManager()
{
	PopAll(NULL);
}

int cProcessManager::Push( void (*Function)(void* CallerPtr, Purpose Purp),  void* CallerPtr)
{
	//check to see if the function is valid
	if(Function != NULL)
	{
		//make the new process
		sProcess* newProcess = new sProcess();
		newProcess->Next = NULL;
		newProcess->Prev = m_LastProcess;
		newProcess->Function = Function;

		//check to see if this is the first process added
		if(m_FirstProcess == NULL)
		{
			m_FirstProcess = newProcess;
			newProcess->Id = 1;
		}
		else
		{
			//set the appropriate process id
			newProcess->Id = m_LastProcess->Id + 1;

			//update the last process
			m_LastProcess->Next = newProcess;
		}

		//setup the new last process
		m_LastProcess = newProcess;

		//call the intializer function
		newProcess->Function(CallerPtr, INIT_PURPOSE);

		return newProcess->Id;
	}

	return 0; //indicate failure
}

bool cProcessManager::Pop( unsigned int id, void* CallerPtr)
{
	sProcess* cur_proc;
	cur_proc = m_FirstProcess;

	//try and find the id
	while(cur_proc != NULL)
	{
		if(cur_proc->Id == id)
		{
			//set the next and previous to the new ptrs

			//see if this is the first process
			if(cur_proc->Prev == NULL)
			{
				m_FirstProcess = cur_proc->Next;
			}
			else //set the previous items's next ptr to the current item's next ptr
			{
				cur_proc->Prev->Next = cur_proc->Next;
			}

			//see if this is the last process
			if(cur_proc->Next == NULL)
			{
				m_LastProcess = cur_proc->Prev;
			}
			else //set the next item's previous ptr to the current previous ptr
			{
				cur_proc->Next->Prev = cur_proc->Prev;
			}

			//tell the process we are done with them
			cur_proc->Function( CallerPtr, STOP_PURPOSE );

			//lastley set the current procs's next and previous to null so we only delete this item
			cur_proc->Next = NULL;
			cur_proc->Prev = NULL;

			//finally delete the sucker
			delete cur_proc;

			return true; //indicate success
		}

		//increment the process
		cur_proc = cur_proc->Next;
	}

	return false; //indicate failure
}

bool cProcessManager::PopAll(void* CallerPtr)
{
	sProcess* cur_proc = m_FirstProcess;
	sProcess* pop_proc;

	//if there is nothing to pop return false
	if(cur_proc == NULL)
	{
		return false;
	}

	//pop all the processes
	while(cur_proc != NULL)
	{
		//get the process to pop and increment the cur_proc
		pop_proc = cur_proc;
		cur_proc = cur_proc->Next;

		//pop the process
		Pop(pop_proc->Id, NULL);
	}

	//set first and last process ptrs to NULL for safety
	m_FirstProcess = NULL;
	m_LastProcess = NULL;

	return true;
}

bool cProcessManager::Process(void* CallerPtr)
{
	//if there are processes then process them
	if(m_FirstProcess != NULL)
	{
		sProcess* cur_proc = m_FirstProcess;

		while(cur_proc != NULL)
		{
			cur_proc->Function(CallerPtr, FRAME_PURPOSE);
			cur_proc = cur_proc->Next;
		}

		return true;
	}

	return false;
}
