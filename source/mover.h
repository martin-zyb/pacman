#pragma once
// This document mainly defines the mover class and its derived monster class

#include "define.h"
#include "flash.h"
#include "astar.h"

// This library can be referenced to use the TransparentBlt function
#pragma comment( lib, "MSIMG32.LIB")

class CMover
{
protected:
	IMAGE* p_background;		// Pointer to background picture
	IMAGE back;							// To accelerate the speed of clear, set the back member to store each background image
	CFlashGroup faces;				// Container for animation sequences in all directions

	CRect rect;							// Current location

	CPoint burn;							// Place of birth

	int* map;								// Pointer of the map

	double speed;
	double speed_d;

	int new_dir;							// New movement direction. This attribute is added to improve the game feel
	int dir;									// Direction of movement


public:
	CMover() :p_background(NULL), map(NULL), speed(0), speed_d(0), new_dir(DIR_NONE), dir(DIR_NONE) 
	{
		faces = CFlashGroup(EAT_FLASH_TIME);
	}

	// Decompose initialization into a series of functions, and execute them in order
	void init_speed(double s)
	{
		// Set Speed Line
		speed = s;
	}

	void init_map(int* m)
	{
		// Set Map Address
		map = m;
	}

	void init_rect(CRect r)
	{
		// Set current position
		rect = r;
		burn = rect.site;
	}

	void init_img(IMAGE* p_back, IMAGE* p_face, int r, int c)
	{
		// Set background image pointer
		p_background = p_back;

		// Generate animation sequence in four directions according to pictures
		int h = rect.shape.x;
		int w = rect.shape.y;

		SetWorkingImage(p_face);

		for (int i = 0; i < r; i++)
		{
			CFlash flash;
			for (int j = 0; j < c; j++)
			{
				IMAGE face;
				int x = i * h;
				int y = j * w;
				getimage(&face, y, x, w, h);
				flash.Add(face);
			}
			faces.Add(flash);
		}

		SetWorkingImage(NULL);
	}

	void Reset()
	{
		rect.site = burn;
		dir = DIR_NONE;
		new_dir = DIR_NONE;
	}

	// Set/read the upper left coordinate of the current position
	CPoint GetSite() { return rect.site; }

	// Read the center point coordinates of the current position
	CPoint GetCenter()
	{
		CPoint center;
		center.x = rect.site.x + rect.shape.x / 2;
		center.y = rect.site.y + rect.shape.y / 2;
		return center;
	};

	void SetFlash(int d)
	{
		switch (d)
		{
		case DIR_DOWN:  faces.SetIdx(3);  break;
		case DIR_UP:	faces.SetIdx(2);  break;
		case DIR_RIGHT: faces.SetIdx(0);  break;
		case DIR_LEFT:	faces.SetIdx(1);  break;
		}
	}

	void Move(double ss)
	{
		int s = (int)ss;
		speed_d += (ss - s);
		if (speed_d >= 1)
		{
			s++;
			speed_d--;
		}

		switch (dir)
		{
		case DIR_DOWN:  rect.site.x += s; break;	// down
		case DIR_RIGHT: rect.site.y += s; break;	// right
		case DIR_UP:	rect.site.x -= s; break;	// up
		case DIR_LEFT:	rect.site.y -= s; break;	// left
		}
	}

	void SetNewDir(int d)
	{
		new_dir = d;
	}

	void GetNearCross(int& dx, int& dy, int& i, int& j)
	{
		CPoint c = GetCenter();

		// Distance from the nearest grid point
		dx = (c.x + BLOCK_SIZE / 2) % BLOCK_SIZE - BLOCK_SIZE / 2;
		dy = (c.y + BLOCK_SIZE / 2) % BLOCK_SIZE - BLOCK_SIZE / 2;

		int x = c.x - dx;
		int y = c.y - dy;

		i = x / BLOCK_SIZE;
		j = y / BLOCK_SIZE;
	}

	bool Turn()
	{
		// Steering judgment
		// We require that the Pac Man, like a train, is always on a fixed track and cannot move freely to arbitrary coordinates,
		// Therefore, strict review is required when turning


		// Whether there is steering,
			// Whether it is within the turning tolerance,
				// Whether to hit the wall after turning
					// All pass, turn correctly, move forward in the new direction, and update the animation sequence
				// Hit the wall. Don't turn. Keep to the old direction
			// Not within the turning tolerance, no turning, go in the old direction
		// No turning, move in the old direction

		if (new_dir == DIR_NONE)
		{
			dir = DIR_NONE;
			return 1;
		}
		else if (abs(new_dir) != abs(dir) )	// Whether there is steering
		{
			// Turning restrictions
			const int t = MOVE_TOLERENCE;	// Tolerance threshold

			int dx, dy, i, j;
			GetNearCross(dx, dy, i, j);

			bool turn_flag = 0;

			if ((abs(new_dir) == 2 && abs(dx) < t) || (abs(new_dir) == 1 && abs(dy) < t))
				turn_flag = 1;

			if (turn_flag)		// Is it within the turning tolerance
			{
				// Next Grid Point
				switch (new_dir)
				{
				case DIR_DOWN:  i++;  break;
				case DIR_UP:	i--;  break;
				case DIR_RIGHT: j++;  break;
				case DIR_LEFT:	j--;  break;
				}

				bool edge_flag = 0;
				if (j < 0 || j >= MAP_COLUMN)
					edge_flag = 1;

				if (!edge_flag)	// Whether it is at the boundary
				{
					bool wall_flag = 0;

					if (map[i * MAP_COLUMN + j] == 3)
						wall_flag = 1;

					if (!wall_flag)	// Whether to hit the wall after turning
					{
						// Direction assignment and modification of animation

						dir = new_dir;	

						// Coordinate correction
						rect.site.x -= dx;
						rect.site.y -= dy;

						return 1;
					}
					else
					{
						return 0;
					}

				}
				else	// No turning if beyond the boundary
				{
					return 0;
				}

			}
			else	// Hit the wall after turning, no turning
			{
				return 0;
			}

		}
		else	// Could be a turn around or from DIR_ NONE start
		{
			dir = new_dir;
			return 1;
		}
	}

	bool Go()
	{
		// Calculate the node nearest to you and know your relative position with this node
		// According to the moving direction and the relative position with the node, the coordinates of the next node
		// Calculate the distance between yourself and the next node
		// Judge three states
			// Will not hit / not the wall
			// It will hit, but there is still a certain distance
			// It's already hit, there's no distance

		int dx, dy, i, j;
		GetNearCross(dx, dy, i, j);

		switch (dir)
		{
		case DIR_DOWN:  if (dx >= 0) i++; break;	// down
		case DIR_RIGHT: if (dy >= 0) j++; break;		// right
		case DIR_UP:	if (dx <= 0) i--; break;			// up
		case DIR_LEFT:	if (dy <= 0) j--; break;			// left
		}

		// If the boundary is exceeded, no wall impact inspection will be conducted
		if (j < 0 || j >= MAP_COLUMN)
		{
			// move
			Move(speed);

			if (rect.site.y < 0)
				rect.site.y += GAME_WIDTH;
			else if (rect.site.y >= GAME_WIDTH)
				rect.site.y -= GAME_WIDTH;

			return 1;
		}


		int s = (int)speed;		// Moving distance

		if (map[i * MAP_COLUMN + j] == 3)	// wall
		{
			const int t = BLOCK_SIZE - PERSON_SIZE / 2;	// Thickness of wall

			// Calculate distance from wall
			int instance = (int)speed;

			switch (dir)
			{
			case DIR_DOWN:  instance = (i * BLOCK_SIZE - t) - (rect.site.x + rect.shape.x);	break;
			case DIR_RIGHT: instance = (j * BLOCK_SIZE - t) - (rect.site.y + rect.shape.y); break;
			case DIR_UP:	instance = (rect.site.x) - (i * BLOCK_SIZE + t);				break;
			case DIR_LEFT:	instance = (rect.site.y) - (j * BLOCK_SIZE + t);				break;
			}

			
			if (instance == 0)
			{
				return 0;
			}

			s = instance;
		}

		// move
		Move(s);

		return 1;
	}

	void Update()
	{
		faces.Update();

		// to turn to
		if (Turn())
			SetFlash(dir);

		// forward
		Go();
	}

private:
	// Transparency Map Function
	// parameter：
	//		dstimg: 目标 IMAGE 对象指针。NULL 表示默认窗体
	//		x, y:	Target Map Position
	//		srcimg: Pointer to the source IMAGE object. NULL indicates the default form
	//		transparentcolor: transparent color. This color of srcimg will not be copied to dstimg to achieve transparency mapping
	void transparentimage(IMAGE* dstimg, int x, int y, IMAGE* srcimg, UINT transparentcolor)
	{
		HDC dstDC = GetImageHDC(dstimg);
		HDC srcDC = GetImageHDC(srcimg);
		int w = srcimg->getwidth();
		int h = srcimg->getheight();

		// Implementing Transparent Bitmaps Using Windows GDI Functions
		TransparentBlt(dstDC, x, y, w, h, srcDC, 0, 0, w, h, transparentcolor);
	}

public:
	void Draw()
	{
		// For the sake of beauty
		// There is a fixed deviation between the drawing coordinate and the actual coordinate site   bias = BLOCK_SIZE / 2
		// center = site + shape / 2
		// draw = center + bias - shape / 2 
		//      = site + bias
		transparentimage(NULL, rect.site.y + BLOCK_SIZE / 2, rect.site.x + BLOCK_SIZE / 2, &faces.GetImage(), BLACK);
		//putimage(rect.site.y + BLOCK_SIZE / 2, rect.site.x + BLOCK_SIZE / 2, &faces.GetImage());
	}

	void Clear()
	{
		SetWorkingImage(p_background);
		getimage(&back, rect.site.y + BLOCK_SIZE / 2, rect.site.x + BLOCK_SIZE / 2, rect.shape.y, rect.shape.x);

		SetWorkingImage(NULL);
		putimage(rect.site.y + BLOCK_SIZE / 2, rect.site.x + BLOCK_SIZE / 2, &back);
	}

	int GetDir() { return dir; }

};

class CMonster :public CMover
{
private:
	int cd;
	int status;					// 0 chase, 1 fear, 2 death
	CPoint escape_rc;		// Escape location
	CAStar brain;

public:
	CMonster() :CMover(), cd(0), status(0) {}

	void SwitchPathShow(COLORREF color = RED)
	{
		brain.SwitchPathShow(color);
	}

	void SetBrainStyle(int instance, int path)
	{
		brain.SetStyle(instance, path);
	}

	void init_chase_mode(CPoint chase_mode)
	{
		// Set Pursuit Mode
		int instance = chase_mode.x;
		int path = chase_mode.y;
		brain.SetStyle(instance, path);
	}

	void init_img(IMAGE* p_back, IMAGE* p_face, IMAGE* p_dead)
	{
		// Set background image pointer
		p_background = p_back;

		int h = rect.shape.x;
		int w = rect.shape.y;

		// Generate animation sequence in four directions according to pictures
		SetWorkingImage(p_face);
		for (int i = 0; i < 4; i++)
		{
			CFlash flash;
			for (int j = 0; j < 2; j++)
			{
				IMAGE face;
				int x = i * h;
				int y = j * w;
				getimage(&face, y, x, w, h);
				flash.Add(face);
			}
			faces.Add(flash);
		}

		// Add monster animation of fear/death
		SetWorkingImage(p_dead);
		for (int i = 0; i < 2; i++)
		{
			CFlash flash;
			for (int j = 0; j < 2; j++)
			{
				IMAGE face;
				int x = i * h;
				int y = j * w;
				getimage(&face, y, x, w, h);
				flash.Add(face);
			}
			faces.Add(flash);
		}

		for (int i = 2; i < 4; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				CFlash flash;
				IMAGE face;
				int x = i * h;
				int y = j * w;
				getimage(&face, y, x, w, h);
				flash.Add(face);
				faces.Add(flash);
			}
		}

		SetWorkingImage(NULL);
	}

	void init(IMAGE* p_back, IMAGE* p_face, IMAGE* p_dead, CPoint mn, CRect r, int* mp, double s, int c)
	{
		init_speed(s);

		init_map(mp);

		init_rect(r);

		init_img(p_back, p_face, p_dead);
	}

	void Fear(int t=0)
	{
		if (t > 0)
		{
			if (status != 2)
			{
				cd = t;
				status = 1;
				SetFlash();
			}
		}
		else
		{
			if (status == 1)
			{
				cd--;
				if (cd == 0)
				{
					status = 0;
					escape_rc.x = 0;
					escape_rc.y = 0;
				}


				if (cd >= MONSTER_FEAR_TIME * 0.1)
					faces.SetIdx(4);
				else if (cd % 10 > 5)
					faces.SetIdx(5);
				else
					faces.SetIdx(4);
			}
		}
	}

	void Die()
	{
		status = 2;
		SetFlash();
	}

	void Burn()
	{
		status = 0;
		SetFlash(dir);
	}

	void SetFlash(int d=0)
	{
		if (status == 0)
		{
			switch (d)
			{
			case DIR_DOWN:  faces.SetIdx(3);  break;
			case DIR_UP:	faces.SetIdx(2);  break;
			case DIR_RIGHT: faces.SetIdx(0);  break;
			case DIR_LEFT:	faces.SetIdx(1);  break;
			}
		}
		else if (status == 2)
		{
			switch (d)
			{
			case DIR_DOWN:  faces.SetIdx(8);  break;
			case DIR_UP:	faces.SetIdx(6);  break;
			case DIR_RIGHT: faces.SetIdx(7);  break;
			case DIR_LEFT:	faces.SetIdx(9);  break;
			}
		}

	}

	int GetStatus() { return status; }
	
	void Update(CMover* p)
	{
		faces.Update();

		Fear();

		// monster's position
		int mdx, mdy, mi, mj;
		GetNearCross(mdx, mdy, mi, mj);

		// player's position
		int pdx, pdy, pi, pj;
		p->GetNearCross(pdx, pdy, pi, pj);

		if (status == 0 )
		{
			// A * algorithm, calculating the direction of movement
			brain.init(map, CPoint(mi, mj), CPoint(pi, pj));
			brain.BuildAWay();

			int dir = brain.GetDir();
			SetNewDir(dir);
				
			// to turn to
			if (Turn())
				SetFlash(dir);

			// forward
			Go();
		}
		else if (status == 1)
		{
			const static int safe_row[7] = { 1,5,8,18,20,23,25 };
			const static int safe_column[6] = { 1,5,9,11,15,19 };

			int danger_row_idx = 0;
			for (int i = 0; i < 6; i++)
				if (safe_row[i] < pi && pi < safe_row[i + 1])
				{
					danger_row_idx = i;
					break;
				}

			int danger_column_idx = 0;
			for (int i = 0; i < 5; i++)
				if (safe_column[i] < pj && pj < safe_column[i + 1])
				{
					danger_column_idx = i;
					break;
				}

			

			if ((escape_rc.x == mi && escape_rc.y == mj ) || (escape_rc.x == 0 && escape_rc.y == 0) 
				||(abs(escape_rc.x - pi) <= 10 && abs(escape_rc.y- pj) <= 5))
			{
				
				while (1)
				{
					int r = rand() % 7;
					if (abs(r - danger_row_idx) > 1)
					{
						escape_rc.x = safe_row[r];
						break;
					}
				}

				while (1)
				{
					int c = rand() % 6;
					if (abs(c - danger_column_idx) > 1)
					{
						escape_rc.y = safe_column[c];
						break;
					}
				}
				//cout << "escape_rc = (" << escape_rc.x << "," << escape_rc.y << ")\n";
			}


			// A * algorithm, calculating the direction of movement
			brain.init(map, CPoint(mi, mj), CPoint(escape_rc.x, escape_rc.y));

			if (abs(mi - pi) <= 4 && abs(mj - pj) <= 3)
			{
				for (int i = -1; i < 2; i++)
				{
					for (int j = -1; j < 2; j++)
					{
						brain.SetNodeWall(pi + i, pj + j);
					}
				}
			}
			else
			{
				for (int i = -3; i < 4; i++)
				{
					for (int j = -2; j < 3; j++)
					{
						brain.SetNodeWall(pi + i, pj + j);
					}
				}
			}

			
			brain.BuildAWay();

			int dir = brain.GetDir();
			SetNewDir(dir);

			// to turn to
			if (Turn())
				SetFlash(dir);

			// forward
			Go();
		}
		else
		{
			// A * algorithm, calculating the direction of movement
			brain.init(map, CPoint(mi, mj), CPoint(13, 10));
			brain.BuildAWay();

			int dir = brain.GetDir();

			if (dir == DIR_NONE)
				Burn();
			else
				SetNewDir(dir);

			// to turn to
			if (Turn())
				SetFlash(dir);

			// forward
			Go();
		}

	}

	void Reset()
	{
		CMover::Reset();
		cd = 0;
		status = 0;
	}
};

class CPacman : public CMover
{
private:
	int score;

public:
	CPacman() : CMover(), score(0) {};

	int GetScore() { return score; }

	void AddScore(int ds)
	{
		score += ds;

		TCHAR str[10];
		_itot_s(score, str, 10);

		outtextxy(1 * BLOCK_SIZE + BLOCK_SIZE / 2, 11 * BLOCK_SIZE, _T("     "));
		outtextxy(1 * BLOCK_SIZE + BLOCK_SIZE / 2, 11 * BLOCK_SIZE, str);
	}

	int Eat()		// Eat beans
	{
		const int t = EAT_TOLERANCE;

		// Node position
		int dx, dy, i, j;
		GetNearCross(dx, dy, i, j);

		if (abs(dx) + abs(dy) < t)
		{
			int m = map[i * MAP_COLUMN + j];
			int r = 0;
			if (m == 1)
			{
				r = POINT_SIZE;
			}
			else if (m == 2)
			{
				r = POINT_BIG_SIZE;
			}

			if (r != 0)
			{
				map[i * MAP_COLUMN + j] = 0;

				AddScore(m);

				SetWorkingImage(p_background);
				setfillcolor(BLACK);
				solidcircle(j * BLOCK_SIZE + BLOCK_SIZE / 2, i * BLOCK_SIZE + BLOCK_SIZE / 2, r);
				SetWorkingImage(NULL);

				return m;
			}
		}

		return 0;
	}

	bool Fight(CMonster** mons_list, int num)		// Eat Monsters
	{
		const int t = 49;

		int pdx, pdy, pi, pj;
		GetNearCross(pdx, pdy, pi, pj);

		for (int i = 0; i < num; i++)
		{
			CMonster* mons = mons_list[i];
			CPoint m_s = mons->GetSite();
			int instance = abs(m_s.x - rect.site.x) + abs(m_s.y - rect.site.y);
			if (instance <= FIGHT_TOLERANCE)
			{
				int st = mons->GetStatus();
				if (st == 0)
				{
					return 1;
				}
				else if (st == 1)
				{
					mons->Die();
					AddScore(10);

				}
			}
		}
		return 0;
	}

	void Reset()
	{
		score = 0;
		CMover::Reset();
	}

	void AddBig()
	{
		int dx, dy, i, j;
		GetNearCross(dx, dy, i, j);
		if (abs(dx) + abs(dy) < EAT_TOLERANCE)
		{
			map[i * MAP_COLUMN + j] = 2;
			score -= 2;
		}
	}

};