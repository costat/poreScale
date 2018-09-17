/**
 * \file
 * \author Timothy B. Costa
 * \brief Voxel mesh source file.
 */

#include "mesh.hpp"

//--- Constructor --//
template <typename T> 
porescale::voxel<T>::voxel(void) : mesh<T>() { }

template <typename T>
porescale::voxel<T>::voxel(parameters<T> * par) : mesh<T>(par) { }

//--- Public member functions ---//
template <typename T>
psErr_t
porescale::voxel<T>::build(void)
{
  if (this->par_->dimension() == 3) return build3d_();
  else return build2d_();
}

template <typename T>
psErr_t
porescale::voxel<T>::checkSanity(void)
{

  psInt_t totalChanged = 0;
  psInt_t nChanged;

  psInt_t nz = this->par_->nz();
  psInt_t ny = this->par_->ny();
  psInt_t nx = this->par_->nx();

  psUInt8_t * voxelGeometryPtr = this->par_->voxelGeometry();

  if (this->par_->dimension() == 3) goto checkSanity3;

checkSanity3:
  {
    nChanged = 0;
    for (psInt_t zi = 0; zi < nz; zi++) {
      for (psInt_t yi = 0; yi < ny; yi++) {
        for (psInt_t xi = 0; xi < nx; xi++) {

          if (voxelGeometryPtr[idx3(zi, yi, xi, ny, nx)] != 1) {
            // xi sanity
            if (xi == 0) {
              if (voxelGeometryPtr[idx3(zi, yi, (xi + 1), ny, nx)] == 1) {
                voxelGeometryPtr[idx3(zi, yi, xi, ny, nx)] = 1;
                nChanged++;
              }
            }
            else if (xi == nx - 1) {
              if (voxelGeometryPtr[idx3(zi, yi, (xi - 1), ny, nx)] == 1) {
                voxelGeometryPtr[idx3(zi, yi, xi, ny, nx)] = 1;
                nChanged++;
              }
            }
            else {
              if (voxelGeometryPtr[idx3(zi, yi, (xi + 1), ny, nx)] == 1 \
                && voxelGeometryPtr[idx3(zi, yi, (xi - 1), ny, nx)] == 1) {
                voxelGeometryPtr[idx3(zi, yi, xi, ny, nx)] = 1;
                nChanged++;
              }
            }
            // yi sanity
            if (yi == 0) {
              if (voxelGeometryPtr[idx3(zi, (yi + 1), xi, ny, nx)] == 1) {
                voxelGeometryPtr[idx3(zi, yi, xi, ny, nx)] = 1;
                nChanged++;
              }
            }
            else if (yi == ny - 1) {
              if (voxelGeometryPtr[idx3(zi, (yi - 1), xi, ny, nx)] == 1) {
                voxelGeometryPtr[idx3(zi, yi, xi, ny, nx)] = 1;
                nChanged++;
              }
            }
            else {
              if (voxelGeometryPtr[idx3(zi, (yi + 1), xi, ny, nx)] == 1 \
                && voxelGeometryPtr[idx3(zi, (yi - 1), xi, ny, nx)] == 1) {
                voxelGeometryPtr[idx3(zi, yi, xi, ny, nx)] = 1;
                nChanged++;
              }
            }
            // zi sanity
            if (zi == 0) {
              if (voxelGeometryPtr[idx3((zi + 1), yi, xi, ny, nx)] == 1) {
                voxelGeometryPtr[idx3(zi, yi, xi, ny, nx)] = 1;
                nChanged++;
              }
            }
            else if (zi == nz - 1) {
              if (voxelGeometryPtr[idx3((zi - 1), yi, xi, ny, nx)] == 1) {
                voxelGeometryPtr[idx3(zi, yi, xi, ny, nx)] = 1;
                nChanged++;
              }
            }
            else {
              if (voxelGeometryPtr[idx3((zi + 1), yi, xi, ny, nx)] == 1 \
                && voxelGeometryPtr[idx3((zi - 1), yi, xi, ny, nx)] == 1) {
                voxelGeometryPtr[idx3(zi, yi, xi, ny, nx)] = 1;
                nChanged++;
              }
            }
          }

        }
      }
    }
    totalChanged += nChanged;
    if (nChanged) goto checkSanity3;
    else goto cleanup;
  }

checkSanity2:
  {
    nChanged = 0;
    for (psInt_t yi = 0; yi < ny; yi++) {
      for (psInt_t xi = 0; xi < nx; xi++) {

        if (voxelGeometryPtr[idx2(yi, xi, nx)] != 1) {
          // xi sanity
          if (xi == 0) {
            if (voxelGeometryPtr[idx2(yi, (xi + 1), nx)] == 1) {
              voxelGeometryPtr[idx2(yi, xi, nx)] = 1;
              nChanged++;
            }
          }
          else if (xi == nx - 1) {
            if (voxelGeometryPtr[idx2(yi, (xi - 1), nx)] == 1) {
              voxelGeometryPtr[idx2(yi, xi, nx)] = 1;
              nChanged++;
            }
          }
          else {
            if (voxelGeometryPtr[idx2(yi, (xi + 1), nx)] == 1 \
              && voxelGeometryPtr[idx2(yi, (xi - 1), nx)] == 1) {
              voxelGeometryPtr[idx2(yi, xi, nx)] = 1;
              nChanged++;
            }
          }
          // yi sanity
          if (yi == 0) {
            if (voxelGeometryPtr[idx2((yi + 1), xi, nx)] == 1) {
              voxelGeometryPtr[idx2(yi, xi, nx)] = 1;
              nChanged++;
            }
          }
          else if (yi == ny - 1) {
            if (voxelGeometryPtr[idx2((yi - 1), xi, nx)] == 1) {
              voxelGeometryPtr[idx2(yi, xi, nx)] = 1;
              nChanged++;
            }
          }
          else {
            if (voxelGeometryPtr[idx2((yi + 1), xi, nx)] == 1 \
              && voxelGeometryPtr[idx2((yi - 1), xi, nx)] == 1) {
              voxelGeometryPtr[idx2(yi, xi, nx)] = 1;
              nChanged++;
            }
          }
        }

      }
    }
    totalChanged += nChanged;
    if (nChanged) goto checkSanity2;
    else goto cleanup;
  }

cleanup:
  if (totalChanged) {
    std::cout << "\nWarning, input geometry was not sane.\n";
    std::cout << totalChanged << " cells, representing ";
    if (this->par_->dimension() == 3) 
      std::cout << (T)100 * totalChanged / (nx * ny * nz);
    else 
      std::cout << (T)100 * totalChanged / (nx * ny);
    std::cout << "% of the input geometry, with boundaries on opposite cell faces \n";
    std::cout << "were found and removed from void space.\n";
  }

  return PORESCALE_SUCCESSFUL;

}

template <typename T>
psErr_t
porescale::voxel<T>::writeVTK(void)
{
  return PORESCALE_UNSUCCESSFUL;
}

//--- Priviate member functions ---//
template <typename T>
psErr_t
porescale::voxel<T>::build2d_(void)
{
  return PORESCALE_UNSUCCESSFUL;
}

template <typename T>
psErr_t
porescale::voxel<T>::build3d_(void)
{
  return PORESCALE_UNSUCCESSFUL;
}

//--- Explicit type instantiations ---//
template class porescale::voxel<double>;
template class porescale::voxel<float>;

