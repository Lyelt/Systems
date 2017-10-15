//
//  badCode.c
//  Part of sysprog Project 3.
//  Created by Alice Fischer on 9/10/17.

#include <stdio.h>
#define print( p ) printf("p= %g\n", p)

int main(int argc, const char * argv[]) {
    double dbl = sin(15);
    long int lint = -1;
    short int k, m, n;

    k = lint + 1;
    m = (10/3 -1) * (10/3 + 1);
    print( dbl );
    print( lint );
    printf( "k=%u  m=%li\n", dbl, lint, k, m );
    sin(20);
}
