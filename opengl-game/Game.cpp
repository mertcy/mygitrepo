#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

#include <string.h>

#include <time.h>

#define M_PI 3.14159265358979323846

GLint windowHeight, windowWidth;

double X1, Y1, X2, Y2, X3, Y3; //triangle agent's coordinates

GLdouble pixX, pixY; // my default pixel size

int score;
int leftClicked; // mouse left click counter
int level;
double levelOffset;
int agentLevelFlag;

bool game_paused;
bool game_over;
bool coin_taken;
bool debug_mode;

int coin_counter;

bool restart;

int indXX, indYY; // the indexes which the coin's center coordinates may take using the array below
double coinXX[25], coinYY[24]; // the values which the coin's center coordinates may take
double coinX, coinY; // coin's center coordinates


double sY; // for drawing sidewalks and roads

double sR[18] = { 37.5, 62.5, 87.5, 112.5, 162.5, 187.5, 212.5, 262.5, 287.5, 312.5, 337.5, 387.5, 412.5, 437.5, 462.5, 512.5, 537.5, 562.5 };
// y coordinate of each road's middle point 


int flag; // agent's sharp side for rotating


class Road {
public:
	struct Center {
		double X;
		double Y;
	};
	// center coordinates of each vehicle

	struct Color {
		double R;
		double G;
		double B;
	};
	// color of each vehicle

	struct Vehicle {
		Center center;
		Color color;
		int type;
		int size;
	};
	// vehicle declarations

	struct Lane {
		struct Vehicle vehicle;
		struct Lane *next;
	};
	//lane declarations

	struct Lane *head = NULL;
	struct Lane *current = NULL;

	int dir; // traffic flow direction of each road

	Vehicle createVehicle(double i) { // function of creating a vehicle
		Vehicle v;
		int r = (rand() % 8); // 1 or 2 or 3 or 4 or 5 or 6 or 7
		v.type = r;

		v.color.R = 0.5;
		int s = 0;

		switch (r) { // giving a randomness everytime a vehicle is to be created
			case 1: // car
			case 2:
			case 3:
			case 4:
			case 6:
				v.color.G = 0.4;
				v.color.B = 0.4;
				s = 20;
				break;

			case 5: // truck
			case 7:
				v.color.G = 0.1;
				v.color.B = 0.1;
				s = 40;
				break;
		}

		v.size = s;
		offset += (v.size * 2);


		switch (dir) {
		case 0: // go from left to right
			v.center.X = offset * -1;
			break;

		case 1: // go from right to left
			v.center.X = offset;
			break;
		}

		v.center.Y = i;
		offset += (v.size * 2);

		return v;
	}

	void addVehicle(Vehicle v) { // function of inserting each vehicle to each road
		struct Lane *ptr = (struct Lane*) malloc(sizeof(struct Lane));
		ptr->vehicle = v;

		if (head == NULL) {
			head = ptr;
			current = head;			
			head->next = NULL;
			current->next = NULL;
		}
		else {
			switch (dir) { // if a vehicle is out of the game box then there is no need to keep it inside the convoy
				case 0: // traffic flow from left to right
					if ((head->vehicle.center.X - head->vehicle.size / 2) == 500.0) {
						deleteVehicle();
					}
					break;
				case 1: // traffic flow from right to left
					if ((head->vehicle.center.X + head->vehicle.size / 2) == 0.0) {
						deleteVehicle();
					}
					break;
			}


			current->next = ptr;
			current = current->next;
			current->next = NULL;
		}
	}

	void deleteVehicle() { // function of removing a vehicle from a road when needed
		struct Lane *ptr = head;
		head = head->next;
	}

	void moveVehicles(double speed) { // function of moving the vehicles
		struct Lane *ptr = head;

		while (ptr != NULL) {
			switch (dir) { // a vehicle's new coordinate according to the flow of traffic on a road
				case 0:
					ptr->vehicle.center.X += speed;
					break;
				case 1:
					ptr->vehicle.center.X -= speed;
					break;
			}
			
			ptr = ptr->next;
		}

	}

	double offset; // putting each road's vehicles to their starting point

	void resetOffset() { // offset is reset everytime a new road is created
		switch (dir) {
			case 0: // go from left to right
				offset = 0.0;
				break;

			case 1: // go from right to left
				offset = 500.0;
				break;
		}
	}

};

Road r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15, r16, r17, r18;
// declarations of each road from bottom to top respectively

void resetSY() { // for drawing the empty road and sidewalk textures
	sY = 0.0;
}

void myInit() { // general initializations
	glClearColor(0.9, 0.9, 0.9, 1.0);		// background color
											// glShadeModel(GL_FLAT);		// flat shading
	// setting a new game
	restart = false;
	r1.head = NULL;
	r2.head = NULL;
	r3.head = NULL;
	r4.head = NULL;
	r5.head = NULL;
	r6.head = NULL;
	r7.head = NULL;
	r8.head = NULL;
	r9.head = NULL;
	r10.head = NULL;
	r11.head = NULL;
	r12.head = NULL;
	r13.head = NULL;
	r14.head = NULL;
	r15.head = NULL;
	r16.head = NULL;
	r17.head = NULL;
	r18.head = NULL;
	//
	level = 1;
	levelOffset = 0.0;
	agentLevelFlag = -1;
	score = 0;
	game_paused = false;
	game_over = false;
	leftClicked = 0;
	debug_mode = false;
	coin_taken = false;
	coin_counter = 0;

	//
	indXX = 0;
	for (double ix = 10.0; ix < 500.0; ix += 20.0) {
		coinXX[indXX] = ix;
		indXX++;
	}

	indYY = 0;
	for (double iy = 12.5; iy < 600.0; iy += 25.0) {
		coinYY[indYY] = iy;
		indYY++;
	}

	coinX = -70.0;
	coinY = 70.0;
	// giving a random place for a coin to appear at the starting of the game

	//
	X1 = 240.0;
	Y1 = 2.5;
	X2 = 260;
	Y2 = 2.5;
	X3 = 250.0;
	Y3 = 22.5;
	//triangle agent's starting position coordinates

	resetSY();

	pixX = pixY = 20.0; // a move's total length along the y-axis

	flag = 1; // agent sharpside looks up

	//
	srand(time(NULL));
	r1.dir = rand() % 2;// each road's traffic flow is instantiated
	r1.resetOffset();
	r2.dir = rand() % 2;
	r2.resetOffset();
	r3.dir = rand() % 2;
	r3.resetOffset();
	r4.dir = rand() % 2;
	r4.resetOffset();
	r5.dir = rand() % 2;
	r5.resetOffset();
	r6.dir = rand() % 2;
	r6.resetOffset();
	r7.dir = rand() % 2;
	r7.resetOffset();
	r8.dir = rand() % 2;
	r8.resetOffset();
	r9.dir = rand() % 2;
	r9.resetOffset();
	r10.dir = rand() % 2;
	r10.resetOffset();
	r11.dir = rand() % 2;
	r11.resetOffset();
	r12.dir = rand() % 2;
	r12.resetOffset();
	r13.dir = rand() % 2;
	r13.resetOffset();
	r14.dir = rand() % 2;
	r14.resetOffset();
	r15.dir = rand() % 2;
	r15.resetOffset();
	r16.dir = rand() % 2;
	r16.resetOffset();
	r17.dir = rand() % 2;
	r17.resetOffset();
	r18.dir = rand() % 2;
	r18.resetOffset();
	//road instantiations
}

void generateRoads() { // function of generating roads
	//
	r1.addVehicle(r1.createVehicle(sR[0]));
	r2.addVehicle(r2.createVehicle(sR[1]));
	r3.addVehicle(r3.createVehicle(sR[2]));
	r4.addVehicle(r4.createVehicle(sR[3]));

	r5.addVehicle(r5.createVehicle(sR[4]));
	r6.addVehicle(r6.createVehicle(sR[5]));
	r7.addVehicle(r7.createVehicle(sR[6]));

	r8.addVehicle(r8.createVehicle(sR[7]));
	r9.addVehicle(r9.createVehicle(sR[8]));
	r10.addVehicle(r10.createVehicle(sR[9]));
	r11.addVehicle(r11.createVehicle(sR[10]));

	r12.addVehicle(r12.createVehicle(sR[11]));
	r13.addVehicle(r13.createVehicle(sR[12]));
	r14.addVehicle(r14.createVehicle(sR[13]));
	r15.addVehicle(r15.createVehicle(sR[14]));

	r16.addVehicle(r16.createVehicle(sR[15]));
	r17.addVehicle(r17.createVehicle(sR[16]));
	r18.addVehicle(r18.createVehicle(sR[17]));
	// a vehicle is created and inserted into each road

	// roads are being updates as vehicles are moving along
	r1.moveVehicles(0.7 + levelOffset);
	r2.moveVehicles(0.3 + levelOffset);
	r3.moveVehicles(0.4 + levelOffset);
	r4.moveVehicles(0.9 + levelOffset);
	r5.moveVehicles(0.5 + levelOffset);
	r6.moveVehicles(0.8 + levelOffset);
	r7.moveVehicles(0.4 + levelOffset);
	r8.moveVehicles(0.3 + levelOffset);
	r9.moveVehicles(0.7 + levelOffset);
	r10.moveVehicles(0.9 + levelOffset);
	r11.moveVehicles(0.6 + levelOffset);
	r12.moveVehicles(0.3 + levelOffset);
	r13.moveVehicles(0.5 + levelOffset);
	r14.moveVehicles(0.6 + levelOffset);
	r15.moveVehicles(0.9 + levelOffset);
	r16.moveVehicles(0.4 + levelOffset);
	r17.moveVehicles(0.3 + levelOffset);
	r18.moveVehicles(0.5 + levelOffset);
	//
}

void updateScore() {
	if (coin_taken) {
		score += 5;
		coin_taken = false;
	}
	else
		score++;
}


//-----------------------------------------------------------------------
// reshape callback function
//	This is called each time the window is reshaped
//-----------------------------------------------------------------------
void myReshape(int winWidth, int winHeight)
{
	windowHeight = winHeight;
	windowWidth = winWidth;
	// update projection

	glViewport(0, 0, winWidth, winHeight);  // update the viewport
	glMatrixMode(GL_PROJECTION);              // update the projection
	glLoadIdentity();

	gluOrtho2D(0.0, 500.0, 0.0, 600.0);           // map unit square to viewport

	glMatrixMode(GL_MODELVIEW);

	glutPostRedisplay();                      // request redisplay



}

void updateAgent(double x1, double y1, double x2, double y2, double x3, double y3) {
	X1 += x1;
	Y1 += y1;
	X2 += x2;
	Y2 += y2;
	X3 += x3;
	Y3 += y3;

	if ((Y3 == 597.5) && (flag == 1) && (agentLevelFlag == -1)) {
		flag = 0; // sharpside needs to look down (rotation of agent)
		Y1 = 597.5;
		Y2 = 597.5;
		Y3 = 577.5;
		agentLevelFlag = 0;
	}
	else if ((Y3 == 2.5) && (flag == 0) && (agentLevelFlag == 0)) {
		flag = 1; // sharpside needs to look up (rotation of agent)
		Y1 = 2.5;
		Y2 = 2.5;
		Y3 = 22.5;
		level++;
		levelOffset += 0.4;
		agentLevelFlag = -1;
	}

}

void drawAgent() {
	glColor3f(0.3, 0.0, 0.0); //red
	glBegin(GL_POLYGON); //triangle
		glVertex2f(X1, Y1);
		glVertex2f(X2, Y2);
		glVertex2f(X3, Y3);
	glEnd();
}

void bitMapString(float x, float y, char str[]) { // drawing bitmap textures
	int i = 0;

	glRasterPos2f(x, y);

	while (str[i] != '\0') {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
		i++;
	}

	i = 0;
}

void drawBoard() {
	glColor3f(0.1, 0.1, 0.6);
	char levelBoard[20];
	char levelDecimal[10];
	char scoreBoard[20];
	char scoreDecimal[10];
	char coinBoard[40];
	char coinDecimal[10];
	sprintf(levelBoard, "%s", "LEVEL: ");
	sprintf(levelDecimal, "%d", level);
	sprintf(scoreBoard, "%s", "====  SCORE: ");
	sprintf(scoreDecimal, "%d", score);
	sprintf(coinBoard, "%s", "====  COIN: ");
	sprintf(coinDecimal, "%d", coin_counter);

	bitMapString(20.0, 80.0, levelBoard);
	bitMapString(110.0, 80.0, levelDecimal);
	bitMapString(140.0, 80.0, scoreBoard);
	bitMapString(310.0, 80.0,scoreDecimal);
	bitMapString(340.0, 80.0, coinBoard);
	bitMapString(480.0, 80.0, coinDecimal);
}

void drawGameOver() {
	glColor3f(0.6, 0.1, 0.1);
	bitMapString(180.0, 230.0, "..GAME OVER..");
}

void drawGamePaused() {
	glColor3f(0.1, 0.6, 0.1);
	bitMapString(180.0, 230.0, "..GAME PAUSED..");
}

void drawDebugMode() {
	glColor3f(0.8, 0.8, 0.8);
	bitMapString(180.0, 355.0, "..DEBUG MODE..");
}


void drawSidewalk() {
	glColor3f(0.6, 0.6, 0.6);
	glBegin(GL_POLYGON);
		glVertex2f(0.0, sY);
		glVertex2f(500.0, sY);		
		sY += 25.0;
		glVertex2f(500.0, sY);
		glVertex2f(0.0, sY);
	glEnd();

	sY += 25.0;
}

void drawEmptyLane(int l) {
	for (int i = 1; i <= l; i++) {
		for (double x = 0.0; x < 500.0; x += pixX) {
			glEnable(GL_LINE_SMOOTH);
			glLineWidth(2.0);
			glColor3f(0.8, 0.8, 0.0);
			glBegin(GL_LINES);
				glVertex2f(x, sY);
				x += pixX;
				glVertex2f(x, sY);
			glEnd();
		}
		sY += 25.0;
	}
}

void generateCoin(int id) {
	srand(time(NULL));

	coin_taken = false;

	if ((!game_over) && (!game_paused)) {
		indXX = (rand() % 25);
		indYY = (rand() % 24);

		coinX = coinXX[indXX];
		coinY = coinYY[indYY];
	}

	glutTimerFunc(10000, generateCoin, 1);
	
}

void drawCoin() {
	glColor4f(0.8, 0.8, 0.0, 1.0);
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(5.0);

	float angle;
	int i;
	glBegin(GL_LINE_LOOP);
	for (i = 0; i < 700; ++i)
	{
		angle = 2 * M_PI * i / 700;
		glVertex2f(coinX, coinY);
		glVertex2f(coinX + cos(angle) * 10.0, coinY + sin(angle) * 10.0);
	}
	glEnd();
}

bool lookAtCoin() { // function of looking for if the coin is taken or not
	int difX = X3 - coinX;
	int difY = Y3 - coinY;

	if ((difX == 0) && ((difY == 10) || (difY == -10))) {
		coin_taken = true;
		coin_counter++;
		updateScore();

		coinX = -70.0;
		coinY = 70.0;

		return true;
	}
	return false;
}

void drawRoads(Road r) { // function of drawing traffic for each road

	struct Road::Lane *ptr = r.head;

	//start from the beginning
	while (ptr != NULL) {
		Road::Vehicle v = ptr->vehicle;

		glColor3f(v.color.R, v.color.G, v.color.B);

		int s = v.size / 2;
		double cX = v.center.X;
		double cY = v.center.Y;

		glBegin(GL_POLYGON);
			glVertex2f(cX - s, cY - 10.0);
			glVertex2f(cX + s, cY - 10.0);
			glVertex2f(cX + s, cY + 10.0);
			glVertex2f(cX - s, cY + 10.0);
		glEnd();

		ptr = ptr->next;
	}
}

void drawTraffic() { // function of calling the drawing traffic function for each road
	drawRoads(r1);
	drawRoads(r2);
	drawRoads(r3);
	drawRoads(r4);
	drawRoads(r5);
	drawRoads(r6);
	drawRoads(r7);
	drawRoads(r8);
	drawRoads(r9);
	drawRoads(r10);
	drawRoads(r11);
	drawRoads(r12);
	drawRoads(r13);
	drawRoads(r14);
	drawRoads(r15);
	drawRoads(r16);
	drawRoads(r17);
	drawRoads(r18);
}


//-----------------------------------------------------------------------
// display callback function
//	This is called each time application needs to redraw itself.
//-----------------------------------------------------------------------

void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);       // clear the window
	
	glColor3f(1.0, 0.0, 0.0);       // set color to red

	glViewport(0.0, 0.0, windowWidth, 60 * windowHeight / 63.0);

	//textures start from bottom to top
	//sidewalks are squares, roads are lines..
	drawSidewalk();
	drawEmptyLane(3); // 4 lane road
	drawSidewalk();
	drawEmptyLane(2); // 3 lane road
	drawSidewalk();
	drawEmptyLane(3); // 4 lane road
	drawSidewalk();
	drawEmptyLane(3); // 4 lane road
	drawSidewalk();
	drawEmptyLane(2); // 3 lane road
	drawSidewalk();
	resetSY();


	drawCoin();

	drawTraffic();

	drawAgent();

	if (game_paused) {
		drawGamePaused();
	}

	if (game_over)
		drawGameOver();

	if (debug_mode)
		drawDebugMode();


	// a new viewport for the information board which also includes the score 
	glColor3f(0.8, 0.8, 0.8);
	glViewport(0, 60 * windowHeight / 63.0, windowWidth, 3 * windowHeight / 63.0);

	glBegin(GL_POLYGON);
		glVertex2f(0.0, 0.0);
		glVertex2f(500.0, 0.0);
		glVertex2f(500.0, 630.0);
		glVertex2f(0.0, 630.0);
	glEnd();

	glColor3f(0.5, 0.5, 0.5);
	drawBoard();

	glFlush(); // force OpenGL to render now

	glutSwapBuffers(); // swap buffers

}

void myKeyboard(unsigned char c, int x, int y) {
	switch (c) {
		case 'q': // exit game
			exit(0);			
			break;
		case 'r': // restart game
			debug_mode = false;
			restart = true;
			game_over = false;
			break;
	}

	glutPostRedisplay();		// request redisplay
}

void specialKeyInput(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_UP:
			if ((flag == 1) && (!game_paused) && (!game_over) && (!debug_mode)) {
				updateAgent(0, 25.0, 0, 25.0, 0, 25.0);
				updateScore();	
			}
			else if ((flag == 0) && (!debug_mode)) // incorrect move then the game is over
				game_over = true;

			break;

		case GLUT_KEY_DOWN:
			if ((flag == 0) && (!game_paused) && (!game_over) && (!debug_mode)) {
				updateAgent(0, -25.0, 0, -25.0, 0, -25.0);
				updateScore(); //score += 1
			}
			else if((flag == 1) && (!debug_mode)) // incorrect move then the game is over
				game_over = true;

			break;

		case GLUT_KEY_LEFT:
			if ((X1 != 0) && (!game_paused) && (!game_over) && (!debug_mode)) {
				updateAgent(-20, 0.0, -20, 0.0, -20, 0.0);
			}
			break;

		case GLUT_KEY_RIGHT:
			if ((X2 != 500) && (!game_paused) && (!game_over) && (!debug_mode)) {
				updateAgent(20, 0.0, 20, 0.0, 20, 0.0);
			}
			break;
	}

	glutPostRedisplay();
}

void printRoad_VehicleCoordinates(Road r) { // function of going over the road's vehicle centers
	struct Road::Lane *ptr = r.head;

		while (ptr != NULL) {
			ptr->vehicle;
			printf("Vehicle: ");
			if (ptr->vehicle.type == 0) {
				printf("Car, ");
			}else
				printf("Truck, ");

			printf("Center: (%.2f,%.2f),", ptr->vehicle.center.X, ptr->vehicle.center.Y);
			printf("Size: %d\n", ptr->vehicle.size);
			ptr = ptr->next;
		}

		printf("\n");
}

void runDebugger() { // function of single state debugging
	printf("Score : %d\n", score);

	printf("Agent is looking ");
	if (flag == 1) {
		printf("UP.\n");
		printf("Locations of the Agent\n");
		printf("--------------------------------------------------------\n");
		printf("---------------------(%.2f,%.2f)\n", X3, Y3);
		printf("---------------------------*\n");
		printf("--------------------------*-*\n");
		printf("-------------------------*---*\n");
		printf("------------------------*-----*\n");
		printf("-----------------------*-------*\n");
		printf("----------------------* * * * * *--(%.2f,%.2f)\n", X2, Y2);
		printf("---------(%.2f,%.2f)\n", X1, Y1);
		printf("--------------------------------------------------------\n");
	}
	else if (flag == 0) {
		printf("DOWN.\n");
		printf("Locations of the Agent\n");
		printf("--------------------------------------------------------\n");
		printf("---------(%.2f,%.2f)\n", X1, Y1);
		printf("----------------------* * * * * *--(%.2f,%.2f)\n", X2, Y2);
		printf("-----------------------*-------*\n");
		printf("------------------------*-----*\n");
		printf("-------------------------*---*\n");
		printf("--------------------------*-*\n");
		printf("---------------------------*\n");
		printf("---------------------(%.2f,%.2f)\n", X3, Y3);
		printf("--------------------------------------------------------\n");
	}


	printf("Road-1 Vehicle Coordinates:\n");
	printf("--------------------------------------------------------\n");
	printRoad_VehicleCoordinates(r1);
	printf("--------------------------------------------------------\n");
	printf("\nRoad-2 Vehicle Coordinates:\n");
	printf("--------------------------------------------------------\n");
	printRoad_VehicleCoordinates(r2);
	printf("--------------------------------------------------------\n");
	printf("\nRoad-3 Vehicle Coordinates:\n");
	printf("--------------------------------------------------------\n");
	printRoad_VehicleCoordinates(r3);
	printf("--------------------------------------------------------\n");
	printf("\nRoad-4 Vehicle Coordinates:\n");
	printf("--------------------------------------------------------\n");
	printRoad_VehicleCoordinates(r4);
	printf("--------------------------------------------------------\n");
	printf("\nRoad-5 Vehicle Coordinates:\n");
	printf("--------------------------------------------------------\n");
	printRoad_VehicleCoordinates(r5);
	printf("--------------------------------------------------------\n");
	printf("\nRoad-6 Vehicle Coordinates:\n");
	printf("--------------------------------------------------------\n");
	printRoad_VehicleCoordinates(r6);
	printf("--------------------------------------------------------\n");
	printf("\nRoad-7 Vehicle Coordinates:\n");
	printf("--------------------------------------------------------\n");
	printRoad_VehicleCoordinates(r7);
	printf("--------------------------------------------------------\n");
	printf("\nRoad-8 Vehicle Coordinates:\n");
	printf("--------------------------------------------------------\n");
	printRoad_VehicleCoordinates(r8);
	printf("--------------------------------------------------------\n");
	printf("\nRoad-9 Vehicle Coordinates:\n");
	printf("--------------------------------------------------------\n");
	printRoad_VehicleCoordinates(r9);
	printf("--------------------------------------------------------\n");
	printf("\nRoad-10 Vehicle Coordinates:\n");
	printf("--------------------------------------------------------\n");
	printRoad_VehicleCoordinates(r10);
	printf("--------------------------------------------------------\n");
	printf("\nRoad-11 Vehicle Coordinates:\n");
	printf("--------------------------------------------------------\n");
	printRoad_VehicleCoordinates(r11);
	printf("--------------------------------------------------------\n");
	printf("\nRoad-12 Vehicle Coordinates:\n");
	printf("--------------------------------------------------------\n");
	printRoad_VehicleCoordinates(r12);
	printf("--------------------------------------------------------\n");
	printf("\nRoad-13 Vehicle Coordinates:\n");
	printf("--------------------------------------------------------\n");
	printRoad_VehicleCoordinates(r13);
	printf("--------------------------------------------------------\n");
	printf("\nRoad-14 Vehicle Coordinates:\n");
	printf("--------------------------------------------------------\n");
	printRoad_VehicleCoordinates(r14);
	printf("--------------------------------------------------------\n");
	printf("\nRoad-15 Vehicle Coordinates:\n");
	printf("--------------------------------------------------------\n");
	printRoad_VehicleCoordinates(r15);
	printf("--------------------------------------------------------\n");
	printf("\nRoad-16 Vehicle Coordinates:\n");
	printf("--------------------------------------------------------\n");
	printRoad_VehicleCoordinates(r16);
	printf("--------------------------------------------------------\n");
	printf("\nRoad-17 Vehicle Coordinates:\n");
	printf("--------------------------------------------------------\n");
	printRoad_VehicleCoordinates(r17);
	printf("--------------------------------------------------------\n");
	printf("\nRoad-18 Vehicle Coordinates:\n");
	printf("--------------------------------------------------------\n");
	printRoad_VehicleCoordinates(r18);
	printf("--------------------------------------------------------\n");

	printf("Coin center (%.2f,%.2f)\n", coinX, coinY);
	printf("Number of coins taken so far: %d\n", coin_counter);
	generateRoads();
}

// called if mouse click
void myMouse(int b, int s, int x, int y) {
	switch (b) {    // b indicates the button

		case GLUT_LEFT_BUTTON:
			if (s == GLUT_DOWN) // button pressed
				if ((leftClicked == 0) && (!game_paused) && (!game_over) && (!debug_mode)) {
					leftClicked = 1;
					game_paused = true;
				}
				else if ((leftClicked == 1) && game_paused && (!game_over) && (!debug_mode)) {
					leftClicked = 0;
					game_paused = false;
				}
				else if ((!game_over) && debug_mode) {
					debug_mode = false;
					game_paused = false;
					leftClicked = 0;
				}

			else if (s == GLUT_UP) // button released
				if(!debug_mode)
					game_paused = false;
			break;

		case GLUT_RIGHT_BUTTON:
			if (s == GLUT_DOWN)      // button pressed
				if ((!game_paused) && (!game_over)){
					game_paused = false;
					debug_mode = true;
					runDebugger();
				}

			break;
			
	}
}

struct Road::Lane *ptrR;

bool crashedIterator(int dir) { // function of looking at crashes
	//start from the beginning
	while ((ptrR != NULL)) {

		if ((Y3 == (ptrR->vehicle.center.Y + 10) && (X1 <= (ptrR->vehicle.center.X + ptrR->vehicle.size / 2)) && (X1 >= (ptrR->vehicle.center.X - ptrR->vehicle.size / 2)))) {
			return true;
		}
		else if ((Y3 == (ptrR->vehicle.center.Y + 10) && (X2 >= (ptrR->vehicle.center.X - ptrR->vehicle.size / 2)) && (X2 <= (ptrR->vehicle.center.X + ptrR->vehicle.size / 2)))) {			
			return true;
		}
		else if ((Y3 == (ptrR->vehicle.center.Y - 10) && (X1 <= (ptrR->vehicle.center.X + ptrR->vehicle.size / 2)) && (X1 >= (ptrR->vehicle.center.X - ptrR->vehicle.size / 2)))) {
			return true;
		}
		else if ((Y3 == (ptrR->vehicle.center.Y - 10) && (X2 >= (ptrR->vehicle.center.X - ptrR->vehicle.size / 2)) && (X2 <= (ptrR->vehicle.center.X + ptrR->vehicle.size / 2)))) {			
			return true;
		}
		

		ptrR = ptrR->next;
	}
	return false; // not crashed..
}

bool crashed() { // function of calling each road if the agent crashed into it or not
	ptrR = r1.head;
	bool c1 = crashedIterator(r1.dir);
	ptrR = r2.head;
	bool c2 = crashedIterator(r2.dir);
	ptrR = r3.head;
	bool c3 = crashedIterator(r3.dir);
	ptrR = r4.head;
	bool c4 = crashedIterator(r4.dir);
	ptrR = r5.head;
	bool c5 = crashedIterator(r5.dir);
	ptrR = r6.head;
	bool c6 = crashedIterator(r6.dir);
	ptrR = r7.head;
	bool c7 = crashedIterator(r7.dir);
	ptrR = r8.head;
	bool c8 = crashedIterator(r8.dir);
	ptrR = r9.head;
	bool c9 = crashedIterator(r9.dir);
	ptrR = r10.head;
	bool c10 = crashedIterator(r10.dir);
	ptrR = r11.head;
	bool c11 = crashedIterator(r11.dir);
	ptrR = r12.head;
	bool c12 = crashedIterator(r12.dir);
	ptrR = r13.head;
	bool c13 = crashedIterator(r13.dir);
	ptrR = r14.head;
	bool c14 = crashedIterator(r14.dir);
	ptrR = r15.head;
	bool c15 = crashedIterator(r15.dir);
	ptrR = r16.head;
	bool c16 = crashedIterator(r16.dir);
	ptrR = r17.head;
	bool c17 = crashedIterator(r17.dir);
	ptrR = r18.head;
	bool c18 = crashedIterator(r18.dir);

	bool crashed = c1 || c2 || c3 || c4 || c5 || c6 || c7 || c8 || c9 || c10 || c11 || c12 || c13 || c14 || c15 || c16 || c17 || c18;

	return crashed; // if there is at least one crash in a road then the game is over
}

void myTimeOut(int id) { // advance the state of animation incrementally
	if (restart) // if the game is restarted after game over then a new game state starts
		myInit();



	if (!debug_mode) {
		if (crashed())
			game_over = true;

		if (((!game_over) && (!game_paused)) && (!debug_mode)) { // if the game state is not GAME OVER		
			generateRoads();
			updateAgent(0, 0, 0, 0, 0, 0); // updating agent movement
		}
	}

	lookAtCoin(); // check if a coin token is collected or not
	

	glutPostRedisplay();

	glutTimerFunc(20, myTimeOut, 0);

}

void main(int argc, char **argv) {
	srand(time(NULL));
	glutInit(&argc, argv);   //Initialize glut and gl
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // double buffering with RGB color mode

	glutInitWindowSize(500, 630);   // 500 px width x 630 px height
	glutInitWindowPosition(200, 20);
	glutCreateWindow("DON'T GET HIT!! by oguzmertcaylar.."); 	// create the window

	//register callbacks
	glutDisplayFunc(myDisplay);		//  call myDisplay to redraw window
	glutKeyboardFunc(myKeyboard);   // call myKeyboard when key is hit
	glutReshapeFunc(myReshape);     // call myReshape if window is resized
	glutMouseFunc(myMouse);         // call in mouse event 
	glutTimerFunc(20, myTimeOut, 0);
	glutTimerFunc(15000, generateCoin, 1);
	glutSpecialFunc(specialKeyInput);

	myInit();				// our own initializations

	glutMainLoop();			// pass control to GLUT, start it running
}