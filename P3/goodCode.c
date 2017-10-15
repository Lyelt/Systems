// goodCode.c
// Nicholas Ghobrial
// P3 - Systems

#include <stdio.h>
// Included math.h to avoid implicit declaration of function sin
#include <math.h>
#define print( p ) printf("p= %g\n", p)
// printf using %li for long int
#define printl( p ) printf("p= %li\n", p)

int main(int argc, const char * argv[]) {
    double dbl = sin(15);
    long int lint = -1;
	// Removed unused variable
    short int k, m; //, n;

    k = lint + 1;
    m = (10/3 -1) * (10/3 + 1);
    print( dbl );
	// Using our newly defined printl for our long int
    printl( lint );
	// Removed incorrect arguments, and used correct formatter for short int
    printf( "k=%hi  m=%hi\n", k, m );
	// Removed value calculated but never used
    //sin(20);
}
