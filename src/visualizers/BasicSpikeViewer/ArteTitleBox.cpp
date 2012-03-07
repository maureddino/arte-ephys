#include "ArteTitleBox.h"
#include "PlotUtils.h"

ArteTitleBox::ArteTitleBox():
ArteUIElement(0,0,15,100,0)
{
	setColor(.15, .15, .50);
	setSelectedColor(.15, .50, .15);
	selected = false;
	ArteUIElement::elementName = (char*) "ArteTitleBox - Un initialized";
	title = (char *) "Tetrode:00 abcdefghijkl";
}

ArteTitleBox::ArteTitleBox(int x, int y,int w,int h, char *n):
ArteUIElement(x,y,w,h,0)
{
	setColor(.15, .15, .50);
	setSelectedColor(.15, .50, .15);
	selected = false;
	ArteUIElement::elementName = (char*) "ArteTitleBox";
	title = n;//(char *) "Tetrode:00 No Port or Label";
}

void ArteTitleBox::redraw(){
	ArteUIElement::redraw();
	
	if(selected)
			glColor3fv(selectedColor);
		else
			glColor3fv(titleColor);

	// draw the colored background for the plot
	glRecti(-1,-1,1,1);

	// Reset color to white so we can draw the title text in white
	glColor3f(1.0, 1.0, 1.0);
	void * font = GLUT_BITMAP_9_BY_15;
	
	// We want the title string to be centered in the box regardless of how 
	// many chars are in the title. To do this we must compute the proper X offset
	// for the title string. Using a 9x15 font each char is 9 pixels wide. 
	// convert from chars to pixels and then normalize using the size of the window.
	// We don't have to devide by two to center the string because the dynamic
	// range of the viewport goes from -1 to 1 and the following equatition treats it as 1
	// we would then have to multiply and divide by 2 which is redundant so we dont
	float xOffset = -1*( (float) strlen(title) * 9.0) / ( ArteUIElement::width);
	if (xOffset<-.95)
		xOffset = -.95;

	drawString(xOffset, -.6, font, title);
	
	ArteUIElement::drawElementEdges();
}


void ArteTitleBox::setTitle(char *n){
	title = n;
}
void ArteTitleBox::setSelected(bool sel){
	selected = sel;
}

void ArteTitleBox::setColor(GLfloat r, GLfloat g, GLfloat b){
	titleColor[0] = r;
	titleColor[1] = g;
	titleColor[2] = b;
}
void ArteTitleBox::setSelectedColor(GLfloat r, GLfloat g, GLfloat b){
	selectedColor[0] = r;
	selectedColor[1] = g;
	selectedColor[2] = b;
}

void ArteTitleBox::setPosition(int x, int y, double w, double h){
	ArteUIElement::setPosition(x,y,w,h);
}