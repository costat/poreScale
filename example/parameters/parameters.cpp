/* Example loads parameters and voxel geometry and prints diagnostic info to console.
   The mesh is saved for visualization. Build with included
   CMakeLists.txt, and use:
      parameters <path/to/problemfolder>
   Some example problem folders are included at examples/geometries.
*/

#include <vector>
#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <ctime>

#include "porescale.hpp"

#define CONTROL_PE 0

int
main( int argc, const char* argv[] )
{


  auto begin = std::chrono::high_resolution_clock::now();

  //--- problem parameters ---//
  std::string problemPath(argv[1]);
  porescale::parameters<float> par( problemPath );

  double para_time = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - begin).count();
  auto rebegin = std::chrono::high_resolution_clock::now();

  //--- mesh ---//
  // check mesh sanity and remove dead pores

  // build the mesh

  double mesh_time = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - rebegin).count();
  rebegin = std::chrono::high_resolution_clock::now();

  // print parameters
  par.printParameters();

  // save the mesh for visualization with paraview

  double postp_time = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - rebegin).count();
  double total_time = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - begin).count();

  // print timings
  std::cout << "\n\n//------------------ Finished! Time reports ------------------//\n";
  std::cout << "Parameter setup time: " << para_time << " ms\n";
  std::cout << "Mesh time: " << mesh_time << " ms\n";
  std::cout << "Post processessing time: " << postp_time << " ms\n";
  std::cout << "Total time: " << total_time << " ms\n";

}
