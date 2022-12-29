#pragma once
#include "define.h"

class CAStarNode
{
public:
	int father;
	int G;
	int H;
	bool close;
	bool open;

	CAStarNode() : father(0), G(0), H(0), close(0), open(0) {}
};


class CAStar
{
private:
	CAStarNode ns[MAP_CNT];	// point set

	vector<int> open_list;		// Open List
	//list<int> close_list;		// Close the list.//The two-dimensional map closes the list. The size of the list increases by n ^ 2. It takes a lot of time to traverse. Therefore, distributed storage is used to directly access the results
										// However, the open list has a small order of magnitude and can be in the form of a list, which does not take much time

	int s;			// Start Node (Monster)
	int e;			// End Node (Player)

	int w_inst;		// Distance weight, the greater this value, the more Monster pays attention to distance control
	int w_path;	// Path weight. The higher this value is, the more Monster pays attention to short paths

	COLORREF color;
	bool path_show;
	int cnt;

public:
	CAStar() :s(0), e(0), w_inst(1), w_path(1), path_show(0), color(RED), cnt(0) {}

	void SetNodeWall(int r,int c)
	{
		if (r < 0 || r >= MAP_ROW || c < 0 || c >= MAP_COLUMN)
			return;
		int k = r * MAP_COLUMN + c;
		ns[k].close = 1;
	}

	void SwitchPathShow(COLORREF c)
	{
		if (path_show)
			path_show = 0;
		else
			path_show = 1;

		color = c;
	}

	void SetStyle(int instance, int path)
	{
		w_inst = instance;
		w_path = path;
	}

	// Init
	void init(int* map, CPoint sp, CPoint ep)
	{
		open_list.clear();
		//close_list.clear();
		
		e = ep.x * MAP_COLUMN + ep.y;
		s = sp.x * MAP_COLUMN + sp.y;

		for (int i = 0; i < MAP_CNT; i++)
		{
			//if (map[i] == 3)
				//close_list.push_back(i);
			ns[i].close = (map[i] == 3) ? 1 : 0;
			ns[i].open = 0;
			ns[i].G = 0;
			
			int dk = abs(i - e);
			ns[i].H = (dk / MAP_COLUMN + dk % MAP_COLUMN) * w_inst;

			ns[i].father = 0;
		}
	}

	int FindMinF()
	{
		int i_min = 0;
		int k_min = 0;
		int F_min = 1000000;

		int n = open_list.size();
		for (int i = 0; i < n; i++)
		{
			int k = open_list[i];
			int F = ns[k].G + ns[k].H;
			if (F < F_min)
			{
				F_min = F;
				k_min = k;
				i_min = i;
			}
		}

		if (k_min != 0)
		{
			open_list[i_min] = open_list[n - 1];
			open_list.pop_back();
		}

		return k_min;
	}

	void BuildAWay()
	{
		open_list.push_back(s);		// Add the first node
		ns[s].open = 1;

		int k_current = s;		// Set Start Point

		while (1)
		{
			if (k_current == e)
				break;

			if (open_list.size() == 0)
				break;

			// Query the next node
			k_current = FindMinF();
			ns[k_current].close = 1;


			// Search surrounding nodes
			for (int i = 0; i < 4; i++)
			{
				int ni = k_current / MAP_COLUMN;
				int nj = k_current % MAP_COLUMN;

				switch (i)
				{
				case 0: ni--; break;
				case 1: ni++; break;
				case 2: nj--; break;
				case 3: nj++; break;
				}

				if (ni < 0 || ni >= MAP_ROW || nj < 0 || nj >= MAP_COLUMN)
					continue;

				int k_next = ni * MAP_COLUMN + nj;

				if (!ns[k_next].close)			// No close (wall or node that has been explored) has been added
				{
					int G_old = ns[k_next].G;
					int G_new = ns[k_current].G + w_path;

					if (ns[k_next].open)		// Has joined open
					{
						// Optimization path
						if (G_old > G_new )	
						{
							ns[k_next].G = G_new;
							ns[k_next].father = k_current;
						}
					}
					else
					{
						ns[k_next].G = G_new;
						ns[k_next].father = k_current;

						open_list.push_back(k_next);			// Add to open_list
						ns[k_next].open = 1;
					}
				}

			}
		}
	}


	// Get directions
	int GetDir()
	{
		if (path_show)
		{
			cnt++;
			if (cnt > 5)
				cnt = 0;

			if (cnt == 0)
				setlinecolor(BLACK);
			else
				setlinecolor(color);
		}
		

		int k[2] = { 0 };
		
		k[0] = e;

		while (1)
		{
			k[1] = ns[k[0]].father;

			if (k[1] == 0 || k[1] == s)
				break;


			if (path_show)
			{
				int xi = k[0] / MAP_COLUMN * BLOCK_SIZE + BLOCK_SIZE / 2;
				int xj = k[0] % MAP_COLUMN * BLOCK_SIZE + BLOCK_SIZE / 2;
				int yi = k[1] / MAP_COLUMN * BLOCK_SIZE + BLOCK_SIZE / 2;
				int yj = k[1] % MAP_COLUMN * BLOCK_SIZE + BLOCK_SIZE / 2;
				line(xj, xi, yj, yi);
			}


			k[0] = k[1];
	
		}

		if (k[0] - k[1] == MAP_COLUMN)	return DIR_DOWN;
		if (k[0] - k[1] == -MAP_COLUMN)	return DIR_UP;
		if (k[0] - k[1] == -1)			return DIR_LEFT;
		if (k[0] - k[1] == 1)			return DIR_RIGHT;

		return DIR_NONE;
	}



};