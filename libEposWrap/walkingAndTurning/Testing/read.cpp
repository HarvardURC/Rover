#include <iostream>
#include <fstream>

std::ifstream myfile;
myfile.open ("controlValues.txt");
myfile >> gameData;
myfile.close();