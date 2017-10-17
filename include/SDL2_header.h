/*
 *
 * Coded by Timer Chen.
 * Part of this code came from https://github.com/Twinklebear/TwinklebearDev-Lessons
 *
 *
 */

#ifndef SDL2_HEADER_H
#define SDL2_HEADER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_mixer.h>
//#include <SDL2/SDL_thread.h>
#include <iconv.h>

#include <iostream>
#include <cmath>
#include <algorithm>
#include <string>

#include "cleanup.h"
#include "res_path.h"

namespace Game {



#define KEY_SPACE  SDLK_SPACE
#define KEY_ENTER  SDLK_RETURN
#define KEY_RETURN SDLK_RETURN
#define KEY_TAB    SDLK_TAB
#define KEY_ESC    SDLK_ESCAPE
#define KEY_UP    SDLK_UP
#define KEY_DOWN  SDLK_DOWN
#define KEY_LEFT  SDLK_LEFT
#define KEY_RIGHT SDLK_RIGHT

#define MOUSE_LEFT   SDL_BUTTON_LEFT
#define MOUSE_MIDDLE SDL_BUTTON_MIDDLE
#define MOUSE_RIGHT  SDL_BUTTON_RIGHT


SDL_Renderer	*renderer	= NULL;
SDL_Window		*window		= NULL;

bool FPS_DISPLAY	= false;
double nowFPS;

bool mousePressed	= false;
bool mouseDragged	= false;
uint8_t mouseButton;
int mouseX = 0;
int mouseY = 0;
int pmouseX = -1;
int pmouseY = -1;
bool keyPressed = false;
SDL_Keycode keyValue;

const unsigned int FPS_RATE = 60;
const std::string RES_PATH_IMG = getResourcePath("image");
const std::string RES_PATH_FONT = getResourcePath("fonts");

// Functions for programming
extern void initialize();
extern void work(const double&);
extern void mousePress();
extern void mouseMove();
extern void mouseRelease();
extern void keyDown();
extern void keyUp();
extern void close();

//Screen attributes
const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;
std::string	fontName = "msyh";
int			fontSize = 25;

/*
 * Log an SDL error with some error message to the output stream of our choice
 * @param os The output stream to write the message too
 * @param msg The error message to write, format will be msg error: SDL_GetError()
 */
void logSDLError(std::ostream &os, const std::string &msg){
	os << msg << " error: " << SDL_GetError() << std::endl;
}
/*
 * Loads an image into a texture on the rendering device
 * @param file The image file to load
 * @param ren The renderer to load the texture onto
 * @return the loaded texture, or nullptr if something went wrong.
 */
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren){
	SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
	if (texture == nullptr){
		logSDLError(std::cout, "LoadTexture");
	}
	return texture;
}
/*
 * Draw an SDL_Texture to an SDL_Renderer at some destination rect
 * taking a clip of the texture if desired
 * @param tex The source texture we want to draw
 * @param rend The renderer we want to draw too
 * @param dst The destination rectangle to render the texture too
 * @param clip The sub-section of the texture to draw (clipping rect)
 *		default of nullptr draws the entire texture
 */
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst,
				   const double &angle = 0, const SDL_Point *center = NULL,
				   const SDL_RendererFlip &flip = SDL_FLIP_NONE,
				   const SDL_Rect *clip = nullptr)
{
	//SDL_RenderCopy(ren, tex, clip, &dst);
	SDL_RenderCopyEx(ren, tex, clip, &dst, angle, center, flip );
}
/*
 * Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
 * the texture's width and height and taking a clip of the texture if desired
 * If a clip is passed, the clip's width and height will be used instead of the texture's
 * @param tex The source texture we want to draw
 * @param rend The renderer we want to draw too
 * @param x The x coordinate to draw too
 * @param y The y coordinate to draw too
 * @param clip The sub-section of the texture to draw (clipping rect)
 *		default of nullptr draws the entire texture
 */
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y,
				   const double &angle = 0, const SDL_Point *center = NULL,
				   const SDL_RendererFlip &flip = SDL_FLIP_NONE,
				   const SDL_Rect *clip = nullptr){
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	if (clip != nullptr){
		dst.w = clip->w;
		dst.h = clip->h;
	}else
	{
		SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	}
	renderTexture(tex, ren, dst, angle, center, flip, clip);
}
/*
 * Render the message we want to display to a texture for drawing
 * @param message The message we want to display
 * @param fontFile The font we want to use to render the text
 * @param color The color we want the text to be
 * @param fontSize The size we want the font to be
 * @return An SDL_Texture containing the rendered message, or nullptr if something went wrong
 */
SDL_Texture* renderText(const std::string &message, const std::string &fontFile, SDL_Color color,
		int fontSize, SDL_Renderer *renderer)
{
	//Open the font
	TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
	if (font == nullptr){
		logSDLError(std::cout, "TTF_OpenFont");
		return nullptr;
	}
	//We need to first render to a surface as that's what TTF_RenderText returns, then
	//load that surface into a texture
	SDL_Surface *surf = TTF_RenderUTF8_Solid(font, message.c_str(), color);
	if (surf == nullptr){
		TTF_CloseFont(font);
		logSDLError(std::cout, "TTF_RenderText");
		return nullptr;
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture == nullptr){
		logSDLError(std::cout, "CreateTexture");
	}
	//Clean up the surface and font
	SDL_FreeSurface(surf);
	TTF_CloseFont(font);
	return texture;
}

// ----------- Functions for users ---------------



#define Image		SDL_Texture
#define Color		SDL_Color
#define Rect		SDL_Rect
#define Point		SDL_Point
#define FlipType	SDL_RendererFlip

#define FLIP_NONE		SDL_FLIP_NONE			/**< Do not flip */
#define FLIP_HORIZONTAL SDL_FLIP_HORIZONTAL		/**< flip horizontally */
#define FLIP_VERTICAL	SDL_FLIP_VERTICAL		/**< flip vertically */

Image* textToImage( const std::string &msg,
					const int32_t &size = fontSize, const Color &color = {255,255,255},
					const std::string &fontType = fontName)
{
	return renderText(msg, RES_PATH_FONT + fontName + ".ttf", color, size, renderer);
}


/*
 * Loads an image
 * @param file The image file to load
 * @return the loaded image, or nullptr if something went wrong.
 */
Image* loadImage( const std::string &file )
{
	return loadTexture( file, renderer );
}


/*
 * Draw image on the screen.
 * @param img The image we want to display
 * @param x The x coordinate to draw too
 * @param y The y coordinate to draw too
 * \param angle		An angle in degrees that indicates the rotation that will be applied to dstrect, rotating it in a clockwise direction
 * \param center	A pointer to a point indicating the point around which dstrect will be rotated
 *					(if NULL, rotation will be done around dstrect.w/2, dstrect.h/2).
 * \param flip		An SDL_RendererFlip value stating which flipping actions should be performed on the texture
 * @param clip		The subsection of the picture to display.
 * @return NULL
 */
void drawImage(Image *img, const int &x, const int &y,
			   const double &angle = 0, const Point *center = NULL,
			   const FlipType &flip = SDL_FLIP_NONE,
			   const Rect *clip = nullptr)
{
	SDL_Renderer *ren = renderer;
	renderTexture( img, ren, x, y, angle, center, flip, clip );
}

void getImageSize( Image *img, int &width, int &height )
{
	SDL_QueryTexture( img, NULL, NULL, &width, &height );
}

void drawText( const std::string &msg, const int &x, const int &y,
			   const int32_t &size = fontSize, const Color &color = { 255, 255, 255 } )
{
	Image *image = textToImage(msg, size, color);

	//Get the texture w/h so we can center it in the screen
	int iW, iH;
	getImageSize(image, iW, iH);
	drawImage(image, x, y);
	cleanup(image);
}


}

int main(int argc, char* args[]) {

	using namespace Game;

	//Start up SDL and make sure it went ok
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		logSDLError(std::cout, "SDL_Init");
		return 1;
	}

	//Also need to init SDL_ttf
	if (TTF_Init() != 0){
		logSDLError(std::cout, "TTF_Init");
		SDL_Quit();
		return 1;
	}

	//return 0;

	//Setup our window and renderer
	window = SDL_CreateWindow("STG_Game", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);
	if (window == nullptr){
		logSDLError(std::cout, "CreateWindow");
		TTF_Quit();
		SDL_Quit();
		return 1;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr){
		logSDLError(std::cout, "CreateRenderer");
		cleanup(window);
		TTF_Quit();
		SDL_Quit();
		return 1;
	}
	//_font = TTF_OpenFont(_fontName, _fontSize);

	//Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 );


	double deltaTime, delta, duration, oneStepTime = 1./FPS_RATE;
	unsigned int t0, t1;
	SDL_Event event;
	bool quit = false;


	t0 = SDL_GetTicks();
	initialize();
	while( !quit )
	{

		while ( SDL_PollEvent( &event ) )
		{
			switch (event.type)
			{
				case SDL_QUIT :
					quit = true;
					break;
				case SDL_KEYDOWN :
					keyValue = event.key.keysym.sym;
					keyPressed = true;
					keyDown();
					break;
				case SDL_KEYUP:
					keyPressed = false;
					keyUp();
					break;
				case SDL_MOUSEBUTTONDOWN :
					mouseButton = event.button.button;
					pmouseX = mouseX;
					pmouseY = mouseY;
					mouseX = event.button.x;
					mouseY = event.button.y;
					mousePressed = true;
					mouseDragged = false;
					mousePress();
					break;
				case SDL_MOUSEMOTION :
					pmouseX = mouseX;
					pmouseY = mouseY;
					mouseX = event.motion.x;
					mouseY = event.motion.y;
					if (event.motion.state & SDL_BUTTON_LMASK || event.motion.state & SDL_BUTTON_RMASK)
					{
						mouseDragged = true;
					}
					mouseMove();
					break;
				case SDL_MOUSEBUTTONUP :
					pmouseX = mouseX;
					pmouseY = mouseY;
					mouseX = event.button.x;
					mouseY = event.button.y;
					mousePressed = false;
					mouseDragged = false;
					mouseRelease();
					break;
				default:
					break;
			}
		}


		//internalRender(duration);

		work(duration);

		t1 = SDL_GetTicks();
		delta = t1 - t0;
		t0 = t1;
		deltaTime = delta / 1000.0;

		if (delta < oneStepTime*1000) {
			SDL_Delay(oneStepTime*1000 - delta);
			deltaTime = oneStepTime;
		}
		duration = duration + deltaTime;


		//Display Fps
		nowFPS = 1.0 / deltaTime;
		if( FPS_DISPLAY )
		{
			char info[20];
			sprintf(info, "FPS: %2d", (int)(nowFPS + 0.5));
			drawText( info, 0, 0 );
		}





		//We can draw our message as we do any other texture, since it's been
		//rendered to a texture

		//SDL_Rect rect = {0,0,SCREEN_WIDTH/2,SCREEN_HEIGHT/2};
		//SDL_RenderSetViewport( renderer, &rect );
		//SDL_RenderSetScale( renderer, 1.1,0.9 );
		SDL_RenderPresent(renderer);
		SDL_RenderClear(renderer);
	}


	close();

	/*
	if (_bgColor != NULL ) SDL_DestroyTexture(_bgColor);
	if (_bgImage != NULL ) SDL_DestroyTexture(_bgImage);
	if (_cursor != NULL) SDL_FreeCursor(_cursor);
	if (_font != NULL) TTF_CloseFont( _font );
	if (_bgMusic != NULL ) {
		Mix_HaltMusic();
		Mix_FreeMusic( _bgMusic );
	}
	*/

	cleanup( window, renderer );

	IMG_Quit();
	TTF_Quit();
	SDL_Quit();

	return 0;
}


#endif

