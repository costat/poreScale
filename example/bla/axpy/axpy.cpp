/* Example tests porescale::axpy for functional correctness and performance..
   Build with included CMakeLists.txt, and use:
      axpy N
   where N is the size of the arrays.
*/

#include "../util/printDeviceProp.hpp"
#include <vector>
#include <random>
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <ctime>
#include "cuda.h"

#include "porescale.hpp"

#ifdef __TEST_DOUBLE__
  #define float_type double
#else
  #define float_type float
#endif

int
main( int argc, const char* argv[] )
{


  cudaSetDevice(0);

  int devCount;
  cudaGetDeviceCount(&devCount);

  std::cout << "There are " << devCount << " CUDA devices.\n";

  for (int i = 0; i < devCount; ++i) {
    std::cout << "------------ Device " << i << " ------------\n";
    cudaDeviceProp devProp;
    cudaGetDeviceProperties(&devProp, i);
    printDevProp(devProp);
    std::cout << "\n";
  }

  psInt N = atoi(problemPath(argv[1]));

  //--- create random arrays ---//

  //--- functional test ---//


  //--- performance test ---//
  //--- porescale::axpy ---//
  for (int i = 0; i < 100; i++) {
    auto rebegin = std::chrono::high_resolution_clock::now();

    // porescale::axpy


    double ps_axpy_time = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - rebegin).count();
    if (i == 0) ps_axpy_min_time = ps_axpy_time;
    else if (ps_axpy_min_time > ps_axpy_time) ps_axpy_min_time = ps_axpy_time;
  }
  //--- cublas ---//

  //--- reference ---//

  // print timings
  std::cout << "\n\n//------------------ Finished! Performance reports ------------------//\n";

}
