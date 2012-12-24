
/* main.cpp
 *  Kore-Engine
 *
 *  Created by Sean Chapel on 11/15/05.
 *  Copyright 2005 Seoushi Games. All rights reserved.
 *
 */
   
#include "globals.h"
#include "example.h"

using namespace std;

int main(int argc, char *argv[])
{	
	Example game;
	
	game.Init();
	game.Run();
	game.Shutdown();
	
    return 0;
}
