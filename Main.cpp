// Created by Reece Kassiram-Brennan | University of Westminster | 3D graphics Programming Coursework 1 | w1728853

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <time.h>
#include "freeglut.h"	// OpenGL toolkit - in the local shared folder

using namespace std;

//Globals
const int startpos = -32; // player start position
const int squaresize = 6; //squares size
const int wallsize = 60; // width/height of the wall
const int gridsize = 3; // grid spacing
const int squarenum = 5;// number of squares

int playerx = startpos;
int playery = startpos;
int playersizex = 2;
int playersizey = 4;
vector<vector<int>> squares;
GLfloat wall[4] = {-35,-35,wallsize,wallsize }; // First [0] element is wall positionX, Second [1] is positionY, Third [2] is Width, Fourth [3] is Height.
GLfloat gameexit[4] = {wall[0]+wallsize-8,wall[1]+ wallsize-4,8,4};
GLfloat gameentrance[4] = { wall[0],wall[1],8,4 };

int score; // player score
bool showgrid = false; 


/* reshape callback function
executed when window is moved or resized */
void reshape(int w, int h)
{
	GLfloat aspect = (GLfloat)w / (GLfloat)h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h) // aspect <= 1
		glOrtho(-50.0, 50.0, -50.0 / aspect, 50.0 / aspect, -50.0, 50.0);
	else // aspect > 1
		glOrtho(-50.0*aspect, 50.0*aspect, -50.0, 50.0, -50.0, 50.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void addsquare(int number) // push back number of squares to the squares vector vector
{
	for (int i = 0; i < number; i++)
	{
		vector<int> squaretemp;
		squaretemp.push_back((rand() % (wallsize-squaresize) + wall[0]));
		squaretemp.push_back((rand() % (wallsize-squaresize-20) + wall[1]+10));
		squares.push_back(squaretemp);
	}
}

void drawsquares()
{
	if(!squares.empty())
	{
		for(int i = 0; i < squares.size(); i++)
		{
			squares[i][0];
			glPushMatrix();
			glBegin(GL_POLYGON); // Creates a polygon with the following vertices specified below.
			glColor3f(1, 0, 1);
			glVertex2f(squares[i][0], squares[i][1]);
			glVertex2f(squares[i][0]+squaresize, squares[i][1]);
			glVertex2f(squares[i][0]+ squaresize, squares[i][1]+ squaresize);
			glVertex2f(squares[i][0], squares[i][1]+ squaresize);
			glEnd();
			glFlush();
			glPopMatrix();
		}
	}
	
}

void drawoutline(GLfloat object[], GLfloat r, GLfloat g, GLfloat b) // draw outline of object array
{
	glPushMatrix();
	glBegin(GL_LINE_LOOP); // Creates a polygon with the following vertices specified below.
	glColor3f(r, g, b);
	glVertex2f(object[0], object[1]);
	glVertex2f(object[0] + object[2], object[1]);
	glVertex2f(object[0] + object[2], object[1] + object[3]);
	glVertex2f(object[0], object[1] + object[3]);
	glEnd();
	glFlush();
	glPopMatrix();
}

void playerwallcollision(int losescore) // checks if player is out of map
{
	// if player is not overlapping on at least one axis
	if (playerx < wall[0] || playerx + playersizex > wall[0] + wall[2] || playery < wall[1] || playery + playersizey > wall[1] + wall[3])
	{
		playerx = startpos;
		playery = startpos;
		score -= losescore;
		cout << "hit wall" << endl;
		cout << "score: " << score << endl;
	}
}

void playergatecollision(int winscore) // check if player is inside the gate
{
	if (playerx + playersizex > gameexit[0] && playerx < gameexit[0] + gameexit[2] && playery + playersizey > gameexit[1] && playery < gameexit[1] + gameexit[3])
	{
		// GLfloat wall[4] = { -50,-35,wallsize,wallsize };
		playerx = startpos;
		playery = startpos;
		score += winscore;
		squares.clear();
		addsquare(squarenum);
		cout << "hit gate" << endl;
		cout << "score: " << score << endl;
	}
}

void playercollision(int losescore)
{
	//if player is overlapping on x and y, collision
	for (int i = 0; i < squares.size(); i++)
	{
		vector<int> tempsquare = squares[i];
		if ((playerx + playersizex) > squares[i][0] && playerx < squares[i][0] + (squaresize) && playery + playersizey > squares[i][1] && playery < squares[i][1] + (squaresize))
		{
			playerx = startpos;
			playery = startpos;
			score -= losescore;
			cout << "hit square" << endl;
			cout <<"score: "<< score << endl;
		}
	}
}

void drawgrid()
{
	glPushMatrix();
	for (int i = wall[0]; i < wall[0]+wall[2]; i += gridsize) // creates vertices from bottom to top and from side to side.
	{
		
		glBegin(GL_LINES); // Creates a polygon with the following vertices specified below.
		glColor3f(0.2, 0.0, 0.2);
		glVertex2f(i, wall[1]); // bottom to top at x coord i
		glVertex2f(i, wall[1]+wall[3]);
		glVertex2f(wall[0], i); // side to side at y coord i
		glVertex2f(wall[0]+wall[2], i);
		glEnd();
		
	}
	glPopMatrix();
	glFlush();
}

void drawplayer() // draw player from position and size
{
	glPushMatrix();
	glBegin(GL_POLYGON); // Creates a polygon with the following vertices specified below.
	glColor3f(1, 0.8, 1);
	glVertex2f(playerx, playery);
	glVertex2f(playerx + playersizex, playery);
	glVertex2f(playerx + playersizex, playery + playersizey);
	glVertex2f(playerx, playery + playersizey);
	glEnd();
	glPopMatrix();
	glFlush();
}



void mymenu(GLint id) // right click menu entries
{
	switch(id)
	{
	case 1:
		showgrid = true;
		break;
	case 2:
		showgrid = false;
		break;
	default: 
		break;
	}
	glutPostRedisplay();
}

void mykeyboard(unsigned char key, int x, int y) // keyboard actions
{
	switch (key)
	{
	case 'w':
		playery += 1;
		break;
	case 's':
		playery -= 1;
		break;
	case 'd':
		playerx += 1;
		break;
	case 'a':
		playerx -= 1;
		break;
	case 'r': // rotate swaps size x and y
		int t = playersizey;
		playersizey = playersizex;
		playersizex = t;
		break;		
	}
	glutPostRedisplay();
}



// Graphics initialization function
void init(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);     // window will be cleared to black
	srand(time(NULL));
	addsquare(squarenum);
	glutCreateMenu(mymenu);
	glutAddMenuEntry("Grid on", 1);
	glutAddMenuEntry("Grid off", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	
}



/* display callback function
called whenever contents of window need to be re-displayed */
//this is the all important drawing method - all drawing code goes in here
void display()
{
	
	glClear(GL_COLOR_BUFFER_BIT);     /* clear window */
	glLoadIdentity();
	if(showgrid==true)
	{
		drawgrid();
	}
	playerwallcollision(10);
	playercollision(10);
	playergatecollision(50);
	drawoutline(gameexit, 1, 0, 0);
	drawoutline(gameentrance, 0, 1, 0);
	drawoutline(wall, 1,1,1);
	//border->Draw(1, 1, 1, 1);
	drawsquares();
	glPushMatrix();
	drawplayer();
	glPopMatrix();
	glutSwapBuffers();
}


//rename this to main(...) and change example 2 to run this main function
int main(int argc, char** argv)
{
	       /* window management code ... */
   
   glutInit(&argc, argv); // initialises GLUT and processes any command line arguments   
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);	// double buffer and rgb  
   glutInitWindowSize (1024, 576);	// window width = 1024 pixels, height = 576 pixels for a 16:9 ascpect ratio    
   glutInitWindowPosition (100, 100);	// window position
   glutCreateWindow ("Car CW Game");	// title
   init();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(mykeyboard);
   glutMainLoop();
   return 0;
}