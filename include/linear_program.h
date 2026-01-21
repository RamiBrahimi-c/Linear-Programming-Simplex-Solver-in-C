#pragma once

#define MAX_NUMBER_VARIABLES 30
#define MAX_NUMBER_CONSTRAINTS 50

#include "fraction.h"
#include "color_printf.h"
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "linear_program_struct.h"



#define false 0
#define true 1




#define MAX_NUMBER_VARIABLES 30
#define MAX_NUMBER_CONSTRAINTS 50




void printPattern(char symbol , int times) ; 

void printConstraints(Constraints constraints) ; 



DynammicArray* getFeasibleSolutionIndicesDynarr(LinearProgram linear_program) ;

DynammicArray* getFeasibleSolutionIndicesDynarr__(LinearProgram linear_program) ;
LinearProgram createLinearProblem(Constraints *constraints , ObjectiveFunction *objective_function ,enum LinearProblemType type) ;
void transformToStandardForm(LinearProgram *linear_program) ;

void transformToStandardForm_(LinearProgram *linear_program) ;


void setInitialFeasibleSolution(LinearProgram *linear_program) ;

void switchColsMatrix(Fraction matrix[MAX_NUMBER_CONSTRAINTS][MAX_NUMBER_VARIABLES] , int x  , int y , int rows) ;
int is_sorted_inc(enum VariableType * array ,int  length) ;

int sort_inc(enum VariableType *array , Constraints *constraints, int length) ;

int sort_inc2(enum VariableType *array , Constraints *constraints, int length) ; 

void setInitialFeasibleSolution2PhaseSimplex(LinearProgram *linear_program) ;

void printObjectiveFunction(ObjectiveFunction objective_function) ;


int* getFeasibleSolutionIndices__(LinearProgram linear_program);

Fraction productFractions(Fraction a , Fraction b) ;


float* getZiValuesf(LinearProgram linear_program) ;


void printSimplexTable(LinearProgram linear_program );

int maxValueInArray(float *arr , int length) ;

int minValueInArray(float *arr , int length) ;
float *getCiMinusZi(LinearProgram linear_program) ;

int selectEnteringVariable(LinearProgram linear_program) ;

int selectPivotRow(LinearProgram linear_program , int pivot_column) ;

void pivotingTransformRow(Fraction *row  , int length, Fraction pivot) ;

void pivotingTransformColumn(LinearProgram linear_program, int length ,int  pivot_row , int pivot_column) ; 

void copyArrayFraction(Fraction arr1 ,Fraction arr2) ;

void pivotingTransformRhs(LinearProgram linear_program  , int pivot_row ,int pivot_column) ;

void pivotingTransformRest(LinearProgram linear_program , int pivot_row , int pivot_column);



void printSortingVariable(LinearProgram linear_program , int index);

int isDoneIterating(LinearProgram linear_program) ;





int needs2PhaseSimplex_(LinearProgram linear_program) ;

int needs2PhaseSimplex(LinearProgram linear_program) ;

int needs2PhaseSimplex__(LinearProgram linear_program) ;

int simplexMethod(LinearProgram *linear_program) ;
int simplexMethodPhases(LinearProgram *linear_program) ;

void FirstPhaseSimplexMethod(LinearProgram linear_program);

void TwoPhaseSimplexMethod(LinearProgram *linear_program) ;

void prepareFractionCoeff(Fraction coeff[MAX_NUMBER_CONSTRAINTS][MAX_NUMBER_VARIABLES]) ;

void printVariableTypes(enum VariableType *arr , int length) ;

