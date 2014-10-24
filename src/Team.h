/*
 *  Team.h
 *  AI Algorithms Class
 *
 *  Created by Ned Bingham on 7/9/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef Team_h
#define Team_h

#include "AI.h"

struct Team
{
	int size;
	GLfloat color[4];
	AI *players;
	bool captured;
	int ident;
	
	vec FlagPos;
	vec ReturnPos;
	vec LinearVelocity;
	
	Team *target;
	
	void Init(int s);
	void Render(Team *teams, int num);
	void Release();
};

#endif