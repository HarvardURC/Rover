HOW I GOT THINGS WORKING:

For simplicity, I moved everything into a single .cpp file 
(this probably didn't change much, but it was a bit simpler)

I also forgot to put declarations for globalString into the 
interface file (WrapperTest.i) Function names were also overloaded, 
because earlier I made a class called WrapperTest.
When declaring functions in the interface file, 
I forgot to include the .h file and did not reference the function in the 
library by using (type) WrapperTest::functName(arg a, arg b);

Also, using the python-config --cflags argument helped simplify Python
library inclusion. See autocompile.sh for the commands used for compilation
and linking.
