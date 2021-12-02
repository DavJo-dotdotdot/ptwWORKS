#pragma once

#include <iostream>
#include <istream>
#include <atomic>
#include <sstream>
#include <chrono>
#include <numeric>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringSerial.h>

class ptwClass {
public:

	// Game states
	bool gameState;
	bool pinFire;

	// Array count in, have to be i and j for convinience for now
	int i = 0;
	int j = 0;

	// Table dimensions
	float tableLength = 750.0; float tableWidth = 450.0; float pinDistance = 10.0;
	float tableAngle;

	// Ball attributes
	float ballSpeed; float warnSpeed = 5.0;
	float ballWeight;

	// Array/matrix attributes
	int ptwCycle = 100;
	// *** int lastPosX, lastPosY, currentPosX, currentPosY; ***
	char left = 'L'; char right = 'R';

	// Matrix vital
	int cols = 11;
	int rows = 8;
	char cArr[1000];
	int i2dArr[8][11];
	int baseLvlMtxL[8][11] = { 100 };

	int cols1 = 11;
	int rows1 = 8;
	char cArr1[1000];
	int i2dArr1[8][11];
	int baseLvlMtxR[8][11] = { 100 };

	// Sensor values
	int lightLevelMed = 255; 	// will make it fire on startup
	float lightThreshold = 0.7;

	// tty startup and read values
	char reqChar = 'a';
	char cArrFeed;
	char cArrFeed1;

	// Array/matrix thresholds
	int arrVal;
	int arrValThreshold;

	/* ---------------------------------------------------------------------------- */

	// Take from tty0, convert c to c++, string to int, int to array
	// react to array
	void sensorReadLeft(int extArr);
	void sensorReadRight(int extArr);

	// Light on sensor set, call at beginning and in ball position
	int setLightLevel(int extVal);

	// Is ball above or beneath threshold
	bool comparePos(char side, int val, int comp);
	
	// Is sensorinfo from black spots
	bool notSensor(char side);

	// React to comparisions (take in comp2darr), function of if statements regarding ball position, flipper fire/no fire/expect
	void fireFlipper(char side);

	//~ // Use pythagoras etc to calculate ball direction
	//~ int ballDirection();

	// Calculate failure/no failure
	// int failToWin(int victoryState);

	// Decides gameState
	bool playGame();

	// Decides left and right matrix
	char decideSide(int extArr);
};
