#pragma once 



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Assuming your existing headers

#include "fraction.h"
#include "linear_program.h"


#define MAX_LINE 1024



// Helper: parse "a/b" or "a"
int parse_fraction(const char *token, Fraction *frac) ;
// Parse sign string
enum EquationSign parse_sign(const char *sign) ;
// Main parser
int parse_lp_file(const char *filename, Constraints *cons, ObjectiveFunction *obj,enum LinearProblemType *obj_type) ;

