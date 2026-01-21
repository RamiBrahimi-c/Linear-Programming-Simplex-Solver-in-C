#include "../include/fraction.h"

#define true 1
#define false 0

void createFraction(Fraction *fraction , int numerator , int denominator ) {
    assert(denominator != 0) ; 

    fraction->x =    abs(numerator) ; 
    fraction->y = abs(denominator) ; 

    fraction->value = ((float) numerator / denominator) ; 
    // fraction->sign = sign ;
    fraction->sign = (numerator * denominator >= 0) ? 1 : -1 ;
    
    fraction->x *= fraction->sign ; 

    int pgcd_result = pgcd(abs(fraction->x) , abs(fraction->y)) ;
    if ( pgcd_result  > 1)
    {
        
        fraction->x /= pgcd_result ; 
        fraction->y /= pgcd_result ; 
    }
    


    // convention is x has its value and the sign of the fraction while y is always positive 
    // the sign is stored in Fraction.sign 
    // and its NOT embedded in Fraction.value     
}

int isFractionPositive(Fraction fraction) {
    return fraction.sign == 1 ? true : false ; 
}

int isFractionZeroVal(Fraction fraction) {
    return fraction.value == 0 ? true : false ; 
}

int isFractionZero(Fraction fraction) {
    return fraction.x == 0 ? true : false ; 
}


void makeFractionPositive(Fraction *fraction) {

    fraction->sign = 1 ; 

    fraction->x = abs(fraction->x) ; 
    fraction->y = abs(fraction->y) ;
    

    fraction->value = fabsf(fraction->value) ; 
    
}

void makeFractionNegative(Fraction *fraction) {

    fraction->sign = -1 ; 

    fraction->x = abs(fraction->x) ; 
    fraction->y = abs(fraction->y) ;
    
    fraction->value = fabsf(fraction->value) ; 

}


Fraction fractionProduct(Fraction a , Fraction b) {
    Fraction f ; 
    createFraction( &f , a.x * b.x , a.y * b.y );
    return f;
}

Fraction fractionDivision(Fraction a , Fraction b ) {
    Fraction f ; 
    createFraction( &f , a.x * b.y , a.y * b.x  );
    return f;
}

Fraction fractionSubtraction(Fraction a , Fraction b) {
    Fraction f ; 
    createFraction( &f , (a.x * b.y)-(a.y * b.x) , a.y * b.y );
    return f;
}

void mapVector(Fraction *arr , int length , Fraction value) {
    for (size_t i = 0; i < length; i++)
    {
        // printf("before : %d \n" , arr[i].sign);
        // arr[i].sign *= sign ;         
        Fraction temp = fractionProduct(arr[i] , value) ; 
        createFraction(arr + i ,temp.x , temp.y ) ; 
        // printf("after : %d \n" , arr[i].sign);
    }
    
}


void setFractionArrayToValue(Fraction arr[] , int length , int value) {
    for (size_t i = 0; i < length; i++)
    {
        createFraction(arr + i , value , 1) ; 
    }
    
}




void printFraction(Fraction fraction) {
    printf("Fraction = { %c %d/%d = %f } \n" ,fraction.sign == 1 ? '+' : '-'  , fraction.x , fraction.y ,  fraction.value   ) ; 
}



void printFractionpretty(Fraction fraction , int i) {
    float numerator ;
    float denomirator ;
    float integer_part ;
    float float_part ;

    float_part = modff(fraction.value  , &integer_part); 
            
    if (float_part > 0)
    {
        printf("%s %d/%d*x%d " , fraction.sign == 1 ? "+" : "-" ,fraction.x ,fraction.y    , i ) ; 
    } else {
        printf("%s %d*x%d " , fraction.sign == 1 ? "+" : "-", fraction.x    , i ) ; 
    }    
}




int pgcd(int a, int b) {
    int r; // Remainder variable

    while (b != 0) {
        r = a % b; // Calculate the remainder
        a = b;     // Replace 'a' with 'b'
        b = r;     // Replace 'b' with the remainder
    }
    return a; // 'a' is the PGCD when 'b' becomes 0
}

