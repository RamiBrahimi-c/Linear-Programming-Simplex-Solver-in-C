#pragma once

#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
// #include "linear_program.h"

typedef struct Fraction
{
    int x ; 
    int y ;
    int sign ;
    float value ;
}Fraction;



void createFraction(Fraction *fraction , int numerator , int denominator ) ;

int isFractionPositive(Fraction fraction);
int isFractionZeroVal(Fraction fraction);

int isFractionZero(Fraction fraction) ;

void makeFractionPositive(Fraction *fraction) ;

void makeFractionNegative(Fraction *fraction) ;


Fraction fractionProduct(Fraction a , Fraction b) ;

Fraction fractionDivision(Fraction a , Fraction b ) ;

Fraction fractionSubtraction(Fraction a , Fraction b) ;

void mapVector(Fraction *arr , int length , Fraction value) ;

void setFractionArrayToValue(Fraction arr[] , int length , int value);



void printFraction(Fraction fraction);


void printFractionpretty(Fraction fraction , int i) ;


int pgcd(int a, int b) ;


