/* Example loads parameters and voxel geometry and prints diagnostic info to console.
   The mesh is saved for visualization. Build with included
   CMakeLists.txt, and use:
      parameters <path/to/problemfolder>
   Some example problem folders are included at examples/geometries.
*/

#include "../util/printDeviceProp.hpp"
#include <vector>
#include <iostream>
#include <stdlib.h>
#include "cuda.h"
#include "nvshmem.h"
#include "nvshmemx.h"

#include "porescale.hpp"

#define CONTROL_PE 0

int
main( int argc, const char* argv[] )
{

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

  // Init MPI
  nvshmem_init();

  int myPe = nvshmem_my_pe();
  int nPes = nvshmem_n_pes();

  cudaSetDevice(myPe);

  //-- timers --//
//  double begin=0, rebegin=0, para_time=0, mesh_time=0, build_time=0, solve_time=0, postp_time=0, total_time=0;

//  begin = MPI_Wtime();
  //--- problem parameters ---//
  std::string problemPath(argv[1]);
  porescale::parameters<float> par( problemPath );

//  para_time = MPI_Wtime() - begin;
//  rebegin = MPI_Wtime();

  //--- mesh ---//
  // check mesh sanity and remove dead pores

  // build the mesh

//  mesh_time = MPI_Wtime() - rebegin;
//  rebegin = MPI_Wtime();
  // print parameters
  if (myPe == CONTROL_PE) par.printParameters();
  nvshmem_barrier_all();

  // save the mesh for visualization with paraview

//  postp_time = MPI_Wtime() - rebegin;
//  total_time = MPI_Wtime() - begin;

  // print timings
/*  if (myPe == CONTROL_PE) {
    std::cout << "\n\n//------------------ Finished! Time reports ------------------//\n";
    std::cout << "Parameter setup time: " << para_time << "\n";
    std::cout << "Mesh time: " << mesh_time << "\n";
    std::cout << "Post processessing time: " << postp_time << "\n";
    std::cout << "Total time: " << total_time << "\n";
  }*/

  nvshmem_finalize();

}
