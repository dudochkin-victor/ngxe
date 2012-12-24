/*
 *  ProcessManager.h
 *  Kore-Engine
 *
 *  Created by Sean Chapel on 11/15/05.
 *  Copyright 2005 Seoushi Games. All rights reserved.
 *
 */

#ifndef PROC_MAN
#define PROC_MAN

#include "../globals.h"
#include "ePurpose.h"

struct sProcess
{
		sProcess* Next;
		sProcess* Prev;
		void (*Function)(void* CallerPtr, Purpose Purp);

		unsigned int Id;

		sProcess()
		{
			Prev = NULL;
			Next = NULL;
			Function = NULL;
			Id = 0;
		}

		~sProcess()
		{
			delete Prev;
			delete Next;
		}
};


class cProcessManager
{
	public:
			cProcessManager();
			~cProcessManager();
			int Push( void (*Function)(void* CallerPtr, Purpose Purp),  void* CallerPtr = NULL);
			bool Pop( unsigned int id, void* CallerPtr = NULL);
			bool PopAll( void* CallerPtr = NULL);
			bool Process( void* CallerPtr = NULL);

	private:
			sProcess* m_FirstProcess;
			sProcess* m_LastProcess;
};

#endif
