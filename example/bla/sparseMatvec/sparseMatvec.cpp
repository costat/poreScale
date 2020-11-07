/* Example tests porescale::sparseMatvec for functional correctness and performance..
   Build with included CMakeLists.txt, and use:
      sparseMatvec N
   where N is the size of the arrays.
*/

#include <vector>
#include <random>
#include <algorithm>
#include <iterator>
#include <functional>
#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <ctime>

#include "porescale.hpp"

#ifdef __TEST_DOUBLE__
  #define float_type double
#else
  #define float_type float
#endif

#define errTol 1e-8
#define NITERS 100

void
naiveSparseMatvec(float_type alpha, porescale::sparseMatrix<float_type>& A, std::vector<float_type>& B, float_type beta, std::vector<float_type>& C)
{

}

int
main( int argc, const char* argv[] )
{

  //--- Input matrix ---//
  std::string matrixPath(argv[1]);
  porescale::sparseMatrix<float_type> A; 
  A.readMTX(matrixPath);

  //--- create random vectors ---//
  psInt N = A.rows();
  std::random_device random_device;
  std::mt19937 random_engine(random_device());
  std::uniform_int_distribution<int> distribution(0,100);

  std::vector<float_type> X, Y, Z;
  X.resize(N);
  Y.resize(N);
  Z.resize(N);

  for (int i = 0; i < N; i++) {
    X[i] = distribution(random_engine) % 100;
    Y[i] = distribution(random_engine) % 100;
    Z[i] = Y[i];
  }

  float_type alpha = 1.0;
  float_type beta = 0.0;

  //--- functional test ---//
  porescale::sparseMatvec(alpha, A, X, beta, Y);
  naiveSparseMatvec(alpha, A, X, beta, Z);

  float_type sparseMatvecError = 0.0;
  for (int i = 0; i < N; i++) {
    sparseMatvecError = abs(Y[i] - Z[i]);
    if (sparseMatvecError > errTol) std::cout << "Error exceeds tolerance on element " << i << ". Expected " << Z[i] << " but got " << Y[i] << ".\n";
  }

  //--- performance test ---//
  double ps_sparseMatvec_min_time, ps_sparseMatvec_time, ps_average_time;
  double naive_sparseMatvec_min_time, naive_sparseMatvec_time, naive_average_time;
  ps_average_time = 0.0;
  naive_average_time = 0.0;
  // Warmup
  porescale::sparseMatvec(alpha, A, X, beta, Y);
  for (int i = 0; i < NITERS; i++) {
    auto rebegin = std::chrono::high_resolution_clock::now();

    // porescale::sparseMatvec
    porescale::sparseMatvec(alpha, A, X, beta, Y);
    ps_sparseMatvec_time = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - rebegin).count();
    ps_average_time += ps_sparseMatvec_time;
    if (i == 0) ps_sparseMatvec_min_time = ps_sparseMatvec_time;
    else if (ps_sparseMatvec_min_time > ps_sparseMatvec_time) ps_sparseMatvec_min_time = ps_sparseMatvec_time;
  }
  ps_average_time /= NITERS; 

  // Warmup
  naiveSparseMatvec(alpha, A, X, beta, Y);
  for (int i = 0; i < NITERS; i++) {
    auto rebegin = std::chrono::high_resolution_clock::now();

    // naive sparseMatvec
    naiveSparseMatvec(alpha, A, X, beta, Y);
    naive_sparseMatvec_time = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - rebegin).count();
    naive_average_time += naive_sparseMatvec_time;
    if (i == 0) naive_sparseMatvec_min_time = naive_sparseMatvec_time;
    else if (naive_sparseMatvec_min_time > naive_sparseMatvec_time) naive_sparseMatvec_min_time = naive_sparseMatvec_time;
  }
  naive_average_time /= NITERS;

  // print timings
  std::cout << "\n\n//------------------ Finished! Performance reports ------------------//\n";
  std::cout << "porescale::sparseMatvec min time = " << ps_sparseMatvec_min_time << " average time = " << ps_average_time << ".\n";
  std::cout << "naive sparseMatvec min time = " << naive_sparseMatvec_min_time << " average time = " << naive_average_time << ".\n";
}
