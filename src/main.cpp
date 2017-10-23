#include "SDL2_header.h"

#include <cstdio>
#include <map>
#include <queue>



const int Game::SCREEN_WIDTH	= 640;
const int Game::SCREEN_HEIGHT	= 700;//480
const std::string Game::TitleName = "A Simple Game Demo";

using namespace Game;

std::map<int, bool> keyboard;
std::map<int, bool> mouse;
//----------surface.h-------------
class Surface
{
private:

public:
	Image *img;
	double size_r;
	Rect *rect;

	Surface( Image* IMG = NULL, double SIZE_R = 1 );
	~Surface();
	void draw( const PointD &pos, const double ang = 0 );
};

//----------surface.cpp-----------
Surface::Surface( Image* IMG, double SIZE_R )
	:img(IMG), size_r(SIZE_R)
{
}
void Surface::draw( const PointD &pos, const double ang )
{
	int w,h;
	getImageSize( img, w, h );
	drawImage( img, pos.x - w*size_r/2, pos.y - h*size_r/2, size_r, size_r, ang );
}

Surface::~Surface()
{
	//cleanup( pic );
}
//----------object.h--------------
class CD
{
private:
	double left, cdMax;
public:
	CD( double M=0 ){left=M;cdMax=0;}

	void setMax( double M )
	{cdMax = M;left = 0;}
	void use()
	{left=cdMax;}

	double t()
	{return left;}

	bool mv(double t=1./FPS_RATE)
	{
		if( cdMax < -1e-7 )
			return false;
		left-=t;
		if( left < 1e-7 )
			return true;
		return false;
	}
};

class Object
{
public:
	bool alive;
	PointD pos, velocity;
	double size, speed, maxSpeed, ang;
	Surface *face;

	Object();
	~Object();
	void move( double  = 1 );
	void draw();
};
bool checkCollision(Object *a, Object *b);

class Bullet : public Object
{
public:
	//destory_CD
	CD desCD;
	double damage;
	void update();
	void draw();
};

class Creature : public Object
{
protected:
public:
	CD damCD, bulCD;
	double hp;

public:
	void hit(double damage);
};

class Enemy : public Creature
{
public:
	void update(const PointD &ae);
	void draw();
};
class Player : public Creature
{
public:
	void update();
	void draw();
};

//----------object.cpp------------

double getAngle( const PointD &ve, double ang=0 )
{
	double v = ve.length();
	if( v > 1e-7 )
	{
		ang = acos( dot(ve, PointD(0,-1))/v )/M_PI*180 ;
		if( cross( ve, PointD(0,-1) ) > -1e-7 )
			ang = -ang;
	}
	return ang;
}

Object::Object()
{
	alive = true;
}
Object::~Object()
{

}

void Object::move( double duration )
{
	double vlen = velocity.length();
	if( vlen > maxSpeed )
		velocity = velocity / vlen * maxSpeed;
	pos = pos + velocity * duration;
}
void Bullet::update()
{
	if(!alive) return;

	desCD.mv();
	move();
}
void Bullet::draw()
{
	if(!alive) return;
	face->draw( pos, ang );

}

void Creature::hit( double damage )
{
	if(!alive)
		return;
	hp -= damage;
	if(hp < 1e-7)
		alive = 0;
	return;
}
void Enemy::update(const PointD &ae)
{
	velocity = velocity + ae*speed;
	move();
	//ang = getAngle(ae, ang);
}

void Enemy::draw()
{
	//ang = getAngle(velocity, ang);
	ang = -180;
	face->draw( pos, ang );
}

void Player::update()
{
	bool moved;
	if( keyboard[KEY_UP]	|| keyboard['w'] )
	{
		velocity = velocity + PointD(0,-1)*speed;
		moved = true;
	}
	if( keyboard[KEY_DOWN]	|| keyboard['s'] )
	{
		velocity = velocity + PointD(0,+1)*speed;
		moved = true;
	}
	if( keyboard[KEY_LEFT]	|| keyboard['a'] )
	{
		velocity = velocity + PointD(-1,0)*speed;
		moved = true;
	}
	if( keyboard[KEY_RIGHT] || keyboard['d'] )
	{
		velocity = velocity + PointD(+1,0)*speed;
		moved = true;
	}

	move();

	//Stop player
	if(!moved)
	{
		velocity = velocity * 0.8;
		if( velocity.length() < 0.1 )
			velocity = PointD();
	}
}

void Player::draw()
{

	//ang = getAngle(velocity, ang);
	//ang = getAngle(PointD(mouseX, mouseY)-pos, ang);
	ang = 0;
	face->draw( pos, ang );
}

bool checkCollision( Object *a, Object *b )
{
	double dis = (a->pos - b->pos).length();
	if( dis - a->size - b->size < 1e-7 )
		return 1;
	return 0;
}
//----------board.h---------------
class Board
{

};


PointD posEnemy[10];
int enemyNumber, imageNumber;

Surface surfacePlayer, surfaceBullet, surfaceEnemy;
int score;
Player player;
Bullet bulletNormal, bulletEnemy;
Enemy enemyNormal;
double bulletSize;
std::deque<Enemy*> enemies;
std::deque<Bullet*> bullets, eBullets;

Image *imagePlayer, *imageBullet, *imageEnemy, *imageEneBullet, *images[100];

void loadPictures()
{
	imagePlayer = loadImage( "player.png"	);
	imageBullet = loadImage( "bullet.png"	);
	imageEnemy	= loadImage( "player_u.png" );
	imageEneBullet = loadImage( "etama.png" );
}

void initialize()
{
	//Display FPS
	FPS_DISPLAY = true;

	//Load pictures from files
	loadPictures();

	bulletSize = 3;
	//Init surface
	surfacePlayer = Surface( imagePlayer, 0.5 );
	surfaceBullet = Surface( imageBullet, bulletSize);
	surfaceEnemy  = Surface( imageEnemy , 0.5 );
	//Initialize vairables
	player.face = &surfacePlayer;
	player.pos = PointD( SCREEN_WIDTH/2, SCREEN_HEIGHT/2 );
	player.size = 5;
	player.hp = 5;
	player.maxSpeed = player.speed = 5;

	bulletNormal.face = &surfaceBullet;
	bulletNormal.size = bulletSize;
	bulletNormal.speed = bulletNormal.maxSpeed = 5;
	bulletNormal.damage = 1;


	//posEnemy[0] = posPlayer;
	enemyNormal.face = &surfaceEnemy;
	enemyNormal.speed = 0.001;
	enemyNormal.maxSpeed = 7;
	enemyNormal.size = 10;
	enemyNormal.hp = 1;

	enemyNumber = 1;

	canvasColor = {0, 0, 0, 255};

}

void drawPlayer()
{
	player.draw();
}
void drawBackground()
{
	/*
	Rect rect = {70, 50, 80, 90};

	//	Pay attention:
	//		(Color){255,255,0} means (Color){255,255,0,0}
	//		and means you will draw nothing
	setPenColor((Color){255, 255, 0, 255});


	drawRect( rect, true );
	*/

}
void drawHint()
{
	std::string hint, sHint;
	char tmp[100];
	sprintf(tmp,"%d",score);
	sHint = "Score:" + std::string(tmp);
	if(player.hp < 1e-7)
	{
		hint = "Game Over";
	}else
	{
		hint = "HP:";
		int hp_i = (int)(player.hp+0.5);
		for( int i=0; i<hp_i; ++i )
			hint = hint + "♥";
	}

	Image *text = textToImage( hint ),
		  *text2 =textToImage( sHint );
	int w,h;
	getImageSize( text, w, h );
	drawImage( text, 30, SCREEN_HEIGHT-h );
	getImageSize( text2, w, h );
	drawImage( text2, SCREEN_WIDTH-w-30, SCREEN_HEIGHT-h );
	cleanup( text, text2 );
}
void drawBullet()
{
	//std::cout << "Size: " << bullets.size() << std::endl;
	for(auto i : bullets )
		i->draw();
}
void drawEnemy()
{
	for(auto i : enemies )
		i->draw();
}

void draw()
{
	drawBackground();
	drawPlayer();
	drawBullet();
	drawEnemy();
	drawHint();
}
uint64_t lastGenBullet = 0;
void genBullet()
{
	double t = duration_i;
	if( mouse[MOUSE_LEFT] )
	{
		if(duration_i - lastGenBullet > 30)
		{
			lastGenBullet = duration_i;
			PointD e = PointD(0,-1).rotate(player.ang/180*M_PI), te;

			Bullet *bul;

			bul = new Bullet(bulletNormal);
			bul->pos = player.pos + e*player.size;
			bul->velocity = e*bul->speed;
			bul->ang = player.ang;
			bullets.push_back( bul );



			bul = new Bullet(bulletNormal);
			te = e;
			bul->pos = player.pos + e*player.size + te.rotate(-M_PI/2)*player.size*2;
			bul->velocity = e*bul->speed;
			bul->ang = player.ang;
			bullets.push_back( bul );


			bul = new Bullet(bulletNormal);
			te = e;
			bul->pos = player.pos + e*player.size + te.rotate(+M_PI/2)*player.size*2;
			bul->velocity = e*bul->speed;
			bul->ang = player.ang;
			bullets.push_back( bul );
		}
	}
}

void updateBullet()
{
	std::deque<Bullet*>tmp;
	while( !bullets.empty() )
	{
		Bullet *bul = bullets.front();
		bullets.pop_front();

		if(!bul->alive)
		{
			delete bul;
			continue;
		}

		bul->update();

		tmp.push_back(bul);
	}
	while( !tmp.empty() )
	{
		bullets.push_back( tmp.front() );
		tmp.pop_front();
	}
}

void destoryBullet()
{
	for( auto i : bullets )
	if( i->pos.x < 0-i->size*2 ||
		i->pos.x > SCREEN_WIDTH+i->size*2 ||
		i->pos.y < 0-i->size*2 ||
		i->pos.y > SCREEN_HEIGHT+i->size*2)
	{
		i->alive = false;
	}

	for( auto i : eBullets )
	if( i->pos.x < 0-i->size*2 ||
		i->pos.x > SCREEN_WIDTH+i->size*2 ||
		i->pos.y < 0-i->size*2 ||
		i->pos.y > SCREEN_HEIGHT+i->size*2)
	{
		i->alive = false;
	}

}

void updateEnemy()
{
	std::deque<Enemy*>tmp;
	while( !enemies.empty() )
	{
		Enemy *ene = enemies.front();
		enemies.pop_front();

		if(!ene->alive)
		{
			delete ene;
			continue;
		}

		ene->update( player.pos - ene->pos );

		tmp.push_back(ene);
	}
	while( !tmp.empty() )
	{
		enemies.push_back( tmp.front() );
		tmp.pop_front();
	}
}
void genEnemy()
{
	while( enemies.size() < enemyNumber )
	{
		Enemy *ene;
		ene = new Enemy(enemyNormal);
		ene->pos = PointD(rand()%1000/1000.0*SCREEN_WIDTH,
						  rand()%1000/1000.0*(SCREEN_HEIGHT*0.3));
		//std::cout << ene->pos.x << " " <<ene->pos.y << std::endl;
		enemies.push_back(ene);
	}
}

void destoryEnemy()
{
	for(auto i : enemies)
	if( i->pos.x < 0-i->size*2 ||
		i->pos.x > SCREEN_WIDTH+i->size*2 ||
		i->pos.y < 0-i->size*2 ||
		i->pos.y > SCREEN_HEIGHT+i->size*2)
	{
		i->alive = false;
	}

	for(auto i : enemies)
	for(auto j : bullets)
	if(checkCollision( i, j ) && i->alive && j->alive)
	{
		i->hit( j->damage );
		j->alive = 0;
		score++;
	}

}
short lastHit;
void destoryPlayer()
{

	for(auto i : enemies)
	if(checkCollision(i ,&player) && i->alive)
	{
		if(duration_i - lastHit > 10)
		{
			lastHit = duration_i;
			player.hit(1);
			i->alive=false;
		}
	}
	for(auto i : eBullets)
	if(checkCollision(i ,&player) && i->alive)
	{
		if(duration_i - lastHit > 10)
		{
			lastHit = duration_i;
			player.hit(1);
			i->alive=false;
		}
	}
}
int lastScore;
void deal()
{
	if(score % 10 == 0 && score!=lastScore)
	{
		lastScore = score;
		enemyNumber++;
	}


	updateBullet();
	player.update();
	updateEnemy();

	genBullet();
	genEnemy();


	destoryEnemy();
	destoryBullet();
	destoryPlayer();
}

int work( bool &quit )
{
	//Calculate sth.
	deal();

	//Draw on the screen
	draw();

	if( keyboard[KEY_ESC] )
		quit = true;
	return 0;
}

void mousePress()
{
	mouse[mouseButton] = true;
}

void mouseMove()
{

}

void mouseRelease()
{
	mouse[mouseButton] = false;
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
	//Delete all images
	cleanup( imagePlayer, imageBullet, imageEnemy );
	for( int i = 0; i < imageNumber; ++i )
		cleanup( images[i] );
}

