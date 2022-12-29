#pragma once
// This document mainly defines the flash class of animation sequences and the management class flash of multiple animation sequences_ Group class

#include "define.h"

class CFlash
{
private:
	vector<IMAGE*> m_list;
	size_t idx_now;				// Current Display Index

public:
	CFlash() :idx_now(0) {}

	void Add(IMAGE image)
	{
		IMAGE* p = new IMAGE(image);
		m_list.push_back(p);
	}

	IMAGE& GetImage()
	{
		return *(m_list[idx_now]);
	}

	// Through the continuous update of the update function, switch the pictures displayed in the animation sequence to achieve action changes
	void Update()
	{
		idx_now++;
		if (idx_now >= m_list.size())
			idx_now = 0;
	}
};

class CFlashGroup
{
private:
	vector<CFlash> m_list;
	int idx_now;	// Switch different animation sequences to achieve different action effects
	int cnt;		// Timers, controlling the speed of animation sequences
	int top;

public:
	CFlashGroup(int t) :idx_now(0), cnt(0), top(t) {}
	CFlashGroup() :CFlashGroup(5) {}

	void Add(CFlash flash)
	{
		m_list.push_back(flash);
	}

	// Set the animation sequence of the current presentation
	void SetIdx(int idx)
	{
		idx_now = idx;
	}

	IMAGE& GetImage()
	{
		return m_list[idx_now].GetImage();
	}

	// Update flash object
	void Update()
	{
		cnt++;
		if (cnt >= top)
		{
			cnt = 0;
			m_list[idx_now].Update();

			// Update All ¡ý or ¡ü Only update the currently displayed animation sequence

			//for (vector<CFlash>::iterator iter = m_list.begin(); iter != m_list.end(); iter++)
			//	(*iter).Update();
		}
	}
};