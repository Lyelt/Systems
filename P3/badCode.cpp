//
//  badCode.cpp
//  Part of sysprog Project 3.
//  Created by Alice Fischer on 9/10/17.

#include <iostream>
#include <cmath>
using namespace std;

#define print( p ) (cout << p <<endl)

int main(int argc, const char * argv[]) {
    double dbl = sin(15);
    long int lint = -1;
    short int k, m, n;

    k = lint + 1;
    m = (10/3 -1) * (10/3 + 1);
    print( dbl );
    print( lint );
    cout <<"k=" <<k <<"  m=" <<m <<endl;
    sin(20) + sin(15);
}
