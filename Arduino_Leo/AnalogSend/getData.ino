#include <Arduino.h>
#include "PINS.h"
#include "getData.h"


void getData() {
    // Read data from all the dials
    //  Mask the high byte and cache the low byte
    //  Mask the low byte, shift the bits, and cahce the high byte
    for (int i = 0; i < NUM_DIALS; i++) {
        int dialRead = analogRead(dialPins[i]);
        dataBuffer[i*2] = (byte)(dialRead & 0xff);         // Low byte
        dataBuffer[(i*2) + 1] = (byte)(dialRead & 0x0300 >> 8);   // High byte

        checksum[0] ^= dataBuffer[2*i];
        checksum[1] ^= dataBuffer[(2*i)+1];
    }


    // Read all of the sliders
    // Doing the same thing as with the dials (see above)
    for (int i = 0; i < NUM_SLIDERS; i++) {
        int readVar = analogRead(sliderPins[i]);
        dataBuffer[2*(NUM_DIALS+i)] = (byte)(readVar & 0xff);
        checksum[0] ^= dataBuffer[2*(NUM_DIALS+i)];
        dataBuffer[2*(NUM_DIALS+i)+1] = (byte)(readVar & 0x0300 >> 8); 
        checksum[1] ^= dataBuffer[2*(NUM_DIALS+i)+1];
    }

    // Read all of the switches
    for (int i = 0; i < NUM_SWITCHES; i++) {
       dataBuffer[2*NUM_SLIDERS + 2*NUM_DIALS + i] = (byte)digitalRead(switchPins[i]);
       checksum[0] ^= dataBuffer[2*NUM_SLIDERS + 2*NUM_DIALS + i];
    }
}



// Print out everyhing in the buffer array
// Should be in the order that they appear on the board
// Put a "|" between the potentiometers and switches
void dumpBuffer(byte* buffer, int len) {
    for (int i = 0; i < len; i++) {
        Serial.print(buffer[i], HEX);
        Serial.print(" ");
        if (i == (2*NUM_SLIDERS + 2*NUM_DIALS)-1) {
            Serial.print("| ");
        }
    }
    Serial.print(checksum[0], HEX);
    Serial.print(checksum[1], HEX);
    Serial.println("");
}

byte calcChecksum(byte vals[], int numVals, int byteNum) {
    for (int i = 0; i < numVals; i++) {
        checksum[byteNum] ^= vals[i];
    }
    return checksum[byteNum];
}


void sendData() {
    byte transmitBuffer[TRANS_BUFFER_LENGTH];
    transmitBuffer[0] = '\xFF';
    transmitBuffer[1] = '\x55';
    transmitBuffer[TRANS_BUFFER_LENGTH-2] = '\xFF';
    transmitBuffer[TRANS_BUFFER_LENGTH-1] = '\x44';
    for (int i = 0; i < DATA_BUFFER_SIZE; i++) {
        transmitBuffer[2 + i] = dataBuffer[i];
    }
    for (int i = 0; i < TRANS_BUFFER_LENGTH; i++) {
        Serial1.print(transmitBuffer[i]);
    }
}