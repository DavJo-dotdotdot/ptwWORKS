#include <iostream>
#include <istream>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <numeric>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringSerial.h>

#include "ptwFunctions.h"

using namespace std;

// NEVER CHANGE I AND J VALUE OUTSIDE SENSORREAD, AFTER INITIATION
int main() {

    // start ttyACM
    int extArr1;
    int extArr2;

    int leftArr = -1;       // initialize with -1 to check when set
    int rightArr = -1;  

    char sideDecide;

    ptwClass ptw;

    // initalize osideDecideutput, input and board
    wiringPiSetup();
    pinMode(0, INPUT); // gameState pin
    pinMode(2, OUTPUT); // Right flipper
    pinMode(3, OUTPUT); // Left flipper
    
    // set low for start up
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);

    // Check sensors etc.
    if ((extArr1 = serialOpen("/dev/ttyACM0", 115200)) < 0) {
        fprintf(stderr, "Could not open ACM0: %s\n", strerror(errno));
        return 1;
    }

    if ((extArr2 = serialOpen("/dev/ttyACM1", 115200)) < 0) {
        fprintf(stderr, "Could not open ACM1: %s\n", strerror(errno));
        return 1;
    }

    if (wiringPiSetup() == -1) {
        fprintf(stdout, "Unable to open WiringPi: %s\n", strerror(errno));
        return 1;
    }

    fprintf(stdout, "successfully opened matrix streams\n");

    fprintf(stdout, "Requesting side data from matrix\n");
    
    //while ((sideDecide = serialGetchar(extArr1)) != '\n') {
    // 
    while(leftArr == -1){
        serialFlush(extArr1);
        serialFlush(extArr2);
        // Set left/right arr by sending signal from stm32
        if (ptw.decideSide(extArr1) == 'L') {
            leftArr = extArr1;
            rightArr = extArr2;
            
        }
        else if (ptw.decideSide(extArr2) == 'L') {
            leftArr = extArr2;
            rightArr = extArr1;
        }
        else {
            cout << "Could not decide right/left array.\n";
            return 1;
        }
    }
    
    fprintf(stdout, "Ready to go!! \n");
    
    ptw.playGame(); // run once to initialize ptw.gameState
    
    //ptw.gameState != false
    
    while (1) {
        
        // sensor read runs everything from here
        ptw.sensorReadLeft(leftArr);
        ptw.sensorReadRight(rightArr);

        // continue/not continue
        ptw.playGame();
        
        // clear screen
        cout << "\033[2J";
        delay(10);
    }
}
