/* Example tests porescale::axpy for functional correctness and performance..
   Build with included CMakeLists.txt, and use:
      axpy N
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
naiveAxpy(float_type alpha, std::vector<float_type>& A, std::vector<float_type>& B)
{
  for (int i = 0; i < (int)A.size(); i++) {
    B[i] += alpha * A[i];
  }
}

int
main( int argc, const char* argv[] )
{

  psInt N = atoi(argv[1]);

  //--- create random arrays ---//
  std::random_device random_device;
  std::mt19937 random_engine(random_device());
  std::uniform_int_distribution<int> distribution(0,100);

  std::vector<float_type> A, B, C;
  A.resize(N);
  B.resize(N);
  C.resize(N);

  for (int i = 0; i < N; i++) {
    A[i] = distribution(random_engine) % 100;
    B[i] = distribution(random_engine) % 100;
    C[i] = B[i];
  }

  float_type alpha = 1.0;

  //--- functional test ---//
  porescale::axpy(alpha, A, B);
  naiveAxpy(alpha, A, C);

  float_type axpyError = 0.0;
  for (int i = 0; i < N; i++) {
    axpyError = abs(B[i] - C[i]);
    if (axpyError > errTol) std::cout << "Error exceeds tolerance on element " << i << ". Expected " << C[i] << " but got " << B[i] << ".\n";
  }

  //--- performance test ---//
  double ps_axpy_min_time, ps_axpy_time, ps_average_time;
  double naive_axpy_min_time, naive_axpy_time, naive_average_time;
  ps_average_time = 0.0;
  naive_average_time = 0.0;
  // Warmup
  porescale::axpy(alpha, A, B);
  for (int i = 0; i < NITERS; i++) {
    auto rebegin = std::chrono::high_resolution_clock::now();

    // porescale::axpy
    porescale::axpy(alpha, A, B);
    ps_axpy_time = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - rebegin).count();
    ps_average_time += ps_axpy_time;
    if (i == 0) ps_axpy_min_time = ps_axpy_time;
    else if (ps_axpy_min_time > ps_axpy_time) ps_axpy_min_time = ps_axpy_time;
  }
  ps_average_time /= NITERS; 

  // Warmup
  naiveAxpy(alpha, A, B);
  for (int i = 0; i < NITERS; i++) {
    auto rebegin = std::chrono::high_resolution_clock::now();

    // naive axpy
    naiveAxpy(alpha, A, B);
    naive_axpy_time = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - rebegin).count();
    naive_average_time += naive_axpy_time;
    if (i == 0) naive_axpy_min_time = naive_axpy_time;
    else if (naive_axpy_min_time > naive_axpy_time) naive_axpy_min_time = naive_axpy_time;
  }
  naive_average_time /= NITERS;

  // print timings
  std::cout << "\n\n//------------------ Finished! Performance reports ------------------//\n";
  std::cout << "porescale::axpy min time = " << ps_axpy_min_time << " average time = " << ps_average_time << ".\n";
  std::cout << "naive axpy min time = " << naive_axpy_min_time << " average time = " << naive_average_time << ".\n";
}
