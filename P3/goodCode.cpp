// goodCode.cpp
// Nicholas Ghobrial
// P3 - Systems

#include <iostream>
#include <cmath>
using namespace std;

#define print( p ) (cout << p <<endl)

int main(int argc, const char * argv[]) {
    double dbl = sin(15);
    long int lint = -1;
	// Removed unused variable n
    short int k, m; //, n;

    k = lint + 1;
    m = (10/3 -1) * (10/3 + 1);
    print( dbl );
    print( lint );
    cout <<"k=" <<k <<"  m=" <<m <<endl;
	// Removed value that is calculated but not used
    //sin(20) + sin(15);
}
