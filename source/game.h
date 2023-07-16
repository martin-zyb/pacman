#pragma once
#include "define.h"
#include "mover.h"
#include "resource.h"


extern int cnt1;
extern int cnt2;

extern int game_mode;

extern bool game_close;					// Global variables that guide game closure
extern bool update_event;				// Global variable to guide drawing update and control frame rate
extern char key;						// Keyboard symbol temporary storage

DWORD WINAPI time_thread(PVOID param);		// Timed capture multithreading
DWORD WINAPI keyboard_thread(PVOID);			// Keyboard capture multithreading

void init();
void init_map();
void init_background();


void menu_page_init();
void gaming_page_init();
void end_page_init();

void menu_page();
void gaming_page();
void end_page();

void menu_deal();
void gaming_deal();
void end_deal();


void set_game_mode(int mode);

void st_cheat();
void Show_Console();
void Set_language(string type = "en_us");
void Show_Path();