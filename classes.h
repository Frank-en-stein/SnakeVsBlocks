#include <bits/stdc++.h>
#ifdef __WIN64
    #include <windows.h>  //for Sleep() only
    #include<GL/glut.h>
#elif __WIN32
    #include <windows.h>  //for Sleep() only
    #include<GL/glut.h>
#elif __APPLE_CC__
    #include <GLUT/glut.h>
#elif __linux
    #include<GL/glut.h>
#elif __unix
    #include<GL/glut.h>
#endif
using namespace std;

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

#include "classes.cpp"

#ifndef _classes
#define _classes
#endif // _classes

#define PI acos(-1)
double WIDTH=600;
double HEIGHT=600;


