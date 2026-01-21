#pragma once

enum EquationSign {
    EQUAL ,
    LOWER_OR_EQUAL , 
    HIGHER_OR_EQUAL 
}  ;

enum VariableType {
    NORMAL ,
    SLACK , 
    SURPLUS , 
    ARTIFICIAL
}  ;


enum LinearProblemType {
    MAX ,
    MIN
};


typedef struct Constraints
{
    int n ;
    int m ;
    Fraction constraints_coeff[MAX_NUMBER_CONSTRAINTS][MAX_NUMBER_VARIABLES] ; 
    Fraction RHS[MAX_NUMBER_CONSTRAINTS] ; 
    enum EquationSign constraints_sign[MAX_NUMBER_CONSTRAINTS] ; 
    enum VariableType variables_type[MAX_NUMBER_VARIABLES] ;

}Constraints;


typedef struct ObjectiveFunction
{
    Fraction coefficiants[MAX_NUMBER_VARIABLES] ;
    int length ;

}ObjectiveFunction;



typedef struct DynammicArray
{
    int *arr ; 
    int length ; 
}DynammicArray;


typedef struct LinearProgram
{
    int original_decision_vars_num ; 
    int artificial_vars_num ; 
    int slack_vars_num ; 
    int surplus_vars_num ; 

    ObjectiveFunction* objective_function  ; 
    Constraints* constraints ; 
    enum LinearProblemType type ; 
    Fraction feasible_solution[MAX_NUMBER_VARIABLES] ; 
    DynammicArray *feasible_solution_indices ; 
}LinearProgram;
