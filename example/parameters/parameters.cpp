/* Example loads parameters and voxel geometry and prints diagnostic info to console. 
   The mesh is saved for visualization. Build with included
   CMakeLists.txt, and use:
      parameters <path/to/problemfolder>
   Some example problem folders are included at examples/geometries.
*/

#include <vector>
#include <iostream>
#include <stdlib.h>
#include <mpi.h>

#include "porescale.hpp"

#define MASTER_RANK 0

int
main( int argc, const char* argv[] )
{

  // Init MPI
  int nRanks, rank;

  MPI_Init(NULL, NULL);

  MPI_Comm_size( MPI_COMM_WORLD, &nRanks );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );  

  //-- timers --//
  double begin, rebegin, para_time, mesh_time, build_time, solve_time, postp_time, total_time;

  begin = MPI_Wtime();
  //--- problem parameters ---//
  std::string problemPath(argv[1]);
  porescale::parameters<double> par( problemPath );

  para_time = MPI_Wtime() - begin;
  rebegin = MPI_Wtime();

  //--- mesh ---//
  // check mesh sanity and remove dead pores

  // build the mesh

  mesh_time = MPI_Wtime() - rebegin;
  rebegin = MPI_Wtime();
  // print parameters 
  if (rank == MASTER_RANK) par.printParameters(); 
  MPI_Barrier( MPI_COMM_WORLD );

  // save the mesh for visualization with paraview

  postp_time = MPI_Wtime() - rebegin;
  total_time = MPI_Wtime() - begin;

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
