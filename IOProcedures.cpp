//
// Created by Nicolás  Rojas on 4/11/20.
//

#include "IOProcedures.h"


void Capture_Params(int argc, char **argv) {
    Seed = atoi(argv[1]);
}

int int_rand(int a, int b) {
    int retorno = 0;

    if (a < b) {
        retorno = (int) ((b - a) * drand48());
        retorno = retorno + a;
    } else {
        retorno = (int) ((a - b) * drand48());
        retorno = retorno + b;
    }

    return retorno;
}

float float_rand(float a, float b) {
    float retorno = 0;

    if (a < b) {
        retorno = (float) ((b - a) * drand48());
        retorno = retorno + a;
    } else {
        retorno = (float) ((a - b) * drand48());
        retorno = retorno + b;
    }

    return retorno;
}

