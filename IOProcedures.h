//
// Created by Nicolás  Rojas on 4/11/20.
//

#ifndef IAA_SEED_IOPROCEDURES_H
#define IAA_SEED_IOPROCEDURES_H

#include <stdlib.h>

void Capture_Params(int argc, char** argv);
int int_rand (int a, int b);
float float_rand (float a, float b);

extern int Seed;

#endif //IAA_SEED_IOPROCEDURES_H
