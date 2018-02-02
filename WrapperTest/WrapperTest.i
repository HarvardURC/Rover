/* WrapperTest.i */
%module WrapperTest
%{
#include <string.h>
extern std::string globalString;
extern int getRandomNumber();
extern int add2Nums(int a, int b);
extern int printGlobalString();
%}

extern std::string globalString;
extern int getRandomNumber();
extern int add2Nums(int a, int b);
extern int printGlobalString();

