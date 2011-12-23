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
	glScalef(2.0f/dx, 2.0f/dy, 1.0);
	glTranslatef(0-xMin, 0-yMin, 0);
	
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

double ad16ToUv(int x, int gain){	
	int result =  (double)(x * 20e6) / (double)(gain * pow(2,16));
	return result;
}
void makeLabel(int val, int gain, bool convert, char * s){
	if (convert){
		val = ad16ToUv(val, gain);
		if (abs(val)>1e6){
			val = val/(1e6);
			sprintf(s, "%dV", (int)val);
		}
		else if(abs(val)>1e3){
			val = val/(1e3);
			sprintf(s, "%dmV", (int)val);
		}
		else
			sprintf(s, "%duV", (int)val);
	}
	else
		sprintf(s,"%d", (int)val);		
}

void n2ProjIdx(int proj, int *p1, int *p2){
    int d1, d2;
	if (proj==PROJ1x2){
		d1 = 0;
		d2 = 1;
	}
	else if(proj==PROJ1x3){
		d1 = 0;
		d2 = 2;
	}
	else if(proj==PROJ1x4){
		d1 = 0;
		d2 = 3;
	}
	else if(proj==PROJ2x3){
		d1 = 1;
		d2 = 2;
	}
	else if(proj==PROJ2x4){
		d1 = 1;
		d2 = 3;
	}
	else if (proj==PROJ3x4){
		d1 = 2;
		d2 = 3;
	}
	else{
		std::cout<<"ArteAxes::plotProjection() invalid projection specified cannot determine d1 and d2"<<std::endl;
        *p1 = -1;
        *p2 = -1;
		return;
	}
    *p1 = d1;
    *p2 = d2;
}

// std::addressof was introduced in C++11, an equivalent function is defined below
// definition from http://en.cppreference.com/w/cpp/memory/addressof

