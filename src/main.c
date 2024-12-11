// Snake, the game
// (C) 2001.01.15 - 2001.01.22 Jan Harries (nmioaon@get2net.dk)

// headerfiles
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <time.h>

//#define INCLUDE_INTRO_SCREEN

#define sound(x)
#define nosound()
#define _setcursortype(x) cursor(x)
#define _NOCURSOR 0
#define _NORMALCURSOR 1
#define textbackground(x) bgcolor(x)
#define putch(x) putchar(x)
#define getch() getchar()
#define randomize() srand(clock())

#define BLACK       0
#define LIGHTGRAY   12
#define RED         3
#define YELLOW      7
#define WHITE       1
#define BLUE        6

void delay(unsigned int d)
{
    d >>= 4;
    while (d--) {
        while((*(char*)0xd011 & 0x80)) ;
        while(!(*(char*)0xd011 & 0x80)) ;
    }
}

int random(int max)
{
    return rand() % max;
}

// keyboard..
#define UP          'w'
#define DOWN        's'
#define LEFT        'a'
#define RIGHT       'd'
//#define ESC         27
#define ESC         3   // runstop?
#define S_key      's'
#define EXTENDED   224

// ascii chars...
#define SPACE       32
#define UPARROW     'w'
#define DOWNARROW   's'
#define LEFTARROW   'a'
#define RIGHTARROW  'd'

#define BARCHAR '#'
#define CHARSNAKEHEAD '*'

// screen..
#if defined(COL80)

#define SCREENX     80
#define SCREENY     25

#define SCORESX     0
#define SCORESY     1

#define min_x       22
#define max_x       79
#define min_y       2
#define max_y       23
#define tmin_x      2
#define tmax_x      21

#else

#define SCREENX     40
#define SCREENY     25

#define SCORESX     0
#define SCORESY     1

#define min_x       20
#define max_x       39
#define min_y       1
#define max_y       23
#define tmin_x      0
#define tmax_x      19

#endif

// global constants....
#define TRUE  1
#define FALSE 0


// global variables....

// the hiscore file...
  FILE *fptr;

// the default hiscore, which is used if "scores.hi" doesn't exist
// or cannot be read for some reason..
  int scores[10] = { 100,90,80,70,60,50,40,30,20,10 };
  char names[][10] = { "nmioaon..","nmioaon..","nmioaon..",\
                       "nmioaon..","nmioaon..","nmioaon..",\
                       "nmioaon..","nmioaon..","nmioaon..",\
                       "nmioaon.." };

// function prototypes...

int Load_Scores (void);
void Intro_Screen (int);
void Sound_FX (int, int);
int Title_Screen (void);
void Draw_Borders (void);
void X_Bar (int, int, int, int);
void Y_Bar (int, int, int, int);
int Snake (void);
void Get_Ready (void);
void Clear_Titles (void);
void Clear_Playfield (void);
void Show_Help (int, int, int, int);
int Generate_X (void);
int Generate_Y (void);
void Set_Point (int, int, int);
void Game_Over (void);
void Sort_Scores (void);
int Save_Scores (void);
void Save_Error (void);

// the main program...

// loads the scores, runs the game, and saves the scores before
// quitting, if any hiscore was achieved..
void main (void)
{
  // load hiscores and display intro-screen...
  // the intro screen will inform the player, if the hiscore file
  // wasn't loaded. (f.ex the first time the game is run)
    if (Load_Scores() == FALSE) {
#if defined(INCLUDE_INTRO_SCREEN)
        Intro_Screen (TRUE);
    } else {
        Intro_Screen (FALSE);
#endif
    }
    // run the game, and save scores (if any is achieved)..
    if (Title_Screen() == TRUE) {
        Save_Scores();
    }

    // exit to DOS.......................
    _setcursortype (_NORMALCURSOR);
    textbackground(BLACK);
    textcolor (LIGHTGRAY);
    clrscr();
} // END main


// functions.....................................

// loads the hiscore file.. (if possible)...
int Load_Scores (void)
{
    // open file........
    if ((fptr = fopen("scores.hi", "rb")) == NULL) {
        // an error occurred..
        return (FALSE);
    }

    // load scores......
    if (fread (scores, sizeof(scores), 1, fptr) !=1) {
        // an error occurred..
        fclose (fptr);
        return (FALSE);
    }

    // load names......
    if (fread (names, sizeof(names), 1, fptr) !=1) {
    // an error occurred..
        fclose (fptr);
        return (FALSE);
    }

    // assertion: file was found & loaded!
    fclose (fptr);
    return (TRUE);
}


// saves the hiscore file.. (if possible)
int Save_Scores (void)
{
    // open file...
    if ((fptr = fopen("scores.hi", "wb")) == NULL) {
        Save_Error();
        return (FALSE);
    }

    // save scores...
    if (fwrite (scores, sizeof(scores), 1, fptr) !=1) {
        // an error occurred..
        fclose (fptr);
        Save_Error();
        return (FALSE);
    }

    // save names...
    if (fwrite (names, sizeof(names), 1, fptr) !=1) {
        // an error occurred..
        fclose (fptr);
        Save_Error();
        return (FALSE);
    }

    // assertion: file was created & saved!
    fclose (fptr);
    return (TRUE);
}

// displays the startup screen - this is just for 'fun'.. :-)
void Intro_Screen (int loaderror)
{
    int countdown=0;

    /*textbackground(RED);*/
    textbackground(BLACK);
    textcolor(YELLOW);
    clrscr();
    cputs ("\n\r\n\r    Initializing: Snake-OS Loader V1.0\n\r    ");
    while (countdown++ <72) {
        cputc ('.');
        delay (100);
    }

    if (loaderror == TRUE) {
        cputs ("\n\r\n\r    Hiscores could not be opened!");
        cputs ("\n\r    Generating defaults.......................");
        delay (4000);
    }
    cputs ("\n\r\n\r    Downloading charsets from ROM BIOS array..");
    Sound_FX (700,100);
    delay (4000);
    cputs ("\n\r    Linking keymaps to gotoxy-driver..........");
    Sound_FX (700,100);
    delay (3000);
    cputs ("\n\r    Optimizing raster-scanline sweeper........");
    Sound_FX (700,100);
    delay (4000);
    cputs ("\n\r    Installing cheatmode-handler..............");
    Sound_FX (700,1000);
    delay (100);
    cputs ("\n\r    Executing gamecore module.................");
    Sound_FX (100,100);
    delay (4000);
}

// plays a sound of any given tone, for any given amount of time..
void Sound_FX (int frequency, int pause)
{
    sound (frequency);
    delay (pause);
    nosound();
}


void Save_Error (void)
// inform the player that the scores couldn't be saved..
// ie: disk was write protected or has errors..
{
    textbackground (RED);
    textcolor (YELLOW);
    clrscr();
    cputs ("\n    Sorry, Hiscores could not be saved!");
    cputs ("\n    Please run SCANDISK, and play again..\n\n");
    textbackground (BLACK);
    textcolor (LIGHTGRAY);
    cputs ("\nPress any key..");
    cgetc();
}


void Sort_Scores (void)
// sorts the hiscore table, high-->low.. (descending)
{
  int scoreswap = 0;
  char nameswap[10] = "123456789";
  int i, j;

  // standard bubble sort..
  for (j=0; j<9; j++)
  {
    for (i=0; i<9; i++)
    {
      if (scores[i+1] > scores[i])
      {
        scoreswap = scores[i];
        strcpy (nameswap, names[i]);
        scores[i] = scores[i+1];
        strcpy (names[i], names[i+1]);
        scores[i+1] = scoreswap;
        strcpy (names[i+1], nameswap);
      }
    }
  }
}


int Title_Screen (void)
// displays the title screen, where the player can either start or quit
// the game. it also display the hiscore table..
{
    char *titles   = "SNAKE";
    char *copyright= "(c) 2001 nmioaon";
    char *startkey = "S = Start";
    char *quitkey  = "ESC = Quit";
    char *ahiscore = "You have achieved a HI-score!";
    char *entername= "Please enter your name";
    int i, finalscore=0;
    static int savestate=FALSE, mkey=0;

    // mkey is kept because we call Snake, and must preserve
    // it until the end of the main loop.
    // if we do not do this, mkey will have a random value at the
    // end of the loop, quite possibly causing the program to freeze.

    // likewise, savestate is kept, because once set to TRUE, it
    // must remain so, in order for us to remember to save the hiscore!
    // ie: it can never be set to FALSE, this is only the default
    // setting, when the game starts.

    // prepare screen..
    _setcursortype (_NOCURSOR);
    textbackground (BLACK);
    textcolor (LIGHTGRAY);
    clrscr();

    // draw the playfield..
    Draw_Borders();

    // main title-screen loop.....
    do
    {
        // display title...
        Clear_Titles();
        textbackground (BLACK);
        textcolor (LIGHTGRAY);
        gotoxy (tmin_x+9, min_y+3);
        cputs (titles);
        gotoxy (tmin_x+3, min_y+4);
        cputs (copyright);

        // display hiscore table..
        for (i=0; i<10; i++) {
            gotoxy (tmin_x+3, min_y+6+i);
            cputs (names[i]);
            gotoxy (tmin_x+15, min_y+6+i);
            cprintf ("%4d", scores[i]);
        }

        // display instructions..  s to start, q to quit...
        gotoxy (tmin_x+6, min_y+17);
        cputs  (startkey);
        gotoxy (tmin_x+6, min_y+18);
        cputs  (quitkey);

        mkey=cgetc();     // player can now either Start or Quit the game..

        if (mkey==S_key) {

            // play a game..
            finalscore = Snake();

            // game over...
            Game_Over();

            // determine if we have a hiscore..
            if (finalscore > scores[9]) {
                // assertion: hiscore is achieved, prompt for player's name..
                // the lowest ranking player's score will be replaced..
                scores[9] = finalscore;

                // info: hiscore achieved....
                Clear_Playfield();
                textcolor (WHITE);
                gotoxy (min_x+14, min_y+10);
                cputs (ahiscore);
                delay (1000);

                // request: please enter playername...
                Clear_Playfield();
                textcolor (WHITE);
                gotoxy (min_x+18, min_y+10);
                cputs (entername);
                delay (1000);

                // get playername from keyboard...
                Clear_Playfield();
                _setcursortype (_NORMALCURSOR);
                textcolor (WHITE);
                gotoxy (min_x+23, min_y+10);
                cputs ("[         ]");
                gotoxy (wherex()-10,wherey());
                scanf ("%9s[A-Za-z0-9]", names[9]);
                _setcursortype (_NOCURSOR);
                Clear_Playfield();
                Sort_Scores();
                savestate=TRUE;    // flags that the hiscore must be saved!
            }
        }
    }
    while (mkey!=ESC);

    return (savestate);
}


void Draw_Borders (void)
// draw the borders that surround the title and playfield areas..
{
  // title & hiscores..
  X_Bar (tmin_x, tmax_x, min_y-1, BLUE);
  X_Bar (tmin_x, tmax_x, min_y, BLUE);
  X_Bar (tmin_x, tmax_x, max_y, BLUE);
  X_Bar (tmin_x, tmax_x, max_y+1, BLUE);

  Y_Bar (min_y-1, max_y+1, tmin_x, BLUE);
  Y_Bar (min_y-1, max_y+1, tmin_x+1, BLUE);

  // playfield..
  X_Bar (min_x, max_x, min_y-1, BLUE);
  X_Bar (min_x, max_x, min_y, BLUE);
  X_Bar (min_x, max_x, max_y, BLUE);
  X_Bar (min_x, max_x, max_y+1, BLUE);
  Y_Bar (min_y, max_y, min_x, BLUE);
  Y_Bar (min_y, max_y, min_x+1, BLUE);
  Y_Bar (min_y, max_y, max_x-1, BLUE);
  Y_Bar (min_y, max_y, max_x, BLUE);
}

// draws a bar across screen..
void X_Bar (int xstart, int xend, int yline, int bfarve) {
    char bx;

    textbackground (bfarve);
    textcolor (bfarve);
    revers(1);
    for (bx=xstart; bx<=xend; bx++) {
        gotoxy (bx,yline);
        cputc (BARCHAR);
    }
    revers(0);
}

// draws a bar downwards screen..
void Y_Bar (int ystart, int yend, int xline, int bfarve)
{
    char by;

    textbackground (bfarve);
    textcolor (bfarve);
    revers(1);
    for (by=ystart; by<=yend; by++) {
        gotoxy (xline, by);
        cputc (BARCHAR);
    }
    revers(0);
}

// int xpos[100] = { 36,37,38,39,40,0,0,0,0,0, \
//                 0,0,0,0,0,0,0,0,0,0, \
//                 0,0,0,0,0,0,0,0,0,0, \
//                 0,0,0,0,0,0,0,0,0,0, \
//                 0,0,0,0,0,0,0,0,0,0, \
//                 0,0,0,0,0,0,0,0,0,0, \
//                 0,0,0,0,0,0,0,0,0,0, \
//                 0,0,0,0,0,0,0,0,0,0, \
//                 0,0,0,0,0,0,0,0,0,0, \
//                 0,0,0,0,0,0,0,0,0,0  };
//
// int ypos[100] = { 12,12,12,12,12,0,0,0,0,0, \
//                 0,0,0,0,0,0,0,0,0,0, \
//                 0,0,0,0,0,0,0,0,0,0, \
//                 0,0,0,0,0,0,0,0,0,0, \
//                 0,0,0,0,0,0,0,0,0,0, \
//                 0,0,0,0,0,0,0,0,0,0, \
//                 0,0,0,0,0,0,0,0,0,0, \
//                 0,0,0,0,0,0,0,0,0,0, \
//                 0,0,0,0,0,0,0,0,0,0, \
//                 0,0,0,0,0,0,0,0,0,0  };
int xpos[100];
int ypos[100];

// moves the snake around in the playfield, collecting points.
int Snake (void)
{
  int score=0, lives=3, level=1, point=1, death=FALSE;
  int i, key, head=5, tail=0, extend=1, xoff=SCREENX/2, yoff=SCREENY/2, pause=50, speed=0;
  int xpoint=4, ypoint=3;
  int yup=FALSE, ydown=FALSE, xleft=FALSE, xright=FALSE;

  randomize();

  lives=3;
  level=1;
  speed=0;
  score=0;

Restart:

  point=1;
  death=FALSE;

  // initialize snake trail pointers..
  for (i=0; i<99; i++) {
    xpos[i]=0;
    ypos[i]=0;
  }

  xpos[0]=min_x+((max_x-min_x)/2)-4;
  xpos[1]=min_x+((max_x-min_x)/2)-3;
  xpos[2]=min_x+((max_x-min_x)/2)-2;
  xpos[3]=min_x+((max_x-min_x)/2)-1;
  xpos[4]=min_x+((max_x-min_x)/2);

  ypos[0]=(SCREENY/2);
  ypos[1]=(SCREENY/2);
  ypos[2]=(SCREENY/2);
  ypos[3]=(SCREENY/2);
  ypos[4]=(SCREENY/2);

  head=5;
  tail=0;
  extend=1;
  //xoff=(SCREENX/2);
  xoff=xpos[4];
  //yoff=(SCREENY/2);
  yoff=ypos[4];
  pause=50;
  yup=FALSE;
  ydown=FALSE;
  xleft=FALSE;
  xright=FALSE;

  Show_Help (lives, score, level, speed);
  Get_Ready();
  Clear_Playfield();

  // generate x,y position within playfield boundaries,
  // and make sure it doesn't land in the snake's body...
  i=0;
  do
  {
        xpoint = Generate_X();
        ypoint = Generate_Y();
  } while ((xpoint==xpos[i]) && (ypoint==ypos[i]) && (i++<=99));

  // valid xypoint position *probably* failed..
  // so do something usefull instead:
  if (i==99) {
        xpoint=max_x-3;
        ypoint=max_y-3;
  }

  // put the point on screen....
  Set_Point (point, xpoint, ypoint);

  // main keycontroller & snakehandler...
  Enter:
  death=FALSE;

  do
  {
    // move snake and check if collision with playfield borders..
    if (yup && death==FALSE) {
        ypos[head] = yoff;
        xpos[head] = xoff;
        yoff--;
        if (yoff==min_y) {
            death=TRUE;
        }
    }

    if (ydown && death==FALSE) {
        ypos[head] = yoff;
        xpos[head] = xoff;
        yoff++;
        if (yoff==max_y) {
            death=TRUE;
        }
    }

    if (xleft && death==FALSE)
    {
      xpos[head] = xoff;
      ypos[head] = yoff;
      xoff--;
      if (xoff==min_x+1) {
        death=TRUE;
      }
    }

    if (xright && death==FALSE)
    {
      xpos[head] = xoff;
      ypos[head] = yoff;
      xoff++;
      if (xoff==max_x-1) {
        death=TRUE;
      }
    }

    // if player dies, take a life and restart level..
    if (death==TRUE)
    {
      lives--;
      if (lives>0) {
            goto Restart;
      }
      goto Endgame;
    }

    // put snake..
    textbackground (BLACK);
    textcolor (LIGHTGRAY);
    gotoxy (xoff, yoff);
    cputc (CHARSNAKEHEAD);

    // delete snake's trail...
    textbackground (BLACK);
    textcolor (BLACK);
    gotoxy (xpos[tail], ypos[tail]);
    cputc (SPACE);

    // update snake trail pointers...
    head++;
    if (head>99) {
        head=0;
    }
    extend--;
    if (extend==0)    // when collision with point, 5 turns will pass
    {                 // before tail increases again.... (head=head+5)
      extend=1;       // otherwise, this will run each frame..
      tail++;
      if (tail>99) {
        tail=0;
      }
    }

    // check if collision with point....
    if (xoff==xpoint && yoff==ypoint)
    {
      score = score+point;
      textbackground (BLUE);
      textcolor (YELLOW);
#if defined(COL80)
      gotoxy (SCORESX+11+(1*13), SCORESY-1);
#else
      gotoxy (min_x+11, min_y);
#endif
      cprintf ("%3d", score);
      point++;
      if (point>9)
      {
        // start over (next level)..
        level++;
        speed=speed+1;    // it's getting harder :-)
        if (speed>pause)  // relax!  (level 25 reached..)
          speed=0;
        goto Restart;
      }
      else
      { // set next point (1-9)...
        i=0;
        do
        {
          xpoint = Generate_X();
          ypoint = Generate_Y();
        }
        while ((xpoint==xpos[i]) && (ypoint==ypos[i]) && (i++<=99));
        if (i==99)   // valid xypoint position *probably* failed..
        {
          xpoint=max_x-3;
          ypoint=max_y-3;
        }
        Set_Point (point, xpoint, ypoint);
        extend=5;      // extend the snake!! (getting harder)
      }
    }
    delay (pause-speed);
  }
  while (!kbhit());


  // assertion: player hit a key..
  // scan arrow keys, and flag which way to go..
  // going in the opposite direction is blocked!

//  key=cgetc();
//  if (key==0 || key==EXTENDED)    // arrow key was pressed..
  {
    key=cgetc();

    if (key==UP && ydown==FALSE)
    {
      yup=TRUE;
      ydown=FALSE;
      xleft=FALSE;
      xright=FALSE;
      pause=100;
    }

    if (key==DOWN && yup==FALSE)
    {
      ydown=TRUE;
      yup=FALSE;
      xleft=FALSE;
      xright=FALSE;
      pause=100;
    }

    if (key==LEFT && xright==FALSE)
    {
      xleft=TRUE;
      xright=FALSE;
      yup=FALSE;
      ydown=FALSE;
      pause=50;
    }

    if (key==RIGHT && xleft==FALSE)
    {
      xright=TRUE;
      xleft=FALSE;
      yup=FALSE;
      ydown=FALSE;
      pause=50;
    }
    // continue game..
    goto Enter;
  }
  goto Enter;    // normal key (illegal) was pressed, so just continue..

  Endgame:
  // assertion: game is over, because player lost 3 lives!
  return (score);
}


void Set_Point (int point, int xpoint, int ypoint)
{
  // set point on screen......
  textbackground (BLACK);
  textcolor(WHITE);
  gotoxy (xpoint,ypoint);
  cputc (point+48);
  Sound_FX (1000,2);
}

int Generate_X (void)
{
  int xp=1;

  while ((xp < min_x+2) || (xp > max_x-2) && (xp > 0)) {
    xp = random(SCREENX);
  }
  return (xp);
}

int Generate_Y (void)
{
  int yp=1;

  while ((yp < min_y+2) || (yp > max_y-2) && (yp > 0)) {
    yp = random(SCREENY);
  }
  return (yp);
}

void Get_Ready (void)
{
  char *getready = "Get Ready!";

  textcolor (LIGHTGRAY);
  textbackground (BLACK);
  gotoxy (min_x+((max_x-min_x)/2)-(strlen(getready)/2), min_y+10);
  cputs (getready);
  Sound_FX (800,100);
  textcolor (WHITE);
  gotoxy (min_x+((max_x-min_x)/2)-(strlen(getready)/2), min_y+10);
  cputs (getready);
  Sound_FX (200,200);
}

void Game_Over (void)
{
  char *gameover = "Game Over";

  textcolor (LIGHTGRAY);
  textbackground (BLACK);
  gotoxy (min_x+((max_x-min_x)/2)-(strlen(gameover)/2), min_y+10);
  cputs (gameover);
  Sound_FX (400,400);
  textcolor (WHITE);
  gotoxy (min_x+((max_x-min_x)/2)-(strlen(gameover)/2), min_y+10);
  cputs (gameover);
  Sound_FX (100,200);
}

void Clear_Titles (void)
{
  unsigned char i,ii;
//  window (tmin_x+3, min_y+3, tmax_x-3, max_y-3);  // titles-field...
  textbackground (BLACK);
  textcolor (WHITE);
//  clrscr();
  for (i=min_y+2; i<max_y-1; i++) {
      gotoxy(tmin_x+3,i);
    for (ii=tmin_x+2; ii<tmax_x-1; ii++) {
      cputc(' ');
//      cputc('t');
    }
  }

//  window (1,1,80,25);  // normal 80x25 screen..
}

void Clear_Playfield (void)
{
  unsigned char i,ii;
//  window (min_x+2, min_y+1, max_x-2, max_y-1);  // playfield..
  textbackground (BLACK);
  textcolor (WHITE);
//  clrscr();
  for (i=min_y+1; i<max_y-1; i++) {
      gotoxy(min_x+2,i);
    for (ii=min_x+2; ii<max_x-2; ii++) {
      cputc(' ');
//      cputc('p');
    }
  }
//  window (1,1,80,25);  // normal 80x25 screen..
}

void Show_Help (int lives, int score, int level, int speed)
{
#if defined(COL80)
    textbackground (BLUE);
    textcolor (WHITE);
    gotoxy    (SCORESX+4+(0*13), SCORESY-1);
    cputs     ("Lives:");
    gotoxy    (SCORESX+4+(1*13), SCORESY-1);
    cputs     ("Score:");
    gotoxy    (SCORESX+4+(2*13), SCORESY-1);
    cputs     ("Speed:");
    gotoxy    (SCORESX+4+(3*13), SCORESY-1);
    cputs     ("Level:");

    textcolor (YELLOW);
    gotoxy    (SCORESX+11+(0*13), SCORESY-1);
    cprintf   ("%3d", lives);
    gotoxy    (SCORESX+11+(1*13), SCORESY-1);
    cprintf   ("%3d", score);
    gotoxy    (SCORESX+11+(2*13), SCORESY-1);
    cprintf   ("%3d", speed);
    gotoxy    (SCORESX+11+(3*13), SCORESY-1);
    cprintf   ("%3d", level);
#else
    textbackground (BLUE);
    textcolor (WHITE);
    gotoxy    (SCORESX+4, SCORESY-1);
    cputs     ("Lives:");
    gotoxy    (SCORESX+4, SCORESY);
    cputs     ("Score:");
    gotoxy    (SCORESX+19, SCORESY-1);
    cputs     ("Speed:");
    gotoxy    (SCORESX+19, SCORESY);
    cputs     ("Level:");

    textcolor (YELLOW);
    gotoxy    (SCORESX+11, SCORESY-1);
    cprintf   ("%3d", lives);
    gotoxy    (SCORESX+11, SCORESY);
    cprintf   ("%3d", score);
    gotoxy    (SCORESX+26, SCORESY-1);
    cprintf   ("%3d", speed);
    gotoxy    (SCORESX+26, SCORESY);
    cprintf   ("%3d", level);
#endif
    // status line
    textcolor (YELLOW);
    gotoxy    (SCORESX+4, SCREENY-1);
    cprintf   ("Use keys %c %c %c %c to control Snake", UPARROW, RIGHTARROW, DOWNARROW, LEFTARROW);
}

