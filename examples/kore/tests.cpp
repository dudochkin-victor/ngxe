/*
 *  tests.cpp
 *  Kore-Engine
 *
 *  Created by Sean Chapel on 11/15/05.
 *  Copyright 2005 Seoushi Games. All rights reserved.
 *
 */

#include "tests.h"

using namespace std;

void fun1(void* CallerPtr, Purpose Purp)
{
	switch(Purp)
	{
		case STOP_PURPOSE:
			cout << "fun1 Stopping" << endl;
			break;
		case INIT_PURPOSE:
			cout << "fun1 Starting" << endl;
			break;
		case FRAME_PURPOSE:
			cout << "fun1 processing" << endl;
			break;
		default:
			cout << "fun1 no purpose" << endl;
			break;
	}
}

void fun2(void* CallerPtr, Purpose Purp)
{

	switch(Purp)
	{
		case STOP_PURPOSE:
			cout << "fun2 Stopping" << endl;
			break;
		case INIT_PURPOSE:
			cout << "fun2 Starting" << endl;
			break;
		case FRAME_PURPOSE:
			cout << "fun2 processing" << endl;
			break;
		default:
			cout << "fun2 no purpose" << endl;
			break;
	}

}

void fun3(void* CallerPtr, Purpose Purp)
{

	switch(Purp)
	{
		case STOP_PURPOSE:
			cout << "fun3 Stopping" << endl;
			break;
		case INIT_PURPOSE:
			cout << "fun3 Starting" << endl;
			break;
		case FRAME_PURPOSE:
			cout << "fun3 processing" << endl;
			break;
		default:
			cout << "fun3 no purpose" << endl;
			break;
	}

}

int TestProcess()
{
	cProcessManager ProcMan;

	cout << "  <process>" << endl;
	ProcMan.Process(NULL);

	cout << "  <create fun1>" << endl;
	int id1 = ProcMan.Push(fun1, NULL);

	cout << "  <create fun2>" << endl;
	ProcMan.Push(fun2, NULL);

	cout << "  <process>" << endl;
	ProcMan.Process(NULL);

	cout << "  <kill fun1>" << endl;
	ProcMan.Pop(id1);

	cout << "  <create fun 3>" << endl;
	ProcMan.Push(fun3);

	cout << "  <process>" << endl;
	ProcMan.Process(NULL);

	cout << "  <kill all>" << endl;
	ProcMan.PopAll(NULL);

	cout << "  <process>" << endl;
	ProcMan.Process(NULL);

	return 0;
}

int TestStates()
{
	cStateManager StateMan;

	cout << "  <process>" << endl;
	StateMan.Process(NULL);

	cout << "  <change to fun1>" << endl;
	StateMan.Push(fun1, NULL);

	cout << "  <change to fun2>" << endl;
	StateMan.Push(fun2, NULL);

	cout << "  <process>" << endl;
	StateMan.Process(NULL);

	cout << "  <kill fun2>" << endl;
	StateMan.Pop();

	cout << "  <change to fun3>" << endl;
	StateMan.Push(fun3);

	cout << "  <process>" << endl;
	StateMan.Process(NULL);

	cout << "  <kill all>" << endl;
	StateMan.PopAll(NULL);

	cout << "  <process>" << endl;
	StateMan.Process(NULL);


	return 0;
}
