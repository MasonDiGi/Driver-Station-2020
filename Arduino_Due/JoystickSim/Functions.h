#ifndef FUNCTIONS_H
#define FUNCTIONS_H


void setupButtons();
void setColor(int button, bool redVal, bool greenVal, bool blueVal);
void prettyColors();
void readButtons();
void sendJoyStates();
void getPiData();
void parseBytes(int);

int receiveBytes();

#endif
