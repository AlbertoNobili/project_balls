//-----------------------------------------------------
// BALLS:	SIMULATION OF JUMPING BALLS
//			with a single display task
//-----------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <sched.h>
#include <allegro.h>
#include <time.h>

#include "ptask.h"
#include "kbfunc.h"
#include "constant.h"


//Ball structure
struct status {
	int		c;		// color [1,15]
	float	r;		// radius (m)
	float	x;		// x coordinate (m)
	float	y;		// y coordinate (m)
	float	vx;		// horizontal velocity (m/s)
	float	vy;		// vertical velocity (m/s)
	float	v0;		// jumping velocity (m/s)
};
struct status ball[MAX_BALLS]; // ball status buffer

//Circular buffer structure
struct cbuf {
	int	top;
	int	x[TLEN];
	int	y[TLEN];
};
struct cbuf trail[MAX_BALLS]; // trail array

int		nab = 0;	// number of active balls
int		tflag = 0;	// trail flag
int		tl = 10;	// actual trail length
int		end = 0;	// end flag
float	g = G0;		// acceleration of gravity

void store_trail(int i) // Insert value of ball i
{
int k;

	if (i>= MAX_BALLS) return;
	k = trail[i].top;
	k = (k + 1)% TLEN;
	trail[i].x[k] = ball[i].x;
	trail[i].y[k] = ball[i].y;
	trail[i].top = k;
}

void draw_trail(int i, int w) // draw w past values
{
int	j, k;
int	x, y;
	
	for (j=0; j<w; j++){
		k = (trail[i].top + TLEN - j) % TLEN;
		x = XBOX + 1 + trail[i].x[k];
		y = YWIN + - FLEV - trail[i].y[k];
		putpixel(screen, x, y, TCOL);
	}
}

//---------------------------------------------
//	FRAND: returns a random float in [xmi, xma)
//---------------------------------------------
float frand (float xmi, float xma)
{
float	r;
	r = rand()/(float)RAND_MAX;	// rand in [0, 1)
	return xmi + (xma - xmi)*r;
}
//---------------------------------------------

void draw_ball(int i)
{
int	x, y;
	
	x = XBOX + 1 + ball[i].x;
	y = YWIN - FLEV - ball[i].y;
	circlefill(screen, x, y, ball[i].r, ball[i].c);
}

void handle_bounce(int i)
{
	if (ball[i].y <= ball[i].r) {
		ball[i].y = ball[i].r;
		ball[i].vy = -DUMP*ball[i].vy;
	}
	if (ball[i].y >= HBOX - ball[i].r) {
		ball[i].y = HBOX - ball[i].r;
		ball[i].vy = -DUMP*ball[i].vy;
	}
	if (ball[i].x >= LBOX - ball[i].r) {
		ball[i].x = LBOX - ball[i].r;
		ball[i].vx = -ball[i].vx;
	}
	if (ball[i].x <= ball[i].r) {
		ball[i].x = ball[i].r;
		ball[i].vx = -ball[i].vx;
	}
}

void init_ball(int i)
{
	ball[i].c = 2 + i%14;
	ball[i].r = frand(RMIN, RMAX);
	
	ball[i].x = ball[i].r + 1;
	ball[i].y = frand(HMIN, HMAX);
	
	ball[i].vx = frand(VXMIN, VXMAX);
	ball[i].vy = 0;
	ball[i].v0 = sqrt(2*g*ball[i].y); 
}

void* balltask(void *arg)
{
int		i;
float	dt;

	i = get_task_index(arg);
	set_activation(i);
	printf("pallina %d: inizio\n", i);
	
	init_ball(i);
	dt = TSCALE*(float)get_task_period(i)/1000;
	while(!end){
		ball[i].vy -= g*dt;
		ball[i].x += ball[i].vx*dt;
		ball[i].y += ball[i].vy*dt - g*dt*dt/2;
		handle_bounce(i);
		store_trail(i);
		deadline_miss(i);
		wait_for_period(i);
	}
	
	return NULL;
}

void show_dmiss(int i)
{
int		dmiss;
char	s[20];

	dmiss = get_task_dmiss(i);
	sprintf(s, "%d", dmiss);
	textout_ex(screen, font, s, RBOX + 55, 70 + i*10, NCOL, BKG);
}

void aggiorna_status()
{
char	s[20];
int		j;

	sprintf(s, "balls: %d", nab);
	textout_ex(screen, font, s, RBOX+15, FLEV+10, NCOL, BKG);
	sprintf(s, "trail: %d", tl);
	textout_ex(screen, font, s, RBOX+15, FLEV+20, NCOL, BKG);
	sprintf(s, "grav:  %3.1f", g);
	textout_ex(screen, font, s, RBOX+15, FLEV+30, NCOL, BKG);
	
	textout_ex(screen, font, "dm", RBOX+50, FLEV+50, MCOL, BKG);
	
	for (j=0; j<nab; j++){
		sprintf(s, "T%d:", i+1);
		textout_ex(screen, font, s, RBOX + 15, 70 + i*10, MCOL, BKG);
		show_dmiss(i);
	}
}

void* display(void* arg)
{
int i, j;

	i = get_task_index(arg);
	set_activation(MAX_BALLS);
	printf("display: inzio (task %d)\n", i);
	
	while (!end){
		rectfill(screen, XBOX+1, YBOX+1, RBOX-1, BBOX-1, BKG);
		for (j=0; j<nab; j++){
			draw_ball(j);
			if(tflag)
				draw_trail(j, tl);
		}
		aggiorna_status();
		deadline_miss(i);	
		wait_for_period(i);
	}
	
	return NULL;
}

void* interpr (void *arg)
{
int i;
char scan;
	
	i = get_task_index(arg);
	set_activation(i);	
	printf("interprete: inizio (task %d)\n", i);

	do{
		scan = get_scancode_nb();
		switch (scan){
			case KEY_SPACE:
				if (nab < MAX_BALLS)
					task_create(balltask, nab++, PER, DL, PRI);
				break;
			case KEY_RIGHT:
				if (tl < 30)
					tl++;
				break;
			case KEY_LEFT:
				if(tl > 0)
					tl--;
				break;
			case KEY_UP:
				g = g + 1;		// increase gravity
				break;
			case KEY_DOWN:
				if (g > 1)
					g = g - 1;
				break;
			case KEY_A:
				for (i=0; i<nab; i++)
					ball[i].vy = ball[i].v0;
				break;
			case KEY_T:
				printf("cambio trail\n");
				if (tflag ==0)	tflag = 1;
				else 			tflag = 0;
				break; 
			default: break;
		}
		deadline_miss(i);
		wait_for_period(i);
	}while (scan != KEY_ESC);
	
	end = 1;
	printf("interprete: ho finito\n");
	return NULL;
}

void disegna_menu()
{
char	s[30];

	rect(screen, XMENU, YMENU, RMENU, BMENU, MCOL);
	
	sprintf(s, "SPACE:   create ball");
	textout_ex(screen, font, s, 15, 15, MCOL, BKG);
	sprintf(s, "A:       jump");
	textout_ex(screen, font, s, 15, 25, MCOL, BKG);
	sprintf(s, "T:       show trail");
	textout_ex(screen, font, s, 15, 35, MCOL, BKG);
	sprintf(s, "ESC:     exit");
	textout_ex(screen, font, s, 15, 45, MCOL, BKG);
	
	sprintf(s, "RIGHT:   trail +");
	textout_ex(screen, font, s, 250, 15, MCOL, BKG);
	sprintf(s, "LEFT:    trail -");
	textout_ex(screen, font, s, 250, 25, MCOL, BKG);
	sprintf(s, "UP:      gravity +");
	textout_ex(screen, font, s, 250, 35, MCOL, BKG);
	sprintf(s, "DOWN:    gravity -");
	textout_ex(screen, font, s, 250, 45, MCOL, BKG);
}

void disegna_status()
{
char	s[20];

	rect(screen, XSTATUS, YSTATUS, RSTATUS, BSTATUS, MCOL);
	
	sprintf(s, "balls: 0");
	textout_ex(screen, font, s, RBOX+15, FLEV+10, NCOL, BKG);
	sprintf(s, "trail: 10");
	textout_ex(screen, font, s, RBOX+15, FLEV+20, NCOL, BKG);
	sprintf(s, "grav:  9.8");
	textout_ex(screen, font, s, RBOX+15, FLEV+30, NCOL, BKG);
	
	textout_ex(screen, font, "dm", RBOX+50, FLEV+50, MCOL, BKG);
}

void init()
{
int	ret1, ret2;

	printf("init: inizio\n");
	
	allegro_init();
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, XWIN, YWIN, 0, 0);
	clear_to_color(screen, BKG);
	install_keyboard();
	
	srand(time(NULL));
	
	rect(screen, XBOX, YBOX, RBOX, BBOX, MCOL);
	disegna_menu();
	disegna_status();
	
	ptask_init(SCHED_FIFO);
	ret1 = task_create(display, MAX_BALLS, PER, DL, PRI1);
	ret2 = task_create(interpr, MAX_BALLS+1, PER, DL, PRI1);
	
	printf("creazione task display ha ritornato %d\n", ret1);
	printf("creazione task interprete ha ritornato %d\n", ret2);
	
	return;
}

int main() 
{
int i;

	init();
	for(i=0; i<=MAX_BALLS; i++){
		wait_for_task_end(i);
		printf("fine ciclo %d\n", i);
	}
	allegro_exit();
	printf("Main: addio\n");
	return 0;
}


