#include "SDL2_header.h"

#include <cstdio>
#include <map>

namespace Game {


const int SCREEN_WIDTH	= 640;
const int SCREEN_HEIGHT	= 480;

std::map<int, bool> keyboard;

void initialize()
{
	FPS_DISPLAY = true;
}

uint32_t menuIndex = 0;

void draw()
{

}
void deal()
{

}

int work( bool &quit )
{
	deal();
	draw();

	if( keyboard[KEY_ESC] )
		quit = true;
	return 0;
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

void finale()
{

}

}
