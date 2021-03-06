 /***************************************************************************
 *                                                                          *
 *            @autor Rafael Nogales - Universidad de Granada (UGR)          *
 *                                                                          *
 *            pi_err.cpp      HPC                                           *
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
 *  Calculate the minimum value of N that is required to give a value for   *
 *  pi(N) that is accurate to some specified value.                         *
 *  We will call this value Nmin.                                           *
 *  By computing pi(N) for increasing values of N,                          *
 *  calculate Nmin such that err(Nmin) < 10−6                               *
 *                                                                          *
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

// Function for aproach PI
inline double pi_calc(int N){
    double sum=0;
#pragma omp parallel for reduction(+:sum)
    for (int i=1; i<N+1; i++){
        sum+=1.0/ (1.0 + pow((i-0.5)/N ,2));
    }
    
    return (4.0/N) * sum;
}

long long int Nmin(double tol){
    long int last_iter = 100 + floor(1/tol);
    bool accurate=false; //  |PI - pi_aproach| < tol
    for(long long int N=1; N<last_iter and accurate==false; N++){
        if (abs(M_PI - pi_calc(N))<tol ){
            return N;
        }
    }
    return -1; //There was a precision error.
}


int main(int argc, char* argv[]){
    long long int rounds = 0;
    double TOL=0;
    for (int i=1; i<13; i++) {
        TOL=pow(10,-i);
        rounds = Nmin(TOL);
        cout << "Se necesitan " << rounds << " para TOL " << TOL << endl;
    }
    
    
    
    
    return 0;
}
