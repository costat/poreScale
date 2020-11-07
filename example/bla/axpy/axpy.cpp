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
    A[i] = distribution(random_engine);
    B[i] = distribution(random_engine);
    C[i] = distribution(random_engine);
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
  //--- porescale::axpy ---//
  for (int i = 0; i < 100; i++) {
    auto rebegin = std::chrono::high_resolution_clock::now();

    // porescale::axpy


    double ps_axpy_time = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - rebegin).count();
    double ps_axpy_min_time;
    if (i == 0) ps_axpy_min_time = ps_axpy_time;
    else if (ps_axpy_min_time > ps_axpy_time) ps_axpy_min_time = ps_axpy_time;
  }
  //--- cublas ---//

  //--- reference ---//

  // print timings
  std::cout << "\n\n//------------------ Finished! Performance reports ------------------//\n";

}
