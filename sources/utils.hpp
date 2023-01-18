#ifndef UTILS
#define UTILS
#include <iostream>
#include<fstream>
#include "ap_int.h"

using namespace std;
unsigned int IdxArrayToIdxLine(unsigned int i,unsigned int j,unsigned int k,unsigned int w,unsigned int * dimensions);
int read_input(float* tmp,  int nb_inputs,char * name);
int read_output(float* tmp, int nb_output,char * name);

#endif
