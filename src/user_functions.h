#ifndef USER_FUNCT
#define USER_FUNCTIONS_H

/** Functions defined, instances of objects actually being user, this is
 * the stuff being called from the command line. Each one needs to accept
 * a list of strings (char* argv[]) which will be provided by the command
 * prompt function. The first argument will always be the name of the
 * function itself, everything after will be arguments passed by the
 * user. 
 *
 * sketch.h includes this file, but this file needs to include sketch.h
 * creating a circular dependancy. So we must give this all the same
 * includes as sketch and use the extern keyword to allow access to the
 * global variables defined in sketch.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Arduino.h>

#include "ashcon.h"
#include <kstrings.h>
#include <MemoryFree.h>
#include <pololu_servo.h>

extern ashcon* Console;

int utest_function(char* argv[]);
int ucheck_mem(char* argv[]);
int uhelp(char* argv[]);
int uabout(char* argv[]);

#endif
