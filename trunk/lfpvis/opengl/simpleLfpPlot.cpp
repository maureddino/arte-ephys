#if defined(__linux__)
	#include <GL/glut.h>
#else // assume OS X
	#include <GLUT/glut.h>
#endif

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include "netcom.h"
#include "datapacket.h"
#include <math.h>
#include <vector>
#include <pthread.h>

#define SCALE_VOLTAGE(v,chan) (v * userScale / nChans + MAX_INT - MAX_INT/2/nChans - chan*MAX_INT/nChans  + userShift)
#define X_TO_PIX(x) ((x*winWidth)/MAX_INT)
#define PIX_TO_X(p) ((p*MAX_INT)/winWidth)

// these offsets create padding between the different plots
// making a more pleasing display

const double MAX_VOLT = pow(2,15);
const int MAX_INT = pow(2,16);
// ===================================
// 		GUI Specific Variables
// ===================================

static int winWidth = 1560, winHeight = 540;
static double commandWinHeight = 20; // Shift the entire UI up by commandWinHeight and reserve this area for text and buttons

static double xBox = 60;
static double yBox = winHeight/8;

static double xPadding = 2;
static double yPadding = 2;

static double const waveformLineWidth = 1;
static bool disableWaveOverlay = true;
static char txtDispBuff[40];

void *font = GLUT_BITMAP_8_BY_13;
static int IDLE_SLEEP_USEC = (1e6)/100;
static int NET_SLEEP_USEC = (1e6)/2000;


// ===================================
// 		Scaling Variables
// ===================================
static int xRange = pow(2,16);
static int yRange = pow(2,16);
static double xScale = 2.0 / xRange;
static double yScale = 2.0 / (double)(yRange);

// Defines how much to shift the waveform within the viewport
//static float dV = 1.0/((float)MAX_VOLT*2);

static float dV = yRange/8;
static float userScale = 1;
static float dUserScale = .1;

static float voltShift = 0;
static float userShift = 0;
static float dUserShift = .01 * pow(2,16);

static int colWave[64];
static float const colSelected[3] = {0.4, 0.4, 0.4};
static float const colFont[3] = {1.0, 1.0, 1.0};

// ===================================
// 		Network Variables
// ===================================

static char host[] = "127.0.0.1";
static char * port;
static NetComDat net; // = NetCom::initUdpRx(host,port);


// ===================================
// 		Data Variables
// ===================================
static int nChans=8;

static int const lfpBuffSize = 500;
static lfp_bank_net_t lfpBuff[lfpBuffSize];
static lfp_bank_net_t lfp;
static int nBuff = 0;
static uint64_t readInd = 0;
static uint64_t writeInd = 0;

int inline IND(uint64_t i)	{	return i%lfpBuffSize;	}



// ===================================
// 		Plotting Variables
// ===================================
static uint32_t curSeqNum = 0;
static uint32_t prevSeqNum = -1;
static int nSampsPerChan = 2;
static double sampRate = 2000;
static double winDt = 2;
static float plotRange = 2.0/nChans;
static int maxIdx = winDt * sampRate;

static uint64_t dIdx = 0;
static uint64_t pIdx = 0;

int inline IDX(uint64_t i)	{	return i%maxIdx;	}

static int xPos = 0;
static int dXPos = xRange/(maxIdx);

static const int MAX_N_CHAN = 32;
static const int MAX_N_SAMP = 4000;
static int waves[MAX_N_CHAN][MAX_N_SAMP];


// ===================================
// 		Misc Variables
// ===================================
static int totalBuffsRead =0;
static timeval startTime, now;

static int const cmdStrLen = 50;
static unsigned char cmd[cmdStrLen];
static int cmdStrIdx = 0;

// ===================================
// 		Command Variables
// ===================================
static int const CMD_CLEAR_WIN = 'c';
static int const CMD_TOGGLE_OVERLAY = 'o';
static int const CMD_SCALE_UP = '=';
static int const CMD_SCALE_DOWN = '-';
static int const CMD_SHIFT_UP = '+';
static int const CMD_SHIFT_DOWN = '_';
static int const CMD_PREV_COL = '[';
static int const CMD_NEXT_COL = ']';

static int const CMD_QUIT = 17;
static int const CMD_RESET_SEQ = 18;

static int const CMD_STR_MAX_LEN = 16;
static int const CMD_GAIN_ALL = 'g';
static int const CMD_GAIN_SINGLE = 'G';
static int const CMD_NULL = 0;

static int const KEY_DEL = 127;
static int const KEY_BKSP = 8;
static int const KEY_ENTER = 13;

static int currentCommand = 0;
static bool enteringCommand = false;
static int selectedWaveform = 0;

// ===================================
// 		General Functions
// ===================================
int incrementIdx(int i);
bool tryToGetLfp(lfp_bank_net_t *s);
void updateNChans(int n);
void updateNSamps(int n);
void updateWaveArray();

void idleFn();
void refreshDrawing();
void drawInfoBox();
void eraseOldWaveform();
void clearWaveforms();
void eraseCommandString();

void setViewportforWaveInfoN(int n);
void setViewportForWaves();
void setViewportForCommandString();
void setWaveformColor(int col);

void drawViewportEdge();

void drawWaveforms();
void drawWaveformN(int n, int *x, uint64_t *yIdx );

void setWaveformColor(int c);
void highlightSelectedWaveform();

void resizeWindow(int w, int h);

int scaleVoltage(int v, int chan);
// ===================================
// 		Keyboard & Command Function Headers
// ===================================

void toggleOverlay();
void clearWindow();

void keyPressedFn(unsigned char key, int x, int y);
void specialKeyFn(int key, int x, int y);

void enterCommandStr(char key);
void dispCommandString();

bool executeCommand(unsigned char * cmd);

void drawString(float x, float y, char *string);
void *readNetworkLfpData(void *ptr);


int main( int argc, char** argv )
{
	if (argc>1)
		port = argv[1];
	else
	{
		std::cout<<"Usage: artelfpViewer port"<<std::endl;
		return 0;
	}

	std::cout<<"================================================"<<std::endl;
	std::cout<<" Starting up Arte Lfp Viewer"<<std::endl;
	std::cout<<"================================================"<<std::endl;

	bzero(colWave, 32);
	bzero(cmd, cmdStrLen);
	bzero(waves,  sizeof(waves[0][0]) * MAX_N_CHAN * MAX_N_SAMP);

	pthread_t netThread;
	net = NetCom::initUdpRx(host,port);
	pthread_create(&netThread, NULL, readNetworkLfpData, (void *)NULL);
	
	srand(time(NULL));
	gettimeofday(&startTime,NULL);


	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB );
//	glDisable(GL_DEPTH_TEST);
	glutInitWindowPosition( 20, 60 );
	glutInitWindowSize( winWidth, winHeight);

	char windowTitle[100];

	sprintf(windowTitle, "Arte lfp Viewer: (port)");

	glutCreateWindow(windowTitle);
	glutReshapeFunc( resizeWindow );
	glutIdleFunc( idleFn );
	glutDisplayFunc( refreshDrawing );
	glutKeyboardFunc(keyPressedFn);
	glutSpecialFunc(specialKeyFn);

	std::cout<<"Starting the GLUT main Loop"<<std::endl;
	glutMainLoop(  );

	return(0);
}
void *readNetworkLfpData(void *ptr){
	while(true)
	{
		lfp_bank_net_t l;
		NetCom::rxWave(net, &l);
		lfpBuff[IND(writeInd)] = l;
		writeInd++;
		nBuff+=1;
		totalBuffsRead++;
		usleep(NET_SLEEP_USEC);
	}
}

bool tryToGetLfp(lfp_bank_net_t *l){

//	std::cout<<"Read Ind:"<<readInd<<" Write Ind:"<<writeInd<<std::endl;
	if  ( readInd >= writeInd )
		return false;

	// Copy the lfp data
	l->name 	= lfpBuff[IND(readInd)].name;
	l->n_chans 	= lfpBuff[IND(readInd)].n_chans;
	l->n_samps_per_chan = lfpBuff[IND(readInd)].n_samps_per_chan;
	l->samp_n_bytes 	= lfpBuff[IND(readInd)].samp_n_bytes;
	for (int i=0; i < lfpBuff[IND(readInd)].n_chans *  lfpBuff[IND(readInd)].n_samps_per_chan; i++)
		l->data[i] = lfpBuff[IND(readInd)].data[i];

	for (int i=0; i < lfpBuff[IND(readInd)].n_chans; i++){
		l->gains[i] = lfpBuff[IND(readInd)].gains[i];
	}

	l->seq_num = lfpBuff[IND(readInd)].seq_num;

	readInd++;
	nBuff--;

	if (l->n_chans != nChans){
		std::cout<<"nChans != number of channels in latest buffer, updating to reflect this change"<<std::endl;
		updateNChans(l->n_chans);
	}
	if (l->n_samps_per_chan != nSampsPerChan){
		std::cout<<"nSampsPerChan != number of samps in latest buffer, updating to reflect this change"<<std::endl;
		updateNSamps(l->n_samps_per_chan);
	}

	return true;
}

void updateNChans(int n){
	nChans = n;
	yBox = winHeight/nChans;
	if (nChans==15) // used for profiling, gprof requires a call to exit() 
		exit(0);

}
void updateNSamps(int n){
	nSampsPerChan = n;
	sampRate = 1000 * nSampsPerChan;
	maxIdx = winDt * sampRate;
	dXPos = xRange/(maxIdx);
}

void updateWaveArray(){
	prevSeqNum = curSeqNum;
	int idx = 0;
	int i = 0;
	int j = 0;

	// if we receive an old packet ignore it
	if (lfp.seq_num < curSeqNum) 
	{
		std::cout<<"Old packet received, ignoring it! ";//<<lfp.seq_num<<" curSeqNum"<<curSeqNum<<std::endl;
		std::cout<<"Press CTRL+R to reset sequence number:"<<std::endl;
		return;
	}

	// if we receive a packet from the future, set the future as now and set everything
	// that was skipped to zero?
	/*
	else if(lfp.seq_num > curSeqNum+1)
	{
		while (lfp.seq_num-1 > curSeqNum)
		{
		    for (i=0; i<lfp.n_samps_per_chan; i++)
		    {
	    	    for (j=0; j<lfp.n_chans; j++)
	        	    waves[j][IDX(dIdx)] = 0;
				dIdx++;
			}
			curSeqNum++;
		}
	}
	*/

	else if(lfp.seq_num == curSeqNum) // if we have the same packet as last time
		return;
	
	int k = 0;
	for (i=0; i<lfp.n_samps_per_chan; i++)
	{
		for (j=0; j<lfp.n_chans; j++)
			waves[j][IDX(dIdx)] = lfp.data[k++];
		dIdx++;
	}

	curSeqNum = lfp.seq_num;
}


void idleFn(void){
	do{
		
		updateWaveArray();
		refreshDrawing();

	}while(tryToGetLfp(&lfp));

    usleep(IDLE_SLEEP_USEC);
}

void refreshDrawing(void)
{

	glLoadIdentity ();
	
	drawWaveforms();
	drawInfoBox();

	dispCommandString();
	glutSwapBuffers();
	glFlush();
}

void eraseOldWaveform(){

	int pixWidth = MAX_INT/winWidth;

	int x = xPos;
	int erasePixWidth = 75;
	int dx = PIX_TO_X(erasePixWidth);

	glColor3f(0.0, 0.0, 0.0);

	glBegin(GL_POLYGON);
		glVertex2i(x, 0);
		glVertex2i(x, MAX_INT);
		glVertex2i(x+dx, MAX_INT);
		glVertex2i(x+dx, 0);
	glEnd();

}
void clearWaveforms(){

	std::cout<<"Clearing all waveforms"<<std::endl;
	glViewport( xPadding, 0, winWidth-xPadding, winHeight );	// View port uses whole window

	glColor3f(0,0,0);
	glRectf(-1, -1, 2, 2);
	
	glLineWidth(1);
	glColor3f(1.0, 1.0, 1.0);

	drawViewportEdge();

}

void eraseCommandString(){
	setViewportForCommandString();
	glColor3f(0,0,0);
	glRectf(-1, -1, 2, 2);
}


void drawWaveforms(void){

	uint64_t tempIdx;
	int tempX;

	if(dIdx<=pIdx)
	{
		return;	
	}

	// Draw the line cursor

	setViewportForWaves();

	if (disableWaveOverlay)
		eraseOldWaveform();	

	glLineWidth(waveformLineWidth);
	for (int i=0; i<lfp.n_chans; i++) 
	{
			tempIdx = pIdx;
			tempX = xPos;
			drawWaveformN(i, &tempX, &tempIdx);
	}

	pIdx = tempIdx;
	xPos = tempX;

	glLoadIdentity();

}


void drawWaveformN(int n, int *x, uint64_t *idx ){

	setWaveformColor(colWave[n]);
	
	glBegin( GL_LINE_STRIP );
		for (int i=0; i<lfp.n_samps_per_chan; i++)
		{
			if(IDX(*idx)!=0)
				glVertex2i((*x)-dXPos, SCALE_VOLTAGE(waves[n][(IDX(*idx))-1],n));
			glVertex2i(*x, SCALE_VOLTAGE(waves[n][IDX(*idx)], n));
			
			(*x) = ((*x) + dXPos)%xRange;
			(*idx)++;
		}

	glEnd();
}

void setViewportForWaveInfoN(int n){
  float viewDx = xBox;
  float viewDy = yBox+.5;
  float viewX = xPadding;
  float viewY = (nChans - (n+1)) * yBox;

  glViewport(viewX, viewY, viewDx, viewDy);
}
void setViewportForWaves(){


	glViewport( xBox+xPadding, 0, winWidth-xBox-xPadding, winHeight-yPadding );	// View port uses whole window
	glLoadIdentity ();
	glTranslatef(-1.0, -1.0, 0.0);
	glScalef(xScale, yScale, 0);
//	glFrustum(0.0, 2^16, 0.0, 2^16, 0, 2^16);

	/*
	float viewDX = winWidth - xBox - 3*xPadding;
	float viewDY = yBox - 3*yPadding;
	float viewX = xBox + xPadding;
	float viewY = (nChans - (n+1)) * yBox  + yPadding;

	glViewport(viewX,viewY,viewDX,viewDY);*/
}


void setViewportForCommandString(){

    float viewX = 0 + xPadding;
    float viewY = 0 + yPadding;
    float viewDX = xBox*3;
    float viewDY =  commandWinHeight;

	glViewport(viewX, viewY, viewDX, viewDY);
}
void setWaveformColor(int c){

	int nColor = 8;
	switch(c%nColor){
		case 0: // red
			glColor3f(1.0, 0.0, 0.0);
			break;
		case 1: // white
		glColor3f(1.0, 1.0, 0.0);
			break;
		case 2: // green
			glColor3f(0.0, 1.0, 0.0);
			break;
		case 3: //
			glColor3f(0.0, 1.0, 1.0);
			break;
		case 4:
			glColor3f(0.0, 0.0, 1.0);
			break;
		case 5:
			glColor3f(1.0, 0.0, 1.0);
			break;
		case 6:
			glColor3f(0.0, 0.0, 0.0);
			break;
		default:
			glColor3f(1.0, 1.0, 1.0);
	}	
}

void drawInfoBox(void){

	char txt[20];
	bzero(txt,20);
    glLineWidth(1.0);
    for (int i=0; i<nChans; i++){
    	setViewportForWaveInfoN(i);
	  	if (i==selectedWaveform)
			glColor3f(.25, .25, .25);
		else 
			glColor3f(0, 0, 0);
		glRectf(-1, -1, 2, 2);
		
		glColor3f(1.0, 1.0, 1.0);
    	drawViewportEdge();

		setWaveformColor(colWave[i]);
		sprintf(txt, "T:18.%d", i);
		drawString(-.9, -.2, txt);
    }
}


void drawViewportEdge(){
	glBegin(GL_LINE_LOOP);
		glVertex2f(-.999, -.999);
		glVertex2f( .999, -.999);
		glVertex2f( .999, .999);
		glVertex2f(-.999, .999);
	glEnd();
}



void resizeWindow(int w, int h)
{
	winWidth = w;
	winHeight = h;

	yBox = (double)h/(double)nChans;

	glViewport( 0, 0, w, h );	// View port uses whole window

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0, 0, winWidth, winHeight, 0, 0 );

	glClear(GL_COLOR_BUFFER_BIT);
	refreshDrawing();
}


void specialKeyFn(int key, int x, int y){
	std::cout<<"Key Pressed:"<<key<<std::endl;
    switch(key){
      case GLUT_KEY_UP: // up
        selectedWaveform -=1;
        break;
      case GLUT_KEY_DOWN: //down
        selectedWaveform +=1;
        break;
    }
    if (selectedWaveform<0)
        selectedWaveform +=nChans;
    if (selectedWaveform>=nChans)
        selectedWaveform -=nChans;
	std::cout<<"SelectedWaveform:"<<selectedWaveform<<std::endl;
}

void keyPressedFn(unsigned char key, int x, int y){

	std::cout<<"Key Pressed:"<<key<<" value:"<<(int)key<<std::endl;

	if (enteringCommand){
		enterCommandStr(key);
		return;
	}
	switch (key){
		case CMD_QUIT:
			exit(1);
			break;
		case CMD_RESET_SEQ:
			curSeqNum = 0;
			clearWindow();
			std::cout<<"Reseting sequence number to 0!"<<std::endl;
			break;
		case CMD_CLEAR_WIN:
			clearWindow();
		break;
		case CMD_TOGGLE_OVERLAY: 
			toggleOverlay();
		break;
		// Scale Waveforms and Projections
		case CMD_SCALE_UP:
			userScale += dUserScale;
			break;
		case CMD_SCALE_DOWN:
			userScale -= dUserScale;
			if (userScale<1)
				userScale = 1;
			break;
		// Shift the waveforms only
		case CMD_SHIFT_UP:
			userShift += dUserShift;
			std::cout<<"User shift raised:"<<userShift<<std::endl;
			break;
		case CMD_SHIFT_DOWN:
			userShift -= dUserShift;
			std::cout<<"User shift lowered:"<<userShift<<std::endl;
			break;		
		// Commands that require additional user input
		case CMD_GAIN_ALL:
			enteringCommand = true;
			currentCommand = key;
			break;
		case CMD_PREV_COL:
			colWave[selectedWaveform]--;
			std::cout<<"Wave:"<<selectedWaveform<<" set to color:"<<colWave[selectedWaveform]<<std::endl;
		break;
		case CMD_NEXT_COL:
			colWave[selectedWaveform]++;
			std::cout<<"Wave:"<<selectedWaveform<<" set to color:"<<colWave[selectedWaveform]<<std::endl;
		break;
		}

 }
void enterCommandStr(char key){
	std::cout<<"Entering command";
	switch(key){
		// Erase command string
		case KEY_BKSP: //  Backspace Key
		case KEY_DEL: // MAC Delete key is pressed
			if (cmdStrIdx<=0){ //if the command string is empty ignore the keypress
				enteringCommand = false;
				return;
			}
			cmd[--cmdStrIdx] = 0; //backup the cursor and set the current char to 0
			eraseCommandString();
			break;
		case KEY_ENTER: // RETURN KEY
			executeCommand(cmd);
			bzero(cmd,cmdStrLen);
			cmdStrIdx = 0;
			eraseCommandString();
			enteringCommand = false;
			refreshDrawing(); // to erase the command window if no lfps are coming in
		break;
		default:
			if(key<' ') // if not a valid Alpha Numeric Char ignore it
				return;
			cmd[cmdStrIdx] = key;
			if (cmdStrIdx<CMD_STR_MAX_LEN)
				cmdStrIdx+=1;
			std::cout<<cmdStrIdx<<std::endl;
			std::cout<<"Command Entered:"<<cmd<<std::endl;
	}
	
}


void dispCommandString(){
	if (enteringCommand)
	{
		setViewportForCommandString();
		//Draw a black rectangle to cover up whatever was previously visible
		glColor3f(0.0, 0.0, 0.0);
		glRectf(-1, -1, 2, 2);
		//Draw the white bounding box
		glColor3f(1.0, 1.0, 1.0);
		drawViewportEdge();

		// Prepend the command char and :  for display purposes
		char dispCmd[CMD_STR_MAX_LEN+3];
		bzero(dispCmd, CMD_STR_MAX_LEN+3);
		for (int i=0; i<cmdStrIdx; i++)
			dispCmd[i+2] = cmd[i];
		dispCmd[0] = currentCommand;
		dispCmd[1] = ':';
		//draw the actuall string
		drawString(-.95,-.35, (char*)dispCmd);
	}
}

void drawString(float x, float y, char *string){

	glRasterPos2f(x, y);

	int len = strlen(string);
	for (int i = 0; i < len; i++) {
    	glutBitmapCharacter(font, string[i]);
	}
}

bool executeCommand(unsigned char *cmd){
	int len = sizeof(cmd);

	std::cout<<"Executing command:"<<cmd<<std::endl;
	currentCommand  = CMD_NULL;
}

void clearWindow(){
	glClear(GL_COLOR_BUFFER_BIT);
	pIdx = 0;
	dIdx = 0;
	xPos = 0;
}

void toggleOverlay(){
	disableWaveOverlay = !disableWaveOverlay;
	if(disableWaveOverlay)
		clearWindow();
}

inline int scaleVoltage(int v, int chan){
//	return (v*userScale) * pow(2,16) / nChans + pow(2,16) * (1-chan*plotRange) - (plotRange/yRange) + userShift;		
	return v * userScale / nChans + MAX_INT - MAX_INT/2/nChans - chan*MAX_INT/nChans  + userShift;	
}

int incrementIdx(int i){
	if(i==lfpBuffSize-1)
		return 0;
	else
		return i+1;
}

