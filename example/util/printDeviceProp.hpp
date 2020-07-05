#include <iostream>

// Print device properties
void printDevProp(cudaDeviceProp devProp)
{
  std::cout << "Major revision number:		 " << devProp.major << "\n";
  std::cout << "Minor revision number:		 " << devProp.minor << "\n";
  std::cout << "Name:				 " << devProp.name << "\n";
  std::cout << "Total global memory:		 " << devProp.totalGlobalMem << "\n";
  std::cout << "Total shared memory per block:	 " << devProp.sharedMemPerBlock << "\n";
  std::cout << "Total registers per block: 	 " << devProp.regsPerBlock << "\n";
  std::cout << "Warp size:			 " << devProp.warpSize << "\n";
  std::cout << "Maximum memory pitch:		 " << devProp.memPitch << "\n";
  std::cout << "Maximum threads per block:	 " << devProp.maxThreadsPerBlock << "\n";
  for (int i = 0; i < 3; ++i)
    std::cout << "Maximum dimension " << i << " of block:	 " << devProp.maxThreadsDim[i] << "\n";
  for (int i = 0; i < 3; ++i)
    std::cout << "Maximum dimension " << i << " of grid:	 " << devProp.maxGridSize[i] << "\n";
  std::cout << "Clock rate:			 " << devProp.clockRate << "\n";
  std::cout << "Total constat memory:		 " << devProp.totalConstMem << "\n";
  std::cout << "Texture alignment:		 " << devProp.textureAlignment << "\n";
  if (devProp.deviceOverlap)
    std::cout << "Concurrent copy and execution:	 " << "Yes\n";
  else
    std::cout << "Concurrent copy and execution:	 " << "No\n";
  std::cout << "Number of multiprocessors:	 " << devProp.multiProcessorCount << "\n";
  if (devProp.kernelExecTimeoutEnabled)
    std::cout << "Kernel execution timeout:	 " << "Yes\n";
  else
    std::cout << "Kernel execution timeout:	 " << "No\n";
  return;
}
