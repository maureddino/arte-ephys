#include "PlotUtils.h"


void checkGlError(){
	GLenum errCode;
	const GLubyte *errString;
	if ((errCode = glGetError()) != GL_NO_ERROR) {
	    errString = gluErrorString(errCode);
	   	fprintf (stderr, "OpenGL Error: %s\n", errString);
		exit(1);
	}
	else
		std::cout<<"OpenGL Okay!"<<std::endl;
}
void drawString(float x, float y, void *f, char *string){
	glRasterPos2f(x, y);
	int len = strlen(string);
	// glColor3f(1.0, 1.0, 1.0);
	for (int i = 0; i < len; i++) {
 		glutBitmapCharacter(f, string[i]);
	}
}

void strokeString(void * font, char *message){
	// glPushMatrix();
	 glLoadIdentity();
   	glTranslatef(-700, 0, 0);
	//glViewport(0,0,500,500);
	int len = (int) strlen(message);
	glColor3f(1.0, 1.0, 1.0);
	for (int i = 0; i < len; i++) {
		glutStrokeCharacter(font, message[i]);
	}

	// glPopMatrix();
}
void drawViewportEdge(){
	glPushMatrix();
	glLoadIdentity();
	
	glBegin(GL_LINE_LOOP);
		glVertex2f(-.995, -.995);
		glVertex2f( .995, -.995);
		glVertex2f( .995, .995);
		glVertex2f(-.995, .995);
	glEnd();
	
	glPopMatrix();
}

void drawViewportCross(){
	glColor3f(0.0,1.0,1.0);
	
	glPushMatrix();
	glLoadIdentity();
	
	glBegin(GL_LINE_LOOP);
		glVertex2f(-.995, -.995);
		glVertex2f( .995, .995);
		glVertex2f( .995, -.995);
		glVertex2f(-.995, .995);
	glEnd();
	
	glPopMatrix();
}

void setViewportRange(int xMin,int yMin,int xMax,int yMax){
	
	float dx = xMax-xMin;
	float dy = yMax-yMin;
	
//	printf("Setting viewport to:%d,%d %d,%d with dims%d,%d %d,%d\n", x,y,w,h, xMin, xMin, xMax,yMax);
//	printf("Dx:%f Dy:%f, Scaling viewport by:%f,%f \n", dx,dy,2.0/dx, 2.0/dy);
	glLoadIdentity();
	glTranslatef(-1.0,-1.0,0.0); 
	glScalef(2.0/dx, 2.0/dy, 1.0);
	glTranslatef(0-xMin, 0-yMin, 0);
	
//	glScalef(0.5,0.5,1); 


//	glScalef(1.0f/dx, 1.0/dy, 1.0);
	
//	glTranslatef(0-xMin, 0-yMin, 0);
}
int roundUp(int numToRound, int multiple) 
{ 
	if(multiple == 0) 
 	{ 
  		return numToRound; 
 	} 

 	int remainder = numToRound % multiple;
 	if (remainder == 0)
  		return numToRound;
 	return numToRound + multiple - remainder;
}