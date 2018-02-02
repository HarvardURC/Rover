//WrapperTest.cpp
//.cpp file for the test thingy
#include <string.h>
#include <iostream>

using namespace std;

string globalString = "This is a string!";

int getRandomNumber() {
	return 4;
	//chosen by a random dice roll
	//ITS RANDOM I SWEAR!
}

int add2Nums(int a, int b) {
	return a+b;
}

void printGlobalString() {
	cout << globalString << endl;
}
