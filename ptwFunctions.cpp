 #pragma once

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
ptwClass ptw;

void ptwClass::sensorReadLeft(int extArr) {

    serialFlush(extArr);
    serialPutchar(extArr, reqChar);

    while (!serialDataAvail(extArr)) {
        ;
    }

    i = 0;

    while ((cArrFeed = serialGetchar(extArr)) != '\n') {
        cArr[i] = cArrFeed;
        i++;
    }

    cArr[i] = 0;

    cout << '\n';
    cout << "\nParsing\n" << "\nLeft\n";

    char* str = cArr;
    char* pch;
    //printf ("String splits at \"%s\"\n into tokens\n", str);
    pch = strtok(str, " ,\t");
    j = 0; 		// arrays start at 0
    while (pch != NULL) {

        // printf("%s\n", pch);
        i2dArr[j / cols][j % cols] = atoi(pch);			// parse into integer after each division

        // run next string token, see if it is dividable even further
        pch = strtok(NULL, " ,\t");

        j++;

    }
    // test run on the matrix
    // parse as it stands in memory: 
    // cout << "in matrix form: " << endl;

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            // cout << i2dArr[i][j] << "\t";
            
            if (i2dArr[i][j] < lightLevelMed * lightThreshold) {
                pinFire = comparePos(left, i2dArr[i][j], baseLvlMtxL[i][j]);
                
                if (pinFire == true) {
                    fireFlipper(left);
                    
                    pinFire = false;
                    return;
                }
            }

            if (ptwCycle >= 100) {
                lightLevelMed = setLightLevel(i2dArr[0][5]);
                
                // cout << "\nLight level is: " << lightLevelMed << '\n';
                // delay(1000);
                ptwCycle = 0;
            }
        baseLvlMtxL[i][j] = baseLvlMtxL[i][j] * 0.9 + 0.1 * i2dArr[i][j];
        
        }
        cout << endl;

    }

    ++ptwCycle;
    serialFlush(extArr);
}

void ptwClass::sensorReadRight(int extArr) {
    serialFlush(extArr);
    serialPutchar(extArr, reqChar);

    while (!serialDataAvail(extArr)) {
        ;
    }

    i = 0;

    while ((cArrFeed1 = serialGetchar(extArr)) != '\n') {
        cArr1[i] = cArrFeed1;
        i++;
    }

    cArr1[i] = 0;

    cout << '\n';
    cout << "\nParsing\n" << "Right\n";

    char* str = cArr1;
    char* pch;
    // printf ("String splits at \"%s\"\n into tokens\n", str);
    pch = strtok(str, " ,\t");
    j = 0; 		// arrays start at 0
    while (pch != NULL) {

        // printf("%s\n", pch);
        i2dArr1[j / cols][j % cols] = atoi(pch);			// parse into integer after each division

        // run next string token, see if it is dividable even further
        pch = strtok(NULL, " ,\t");

        j++;

    }
    // test run on the matrix
    // parse as it stands in memory: 
    // cout << "in matrix form: " << endl;

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            // cout << i2dArr1[i][j] << "\t";

            if (i2dArr1[i][j] < lightLevelMed * lightThreshold) {
                pinFire = comparePos(right, i2dArr1[i][j], baseLvlMtxR[i][j]);
                
                if (pinFire == true) {
                    fireFlipper(right);
                    // cout << "\nFlipper fire at pos: " << i << ", " << j << ". \n"; 
                    
                    pinFire = false;
                    return;
                }
                
            baseLvlMtxR[i][j] = baseLvlMtxR[i][j] * 0.9 + 0.1 * i2dArr1[i][j];
            }
        }
        cout << endl;
    }
    serialFlush(extArr);
}

void ptwClass::fireFlipper(char side) {

    //send signal to arduino go high
    if (side == 'L') {
        cout << "\nfireFlipper LEFT triggered\n";
        
        digitalWrite(3, HIGH);
        delay(200);
        cout << "\nfireFlipper LEFT mid stage\n";
        
        digitalWrite(3, LOW);
        delay(100);
        
        cout << "\nfireFlipper LEFT end\n";
    }
    else if (side == 'R') {
        cout << "\nfireFlipper RIGHT triggered\n";
        
        digitalWrite(2, HIGH);
        delay(200);
        cout << "\nfireFlipper RIGHT mid stage\n";
        
        digitalWrite(2, LOW);
        delay(100);
        
        cout << "\nfireFlipper RIGHT end\n";
    }
    else {
        digitalWrite(2, HIGH);
        digitalWrite(3, HIGH);
        delay(200);
        digitalWrite(2, LOW);
        digitalWrite(3, LOW);
        delay(100);
    }
}

int ptwClass::setLightLevel(int extVal) {

    lightLevelMed = extVal;

    return lightLevelMed;
}

bool ptwClass::comparePos(char side, int val, int comp) {
    // bool checkSensor = notSensor(side);
    if (side == left && notSensor(side) == true && val < comp * lightThreshold) {
        return true;
    }
    else if (side == right && notSensor(side) == true && val < comp * lightThreshold1) {
        return true;
    }
    else {
        return false;
    }
}

bool ptwClass::notSensor(char side) {

    if (side == left) {
        if (i < 3) {
            return false;
        }
        else if (i == 5 && j <= 2) {
            return false;
        }
        else if (i == 6 && j <= 4) {
            return false;
        }
        else if (i == 7 && j <= 6) {
            return false;
        }
        else {
            return true;
        }
    }
    
    if (side == right) {
        if (i < 3) {
            return false;
        }
        else if (i == 5 && j >= 7) {
            return false;
        }
        else if (i == 6 && j >= 5) {
            return false;
        }
        else if (i == 7 && j >= 3) {
            return false;
        }
        else {
            return true;
        }
    }
    return 0;
}

bool ptwClass::playGame() {
    if (digitalRead(0) == LOW) {
        gameState = true;
        
        cout << "\n Game state is true \n";
        delay (100);
        
        return gameState;
    }
    else if (digitalRead(0) == HIGH) {
        gameState = false;
        return gameState;
    }
    else {
        gameState = true;
        return gameState;
    }
}

char ptwClass::decideSide(int extArr) {
    serialPutchar(extArr, '?');
    char gotChar;
    gotChar = serialGetchar(extArr);
    if ( gotChar== 'R') {
        //gotChar = serialGetchar(extArr);

        return gotChar;
    }
    else if (gotChar == 'L') {
        // gotChar = serialGetchar(extArr);

        return gotChar;
    }
    else {
        return 1;
    }
    
}

//~ // called for both array sets -- use left array and right array
//~ int ptwClass::ballPosition(int extArr[], int interMat[8][11]) {

    //~ // convert ext arr to array of ints
    //~ // becomes eight sets of arrays
    //~ // need own function? c => c++, string => int, arr => matrix

    //~ for (int i = 0; i < rows; ++i) {
        //~ for (int j = 0; j < columns; ++j) {

            //~ if (interMat[i][j] <= arrVal * arrValThreshold) {

                //~ currentPosX = j;
                //~ currentPosY = i;

                //~ interMat[i][j] = extArr[j];

                //~ // ball direction
                //~ if (currentPosY < lastPosY) {
                    //~ // ball down test
                    //~ cout << "Ball down" << endl;

                //~ }
                //~ else if (currentPosY > lastPosY) {
                    //~ // Ball up test
                    //~ cout << "Ball up" << endl;

                //~ }

                //~ lastPosX = currentPosX;
                //~ lastPosY = currentPosY;
            //~ }
        //~ }
    //~ }

//~ }

