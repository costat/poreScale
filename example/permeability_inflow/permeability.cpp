/* Example computes the upscaled permeability in the x-direction from the solution to Stokes and prints
   the result to console. The x-flow solution is saved for visualization. Build with included
   CMakeLists.txt, and use:
     permeability <path/to/problemfolder>
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
main( int argc, const char* argv[] )
{

  // Init MPI
  int nRanks, rank;

  MPI_Init(NULL, NULL);

  MPI_Comm_size( MPI_COMM_WORLD, &nRanks );
  MPI_Comm_size( MPI_COMM_WORLD, &rank );  

  //----- STOKES example -----//
  if (rank == MASTER_RANK) std::cout << "\n//----Solving STOKES----//\n";   
  MPI_Barrier( MPI_COMM_WORLD );

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

  //--- stokes model ---//

  // build the degrees of freedom and the array for interior cells, initializes viscosity to 1.0

  // if viscosity != 1, set after build
  // x_stks.viscosity = 0.5;

  // add penalty for immersed boundary cells

  // set up boundary conditions

  build_time = omp_get_wtime() - rebegin;
  rebegin = omp_get_wtime();

  // solve

  solve_time = omp_get_wtime() - rebegin;
  rebegin = omp_get_wtime();

  // compute permeability and print to console

  // save the x-flow solution for visualization with paraview

  // save the full state of the flow simulation to .dat file

  postp_time = omp_get_wtime() - rebegin;
  total_time = omp_get_wtime() - begin;

  // print timings
  if (rank == MASTER_RANK) {
    std::cout << "\n\n//------------------ Stokes Finished! Time reports ------------------//\n";
    std::cout << "Parameter setup time: " << para_time << "\n";
    std::cout << "Mesh time: " << mesh_time << "\n";
    std::cout << "Array construction time: " << build_time << "\n";
    std::cout << "Solver time: " << solve_time << "\n";
    std::cout << "Post processessing time: " << postp_time << "\n";
    std::cout << "Total time: " << total_time << "\n";
  }

}
