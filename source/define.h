#pragma once

// Defines the point class of two-dimensional space and the rect class used to label regions

#include <easyx.h>
#include <conio.h>
#include <time.h>

#include <vector>
#include <iostream>
#include <iterator>
#include <string>
#include <list>
#include <random>
using namespace std;

// Direction Macro Definition
#define DIR_NONE	 0	
#define DIR_UP		-1
#define DIR_DOWN	 1
#define DIR_LEFT	-2
#define DIR_RIGHT	 2

// Size definition
#define BLOCK_SIZE 20			// Grid size
#define PERSON_SIZE 30			// Character size
#define POINT_SIZE 1				// Bean size
#define POINT_BIG_SIZE 3		// Big Power Pill Size

#define BEAN_NUM 195			// Total number of beans (including Big Power Pills)

#define FPS 60
#define PACMAN_SPEED 3
#define MONSTER_SPEED_0 1.2
#define MONSTER_SPEED_1 2
#define MONSTER_FEAR_TIME 600
#define EAT_FLASH_TIME 5

#define MOVE_TOLERENCE 5		// Turning tolerance
#define EAT_TOLERANCE 8			// Bean eating tolerance
#define FIGHT_TOLERANCE 12		// Combat tolerance

#define MAP_COLUMN 21			// Number of map grid columns
#define MAP_ROW 27					// Number of map grid lines
#define MAP_CNT (MAP_COLUMN * MAP_ROW)	// Total number of map squares

#define GAME_WIDTH (MAP_COLUMN*BLOCK_SIZE)	// Game Window Width
#define GAME_HEIGHT (MAP_ROW*BLOCK_SIZE)		// Game Window Height

#define END 1
#define MENU 2
#define GAMING 0


class CPoint
{
public:
	int x;
	int y;
	CPoint() :x(0), y(0) {};
	CPoint(int x, int y) :x(x), y(y) {}
};

class CRect
{
public:
	CPoint site;			// Top left coordinate=[row, column]
	CPoint shape;		// Area size=[height, width]
	CRect() :site(CPoint()), shape(CPoint()) {}
	CRect(int x, int y, int h, int w) :site(CPoint(x, y)), shape(CPoint(h, w)) {}
};