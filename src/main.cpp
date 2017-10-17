#include "SDL2_header.h"

#include <cstdio>
#include <map>

namespace Game {



std::map<int, bool> keyboard;
//int main(){}
void initialize()
{
	FPS_DISPLAY = true;
}

uint32_t menuIndex = 0;

void draw()
{

	drawText( "BELOWTALE", SCREEN_WIDTH/2-165, SCREEN_HEIGHT/2-110, 60 );
	drawText( "TM", SCREEN_WIDTH/2+170, SCREEN_HEIGHT/2-110, 13 );

	uint32_t ox = -50, oy = +40;

	SDL_Color col[3];

	for(int i=0;i<3;++i)
		col[i] = {255,255,255};
	col[menuIndex] = {255,255,0};
	drawText( "Continue", SCREEN_WIDTH/2 +ox, SCREEN_HEIGHT/2 +oy, fontSize, col[0] );
	drawText( "Reset", SCREEN_WIDTH/2 +ox, SCREEN_HEIGHT/2+30 +oy, fontSize, col[1] );
	drawText( "Settings", SCREEN_WIDTH/2 +ox, SCREEN_HEIGHT/2+60 +oy, fontSize, col[2] );

	uint32_t addition = menuIndex*30;

	drawText( "♥", SCREEN_WIDTH/2-30 +ox, SCREEN_HEIGHT/2+addition +oy ,fontSize, {255,0,0} );



	setPenColor({0,255,0,255});
	Rect rect = {0,0,75,100};
	//drawRect(&rect,true);

	rect = {30,30,75-20,100-20};
	setPenColor({0,0,0,175});
	rect = {SCREEN_WIDTH/2-165, SCREEN_HEIGHT/2-110+27,200,100-20};

	drawRect(rect,true);

	setPenColor({0,255,0});
	drawLine(rand()%SCREEN_WIDTH,rand()%SCREEN_HEIGHT,rand()%SCREEN_WIDTH,rand()%SCREEN_HEIGHT);
}
double coolTime = 0;
int lastMove=-1;
void deal()
{
	bool change = 0;
	if( coolTime < duration )
	{
		lastMove = -1;
	}
	if(lastMove != KEY_UP && keyboard[KEY_UP])
	{
		menuIndex = (menuIndex-1+3)%3;
		lastMove = KEY_UP;
		change = 1;
		setCanvas(-10,rand()%20-10);
	}
	if(lastMove != KEY_DOWN && keyboard[KEY_DOWN])
	{
		menuIndex = (menuIndex+1+3)%3;
		lastMove = KEY_DOWN;
		change = 1;
		setCanvas(+10,rand()%20-10);
	}
	if(change)
	{
		coolTime = duration + 0.1;

	}else
		setCanvas(0,0);

}

void work()
{
	deal();
	draw();
}

void mousePress()
{
}

void mouseMove()
{

}

void mouseRelease()
{

}

void keyDown()
{
	keyboard[keyValue] = true;
}

void keyUp()
{
	keyboard[keyValue] = false;
}

void close()
{

}

}
