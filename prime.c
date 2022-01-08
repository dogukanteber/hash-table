#include <stdio.h>
#include <stdlib.h>

#include "prime.h"

int is_prime(int number) {
    if ( number < 2 ) return 0;
    if ( number == 2 ) return 1;
    for ( int i=2; i*i<=number; ++i ) {
        if ( number % i == 0 )
            return 0;
    }
    return 1;
}


int next_prime(int number) {
    while ( !is_prime(number) ) {
        number++;
    }

    return number;
}