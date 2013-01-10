#include <stdlib.h>

/**=====================================================================
 * C functions to be tested
 *====================================================================*/

/** Calculates the sum of two integers */
int calc_sum(int x, int y){
    return(x+y);
}

/** Calculates the difference of the two integers */
int calc_diff(int x, int y){
    return(x-y);
}

/** Calculates the product of two integers */
int calc_product(int x, int y){
    return(x*y);
}

/** Calculates the quotient of two integers. Returns 0 if the operation
 *  is successful, -1 otherwise.
 */
int calc_quotient(int x, int y, int *quotient){
    if (y == 0 ){
        *quotient=0;
        return(-1);
    }
    *quotient = (int)x/y;
    return(0);
}

/** Returns the minimum of two integers */
int get_min(int x, int y){
    return (( x < y)? x:y);
}

/** Returns the maximum of two integers */
int get_max(int x, int y){
    return ((x>y)?x:y);
}

