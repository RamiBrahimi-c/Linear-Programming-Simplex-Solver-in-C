#include "../include/linear_program.h"



DynammicArray* getFeasibleSolutionIndicesDynarr(LinearProgram linear_program) {
    int *arr = malloc(sizeof(int) * MAX_NUMBER_VARIABLES) ; 

    int total = linear_program.original_decision_vars_num + linear_program.slack_vars_num + linear_program.surplus_vars_num + linear_program.artificial_vars_num ; 
    int k = 0 ; 
    for (size_t i = 0; i < total ; i++)
    {

        if (!isFractionZero(linear_program.feasible_solution[i]))
        {
            arr[k] = i ; 
            k++ ; 
        }
               
    }
    printf("k = %d \n" , k) ; 
    printf("linear_program.constraints->m = %d \n" , linear_program.constraints->m) ; 
    assert(linear_program.constraints->m <= k) ; 
    DynammicArray *temp = malloc(sizeof(DynammicArray) * k) ;
    temp->arr = arr ; 
    temp->length = k ; 

    return temp ; 
}

DynammicArray* getFeasibleSolutionIndicesDynarr__(LinearProgram linear_program) {
    int *arr = malloc(sizeof(int) * MAX_NUMBER_VARIABLES) ; 

    int total = linear_program.original_decision_vars_num + linear_program.slack_vars_num + linear_program.surplus_vars_num + linear_program.artificial_vars_num ; 
    int k = 0 ; 
    int start = total - linear_program.artificial_vars_num - linear_program.surplus_vars_num - linear_program.slack_vars_num ;                             
        for (size_t i = 0; i < linear_program.constraints->m; i++)
        {
            // printf("constraint %d : \n" , i + 1) ; 
            
            for (size_t j = start ; j < total; j++)
            {
                // printf("j = %ld \n" , j) ;
                // printf("!isFractionZero(linear_program->constraints->constraints_coeff[i][j] || !ispositive ? %s \n" , (!isFractionZero(linear_program->constraints->constraints_coeff[i][j]) || 
            //  !isFractionPositive(linear_program->constraints->constraints_coeff[i][j])  ) ? "false " : "true") ; 
                

                if ( isFractionZero(linear_program.constraints->constraints_coeff[i][j]) || !isFractionPositive(linear_program.constraints->constraints_coeff[i][j])  )
                continue;



                if ( (linear_program.constraints->variables_type[j] == NORMAL || 
                                    linear_program.constraints->variables_type[j] == SURPLUS )  )
                    continue;

                    
                // if (counter == linear_program->constraints->m)
                // break;;

                // well look : 
                arr[k] = j ; 
                // createFraction(&linear_program.feasible_solution[j] , 1 , 1); 
                k++ ; 
                
                // counter++ ;
                // printf("added and counted \n") ; 
                break;
                /* code */
            }
                
            
        }
    // printf("k = %d \n" , k) ; 
    // printf("linear_program.constraints->m = %d \n" , linear_program.constraints->m) ; 
    assert(linear_program.constraints->m <= k) ; 
    DynammicArray *temp = malloc(sizeof(DynammicArray) * k) ;
    temp->arr = arr ; 
    temp->length = k ; 

    return temp ; 
}


LinearProgram createLinearProblem(Constraints *constraints , ObjectiveFunction *objective_function ,enum LinearProblemType type) {
    LinearProgram linear_program = {
        .constraints = constraints ,
        .type = type , 
        .objective_function = objective_function , 
        .artificial_vars_num = 0 ,
        .slack_vars_num = 0 ,
        .surplus_vars_num = 0 ,
        .original_decision_vars_num = constraints->n ,
        
    };
    // linear_program.feasible_solution_indices = getFeasibleSolutionIndicesDynarr(linear_program) ; 
    setFractionArrayToValue(linear_program.feasible_solution , constraints->n , 0) ; 

    return linear_program ; 
}


void transformToStandardForm(LinearProgram *linear_program) {
    int total = linear_program->original_decision_vars_num + linear_program->surplus_vars_num + linear_program->slack_vars_num + linear_program->artificial_vars_num ; 
    assert(linear_program->constraints->n == total);
    // linear_program->original_decision_vars_num = 

    // first of all lets clean it shall we ?

    for (size_t i = 0; i < linear_program->constraints->m ; i++)
    {
         ; 
        
        
    }
    



    // convention is the variables order in the constraints is : 
    // NORMAL -> SLACK -> SURPLUS -> ARTIFICIAL 


    for (size_t i = 0; i < linear_program->constraints->m; i++)
    {
        // printf("iteration %ld \n" , i) ; 
        
        int index = linear_program->surplus_vars_num + linear_program->slack_vars_num + linear_program->artificial_vars_num;
        int base_index = linear_program->original_decision_vars_num ;
        
        if (!isFractionPositive(linear_program->constraints->RHS[i]))
        {
            if (linear_program->constraints->constraints_sign[i] == HIGHER_OR_EQUAL)
            {
                Fraction temp  ;
                createFraction(&temp , -1 , 1 ) ;  
                
                // warning ???
                // mapVector(&linear_program->constraints->constraints_coeff[i] , total ,temp  ) ;
                mapVector(linear_program->constraints->constraints_coeff[i], total, temp);
                
                linear_program->constraints->constraints_sign[i] = LOWER_OR_EQUAL ; 
                 
                Fraction value = linear_program->constraints->RHS[i] ; 
                // BUG
                createFraction(&linear_program->constraints->RHS[i] ,  -value.x , value.y ) ;  
                // createFraction(&linear_program->constraints->RHS[i] ,-1* value.sign *  value.x , value.y ) ;  
            }
            
        }



        if (linear_program->constraints->constraints_sign[i] == LOWER_OR_EQUAL)
        {
            // printf("iteration %ld  : added slack variable \n"  , i) ; 
            // printf("<= constraint \n");
            createFraction(&linear_program->constraints->constraints_coeff[i][base_index+index] , 1 , 1) ;  
            linear_program->constraints->constraints_sign[i] = EQUAL ; 
            linear_program->slack_vars_num++ ;

            linear_program->constraints->variables_type[base_index+index] = SLACK ;  
            linear_program->constraints->n++ ;
            
        } else  if (linear_program->constraints->constraints_sign[i] == HIGHER_OR_EQUAL) {
            // printf(">= constraint \n");
            // printf("iteration %ld  : added surplus variable \n"  , i) ; 
            // mapVector(constraints->constraints_coeff[i] , constraints->n ,-1 ) ; 
            // constraints->RHS[i].sign *= -1 ;
            createFraction(&linear_program->constraints->constraints_coeff[i][base_index+index] , -1 , 1) ;  
            linear_program->constraints->constraints_sign[i] = EQUAL ; 
            linear_program->surplus_vars_num++ ; 
            
            linear_program->constraints->variables_type[base_index+index] = SURPLUS ;  
            linear_program->constraints->n++ ;
            
            if (!isFractionPositive(linear_program->constraints->RHS[i]))
            {
                createFraction(&linear_program->constraints->constraints_coeff[i][base_index+index+1] , 1 , 1) ;  
                // linear_program->constraints->constraints_sign[i] = EQUAL ; 
                linear_program->artificial_vars_num++ ; 
                
                linear_program->constraints->variables_type[base_index+index+1] = ARTIFICIAL ;  
                linear_program->constraints->n++ ;
                
            }
            
        } else if (linear_program->constraints->constraints_sign[i] == EQUAL) {
            // printf("= constraint \n");
            createFraction(&linear_program->constraints->constraints_coeff[i][base_index+index] , 1 , 1) ;  
            // linear_program->constraints->constraints_sign[i] = EQUAL ; 
            linear_program->artificial_vars_num++ ; 
                
            linear_program->constraints->variables_type[base_index+index] = ARTIFICIAL ;  
            linear_program->constraints->n++ ;

        }


        // printSimplexTable(*linear_program) ; 
    }
    
}


void transformToStandardForm_(LinearProgram *linear_program) {
    
    // TODO : for now correct but needs an optimization

    int total = linear_program->original_decision_vars_num + linear_program->surplus_vars_num + linear_program->slack_vars_num + linear_program->artificial_vars_num ; 
    assert(linear_program->constraints->n == total);
    // linear_program->original_decision_vars_num = 

    // first of all lets clean it shall we ?

    for (size_t i = 0; i < linear_program->constraints->m ; i++)
    {
         ; 
        
        
    }
    



    // convention is the variables order in the constraints is : 
    // NORMAL -> SLACK -> SURPLUS -> ARTIFICIAL 


    for (size_t i = 0; i < linear_program->constraints->m; i++)
    {
        // printf("iteration %ld \n" , i) ; 
        
        int index = linear_program->surplus_vars_num + linear_program->slack_vars_num + linear_program->artificial_vars_num;
        int base_index = linear_program->original_decision_vars_num ;
        
        if (!isFractionPositive(linear_program->constraints->RHS[i]))
        {
            // printf("condition 1 boooo\n") ; 
            if (linear_program->constraints->constraints_sign[i] == HIGHER_OR_EQUAL)
            {
                // printf("condition 2 noooo\n") ; 
                // printf("MTC   %f  \n\n " , linear_program->constraints->RHS[i].value) ; 
                Fraction temp  ;
                createFraction(&temp , -1 , 1 ) ;  
                // warning 
                // mapVector(&linear_program->constraints->constraints_coeff[i] , total ,temp  ) ;
                mapVector(linear_program->constraints->constraints_coeff[i], total, temp);

                linear_program->constraints->constraints_sign[i] = LOWER_OR_EQUAL ; 
                 
                Fraction value = linear_program->constraints->RHS[i] ; 
                // BUG
                createFraction(&linear_program->constraints->RHS[i] ,  -value.x , value.y ) ;  
                // createFraction(&linear_program->constraints->RHS[i] ,-1* value.sign *  value.x , value.y ) ;  
            }
            if (linear_program->constraints->constraints_sign[i] == LOWER_OR_EQUAL)
            {
                // printf("condition 2 noooo\n") ; 
                // printf("MTC   %f  \n\n " , linear_program->constraints->RHS[i].value) ; 
                Fraction temp  ;
                createFraction(&temp , -1 , 1 ) ;  
                // warning 
                // mapVector(&linear_program->constraints->constraints_coeff[i] , total ,temp  ) ;
                mapVector(linear_program->constraints->constraints_coeff[i], total, temp);
                linear_program->constraints->constraints_sign[i] = HIGHER_OR_EQUAL ; 
                 
                Fraction value = linear_program->constraints->RHS[i] ; 
                // BUG
                createFraction(&linear_program->constraints->RHS[i] ,  -value.x , value.y ) ;  
                // createFraction(&linear_program->constraints->RHS[i] ,-1* value.sign *  value.x , value.y ) ;  
            }
            
        }



        // printSimplexTable(*linear_program) ; 
    }

    for (size_t i = 0; i < linear_program->constraints->m; i++)
    {
        // printf("iteration %ld \n" , i) ; 
        
        int index = linear_program->surplus_vars_num + linear_program->slack_vars_num + linear_program->artificial_vars_num;
        int base_index = linear_program->original_decision_vars_num ;
        

        if (linear_program->constraints->constraints_sign[i] == LOWER_OR_EQUAL)
        {
            // printf("iteration %ld  : added slack variable \n"  , i) ; 
            // printf("<= constraint \n");
            createFraction(&linear_program->constraints->constraints_coeff[i][base_index+index] , 1 , 1) ;  
            linear_program->constraints->constraints_sign[i] = EQUAL ; 
            linear_program->slack_vars_num++ ;
            
            linear_program->constraints->variables_type[base_index+index] = SLACK ;  
            linear_program->constraints->n++ ;
            
        } else  if (linear_program->constraints->constraints_sign[i] == HIGHER_OR_EQUAL) {
            // printf(">= constraint \n");
            // printf("iteration %ld  : added surplus variable \n"  , i) ; 
            // mapVector(constraints->constraints_coeff[i] , constraints->n ,-1 ) ; 
            // constraints->RHS[i].sign *= -1 ;
            createFraction(&linear_program->constraints->constraints_coeff[i][base_index+index] , -1 , 1) ;  
            linear_program->constraints->constraints_sign[i] = EQUAL ; 
            linear_program->surplus_vars_num++ ; 
            
            linear_program->constraints->variables_type[base_index+index] = SURPLUS ;  
            linear_program->constraints->n++ ;

            if (isFractionPositive(linear_program->constraints->RHS[i]))
            {
                createFraction(&linear_program->constraints->constraints_coeff[i][base_index+index+1] , 1 , 1) ;  
                // linear_program->constraints->constraints_sign[i] = EQUAL ; 
                linear_program->artificial_vars_num++ ; 
                
                linear_program->constraints->variables_type[base_index+index+1] = ARTIFICIAL ;  
                linear_program->constraints->n++ ;
                
            }
            
            
        }  else if (linear_program->constraints->constraints_sign[i] == EQUAL) {
            // printf("= constraint \n");
            createFraction(&linear_program->constraints->constraints_coeff[i][base_index+index] , 1 , 1) ;  
            // linear_program->constraints->constraints_sign[i] = EQUAL ; 
            linear_program->artificial_vars_num++ ; 
                
            linear_program->constraints->variables_type[base_index+index] = ARTIFICIAL ;  
            linear_program->constraints->n++ ;

        } 
            
        if (linear_program->constraints->constraints_sign[i] == HIGHER_OR_EQUAL) {
            // printf(">= constraint \n");
            
            
        }


        // printSimplexTable(*linear_program) ; 
    }
    
}



void setInitialFeasibleSolution(LinearProgram *linear_program) {
    Constraints constraints = *linear_program->constraints ; 
    int k = linear_program->original_decision_vars_num ; 
    // first clean 
    for (size_t i = 0; i < linear_program->constraints->m ; i++)
    {
        for (size_t j = 0; j < k; j++)
        {
            createFraction(&constraints.constraints_coeff[i][j] , 0 , 1) ; 
            
        }
    }
    int counter = 0 ; 

    for (size_t i = 0; i < linear_program->constraints->m; i++)
    {
        int total = linear_program->original_decision_vars_num +  linear_program->artificial_vars_num + linear_program->slack_vars_num + linear_program->surplus_vars_num ; 
        for (size_t j = k; j < total ; j++)
        {
            if (!isFractionZero(constraints.constraints_coeff[i][j]))
            {
                 ;
                 counter++ ; 
                createFraction(&linear_program->feasible_solution[j] , 1 , 1); 
                // printf("DEBUG : added %d %d " , i , j ) ; 
            }
            
        }
             
    }
    // printf("our counter : %d \n" , counter) ;
    assert(counter== linear_program->constraints->m && "best feasible solution exceeds our number of constraints" ) ;  
    
}


void switchColsMatrix(Fraction matrix[MAX_NUMBER_CONSTRAINTS][MAX_NUMBER_VARIABLES] , int x  , int y , int rows) {
    for (size_t i = 0; i < rows; i++)
    {
        Fraction temp = matrix[i][x] ; 
        // matrix[i][x] = matrix[i][y] ;
        // printf("swap \n ") ; 
        // printf("temp :  ") ; 
        // printFraction(temp) , 
        // printFraction(matrix[i][x]) , 
        createFraction(&matrix[i][x] , matrix[i][y].x , matrix[i][y].y ) ;
        // printf("swap %d and %d \n" , x , y) ;  
        // matrix[i][y] = temp ; 
        // printFraction(matrix[i][y]) , 
        createFraction(&matrix[i][y] , temp.x , temp.y ) ;  
    
    }
    
}

int is_sorted_inc(enum VariableType * array ,int  length) {
    for (size_t i = 0; i < length; i++)
        if (array[i]>array[i+1])
            return 0 ; 
    
    return 1 ; 
}

int sort_inc(enum VariableType *array , Constraints *constraints, int length) {
    // Check for invalid inputs
    if (array == NULL || length <= 0) {
        return -1; // Error code for invalid input
    }
    
    // If array has only one element, it's already sorted
    if (length == 1) {
        return 0; // Success
    }
    
    // Insertion sort algorithm
    for (int i = 1; i < length; i++) {
        enum VariableType key = array[i];
        int j = i - 1;
        
        // Move elements of array[0..i-1] that are greater than key
        // to one position ahead of their current position
        while (j >= 0 && array[j] > key) {
            array[j + 1] = array[j];
            switchColsMatrix(constraints->constraints_coeff , j+1 , j  , constraints->m) ; 
            j = j - 1;
        }
        array[j + 1] = key;
        switchColsMatrix(constraints->constraints_coeff , j+1 , j  , constraints->m) ; 
    }
    
    return 0; // Success
}

int sort_inc2(enum VariableType *array , Constraints *constraints, int length) {

    // printf("in sorting : \n") ; 
    // printConstraints(*constraints) ; 

    

    // Check for invalid inputs
    if (array == NULL || length <= 0) {
        return -1; // Error code for invalid input
    }
    
    // If array has only one element, it's already sorted
    if (length == 1) {
        return 0; // Success
    }
    
    // Insertion sort algorithm
    for (int i = 1; i < length; i++) {
        enum VariableType min = array[i]  ;
        int min_index = i ;  
        for (size_t j = i+1; j < length; j++)
        {
            if (array[j] < min)
            {
                min_index = j ; 
                min = array[j] ; 
            }
            
        }
        if (i != min_index)
        {
            // do swap 
            enum VariableType temp = array[i] ; 
            array[i] = array[min_index] ; 
            array[min_index] = temp ; 

            switchColsMatrix(constraints->constraints_coeff , i , min_index , constraints->m) ; 
        }
        
        
    }
    
    return 0; // Success
}


void setInitialFeasibleSolution2PhaseSimplex(LinearProgram *linear_program) {
    Constraints constraints = *linear_program->constraints ; 
    int k = linear_program->original_decision_vars_num ; 
    // peintf("possible bug \n") ; 
    // first clean 
    // printf("possibke bug 1 ??? \n") ; 
        
    for (size_t i = 0; i < linear_program->constraints->m ; i++)
    {
        for (size_t j = 0; j < k; j++)
        {
            createFraction(&constraints.constraints_coeff[i][j] , 0 , 1) ; 
            
        }
    }
    
    
    int counter = 0 ; 

    int total = linear_program->original_decision_vars_num +  linear_program->artificial_vars_num + linear_program->slack_vars_num + linear_program->surplus_vars_num ; 
    int start = total -  (linear_program->artificial_vars_num)  ; 
    // printf("total : %d \n" , total);
    // printf("start : %d \n" , start);
    // printf("linear_program->original_decision_vars_num : %d \n" , linear_program->original_decision_vars_num);
    // printf("linear_program->slack_vars_num : %d \n" , linear_program->slack_vars_num);
    // printf("linear_program->surplus_vars_num : %d \n" , linear_program->surplus_vars_num);
    // printf("linear_program->artificial_vars_num : %d \n" , linear_program->artificial_vars_num);

    for (size_t j =start ; j < total ; j++)
    {
        counter++ ;
        // printf("j = %ld \n" , j);
        createFraction(&linear_program->feasible_solution[j] , 1 , 1); 
        // printf("DEBUG : added %d %d " , i , j ) ; 
            
    }

    

    // printf("artificial var : %d \n" , linear_program->artificial_vars_num) ;
    // printf("our counter : %d \n" , counter) ;

    int result = is_sorted_inc(linear_program->constraints->variables_type , linear_program->constraints->n) ; 
    // printf(" sorted ? %s \n" , result == 0 ? "false" : "true") ;
    if(!result) {
        
        sort_inc2(linear_program->constraints->variables_type , linear_program->constraints , linear_program->constraints->n) ; 
        
    } 
    // printConstraints(*linear_program->constraints) ; 

    if (counter < linear_program->constraints->m)
    {
        // ?????????????????????????????????????????????????????
        // TODO : this logic is so weak 
        // start = total - linear_program->artificial_vars_num - 1 ;
        start = total - linear_program->artificial_vars_num - linear_program->slack_vars_num - linear_program->surplus_vars_num;
        counter = 0 ; 

        // exit(0) ; 

        // printf("order of variables : \n") ; 
        // for (size_t k = 0; k < linear_program->constraints->n  ; k++)
        //             {
        //                     printf("variable type : " ) ;
                        
        //                 switch (linear_program->constraints->variables_type[k])
        //                 {
        //                     case ARTIFICIAL:
        //                     printf(" artificial \n"  ) ; 
        //                     /* code */
        //                     break;
        //                     case SLACK : 
        //                     printf(" slack \n"  ) ; 
        //                     break;
        //                     case SURPLUS : 
        //                     printf(" surplus \n"  ) ; 
        //                     break;
        //                     case NORMAL : 
        //                     printf(" normal \n"  ) ; 
        //                     break;
                            
        //                     default:
        //                     printf(" huh??? \n"  ) ; 
        //                     break;
        //                 } 

        //             }
                            
        for (size_t i = 0; i < linear_program->constraints->m; i++)
        {
            // printf("constraint %d : \n" , i + 1) ; 
            
            for (size_t j = start ; j < total; j++)
            {
                #if 0
                // printf("j = %ld \n" , j) ;
                // printf("!isFractionZero(linear_program->constraints->constraints_coeff[i][j] || !ispositive ? %s \n" , (!isFractionZero(linear_program->constraints->constraints_coeff[i][j]) || 
            //  !isFractionPositive(linear_program->constraints->constraints_coeff[i][j])  ) ? "false " : "true") ; 
                
                // printf("variable type : " ) ;
                
                switch (linear_program->constraints->variables_type[j])
                {
                    case ARTIFICIAL:
                    printf(" artificial \n"  ) ; 
                    /* code */
                    break;
                    case SLACK : 
                    printf(" slack \n"  ) ; 
                    break;
                    case SURPLUS : 
                    printf(" surplus \n"  ) ; 
                    break;
                    case NORMAL : 
                    printf(" normal \n"  ) ; 
                    break;
                    
                    default:
                    printf(" huh??? \n"  ) ; 
                    break;
                } 
                #endif
                if ( isFractionZero(linear_program->constraints->constraints_coeff[i][j]) || !isFractionPositive(linear_program->constraints->constraints_coeff[i][j])  )
                continue;



                if ( (linear_program->constraints->variables_type[j] == NORMAL || 
                                    linear_program->constraints->variables_type[j] == SURPLUS )  )
                    continue;

                    
                // if (counter == linear_program->constraints->m)
                // break;;

                // well look : 
                createFraction(&linear_program->feasible_solution[j] , 1 , 1); 
                counter++ ;
                // printf("added and counted \n") ; 
                break;
                /* code */
            }
                
            
        }
        

    }
    for (size_t i = 0; i < linear_program->constraints->m; i++)
    {
        // printFraction(linear_program->feasible_solution[i]) ; 
    }
    

    

    // printConstraints(*linear_program->constraints) ; 
    assert(counter== linear_program->constraints->m && "best feasible solution exceeds our number of constraints" ) ;  
    
}


void printObjectiveFunction(ObjectiveFunction objective_function) {
    for (size_t i = 0; i < objective_function.length; i++)
    {
        // printFractionpretty(objective_function.coefficiants[i] , i+1);
        printf("Fraction = { %d/%d = %f } \n" ,objective_function.coefficiants[i].x , objective_function.coefficiants[i].y ,  objective_function.coefficiants[i].value   ) ; 
    }printf("\n") ; 
    
}


int* getFeasibleSolutionIndices(LinearProgram linear_program) {
    int *arr = malloc(sizeof(int) * MAX_NUMBER_VARIABLES) ; 

    int total = linear_program.original_decision_vars_num + linear_program.slack_vars_num + linear_program.surplus_vars_num + linear_program.artificial_vars_num ; 
    int k = 0 ; 
    int start = total - linear_program.artificial_vars_num - linear_program.surplus_vars_num - linear_program.slack_vars_num ;                             
        for (size_t i = 0; i < linear_program.constraints->m; i++)
        {
            // printf("constraint %d : \n" , i + 1) ; 
            
            for (size_t j = start ; j < total; j++)
            {
                // printf("j = %ld \n" , j) ;
                // printf("!isFractionZero(linear_program->constraints->constraints_coeff[i][j] || !ispositive ? %s \n" , (!isFractionZero(linear_program->constraints->constraints_coeff[i][j]) || 
            //  !isFractionPositive(linear_program->constraints->constraints_coeff[i][j])  ) ? "false " : "true") ; 
                

                if ( isFractionZero(linear_program.constraints->constraints_coeff[i][j]) || !isFractionPositive(linear_program.constraints->constraints_coeff[i][j])  )
                continue;



                if ( (linear_program.constraints->variables_type[j] == NORMAL || 
                                    linear_program.constraints->variables_type[j] == SURPLUS )  )
                    continue;

                    
                // if (counter == linear_program->constraints->m)
                // break;;

                // well look : 
                arr[k] = j ; 
                // createFraction(&linear_program.feasible_solution[j] , 1 , 1); 
                
                
                // counter++ ;
                // printf("added and counted \n") ; 
                break;
                /* code */
            }
                
            
        }



    return arr ; 
}

int* getFeasibleSolutionIndices__(LinearProgram linear_program) {
    int *arr = malloc(sizeof(int) * MAX_NUMBER_VARIABLES) ; 

    int total = linear_program.original_decision_vars_num + linear_program.slack_vars_num + linear_program.surplus_vars_num + linear_program.artificial_vars_num ; 
    int k = 0 ; 
    for (size_t i = 0; i < total ; i++)
    {

        if (!isFractionZero(linear_program.feasible_solution[i]))
        {
            arr[k] = i ; 
            k++ ; 
        }
               
    }
    return arr ; 
}



Fraction productFractions(Fraction a , Fraction b) {
    Fraction result ;
    result.x = a.x * b.x ;  
    result.y = a.y * b.y ;
    result.value = a.value * b.value ; 
    result.sign = !(a.sign ^ b.sign) ;   

    // printFraction(result) ; 
    return result ; 
}


float* getZiValuesf(LinearProgram linear_program) {
    float *arr = malloc(sizeof(float) * MAX_NUMBER_VARIABLES) ; 
    // printf("its here ?\n") ; 
    int *feasible_solutions_indices = linear_program.feasible_solution_indices->arr ;
    // printf("am i wrong tho ?? ?\n") ; 
    // int *feasible_solutions_indices = getFeasibleSolutionIndices(linear_program) ;

    int total = linear_program.original_decision_vars_num + linear_program.slack_vars_num + linear_program.surplus_vars_num + linear_program.artificial_vars_num ; 
    float result = 0 ; 
    int k = 0 ;
    for (size_t i = 0; i < total; i++)
    {
        result = 0 ; 
        
        for (size_t j = 0; j < linear_program.constraints->m; j++)
        {
            int temp = feasible_solutions_indices[j] ; 
            result +=  linear_program.objective_function->coefficiants[temp].value * linear_program.constraints->constraints_coeff[j][i].value ;
            
        }
        // printf("  %.2f  " , result) ;         
        arr[k] = result ; 
        ++k ; 
    }
    
    // free(feasible_solutions_indices) ; 
    return arr ; 
}



void printSimplexTable(LinearProgram linear_program ) {
    
    // int *feasible_solutions_indices = getFeasibleSolutionIndices(linear_program) ;
    int *feasible_solutions_indices = linear_program.feasible_solution_indices->arr ;
    int t = linear_program.original_decision_vars_num  +  linear_program.surplus_vars_num + linear_program.slack_vars_num  ; 
    int total = linear_program.original_decision_vars_num + linear_program.slack_vars_num + linear_program.surplus_vars_num + linear_program.artificial_vars_num ; 

    // for (size_t i = 0; i < t; i++)
    // {
        // printf("index  : %d \n"  , feasible_solutions_indices[i]) ; 
    // }
    
    int num_stars = 80 ;

    printPattern('*' , num_stars) ; 
    
    printf("\t\t\t\tSimplex Table\n") ;     
    
    printPattern('*' , num_stars) ; 
    
    // first row 

    printf(" %s " , linear_program.type == MIN ? "Min" : "Max") ; 
    printf(" Ci     ") ; 

    for (size_t i = 0; i < total; i++)
    {
        float value = 0.0 ; 
        value = linear_program.objective_function->coefficiants[i].value ; 
        if (i < linear_program.objective_function->length)
        {
        }
        
        printf("  %.2f  " , value);
    }

    // for (size_t i = 0; i < linear_program.objective_function->length; i++)
    // {
    //     printf(" %.2f " , linear_program.objective_function->coefficiants[i].value);
    // }

    // setting up variables names  
    
    printf("\n");
    printf(" Cb ") ; 
    printf(" B \t") ; 
    printf(" b \t") ; 
    for (size_t i = 0; i < linear_program.constraints->n; i++)
    {
        if (linear_program.constraints->variables_type[i] == NORMAL)
        {
            printf("   x%ld   " , i) ; 
        } else if (linear_program.constraints->variables_type[i] == ARTIFICIAL) {
            printf("   a%ld   " , i ) ; 
            
        } else {
            
            printf("   s%ld   " , i) ; 
        }
        // printf("\t");
        
    }printf("\n");


    // printing constraints coefficents
    int temp_index = 0 ; 

    for (size_t i = 0; i < linear_program.constraints->m; i++)
    {

        int current_index = feasible_solutions_indices[i] ; 
        float val = linear_program.objective_function->coefficiants[current_index].value ; 
        if (linear_program.constraints->variables_type[current_index] == NORMAL)
        {
            // be careful
            printf("%.1f  x%d " , val , current_index) ; 
        } else if (linear_program.constraints->variables_type[current_index] == ARTIFICIAL) {
            printf("%.1f  a%d  " , val, current_index ) ; 
            
        } else {
            
            printf("%.1f  s%d  " , val, current_index) ; 
        }
        
        // printFractionpretty(linear_program.constraints.RHS[i] , i) ; 
        printf(" %.2f " , linear_program.constraints->RHS[i].value) ;

        // int total = linear_program.original_decision_vars_num  +  linear_program.surplus_vars_num + linear_program.slack_vars_num ; 
        for (size_t j = 0; j < total; j++)
        {
            int sign = linear_program.constraints->constraints_coeff[i][j].sign ;
            printf("  %.2f  " ,   linear_program.constraints->constraints_coeff[i][j].value) ; 
            // printFraction(linear_program.constraints->constraints_coeff[i][j]) ;  
        }printf("\n") ; 
        

    }


    // printing Zi for current feasible solution
    printf("    ");
    printf(" Zi   ");
    
    float Zi = 0 ;
    for (size_t i = 0; i < linear_program.constraints->m; i++)
    {
        int temp = feasible_solutions_indices[i] ; 
        // productFractions(linear_program.objective_function->coefficiants[temp] , linear_program.constraints->RHS[i]) ; 
        float result =  linear_program.objective_function->coefficiants[temp].value * linear_program.constraints->RHS[i].value ; 
        Zi += result ; 
    }
    printf(" %.2f  " , Zi);
    
    
    // printing Zi row
    float *Zis = getZiValuesf(linear_program) ; 
    for (size_t i = 0; i < total ; i++)
    {
        printf("  %.2f  " , Zis[i]);
        
    }    
    printf("\n\t\t");

    for (size_t i = 0; i < total ; i++)
    {
        printf("  %.2f  " ,linear_program.objective_function->coefficiants[i].value -  Zis[i]);
        
    }    
    printf("\n");

    printPattern('*' , num_stars) ; 
    printPattern('*' , num_stars) ; 


    free(Zis) ; 
    // free(feasible_solutions_indices) ; 
}

int maxValueInArray(float *arr , int length) {
    int max_index = 0 ; 
    float max = arr[0] ; 
    for (size_t i = 0; i < length; i++)
    {
        if ( max < arr[i])
        {
            max_index = i ; 
            max = arr[i] ;
        }
        
    }

    return max_index ; 
}

int minValueInArray(float *arr , int length) {
    int min_index = 0 ; 
    float min = arr[0] ; 
    for (size_t i = 0; i < length; i++)
    {
        if ( min > arr[i])
        {
            min_index = i ; 
            min = arr[i] ;
        }
        
    }

    return min_index ; 
}

float *getCiMinusZi(LinearProgram linear_program) {
    int total = linear_program.original_decision_vars_num + linear_program.surplus_vars_num + linear_program.slack_vars_num + linear_program.artificial_vars_num;
    float *arr = malloc(sizeof(float) * total)  ; 
    
    float *Zi = getZiValuesf(linear_program) ; 
    for (size_t i = 0; i < total; i++)
    {
        // printf(" %d \n" , i) ;
        arr[i] = linear_program.objective_function->coefficiants[i].value - Zi[i] ;
    }
    
    free(Zi) ; 
    return arr ; 
}

int selectEnteringVariable(LinearProgram linear_program) {
    // max one 

    if (linear_program.type == MAX)
    {
        float *Zi = getZiValuesf(linear_program) ; 
        int total = linear_program.original_decision_vars_num + linear_program.surplus_vars_num + linear_program.slack_vars_num + linear_program.artificial_vars_num;
        float *Ci_minus_Zi = getCiMinusZi(linear_program) ; 

        free(Zi) ; 
        int max_pos =  maxValueInArray(Ci_minus_Zi , total) ; 
        float *ZiMinusCi = getCiMinusZi(linear_program) ; 
        if (ZiMinusCi[max_pos] > 0)
        {
            return max_pos ; 
        } else {
            return -1 ; 
        }
        
    } else if (linear_program.type == MIN) {
        float *Zi = getZiValuesf(linear_program) ; 
        int total = linear_program.original_decision_vars_num + linear_program.surplus_vars_num + linear_program.slack_vars_num + linear_program.artificial_vars_num;
        float *Ci_minus_Zi = getCiMinusZi(linear_program) ; 

        free(Zi) ; 
        int min_pos =  minValueInArray(Ci_minus_Zi , total) ; 
        float *ZiMinusCi = getCiMinusZi(linear_program) ; 
        if (ZiMinusCi[min_pos] < 0)
        {
            return min_pos ; 
        } else {
            return -1 ; 
        }

    }
    
}


int selectPivotRow(LinearProgram linear_program , int pivot_column) {
    // max one 

    // float *Zi = getZiValuesf(linear_program) ; 
    int total = linear_program.original_decision_vars_num + linear_program.surplus_vars_num + linear_program.slack_vars_num + linear_program.artificial_vars_num;
    float Ci_minus_Zi[total]  ;
    int found = 0 ;
    float min ;   
    float temp_array[ linear_program.constraints->m ] ; 
    for (size_t i = 0; i < linear_program.constraints->m; i++)
    {
        // Ci_minus_Zi[i] = linear_program.objective_function->coefficiants[i].value - Zi[i] ;
        if(linear_program.constraints->constraints_coeff[i][pivot_column].value > 0) {
            temp_array[i] = linear_program.constraints->RHS[i].value / linear_program.constraints->constraints_coeff[i][pivot_column].value ; 
        } else {
            temp_array[i] = INFINITY; 

        }
    }

    int min_pos =  minValueInArray(temp_array , linear_program.constraints->m) ; 
    if (temp_array[min_pos] < INFINITY)
    {
        return min_pos ; 
    } else {
        return -1 ; 
    }

    // free(Zi) ; 
    // return maxValueInArray(Ci_minus_Zi , total) ; 
    
    
}


void pivotingTransformRow(Fraction *row  , int length, Fraction pivot) {
    for (size_t i = 0; i < length; i++)
    {
        // printf(" %d \t %d \n" ,row[i].x * pivot.y ,row[i].y * pivot.x );
        createFraction(row + i ,row[i].x * pivot.y ,row[i].y * pivot.x  ) ;

    }
    
}

void pivotingTransformColumn(LinearProgram linear_program, int length ,int  pivot_row , int pivot_column) {
    for (size_t i = 0; i < linear_program.constraints->m ; i++)
    {
        // printf(" %d \t %d \n" ,row[i].x * pivot.y ,row[i].y * pivot.x );
        if (i != pivot_row)
        {
            createFraction(&linear_program.constraints->constraints_coeff[i][pivot_column] ,0 ,1  ) ;
            /* code */
        }
        

    }
    
}

void copyArrayFraction(Fraction arr1 ,Fraction arr2) {

}


void pivotingTransformRhs(LinearProgram linear_program  , int pivot_row ,int pivot_column) {
    // createFraction(rhs[] ,row[i].x * pivot.y ,row[i].y * pivot.x  ) ;
    // createFraction(rhs , rhs->x * pivot.y , rhs->y * pivot.x ) ;
    Constraints constraints = *linear_program.constraints ;
    Fraction rhs[MAX_NUMBER_CONSTRAINTS]  ; 
    
    // printf("original \n") ;
    for (size_t i = 0; i < constraints.m; i++)
    {
        // printFraction(  linear_program.constraints->RHS[i]) ; 
    }
    memcpy(rhs  , linear_program.constraints->RHS  , MAX_NUMBER_CONSTRAINTS * sizeof(Fraction) ) ; 
     
    // printf("copy \n") ; 
    for (size_t i = 0; i < constraints.m; i++)
    {
        // printFraction(  rhs[i]) ; 
    }
     

    // exit(0) ; 
    Fraction pivot = linear_program.constraints->constraints_coeff[pivot_row][pivot_column];
    // printf( "pivot : ") ; 
    printFraction(pivot) ; 
    for (size_t i = 0; i < linear_program.constraints->m; i++)
    {
        // printf("iteration %d " , i) ; 
        // printf("iteration %d \n" , i) ; 
        if (i == pivot_row) {
            // printf("pivot row \n" , i) ; 
            Fraction temp =  fractionDivision(linear_program.constraints->RHS[i] , pivot) ;
            
            
            createFraction(&linear_program.constraints->RHS[i] ,temp.x , temp.y ) ; 
        }
        else {
            // printf("not pivot row \n" , i) ; 
            // Fraction b = linear_program.constraints->constraints_coeff[pivot_row][i] ; 
            Fraction b = rhs[pivot_row] ; 
            Fraction c = constraints.constraints_coeff[i][pivot_column] ; 
            // Fraction pivot = linear_program.constraints->constraints_coeff[pivot_row][pivot_column] ; 
            Fraction value = rhs[i] ; 
            // printf(" b : ");
            // printFraction(b) ; 
            // printf(" c : ");
            // printFraction(c) ; 
            // printf(" pivot : ");
            // printFraction(pivot) ; 
            // printf(" value : ");
            // printFraction(value) ; 
            
            Fraction temp =   fractionProduct(b , c ) ; 
            // printf(" b * c : ");
            // printFraction(temp) ;
            // printFractionpretty(temp , 1) ;
            
            Fraction temp2 =   fractionDivision(temp , pivot ) ; 
            // printf(" b * c / pivot  : ");
            // printFraction(temp2) ;
            // printFractionpretty(temp2 , 1 ) ;
            Fraction temp3 = fractionSubtraction(value , temp2) ; 
            // printf("value -  (b * c / pivot)  : ");
            // printFraction( temp3) ;
            
            createFraction(&linear_program.constraints->RHS[i] ,temp3.x , temp3.y ) ; 
        }

    }
    
    
}


void pivotingTransformRest(LinearProgram linear_program , int pivot_row , int pivot_column) {
    int total = linear_program.original_decision_vars_num + linear_program.slack_vars_num + linear_program.surplus_vars_num + linear_program.artificial_vars_num ; 
    for (size_t i = 0; i < linear_program.constraints->m; i++)
    {
        for (size_t j = 0; j < total; j++)
        {
            if (i == pivot_row || j == pivot_column)
            {
                continue;
            }
            // printf("DEBUG : iteration i=%d j=%d : " , i , j ) ; 
            // float a = a' - b * c / pivot ; 
            Fraction b = linear_program.constraints->constraints_coeff[pivot_row][j] ; 
            Fraction c = linear_program.constraints->constraints_coeff[i][pivot_column] ; 
            Fraction pivot = linear_program.constraints->constraints_coeff[pivot_row][pivot_column] ; 
            Fraction value = linear_program.constraints->constraints_coeff[i][j] ; 
            // printf(" b : ");
            // printFraction(b) ; 
            // printf(" c : ");
            // printFraction(c) ; 
            // printf(" pivot : ");
            // printFraction(pivot) ; 
            // printf(" value : ");
            // printFraction(value) ; 
            
            Fraction temp =   fractionProduct(b , c ) ; 
            // printf(" b * c : ");
            // printFraction(temp) ;
            
            Fraction temp2 =   fractionDivision(temp , pivot ) ; 
            // printf(" b * c / pivot  : ");
            // printFraction(temp2) ;
            linear_program.constraints->constraints_coeff[i][j] = fractionSubtraction(value , temp2) ; 
            // printf("value -  (b * c / pivot)  : ");
            // printFraction(fractionSubtraction(value , temp2)) ;

        
        }
        
    }
    
}



void printSortingVariable(LinearProgram linear_program , int index) {
    // printf("" , linear_program.feasible_solution[index] ) ; 
}


int isDoneIterating(LinearProgram linear_program) {
    float *CiMinusZi = getCiMinusZi(linear_program) ;
    int total = linear_program.original_decision_vars_num + linear_program.slack_vars_num + linear_program.surplus_vars_num + linear_program.artificial_vars_num ; 
    if (linear_program.type == MAX)
    {
        for (size_t i = 0; i < total; i++)
        {
            if (CiMinusZi[i] > 0.0)
                return 0 ; 
        }
        
        
        
    } else {
        for (size_t i = 0; i < total; i++)
        {
            if (CiMinusZi[i] < 0.0)
                return 0 ; 
        }

    }
    return 1 ; 
    
}





int needs2PhaseSimplex_(LinearProgram linear_program) {
    int total = linear_program.original_decision_vars_num + linear_program.surplus_vars_num + linear_program.slack_vars_num + linear_program.artificial_vars_num ; 
    assert(total == linear_program.constraints->n) ; 
    for (size_t i = 0; i < total; i++)
    {
        if (linear_program.constraints->variables_type[i] == ARTIFICIAL)  {
            return 1 ; 
        } 
    }
    
}

int needs2PhaseSimplex(LinearProgram linear_program) {
    return linear_program.artificial_vars_num > 0  ; 
    
}

int needs2PhaseSimplex__(LinearProgram linear_program) {
    return 0  ; 
    
}



int simplexMethod(LinearProgram *linear_program) {

    if (!linear_program)
    {
        printf("ERROR : fatal passed NULL pointer to LinearProgram \n") ; 
        return -1; 
    }
    

    setInitialFeasibleSolution2PhaseSimplex(linear_program) ; 
    linear_program->feasible_solution_indices = getFeasibleSolutionIndicesDynarr__(*linear_program) ; 


    if (needs2PhaseSimplex(*linear_program))
    {
        cprintf(C_RED , "INFO : this linear program has artificial variables therefore standard simplex is inefficient \n") ;
        // cprintf(C_YELLOW , "INFO : calling FirstPhaseSimplexMethod() \n") ;
        // FirstPhaseSimplexMethod(*linear_program) ; 

        return -2;  
    }

    printSimplexTable(*linear_program) ; 
    cprintf(C_RED , "\t\t\t >> STANDARD SIMPLEX METHOD <<\n") ; 
    while (!isDoneIterating(*linear_program)){
        int total = linear_program->original_decision_vars_num + linear_program->surplus_vars_num + linear_program->slack_vars_num +  linear_program->artificial_vars_num ; 
        int *feasible_solutions_indices = linear_program->feasible_solution_indices->arr ;
        // int *feasible_solutions_indices = getFeasibleSolutionIndices(linear_program) ;
        float *Zis = getZiValuesf(*linear_program) ; 



        int pivot_column = selectEnteringVariable(*linear_program) ; 
        if (pivot_column < 0)
        {
            printf("INFO : done Iterating no entering variable to be chosen \n") ;

            return -3;
        }
        
        int pivot_row = selectPivotRow(*linear_program , pivot_column) ; 
        if (pivot_row < 0)
        {
            printf("INFO : done Iterating no sorting variable to be chosen \n") ; 
            cprintf(C_BLUE , "INFO : Unbounded Solution value max = + inf\n") ; 
            return -4;
        }



        printf("pivot :  %d,%d \n" , pivot_row , pivot_column);
        printf("pivoting ... \n") ; 
        
        Fraction pivot = linear_program->constraints->constraints_coeff[pivot_row][pivot_column]  ; 
        linear_program->feasible_solution[pivot_row] ; 
        linear_program->objective_function->coefficiants[pivot_column] ; 
        
        // feasible_solutions_indices[pivot_row] ; 
        printf("index of sorting variable : %d \n" , feasible_solutions_indices[pivot_row]) ;
        printf("index of entering variable : %d \n" , pivot_column) ;
        // for (size_t i = 0; i < total; i++)
        // {
            // printf("%ld : " , i  ) ; 
            // printFraction(linear_program.feasible_solution[i]) ; 
        // }
        
        createFraction(&linear_program->feasible_solution[feasible_solutions_indices[pivot_row]] ,0 , 1 ) ; 
        // printf("after setting sorting variable to 0") ; 
        // for (size_t i = 0; i < total; i++)
        // {
            // printf("%ld : " , i  ) ; 
            // printFraction(linear_program.feasible_solution[i]) ; 
        // }
        
        createFraction(&linear_program->feasible_solution[pivot_column] ,1 , 1 ) ; 

        // printf("after setting entering variable to 1") ; 
        // for (size_t i = 0; i < total; i++)
        // {
            // printf("%ld : " , i  ) ; 
            // printFraction(linear_program.feasible_solution[i]) ; 
        // }
        ; 
        // printf("index of sorting variable : %d \n" , linear_program.feasible_solution_indices->arr[pivot_row]) ;
        // printf("index of entering variable : %d \n" , pivot_column) ;
        linear_program->feasible_solution_indices->arr[pivot_row] = pivot_column ;  

        pivotingTransformRhs( *linear_program,pivot_row , pivot_column ) ; 
        pivotingTransformRest(*linear_program , pivot_row , pivot_column) ; 
        pivotingTransformRow(  (Fraction *) (linear_program->constraints->constraints_coeff + pivot_row) , total , pivot ) ; 
        // createFraction(&linear_program.constraints->RHS[pivot_row] ,linear_program.constraints->RHS[pivot_row].x ,   ) ; 
        // pivotingTransformRhs(linear_program.constraints->RHS  ,pivot , pivot_row , pivot_column ) ;
        // linear_program.constraints->RHS
        pivotingTransformColumn(*linear_program , linear_program->constraints->m , pivot_row , pivot_column) ; 



        printSimplexTable(*linear_program) ; 
        printf("%s\n" , isDoneIterating(*linear_program) == 1 ? "done iterating" : "still need another iteration");

        free(Zis) ; 
    }
    cprintf(C_GREEN , "\t\t\tDone\n") ; 
    // free(feasible_solutions_indices) ; 
    return 1 ; 
}


int simplexMethodPhases(LinearProgram *linear_program) {

    if (!linear_program)
    {
        printf("ERROR : fatal passed NULL pointer to LinearProgram \n") ; 
        return -1; 
    }
    

    cprintf(C_RED , "\t\t\t >> SIMPLEX METHOD <<\n") ; 
    while (!isDoneIterating(*linear_program)){
        int total = linear_program->original_decision_vars_num + linear_program->surplus_vars_num + linear_program->slack_vars_num +  linear_program->artificial_vars_num ; 
        int *feasible_solutions_indices = linear_program->feasible_solution_indices->arr ;
        // int *feasible_solutions_indices = getFeasibleSolutionIndices(linear_program) ;
        float *Zis = getZiValuesf(*linear_program) ; 



        int pivot_column = selectEnteringVariable(*linear_program) ; 
        if (pivot_column < 0)
        {
            printf("INFO : done Iterating no entering variable to be chosen \n") ;

            return -3;
        }
        
        int pivot_row = selectPivotRow(*linear_program , pivot_column) ; 
        if (pivot_row < 0)
        {
            printf("INFO : done Iterating no sorting variable to be chosen \n") ; 
            cprintf(C_BLUE , "INFO : Unbounded Solution value max = + inf\n") ; 
            return -4;
        }



        printf("pivot :  %d,%d \n" , pivot_row , pivot_column);
        printf("pivoting ... \n") ; 
        
        Fraction pivot = linear_program->constraints->constraints_coeff[pivot_row][pivot_column]  ; 
        linear_program->feasible_solution[pivot_row] ; 
        linear_program->objective_function->coefficiants[pivot_column] ; 
        
        // feasible_solutions_indices[pivot_row] ; 
        printf("index of sorting variable : %d \n" , feasible_solutions_indices[pivot_row]) ;
        printf("index of entering variable : %d \n" , pivot_column) ;
        // for (size_t i = 0; i < total; i++)
        // {
            // printf("%ld : " , i  ) ; 
            // printFraction(linear_program.feasible_solution[i]) ; 
        // }
        
        createFraction(&linear_program->feasible_solution[feasible_solutions_indices[pivot_row]] ,0 , 1 ) ; 
        // printf("after setting sorting variable to 0") ; 
        // for (size_t i = 0; i < total; i++)
        // {
            // printf("%ld : " , i  ) ; 
            // printFraction(linear_program.feasible_solution[i]) ; 
        // }
        
        createFraction(&linear_program->feasible_solution[pivot_column] ,1 , 1 ) ; 

        // printf("after setting entering variable to 1") ; 
        // for (size_t i = 0; i < total; i++)
        // {
            // printf("%ld : " , i  ) ; 
            // printFraction(linear_program.feasible_solution[i]) ; 
        // }
        ; 
        // printf("index of sorting variable : %d \n" , linear_program.feasible_solution_indices->arr[pivot_row]) ;
        // printf("index of entering variable : %d \n" , pivot_column) ;
        linear_program->feasible_solution_indices->arr[pivot_row] = pivot_column ;  

        pivotingTransformRhs( *linear_program,pivot_row , pivot_column ) ; 
        pivotingTransformRest(*linear_program , pivot_row , pivot_column) ; 
        pivotingTransformRow(  (Fraction *) (linear_program->constraints->constraints_coeff + pivot_row) , total , pivot ) ; 
        // createFraction(&linear_program.constraints->RHS[pivot_row] ,linear_program.constraints->RHS[pivot_row].x ,   ) ; 
        // pivotingTransformRhs(linear_program.constraints->RHS  ,pivot , pivot_row , pivot_column ) ;
        // linear_program.constraints->RHS
        pivotingTransformColumn(*linear_program , linear_program->constraints->m , pivot_row , pivot_column) ; 



        printSimplexTable(*linear_program) ; 
        printf("%s\n" , isDoneIterating(*linear_program) == 1 ? "done iterating" : "still need another iteration");

        free(Zis) ; 
    }
    cprintf(C_GREEN , "\t\t\tDone\n") ; 
    // free(feasible_solutions_indices) ; 
    return 1 ; 
}




int canGoToPhase2(LinearProgram linear_program) {
    
    int *feasible_solutions_indices = linear_program.feasible_solution_indices->arr ;
    int total = linear_program.constraints->n ; 

    for (size_t i = 0; i < linear_program.feasible_solution_indices->length ; i++)
    {

        int current_index = feasible_solutions_indices[i] ; 
        float val = linear_program.objective_function->coefficiants[current_index].value ; 
         if (linear_program.constraints->variables_type[current_index] == ARTIFICIAL) {
            // printf("%.1f  a%d  " , val, current_index ) ; 
            return 0 ;    
        }
    }
    return 1 ; 

}

void FirstPhaseSimplexMethod(LinearProgram linear_program) {

    // printSimplexTable(linear_program) ; 
    // printObjectiveFunction(*linear_program.objective_function) ;
    // printf("linear_program.objective_function->length)  %d \n" , linear_program.objective_function->length) ; 
    
    // printf("uhh : \n") ; 
    for (size_t i = 0; i < linear_program.objective_function->length; i++)
    {
        // printf("%d/%d\n" , linear_program.objective_function->coefficiants[i].x , 
        // linear_program.objective_function->coefficiants[i].y);
        // printFraction(linear_program.objective_function->coefficiants[i]);
    }
    // printf("done : \n") ; 
    
    LinearProgram linear_program_artificial ; 
    memcpy(&linear_program_artificial , &linear_program , sizeof(linear_program)) ; 
    
    linear_program_artificial.constraints = malloc(sizeof(Constraints)) ; 
    memcpy(linear_program_artificial.constraints , (linear_program.constraints) , sizeof(Constraints)) ; 
    // printf("rami\n");


    linear_program.feasible_solution_indices = malloc(sizeof(DynammicArray)) ;
    // printf("brahimi\n");
    
    memcpy(&linear_program_artificial.feasible_solution_indices->arr , &linear_program.feasible_solution_indices->arr , sizeof(int)*linear_program.feasible_solution_indices->length) ;  
    // linear_program_artificial.feasible_solution_indices->length = 0 ;
    // printf("chemsou\n");

    linear_program_artificial.objective_function = malloc(sizeof(ObjectiveFunction)) ; 
    // printf("linear_program.objective_function->length)  %d \n" , linear_program.objective_function->length) ; 
    memcpy(linear_program_artificial.objective_function , linear_program.objective_function , sizeof(ObjectiveFunction)) ; 
    // printf("linear_program.objective_function->length)  %d \n" , linear_program.objective_function->length) ; 




    linear_program_artificial.type = MIN ; 
    
    int total = linear_program_artificial.original_decision_vars_num + linear_program_artificial.slack_vars_num + linear_program_artificial.surplus_vars_num + linear_program_artificial.artificial_vars_num ; 
    int index = total - linear_program_artificial.artificial_vars_num ; 
    
    for (size_t i = 0; i < linear_program_artificial.objective_function->length; i++)
    {
        // printf("%ld\n" , i) ;
        // printFraction(linear_program_artificial.objective_function->coefficiants[i]);
        createFraction(&linear_program_artificial.objective_function->coefficiants[i] , 0 , 1) ;  
        // printFraction(linear_program_artificial.objective_function->coefficiants[i]);
    }
    
    for (size_t i = index; i < total; i++)
    {
        // printf("%ld\n" , i) ;
        // printFraction(linear_program_artificial.objective_function->coefficiants[i]);
        createFraction(&linear_program_artificial.objective_function->coefficiants[i] , 1 , 1) ;  
        // printFraction(linear_program_artificial.objective_function->coefficiants[i]);
    }

    setInitialFeasibleSolution2PhaseSimplex(&linear_program_artificial) ; 
    // exit(0) ; 
    for (size_t i = 0; i < linear_program_artificial.constraints->n; i++)
    {
        // printf(" %.2f " , linear_program_artificial.feasible_solution[i].value) ; 
        
    }
    printf("\n");
    
    
    linear_program_artificial.feasible_solution_indices = getFeasibleSolutionIndicesDynarr__(linear_program_artificial) ; 
    // printf(" best feasible solution indecise " ) ; 
    for (size_t i = 0; i < linear_program_artificial.feasible_solution_indices->length; i++)
    {
        // printf(" %d " , linear_program_artificial.feasible_solution_indices->arr[i]) ; 
    }
    // printf(" \n") ;         
    // linear_program_artificial.objective_function->coefficiants ; 

    printSimplexTable(linear_program_artificial) ; 

    printPattern('-' , 80) ;
    cprintf(C_YELLOW , "\t\t\tstart PHASE 1\n"); 
    printPattern('-' , 80) ;
    simplexMethodPhases(&linear_program_artificial) ; 
    // printSimplexTable(linear_program_artificial) ; 

    // return;
    printPattern('-' , 80) ;

    
    cprintf(C_YELLOW , "\t\t\tdone PHASE 1\n"); 
    printPattern('-' , 80) ; 
    linear_program_artificial.type = linear_program.type ; 

    for (size_t i = index; i < total; i++)
    {
        // printf("%ld\n" , i) ;
        // printFraction(linear_program_artificial.objective_function->coefficiants[i]);
        createFraction(&linear_program_artificial.objective_function->coefficiants[i] , 0 , 1) ;  
        // printFraction(linear_program_artificial.objective_function->coefficiants[i]);
    }
    // printf("loop 2 \n");

    // printf("linear_program.objective_function->length)  %d \n" , linear_program.objective_function->length) ; 

    for (size_t i = 0; i < linear_program.objective_function->length; i++)
    {
        // printf("%ld\n" , i) ;
        // printf("origin : ") ; 
        // printFraction(linear_program_artificial.objective_function->coefficiants[i]);
        // printf("before : ") ; 
        // printFraction(linear_program.objective_function->coefficiants[i]);
        createFraction(&linear_program_artificial.objective_function->coefficiants[i] , linear_program.objective_function->coefficiants[i].x , linear_program.objective_function->coefficiants[i].y) ;  
        // printf("after : ") ; 
        // printFraction(linear_program.objective_function->coefficiants[i]);
        // printFraction(linear_program_artificial.objective_function->coefficiants[i]);
    }

    // printf("uhh : \n") ; 
    for (size_t i = 0; i < linear_program.objective_function->length; i++)
    {
        // printf("%d/%d\n" , linear_program.objective_function->coefficiants[i].x , 
        // linear_program.objective_function->coefficiants[i].y);
        // printFraction(linear_program.objective_function->coefficiants[i]);
    }
    // printf("done : \n") ; 
    
    linear_program_artificial.constraints->n -= linear_program_artificial.artificial_vars_num ; 
    linear_program_artificial.artificial_vars_num  = 0 ; 

    printSimplexTable(linear_program_artificial) ; 
    
    int phase2_simplex = canGoToPhase2(linear_program_artificial) ;
    if (phase2_simplex < 1)
    {
        cprintf(C_RED , "impossible solution cant go to phase 2 \n") ; 
        return ; 
    }
    

    // phase 2

    printPattern('-' , 80) ; 
    cprintf(C_YELLOW , "\t\t\tstart PHASE 2\n"); 
    printPattern('-' , 80) ; 

    simplexMethodPhases(&linear_program_artificial) ;
    
    printPattern('-' , 80) ; 
    cprintf(C_YELLOW , "\t\t\tdone PHASE 2\n"); 
    printPattern('-' , 80) ; 
    
    cprintf(C_GREEN , "result : \n"); 
    printSimplexTable(linear_program_artificial) ; 

    for (size_t i = 0; i < linear_program_artificial.feasible_solution_indices->length; i++)
    {
        // printFraction(linear_program_artificial.feasible_solution[i]) ; 
        printFraction(linear_program_artificial.constraints->RHS[i]) ; 
        
    }
    


}

void TwoPhaseSimplexMethod(LinearProgram *linear_program) {
    FirstPhaseSimplexMethod(*linear_program) ; 

}

void prepareFractionCoeff(Fraction coeff[MAX_NUMBER_CONSTRAINTS][MAX_NUMBER_VARIABLES]) {
    for (size_t i = 0; i < MAX_NUMBER_CONSTRAINTS; i++)
    {
        for (size_t j = 0; j < MAX_NUMBER_VARIABLES; j++)
        {
            createFraction(&coeff[i][j] , 0 , 1) ; 
        }
        
    }
    
}

void printVariableTypes(enum VariableType *arr , int length) {


    // printing variables type 
    for (size_t i = 0; i < length; i++)
    {
         ; 
         switch (arr[i])
         {
             case ARTIFICIAL:
             printf(" artificial \n"  ) ; 
             /* code */
             break;
             case SLACK : 
             printf(" slack \n"  ) ; 
             break;
             case SURPLUS : 
             printf(" surplus \n"  ) ; 
             break;
             case NORMAL : 
             printf(" normal \n"  ) ; 
             break;
             
             default:
             printf(" huh??? \n"  ) ; 
            break;
        }
    }

}
void printConstraints(Constraints constraints) {
    for (size_t i = 0; i < constraints.m; i++)
    {
        
        for (size_t j = 0; j < constraints.n; j++)
        {
            float numerator ;
            float denomirator ;
            float integer_part ;
            float float_part ;

            float_part = modff(constraints.constraints_coeff[i][j].value  , &integer_part); 
            
            char var_type ;
            switch (constraints.variables_type[j])
            {
            case NORMAL:
                var_type = 'x' ; 
                break;
            case SLACK:
                var_type = 's' ; 
                break;
            case SURPLUS:
                var_type = 's' ; 
                break;
            case ARTIFICIAL:
                var_type = 'a' ; 
                break;
            
            default:
                break;
            }
            printf(" %.2f %c%ld " ,constraints.constraints_coeff[i][j].value , var_type , j+1 ) ; 
            // if (float_part > 0)
            // {

                // printf("%s %d/%d*%ld "  ,constraints.constraints_coeff[i][j].x ,constraints.constraints_coeff[i][j].y  , var_type , j+1  ) ; 
            // } else {
                // printf("%s %d %ld " , constraints.constraints_coeff[i][j].x   , var_type, j+1 ) ; 
            // }
            
        }
        if (constraints.constraints_sign[i] == EQUAL)
        {
            printf(" = ");
        } else if (constraints.constraints_sign[i] == LOWER_OR_EQUAL) {
            printf(" <= ");
            
        } else if (constraints.constraints_sign[i] == HIGHER_OR_EQUAL) {
            printf(" >= ");

        }
        
        float numerator ;
        float denomirator ;
        float integer_part ;
        float float_part ;

        float_part = modff(constraints.RHS[i].value  , &integer_part); 
            
        if (float_part > 0)
        {
            printf(" %d/%d "  ,constraints.RHS[i].x ,constraints.RHS[i].y  ) ; 
        } else {
            printf(" %d " , constraints.RHS[i].x ) ; 
        }
            printf("\n") ; 
    }
    
}


void printPattern(char symbol , int times) {
    for (size_t i = 0; i < times; i++)
    {
        printf("%c" , symbol ) ; 
    }printf("\n") ; 
    
}
