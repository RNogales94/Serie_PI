 /***************************************************************************
 *                                                                          *
 *            @autor Rafael Nogales - Universidad de Granada (UGR)          *
 *                                                                          *
 *            pi_serie.cpp      HPC                                         *
 *                                                                          *
 *  Calculate pi using:                                                     *
 *                                                                          *
 *                      N                                                   *
 *                    -----                                                 *
 *                 4  \                    1                                *
 *        pi(N) = ---  \   _________________________________                *
 *                 N   /            /    i - 1/2     \ 2                    *
 *                    /        1 + (  -------------   )                     *
 *                    -----         \        N       /                      *
 *                     i=1                                                  *
 *                                                                          *
 *                                                                          *   
 ****************************************************************************/

//See other ways to calculate Pi:
//http://dgiim.github.io/blog/2015/03/14/aproximacion-a-pi/
//UGR

//Compile with `` g++ pi_serie.cpp -o pi_serie -O3 ``

#include <iostream>
#include <cmath>
#include <limits>
#include <ctype.h>

#ifdef _OPENMP
  #include <omp.h>
#else
  #define omp_get_thread_num() 0
#endif

using namespace std;
typedef std::numeric_limits< double > dbl;

const long long int last_iter=60000000;


// Function for aproach PI
inline double pi_calc(int N){
    double sum=0;
    #pragma omp parallel for reduction(+:sum)
    for (int i=1; i<N+1; i++){
        sum+=1.0/ (1.0 + pow((i-0.5)/N ,2));
    }
        
    return (4.0/N) * sum;
}


int main(int argc, char* argv[]){
    
    double aproach_pi;
    
    cout << "First terms\n";
    for (int i=1; i<=20; i++) {
        cout << "Term " << i << ":    "<< pi_calc(i) << endl;
    }
    cout.precision(dbl::digits10);
    aproach_pi = pi_calc(last_iter);
    cout << "Term "<< last_iter << " pi ~ "<<
    aproach_pi << endl;
        
    return 0;
}
