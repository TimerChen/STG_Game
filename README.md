# STG_Game
A simple STG Game for 2017 Programming Lesson.

## Files
src/main.cpp

include/SDL_Header.h

res/

> You *needn't* pay attention to these files:
>
> include/res_path.h
>
> include/cleanup.h

## The functions you need to complete

```c++
// Functions need program
void initialize();
void work(const double&);
void mousePress();
void mouseMove();
void mouseRelease();
void keyDown();
void keyUp();
void close();

```

## The functions you can use

```c++
// Functions you can use
void setPenColor( const uint8_t &r, const uint8_t &g, const uint8_t &b, const uint8_t &a);
void setPenColor( const Color &color );

void drawPoint( int x, int y );
void drawPoint( const Point& p );

void drawLine( int x1, int y1, int x2, int y2 );
void drawLine( const Point &p1 ,const Point &p2 );

void drawLines(const SDL_Point* points, int count);

void drawRect( const Rect& rect, const bool& fill = false );

void drawText( const std::string &msg, const int &x, const int &y,
			   const int32_t &size = fontSize, const Color &color = { 255, 255, 255 } )

void setCanvas( int x, int y, int width=SCREEN_WIDTH, int height=SCREEN_HEIGHT )

```

> P.S.: The function *drawText()* is just designed for testing, it may be removed in further versions.
