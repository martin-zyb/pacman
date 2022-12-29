#include "game.h"
#include "setting.h"

int main()
{
	// Get Settings
	get_setting();
	
	// init
	init();

	BeginBatchDraw();

	set_game_mode(MENU);

	// The Main cycle
	while (!game_close)
	{
		if (key)
		{
			switch (game_mode)
			{
			case GAMING:	gaming_deal();		break;
			case END:		end_deal();			break;
			case MENU:		menu_deal();		break;
			}
			

			key = 0;
		}

		if (update_event)
		{
			update_event = 0;
			
			if (cnt1)
			{
				cnt2++;
				// Used for debugging, to detect whether there is frame loss
			}

			switch (game_mode)
			{
			case GAMING:	gaming_page();		break;
			case END:		end_page();			break;
			case MENU:		menu_page();		break;
			}

		}

	}
	
	EndBatchDraw();

	// Game Exit

	closegraph();

	return 0;
}
