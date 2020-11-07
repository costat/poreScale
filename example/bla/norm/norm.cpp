/* Example tests porescale::norm for functional correctness and performance..
   Build with included CMakeLists.txt, and use:
      norm N
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
naiveAxpy(std::vector<float_type>& A, float_type& result)
{
  result = 0.0;
  for (int i = 0; i < (int)A.size(); i++)
    result += A[i]*A[i];
  result = std::sqrt(result);
  return;
}

int
main( int argc, const char* argv[] )
{

  psInt N = atoi(argv[1]);

  //--- create random arrays ---//
  std::random_device random_device;
  std::mt19937 random_engine(random_device());
  std::uniform_int_distribution<int> distribution(0,100);

  std::vector<float_type> A;
  A.resize(N);

  for (int i = 0; i < N; i++) {
    A[i] = distribution(random_engine) % 100;
  }

  float_type result = 0.0;
  float_type result2 = 0.0;

  //--- functional test ---//
  porescale::norm(A, result);
  naiveAxpy(A, result2);

  float_type normError = abs(result2 - result);
  if (normError > errTol) std::cout << "Error exceeds tolerance. Expected " << result2 << " but got " << result << ".\n";

  //--- performance test ---//
  double ps_norm_min_time, ps_norm_time, ps_average_time;
  double naive_norm_min_time, naive_norm_time, naive_average_time;
  ps_average_time = 0.0;
  naive_average_time = 0.0;

  // Warmup
  porescale::norm(A, result);
  for (int i = 0; i < NITERS; i++) {
    auto rebegin = std::chrono::high_resolution_clock::now();

    // porescale::norm
    porescale::norm(A, result);
    ps_norm_time = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - rebegin).count();
    ps_average_time += ps_norm_time;
    if (i == 0) ps_norm_min_time = ps_norm_time;
    else if (ps_norm_min_time > ps_norm_time) ps_norm_min_time = ps_norm_time;
  }
  ps_average_time /= NITERS; 

  // Warmup
  naiveAxpy(A, result);
  for (int i = 0; i < NITERS; i++) {
    auto rebegin = std::chrono::high_resolution_clock::now();

    // naive norm
    naiveAxpy(A, result);
    naive_norm_time = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - rebegin).count();
    naive_average_time += naive_norm_time;
    if (i == 0) naive_norm_min_time = naive_norm_time;
    else if (naive_norm_min_time > naive_norm_time) naive_norm_min_time = naive_norm_time;
  }
  naive_average_time /= NITERS;

  // print timings
  std::cout << "\n\n//------------------ Finished! Performance reports ------------------//\n";
  std::cout << "porescale::norm min time = " << ps_norm_min_time << " average time = " << ps_average_time << ".\n";
  std::cout << "naive norm min time = " << naive_norm_min_time << " average time = " << naive_average_time << ".\n";
}
