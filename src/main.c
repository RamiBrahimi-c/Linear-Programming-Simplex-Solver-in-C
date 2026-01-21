#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

// #include "../include/color_printf.h"
// #include "../include/fraction.h"
#include "../include/linear_program.h"
#include "../include/parser.h"










// void 


LinearProgram interoExample() ;

LinearProgram courExample();
LinearProgram tdexo4() ;





int main()  {

    Constraints constraints;
    ObjectiveFunction objective_function;
    enum LinearProblemType  obj_type;
    prepareFractionCoeff(constraints.constraints_coeff) ; 

    if (!parse_lp_file("example.lp", &constraints, &objective_function, &obj_type)) {
        printf("Failed to parse file.\n");
        return 1;
    }

    // Print to verify
    printPattern('*', 50);
    // printConstraints(constraints);

    
    LinearProgram linear_problem = createLinearProblem(&constraints , &objective_function , obj_type) ; 
    

    

    printObjectiveFunction(objective_function) ; 
    
    
    printf("before transformation \n");
    
    
    printPattern('*' , 50) ; 
    printConstraints(constraints) ;
    printPattern('*' , 50) ; 
    
    
    transformToStandardForm_(&linear_problem) ; 

    
    
    printf("after transformation \n");


    printPattern('*' , 50) ; 
    printConstraints(constraints) ;
    printPattern('*' , 50) ; 
    printf("here \n") ; 
    int result = simplexMethod(&linear_problem) ; 
    printf("here \n") ; 
    if (result == -2)
    {
        FirstPhaseSimplexMethod(linear_problem) ; 
    }
    
    


    printf("\n") ; 
    return 0 ; 
}


LinearProgram interoExample()  {

    Constraints constraints ; 
    prepareFractionCoeff(constraints.constraints_coeff) ; 

    createFraction(&constraints.constraints_coeff[0][0] , 1 , 1) ;  
    createFraction(&constraints.constraints_coeff[0][1] , 2 , 1) ;  
    createFraction(&constraints.constraints_coeff[0][2] , 1 , 1) ;  
    constraints.constraints_sign[0] = HIGHER_OR_EQUAL ; 
    
    createFraction(&constraints.constraints_coeff[1][0] , 2 , 1) ;  
    createFraction(&constraints.constraints_coeff[1][1] , 1 , 1) ;  
    createFraction(&constraints.constraints_coeff[1][2] , 1 , 1) ;  
    constraints.constraints_sign[1] = LOWER_OR_EQUAL ; 
    
    createFraction(&constraints.constraints_coeff[2][0] , 1 , 1) ;  
    createFraction(&constraints.constraints_coeff[2][1] , 1 , 1) ;  
    createFraction(&constraints.constraints_coeff[2][2] , 2 , 1) ;  
    constraints.constraints_sign[2] = EQUAL ; 

    createFraction(&constraints.RHS[0] , 4 , 1 ) ; 
    createFraction(&constraints.RHS[1] , 6  , 1) ; 
    createFraction(&constraints.RHS[2] , 4  , 1) ; 

    constraints.variables_type[0] = NORMAL ; 
    constraints.variables_type[1] = NORMAL ; 
    constraints.variables_type[2] = NORMAL ; 
    constraints.m = 3 ; 
    constraints.n = 3 ; 
    
    printPattern('*' , 50) ; 
    // printConstraints(constraints) ; 
    
    ObjectiveFunction objective_function = {
        .length = constraints.n
    } ;

    createFraction(&objective_function.coefficiants[0] , 2 , 1) ; 
    createFraction(&objective_function.coefficiants[1] , 3 , 1) ; 
    createFraction(&objective_function.coefficiants[2] , 4 , 1) ; 
 
    
    LinearProgram linear_problem = createLinearProblem(&constraints , &objective_function , MAX) ; 
    
    return linear_problem ; 
}

LinearProgram courExample() {
    Constraints constraints ; 
    prepareFractionCoeff(constraints.constraints_coeff) ; 

    createFraction(&constraints.constraints_coeff[0][0] , 2 , 1) ;  
    createFraction(&constraints.constraints_coeff[0][1] , -1 , 1) ;  
    // createFraction(&constraints.constraints_coeff[0][2] , 1 , 1) ;  
    constraints.constraints_sign[0] = LOWER_OR_EQUAL ; 
    
    createFraction(&constraints.constraints_coeff[1][0] , 1 , 1) ;  
    createFraction(&constraints.constraints_coeff[1][1] , 1 , 1) ;  
    // createFraction(&constraints.constraints_coeff[1][2] , 1 , 1) ;  
    constraints.constraints_sign[1] = EQUAL ; 
    
    // createFraction(&constraints.constraints_coeff[2][0] , 1 , 1) ;  
    // createFraction(&constraints.constraints_coeff[2][1] , 1 , 1) ;  
    // createFraction(&constraints.constraints_coeff[2][2] , 2 , 1) ;  
    // constraints.constraints_sign[2] = EQUAL ; 

    createFraction(&constraints.RHS[0] , -1 , 1 ) ; 
    createFraction(&constraints.RHS[1] , 3  , 1) ; 
    // createFraction(&constraints.RHS[2] , 4  , 1) ; 

    constraints.variables_type[0] = NORMAL ; 
    constraints.variables_type[1] = NORMAL ; 
    // constraints.variables_type[2] = NORMAL ; 

    // number of constraints
    constraints.m = 2 ; 
    // number of variables
    constraints.n = 2 ; 
    
    printPattern('*' , 50) ; 
    // printConstraints(constraints) ; 
    
    ObjectiveFunction objective_function = {
        .length = constraints.n
    } ;

    createFraction(&objective_function.coefficiants[0] , 1 , 1) ; 
    createFraction(&objective_function.coefficiants[1] , 3 , 1) ; 
    // createFraction(&objective_function.coefficiants[2] , 4 , 1) ; 
 
    
    LinearProgram linear_problem = createLinearProblem(&constraints , &objective_function , MAX) ; 
    

    return linear_problem ; 
}

LinearProgram tdexo4() {

    Constraints constraints ; 
    prepareFractionCoeff(constraints.constraints_coeff) ; 

    createFraction(&constraints.constraints_coeff[0][0] , 2 , 1) ;  
    createFraction(&constraints.constraints_coeff[0][1] , -1 , 1) ;  
    createFraction(&constraints.constraints_coeff[0][2] , -2 , 1) ;  
    constraints.constraints_sign[0] = LOWER_OR_EQUAL ; 
    
    createFraction(&constraints.constraints_coeff[1][0] , 2 , 1) ;  
    createFraction(&constraints.constraints_coeff[1][1] , -3 , 1) ;  
    createFraction(&constraints.constraints_coeff[1][2] , 1 , 1) ;  
    constraints.constraints_sign[1] = LOWER_OR_EQUAL ; 
    
    createFraction(&constraints.constraints_coeff[2][0] , -1 , 1) ;  
    createFraction(&constraints.constraints_coeff[2][1] , 1 , 1) ;  
    createFraction(&constraints.constraints_coeff[2][2] , -2 , 1) ;  
    constraints.constraints_sign[2] = LOWER_OR_EQUAL ; 

    createFraction(&constraints.RHS[0] , 4 , 1 ) ; 
    createFraction(&constraints.RHS[1] , -5  , 1) ; 
    createFraction(&constraints.RHS[2] , -1  , 1) ; 

    constraints.variables_type[0] = NORMAL ; 
    constraints.variables_type[1] = NORMAL ; 
    constraints.variables_type[2] = NORMAL ; 

    // number of constraints
    constraints.m = 3 ; 
    // number of variables
    constraints.n = 3 ; 
    
    printPattern('*' , 50) ; 
    // printConstraints(constraints) ; 
    
    ObjectiveFunction objective_function = {
        .length = constraints.n
    } ;

    createFraction(&objective_function.coefficiants[0] , 1 , 1) ; 
    createFraction(&objective_function.coefficiants[1] , -1 , 1) ; 
    createFraction(&objective_function.coefficiants[2] , 1 , 1) ; 
 
    
    LinearProgram linear_problem = createLinearProblem(&constraints , &objective_function , MAX) ; 
    return linear_problem ;     
}




