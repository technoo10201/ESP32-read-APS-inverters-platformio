#ifndef TIJD_CALC_H
#define TIJD_CALC_H

#include <sunMoon.h>


void sun_setrise();
bool zomertijd();
int dow(int y, int m, int d);
void tijd_convert ();
int tijd_cvrt(char TIJD[6], bool wat);

#endif