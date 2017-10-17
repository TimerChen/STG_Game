#include "SDL2_header.h"

#include <cstdio>

namespace Game {



bool keyboard[1000];
//int main(){}
void initialize()
{

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
}

void work(const double &duration)
{

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
	if(keyValue == SDLK_UP)
		menuIndex = (menuIndex+3-1)%3;
	if(keyValue == SDLK_DOWN)
		menuIndex = (menuIndex+3+1)%3;
	return ;
	std::cout << keyValue << std::endl;
	if(keyValue >= 0)
	keyboard[keyValue] = true;
}

void keyUp()
{
	return ;
	std::cout << keyValue << std::endl;
	if(keyValue >= 0)
	keyboard[keyValue] = false;

}

void close()
{

}

}
