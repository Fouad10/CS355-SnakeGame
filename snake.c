// Everybody worked on this together. We had a meeting and assigned (Fouad) as team leader
#include <stdio.h>
#include <time.h>
#include <curses.h>
#include <unistd.h>
#include <stdlib.h>

#define DELAY 100000  // Speed of the snake
#define TIMEOUT 10 

    //---Fouad and Justin----------------------------------------------
    /*Here is the Global Variables */
int trophyValue;
int trophyExpiration;
void makeFood();

    // Defining directions
	typedef enum {
		LEFT,
		RIGHT,
		UP,
		DOWN
	} direction_type;

	typedef struct point {
		int x;
		int y;
	} point;
    
    int SPEED = 100000; // Speed of snake
    int height = 00, 
    width = 00; // height and width of the border
    int i, j;
	int x = 0,
		y = 0,

		nextX = 0,
		nextY = 0,
		tailLength = 3; // length of the snake
            
	bool gameOver = false;
    bool gameWon = false; 
    bool reverse = false;
    bool selfcollision = false;
    bool bordercollision = false;

	direction_type currentDir; // current direction of the snake
	point snakeParts[1000] = {}; //snake max value of 1000 even though game win is probably less
	point food;
    point drawBorder;

    //--------------Mario Lito and Saquib kkan-----------------------------------------------
	void drawPart(point drawPoint) { // draws snake and border
		mvprintw(drawPoint.y, drawPoint.x, "O");        
    }

    void drawFood(point drawPoint) { // draws food
		mvprintw(drawPoint.y, drawPoint.x, "%d",trophyValue);        
    }

    void makeFood(){ // spawns the food (making sure as to not spawn inside snake)
        //get random value from 1 to 9
        trophyValue = (rand() % 9) + 1;
        trophyExpiration = ((rand() % 9) + 1) * 1000000;

        //print the value of the trophy on a random spot of the pit

        food.x = (rand() % (COLS -3)) + 2;
        food.y = (rand() % (LINES -3)) + 2;
        
        for(int i = 0; i < tailLength; i++) {
                if(food.x == snakeParts[i].x && food.y == snakeParts[i].y)
                    makeFood();
            }
        for(int i = 0; i < tailLength; i++) {
            if(food.x != snakeParts[i].x && food.y != snakeParts[i].y)
                mvprintw(food.y, food.x, "%d", trophyValue);
        }
    }
    //----------Saquib Khan and Michael------------------------------------

    //This function is for drawing the boundary
    void drawBorderline()
    {
        point p; 
        int i;
        for( i=0; i<height; i++)
        {   
            p.y = i;
            p.x = 0;
            drawPart(p);
            p.y = i;
            p.x = width - 1;
            drawPart(p);
        }
        for( i=0; i<width -1 ; i++ )
        {
            p.y = 0;
            p.x = i;
            drawPart(p);
            p.y = height-1;
            p.x = i;
            drawPart(p);
        }
    }
    //---------Fouad and Justin-----------------------------------------

    // initializing functions for curses
	void cursesInit() {
		initscr(); //Initialize the window
		noecho(); //Don't echo keypresses
		keypad(stdscr, TRUE);
        nodelay(stdscr, TRUE);   
		timeout(TIMEOUT);
		//Global var stdscr is created by the call to initscr()
		getmaxyx(stdscr, height, width);
	}
    //--------------Mario, Justin and Saquib----------------------------

    //other initializations
	void init() {
		srand(time(NULL));
		currentDir = rand() %4;
		tailLength = 3;
		gameOver = false;
		clear(); //Clears the screen
		
		//Set the initial snake coordinates
		int j = 0;
		for(int i = tailLength; i >= 0; i--) {
			point currPoint;
			
			currPoint.x = width / 1/2;
			currPoint.y = height / 1/2; // Snake start int the top left of the border

			snakeParts[j] = currPoint;
			j++;
		}
		makeFood();
		refresh();
	}

    //moves the snake in the pit
	void moveSnake() {
		point tmp = snakeParts[tailLength - 1];

		for(int i = tailLength - 1; i > 0; i--) {
			snakeParts[i] = snakeParts[i-1];
		}
		snakeParts[0] = tmp;
	}
     //------------Fouad and Justin and Michael--------------------------------------

	void drawScreen() {
		//Clears the screen - put all draw functions after this
		clear(); 
        drawBorderline();
		//Print game over if gameOver is true
		if(gameOver){
            if(selfcollision)
                mvprintw(height /2 + 2, height / 2, "You collided with yourself. Oops!");
            else if(bordercollision)
                mvprintw(height /2 + 2, height / 2, "You collided with the wall. Darn.");
            else if(reverse)
                mvprintw(height /2 + 2, height / 2, "You attempted to reverse your direction. Wow.");
            
            mvprintw(height /2, height / 2, "Game Over!");
            reverse = false;
            selfcollision = false;
            bordercollision = false;            
        }
		
       else if (gameWon) {
       		mvprintw(width /2, width / 2, "Game Won!");
       }
		//Draw the snake to the screen
		for(int i = 0; i < tailLength; i++) {
			drawPart(snakeParts[i]);
		}
		//Draw the current food
		drawFood(food);

        //Delay between movements
		usleep(SPEED); 
        trophyExpiration-=DELAY;
        if(trophyExpiration <= 0){
            mvprintw(food.x, food.y, " ");
            makeFood();
        }
		//ncurses refresh
		refresh();	 
	}

/* Main */
        //--------------Justin, Michael, Fouad---------------------------

	int main(int argc, char *argv[]) { // we have to somehow call the draw function
		cursesInit();
		init();
        curs_set(FALSE); // no cursor, doesn't work in google cloud shell though
		int ch;
		while(1) {
			//Global var stdscr is created by the call to initscr()
			//This tells us the max size of the terminal window at any given moment
			getmaxyx(stdscr, nextX, nextY);
			
			if(gameOver) {
                SPEED = 100000;
				sleep(2);
				init();                
			}

        /* Input Handler */
            ch = getch();
            if(( ch=='l' || ch=='L' || ch == KEY_RIGHT) && (currentDir != RIGHT && currentDir != LEFT)) {
                currentDir = RIGHT;
            } 
            else if (( ch=='h' || ch=='H' || ch == KEY_LEFT) && (currentDir != RIGHT && currentDir != LEFT)) {
                currentDir = LEFT;
            } 
            else if((ch=='j' || ch=='J' || ch == KEY_DOWN) && (currentDir != UP && currentDir != DOWN)) {
                currentDir = DOWN;
            } 
            else if((ch=='k' || ch=='K' || ch == KEY_UP) && (currentDir != UP && currentDir != DOWN)) {
                currentDir = UP;
            }
            //---------Justin-------------------------------------------

            // Game over if user tries to reverse snake direction
            else if((( ch=='l' || ch=='L' || ch == KEY_RIGHT)) && currentDir == LEFT){
                reverse = true;
                gameOver = true;
            }
            else if((( ch=='h' || ch=='H' || ch == KEY_LEFT)) && currentDir == RIGHT){
                reverse = true;
                gameOver = true;
            }
            else if(((ch=='j' || ch=='J' || ch == KEY_DOWN)) && currentDir == UP){
                reverse = true;
                gameOver = true;
            }
            else if(((ch=='k' || ch=='K' || ch == KEY_UP)) && currentDir == DOWN){
                reverse = true;
                gameOver = true;
            }                
            //------------Mario and Saquib------------------------------

            /* Movement */
            nextX = snakeParts[0].x;
            nextY = snakeParts[0].y;
            if(currentDir == RIGHT) 
                nextX++;
            else if(currentDir == LEFT) 
                nextX--;
            else if(currentDir == UP) 
                nextY--;
            else if(currentDir == DOWN) 
                nextY++;

            if(nextX == food.x && nextY == food.y) {
                point tail;
                tail.x = nextX;
                tail.y = nextY;

                if(tailLength < 255)
                {
                    tailLength+=trophyValue;
                    SPEED-=(trophyValue * 500);
                }		
            makeFood();
            } 
            //-----------------Fouad--------------------------------------
            //We are going to set the tail as the new head
            snakeParts[tailLength - 1].x = nextX;
            snakeParts[tailLength - 1].y = nextY;

			//Shift all the snake parts
			moveSnake();

            //Game Over if the player hits the screen edges
            if((nextX >= width || nextX <= 0) || (nextY >= height|| nextY <= 0)) {  
                bordercollision = true;
                gameOver = true;
            }
            //Game over if the snake hits itself
            for(int i = 3; i < tailLength; i++) {
                    if(nextX == snakeParts[i].x && nextY == snakeParts[i].y) {
                        selfcollision = true;
                        gameOver = true;
                    }
                }
            if (tailLength == (height + width)/4){
                gameWon=true;
            }
        /* Draw the screen */
        drawScreen();
		}           
		endwin(); //Restore normal terminal behavior
		nocbreak();
		return 0;
	}