//-----------------------------------------------------
// GRAPHICAL CONSTANTS
//-----------------------------------------------------
#define XWIN	640		// windows x resolution
#define YWIN	480		// window y resolution
#define BKG		0		// background color
#define MCOL	14		// menu color
#define NCOL	7		// numbers color
#define TCOL	6		// trail color
//-----------------------------------------------------
#define LBOX	489 	// X length of the ball box
#define HBOX	399		// Y heightof the ball box
#define XBOX	5		// left box X coordinate
#define YBOX	75		// upper box Y coordinate
#define RBOX	495		// right box X coordinate
#define BBOX	475 	// bottom box Y coordinate
#define FLEV	5		// floor Y level (in world)
//-----------------------------------------------------
#define LMENU	489 	// X length of the planet box
#define HMENU	69		// Y heightof the planet box
#define XMENU	5		// left box X coordinate
#define YMENU	5		// upper box Y coordinate
#define RMENU	495		// right box X coordinate
#define BMENU	70 		// bottom box Y coordinate
//-----------------------------------------------------
#define LSTATUS		134 	// X length of the planet box
#define HSTATUS		469		// Y heightof the planet box
#define XSTATUS		500		// left box X coordinate
#define YSTATUS		5		// upper box Y coordinate
#define RSTATUS		635		// right box X coordinate
#define BSTATUS		475		// bottom box Y coordinate
//-----------------------------------------------------
// TASK CONSTANTS
//----------------------------------------------------
#define PER		20		// task period in ms 
#define DL		20		// relative deadline in ms
#define PRI		50		// task priority
#define PRI1	80		// display and interp priority
//---------------------------------------------------
// BALL CONSTANTS
//----------------------------------------------------
#define MAX_BALLS	40	// max number of balls
#define	G0			9.8	// acceleration of gravity
#define	TLEN		30	// trail length
#define HMIN		200 // min initial height
#define HMAX		390 // max initial height
#define VXMIN		20	// min initial hor. speed
#define VXMAX		10	// max initial hor. speed
#define DUMP		0.9	// dumping coefficient
#define TSCALE		10	// time scale factor
#define RMAX		20	// max ball radius
#define RMIN		10	// min ball radius
