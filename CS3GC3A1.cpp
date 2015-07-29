//Dragan Erak
//Student #: 1205351
//CS 3GC3
//Assignment 1
//October 8, 2014


//Just to make it easier for the marker, the things I didnt include that were asked of us was:
//Each vertex colour is (initially) determined randomly.
//The animation doesn't run properly in fullscreen mode.
//Pressing the “A” key should rAndomize the scene – i.e., add a random number (e.g., 50 – 100
	//dots or lines, several polygons) of randomly initialized primitives to the scene.
//Line and point width: Add controls to adjust the size of points and lines interactively (minimum
	//size of 1)
//****Although, I did have Extra Feature #2 working, the sticky vertex list. Look at the beginning of the 
	//function void display.




#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include <iostream> //Used for the vector class.
#include <vector>
//I used http://www.cplusplus.com/reference/cstdlib/rand/ to find out how to used the rand() function. 
//I used http://stackoverflow.com/questions/4184468/sleep-for-milliseconds to find out how to used the Sleep() function.
#include <conio.h>


using namespace std;

typedef struct point //This struct holds all of the variables that will be needed for every point of type point made.
{
  float x, y; //x and y coordinates of the point
  float dx, dy; //direction vectors of the point
  float r, g, b; //rgb color values of the point
}point;

std::vector<point> myList; //making the list that I will be using to store all of my points. 


//global variable declarations

int counter; //counter used in idle 
int clickcounter=0; //Counts the number of times the mouse button has been clicked.
int mode; //Mode determines whether Dot mode, Line mode, or Polygon mode will 
		  //be used(it is set using the keyboard function kbd later in the code.
int timervar=8; //The initialization of the first parameter(speed control variable) for the timer function called void timer.
int button; //Button variable used for void mouseclick.
int state; //State variable used for void mouseclick.



void init(void)
{
	gluOrtho2D(0, 599, 0, 599); //Setting screen size.
	glPointSize(3); //Setting point size.
}
	
	

void calc(float x1, float y1){ 	//This function calculates dx and dy for every point in myList. It's used in the function void mouseclick(). 
								//x1 and y1 are the x and y coordinates of the second click for each point that determines dx and dy. 
								//So each point has something like x0 and y0 and the direction vector click has something like x1 and y1.
								
		myList.back().dx = (x1-myList.back().x) / 100; //Divided by 100 to make it as small as possible(so an attempt at normalization).
		myList.back().dy = (-1) * (myList.back().y-y1) / 100;
}


void mouseclick(int button, int state, int x, int y){ //What happens if the mouse is clicked.
	if(state == GLUT_DOWN){
	
		if(clickcounter % 2 == 1){ //If odd numbered mouse click.
			float x1 = x;			//Set the mouse click coordinates to x1 and y1 for temporary use in the calc function.
			float y1 = y;
			calc(x1, y1);			//Calculates the direction vectors using the points x and y values and 
									//the x and y values from the odd numbered click.
		}
		
		if(clickcounter % 2 == 0){	//If even numbered mouse click.
			point myPoint;			//Make a point called myPoint.
			myPoint.x=x;			//Set it's x and y coordinates to the x and y coordinates of the click.
			myPoint.y=y;
			myList.push_back(myPoint); //Push the point into the back of myList.
		}
		
		clickcounter++; 
		
	}
	
}



void kbd(unsigned char key, int x, int y) //Keyboard function.
{
	switch(key) //Cases depending on what key is pressed.
	{
		case '1':  //Cases 1-3 set the mode variable.
			mode=1;
			break;

		case '2':
			mode=2;
			break;

		case '3':
			mode=3;
			break;
			
		case '+': //Increases the animation speed.
			timervar-=16;
			break;
			
		case '-': //Decreases the animation speed.
			timervar+=16;
			break;
			
		case 'r': //Reset the list of all shapes to a blank scene.
			myList.clear();
			break;
			
		case 'p': //Pauses the animation. It will crash though because I could figure out how to stop the pause.
			
				Sleep(1000000);
	}
}


void idle(void)
{
	if(myList.size()>0){ //As long as the size of myList is greater than zero.
		counter++;
	
		if((counter % 100)==0)
		{
			for(int i = 0; i < myList.size(); i++){
			glColor3f(myList[i].r, myList[i].g, myList[i].b);
			myList[i].x+=myList[i].dx; //Increment the x value of every point in MyList by the dx value of the respective point forever.
			if(myList[i].x>599 || myList[i].x<0) //If the point goes out of the bounds set by gluOrtho2D, make it go the opposite way 									
				myList[i].dx*=(-1);				 ////by reversing the direction vectors dx and dy.
			myList[i].y+=myList[i].dy;
			if(myList[i].y>599 || myList[i].y<0)
				myList[i].dy*=(-1);
			}
		}
		
	glutPostRedisplay();
	}
}



void timer(int value) //Timer function.	
{
	for(int i = 0; i < myList.size(); i++){
			glColor3f(myList[i].r, myList[i].g, myList[i].b); //Increment the x value of every point in MyList by the dx value 
			myList[i].x+=myList[i].dx;						  //of the respective point forever. If the point goes out of the bounds set by gluOrtho2D, 
			if(myList[i].x>599 || myList[i].x<0) 			  //make it go the opposite way 	
				myList[i].dx*=(-1);
			myList[i].y+=myList[i].dy;
			if(myList[i].y>599 || myList[i].y<0)
				myList[i].dy*=(-1);
	}

	glutTimerFunc(timervar, timer, 0); 
	glutPostRedisplay();

}



void display(void) //display function - GLUT display callback function
//The beginning of this function implements Extra Feature #2, the sticky vertex list.
{
	glClear(GL_COLOR_BUFFER_BIT);		
		if(mode==1) //Set glBegin depending on what mode is using the keyboard function kbd.
			glBegin(GL_POINTS);
		
		if(mode==2)
			glBegin(GL_LINES);
		
		if(mode==3)
			glBegin(GL_POLYGON);
				
		for(int i = 0; i < myList.size(); i++) //Go through every element(point) of myList and display it.
		glVertex2f((int) (myList[i].x), (int) (600-(myList[i].y)));
			
		//When the figures are drawn, for the points, it will be the first click will	
		//draw the point, then the second will determine the direction vector and make 
		//the point move.
		
		//For the lines, it will be the first click will make the first point,
		//the second click will determine the direction vector and determine it the 
		//first point's movement. The third click will draw the line connected by the 2 points
		//and only the 1st point will be moving. Then the 4th click will determine
		//the direction vector of the 2nd point and move it.
		
		//For the polygons, I was not able to make it go into seperate polygons.
		//So it will go the first click will make the first point.
		//The second click will determine the direction vector and determine it the 
		//first point's movement. The third click will make the second point.
		//The fourth click will determine the direction vector and determine it the 
		//third point's movement. The fifth click will draw those 3 points and
		//have the first 2 moving. The next click will determine the direction	
		//vector for the fifth click and move it. The next (odd)click and every other
		//click will draw points and extend them from the polygon, The next (even)click 
		//will determine the direction vector for the previous click's point and move it.
		
		
		
		glEnd();
		
		glFlush();
		glutSwapBuffers();
}



int main(int argc, char** argv) //main function - program entry point
{
	glutInit(&argc, argv);		//starts up GLUT
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

	glutInitWindowSize(600, 600); 
	glutInitWindowPosition(50, 50);

	glutCreateWindow("Comp Sci 3GC3 Assignment 1");	//creates the window
	glutKeyboardFunc(kbd); //keyboard function
	glutDisplayFunc(display); //display function
	glutMouseFunc(mouseclick); //mouseclick function

	init(); //init function
	glutIdleFunc(idle); //idle function
	glutTimerFunc(timervar, timer, 0); //timer function

	glutMainLoop();				//starts the event glutMainLoop
	return(0);					
}
	