#include "menu.h"


void score()
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////// NEEDS TO BE MADE ///////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
}

void print_letter()
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////// NEEDS TO BE MADE ///////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
}

void erase_letter()
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////// NEEDS TO BE MADE ///////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
}

void menu_handler (char *option, mouse_st *m)
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////// NEEDS TO BE MADE ///////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
}

void menu_handle_h_vs_m(char *option, mouse_st *m)
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////// NEEDS TO BE MADE ///////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
}

void bitmaps_load(bitmap_struct *t)
{
	t->background = loadBitmap("home/lcom/proj/code/images/Fundo.bmp");
	t->highscores = loadBitmap("home/lcom/proj/code/images/Highscores.bmp");
	t->mouse = loadBitmap("home/lcom/proj/code/images/Mouse.bmp");
	t->frame = loadBitmap("home/lcom/proj/code/images/Frame.bmp");
	t->pre_head_to_head = loadBitmap("home/lcom/proj/code/images/Pre-Head_to_Head.bmp");
	t->lost = loadBitmap("home/lcom/proj/code/images/Lost.bmp");
	t->win = loadBitmap("home/lcom/proj/code/images/Win.bmp");
}

int exit_menu(bitmap_struct bitmaps)
{
	deleteBitmap(bitmaps.background);
	deleteBitmap(bitmaps.mouse);
	deleteBitmap(bitmaps.frame);
	vg_exit();

	//estas duas opearacoes sao feitas para assegurar o normal funcionamento do rato quando acabar a funcao
	mouse_int_handler(DISABLE_STREAM); //desativa a stream
	mouse_int_handler(SET_STREAM); //volta a ativar a stream, isto foi feito para desativar o envio dos pacotes

	if (unsubscribe_all() == -1)
	{
		printf("Failure to unsubscribe!! \n\n");
		return -1;
	}
	return 0;
}

char position_menu(bitmap_struct bitmaps, char * video_copy)
{
	if ((mouse_t.x_mouse >= 438) && (mouse_t.x_mouse <= 591))
	{
		if ((mouse_t.y_mouse >= 650) && (mouse_t.y_mouse <= 716))
		{
			if (mouse_t.LB == 1)
			{
				if (0 == exit_menu(bitmaps))
					return 0;
				else return 1;
			}
			else
			{
				change_color(438, 591, 650, 716, rgb(0,0,0), rgb(100, 200, 0), video_copy);
			}
		}
	}

	if ((mouse_t.x_mouse >= 305) && (mouse_t.x_mouse <= 718))
	{
		if ((mouse_t.y_mouse >= 555) && (mouse_t.y_mouse <= 644))
		{
			if (mouse_t.LB == 1)
			{
				exit_menu(bitmaps);
				HighScores_menu(); //not done
				//funcao HighScores
				return 0;
			}
			else
			{

			}
		}
	}

	if ((mouse_t.x_mouse >= 341) && (mouse_t.x_mouse <= 633))
	{
		if ((mouse_t.y_mouse >= 484) && (mouse_t.y_mouse <= 550))
		{
			if (mouse_t.LB == 1)
			{
				exit_menu(bitmaps);
				online_menu();
				//funcao online
				return 0;
			}
			else
			{

			}
		}
	}

	if ((mouse_t.x_mouse >= 248) && (mouse_t.x_mouse <= 787))
	{
		if ((mouse_t.y_mouse >= 412) && (mouse_t.y_mouse <= 466))
		{
			if (mouse_t.LB == 1)
			{
				exit_menu(bitmaps);
				Head_to_Head();
				//funcao Head To Head
				return 0;
			}
			else
			{

			}
		}
	}

	if ((mouse_t.x_mouse >= 149) && (mouse_t.x_mouse <= 871))
	{
		if ((mouse_t.y_mouse >= 328) && (mouse_t.y_mouse <= 396))
		{
			if (mouse_t.LB == 1)
			{
				exit_menu(bitmaps);
				HumanMachine();
				//funcao human vs machine
				return 0;
			}
			else
			{

			}
		}
	}
	return 1;
}

int change_color(unsigned xi, unsigned xf, unsigned yi, unsigned yf, int color_init, int color_final, char * video_copy)
{
	video_copy = video_copy + xi * 2 + (1024 * yi) * 2;
	unsigned int i = xi;
	unsigned int j = yi;
	char bool = 1;
	for (j; j < xf; j++)
	{
		for (i; i < yf; i++)
		{
			if (color_init == *video_copy)
			{
				bool = 0;
				*video_copy = color_final;
				printf("entrei\n");
			}
			video_copy+=2;
		}
		i = 0;
		video_copy = video_copy + 1024 - (xf - xi);
	}
	video_copy = getVideoMem();
	return bool;
}

int HighScores_menu()
{
	return 0;
}

int online_menu()
{
	return 0;
}

int Head_to_Head()
{
	return 0;
}

int HumanMachine()
{
	return 0;
}