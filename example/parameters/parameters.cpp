/* Example loads parameters and voxel geometry and prints diagnostic info to console. 
   The mesh is saved for visualization. Build with included
   CMakeLists.txt, and use:
      parameters <path/to/problemfolder>
   Some example problem folders are included at examples/geometries.
*/

#include <vector>
#include <iostream>
#include <stdlib.h>
#include <omp.h>
#include <mpi.h>

#include "porescale.hpp"

#define MASTER_RANK 0

int
main( int argc, char* argv[] )
{

  // Init MPI
  int nRanks, rank;

  MPI_Init(NULL, NULL);

  MPI_Comm_size( MPI_COMM_WORLD, &nRanks );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );  

  //-- timers --//
  double begin, rebegin, para_time, mesh_time, build_time, solve_time, postp_time, total_time;

  begin = omp_get_wtime();
  //--- problem parameters ---//

  para_time = omp_get_wtime() - begin;
  rebegin = omp_get_wtime();

  //--- mesh ---//
  // check mesh sanity and remove dead pores

  // build the mesh

  mesh_time = omp_get_wtime() - rebegin;
  rebegin = omp_get_wtime();

  // save the mesh for visualization with paraview

  postp_time = omp_get_wtime() - rebegin;
  total_time = omp_get_wtime() - begin;

  // print timings
  if (rank == MASTER_RANK) {
    std::cout << "\n\n//------------------ Stokes Finished! Time reports ------------------//\n";
    std::cout << "Parameter setup time: " << para_time << "\n";
    std::cout << "Mesh time: " << mesh_time << "\n";
    std::cout << "Post processessing time: " << postp_time << "\n";
    std::cout << "Total time: " << total_time << "\n";
  }

  MPI_Finalize();

}
