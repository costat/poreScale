/**
 * \file
 * \author Timothy B. Costa
 * \brief Parameters source.
 */

#include "parameters.hpp"

//--- Constructors ---//
template <typename T>
porescale::parameters<T>::parameters(void) : dimension_(0), length_(0), width_(0), height_(0),
                                                             inflowMax_(1.0), voxelGeometry_(NULL),
                                                             localGeometryIndex_(NULL),
                                                             nx_(0), ny_(0), nz_(0),
                                                             solverMaxIterations_(100),
                                                             solverAbsoluteTolerance_(1e-4),
                                                             solverRelativeTolerance_(1e-4),
                                                             solverVerbose_(1),
                                                             nPes_(0), myPe_(0) {}

template <typename T>
porescale::parameters<T>::parameters( std::string& problemPath ) : dimension_(0), length_(0), width_(0), height_(0),
                                                             inflowMax_(1.0), voxelGeometry_(NULL),
                                                             localGeometryIndex_(NULL),
                                                             nx_(0), ny_(0), nz_(0),
                                                             solverMaxIterations_(100),
                                                             solverAbsoluteTolerance_(1e-4),
                                                             solverRelativeTolerance_(1e-4),
                                                             solverVerbose_(1),
                                                             nPes_(0), myPe_(0)
{
  initParameters_( problemPath );
}

//--- Destructor ---//
template <typename T>
porescale::parameters<T>::~parameters(void) {}

//--- Public Member Functions ---//

template <typename T>
void
porescale::parameters<T>::init(
  std::string& problemPath
)
{
  return initParameters_(problemPath);
}

template <typename T>
psInt
porescale::parameters<T>::dimension(void) const { return dimension_; }

template <typename T>
T
porescale::parameters<T>::length(void) const { return length_; }

template <typename T>
T
porescale::parameters<T>::width(void) const { return width_; }

template <typename T>
T
porescale::parameters<T>::height(void) const { return height_; }

template <typename T>
T
porescale::parameters<T>::inflowMax(void) const { return inflowMax_; }

template <typename T>
psInt
porescale::parameters<T>::nx(void) const { return nx_; }

template <typename T>
psInt
porescale::parameters<T>::ny(void) const { return ny_; }

template <typename T>
psInt
porescale::parameters<T>::nz(void) const { return nz_; }

template <typename T>
psInt
porescale::parameters<T>::solverMaxIterations(void) const { return solverMaxIterations_; }

template <typename T>
T
porescale::parameters<T>::solverAbsoluteTolerance(void) const { return solverAbsoluteTolerance_; }

template <typename T>
T
porescale::parameters<T>::solverRelativeTolerance(void) const { return solverRelativeTolerance_; }

template <typename T>
psInt
porescale::parameters<T>::solverVerbose(void) const { return solverVerbose_; }

template <typename T>
std::string&
porescale::parameters<T>::problemPath(void) { return problemPath_; }

template <typename T>
psUInt8 *
porescale::parameters<T>::voxelGeometry(void) { return voxelGeometry_.data(); }

template <typename T>
psInt
porescale::parameters<T>::myPe(void) const { return myPe_; }

template <typename T>
psInt
porescale::parameters<T>::nPes(void) const { return nPes_; }

template <typename T>
void
porescale::parameters<T>::printParameters(void)
{
  std::cout << "Number of NVSHMEM Processing Elements= " << nPes_ << "\n";
  std::cout << "Dimension= " << dimension_ << "\n";
  std::cout << "Geometry length= " << length_ << "\n";
  std::cout << "Geometry width= " << width_ << "\n";
  std::cout << "Geometry height= " << height_ << "\n";
  std::cout << "Solver maximum iterations= " << solverMaxIterations_ << "\n";
  std::cout << "Solver absolute tolerance= " << solverAbsoluteTolerance_ << "\n";
  std::cout << "Solver relative tolerance= " << solverRelativeTolerance_ << "\n";
  std::cout << "Solver verbose= " << solverVerbose_ << "\n";
  std::cout << "Problem path= " << problemPath_ << "\n";
}

//--- Private Member Functions ---//

template <typename T>
void
porescale::parameters<T>::initParameters_(
  std::string& problemPath
)
{

  // NVSHMEM Setup
  nPes_ = nvshmem_n_pes();
  myPe_ = nvshmem_my_pe();

  problemPath_ = problemPath;
  std::string Parameters = problemPath_ + "Parameters.dat";
  std::string Geometry = problemPath_ + "Geometry.dat";

  loadParameters_(Parameters);
  // upon exit all PEs own a copy of the imported voxel geometry
  importVoxelGeometry_(Geometry);

  // upon exit each PE knows index of its voxels
  partitionVoxelGeometry_();

}

template <typename T>
void
porescale::parameters<T>::loadParameters_(
  std::string& problemPath
)
{
  std::string line;
  std::string str;
  std::ifstream ifs(problemPath);

  //--- Read info ---//
  while (std::getline(ifs, line))
  {
    std::istringstream iss(line);
    iss >> str;
    if (!str.compare("length") || !str.compare("length=")) iss >> length_;
    else if (!str.compare("width") || !str.compare("width=")) iss >> width_;
    else if (!str.compare("height") || !str.compare("height=")) iss >> height_;
    else if (!str.compare("solverMaxIterations") || !str.compare("solverMaxIterations=")) iss >> solverMaxIterations_;
    else if (!str.compare("solverAbsoluteTolerance") || !str.compare("solverAbsoluteTolerance=")) iss >> solverAbsoluteTolerance_;
    else if (!str.compare("solverRelativeTolerance") || !str.compare("solverRelativeTolerance=")) iss >> solverRelativeTolerance_;
    else if (!str.compare("solverVerbose") || !str.compare("solverVerbose=")) iss >> solverVerbose_;
    else {
      std::cout << "\nPORESCALE Warning :: input token " << str << " in Parameters.dat is undefined\n";
    }
  }

}

template <typename T>
void
porescale::parameters<T>::importVoxelGeometry_(
  std::string& problemPath
)
{
  std::string line;
  std::string str;
  std::ifstream ifs(problemPath);

  // grab nx
  if (ifs.good())
  {
    std::getline(ifs, line);
  }
  std::istringstream ix(line);
  ix >> str >> nx_;

  // grab ny
  if (ifs.good())
  {
    std::getline(ifs, line);
  }
  std::istringstream iy(line);
  iy >> str >> ny_;

  // grab nz
  if (ifs.good())
  {
    std::getline(ifs, line);
  }
  std::istringstream iz(line);
  iz >> str >> nz_;

  if (!nz_) dimension_ = 2;
  else dimension_ = 3;

  psInt nzFactor = (!nz_) ? 1 : nz_;
  voxelGeometry_.resize(nx_ * ny_ * nzFactor);

  std::string hold;
  // read remaining lines of geometry into parameters file
  psUInt inputIter = 0;
  if (nz_) { // 3d voxel file
    for (int nslices = 0; nslices < nz_; nslices++) {
      for (int nrows = 0; nrows < ny_; nrows++) {
        if (ifs.good()) {
          std::getline(ifs, line);
        }
        // place line entries into parameters data
        std::stringstream stream(line);
        while (1) {
          int n;
          stream >> n;
          if (!stream) break;
          voxelGeometry_[inputIter] = n;
          inputIter++;
        }
      }
      // add the empty row between z slices
      if (ifs.good()) {
        std::getline(ifs, line);
      }
    }
  }
  else { // 2d voxel file
    for (int nrows = 0; nrows < ny_; nrows++) {
      // grab row
      if (ifs.good()) {
        std::getline(ifs, line);
      }
      // place line entries into param data
      std::stringstream stream(line);
      while (1) {
        int n;
        stream >> n;
        if (!stream) break;
        voxelGeometry_[inputIter] = n;
        inputIter++;
      }
    }
  }

}

template <typename T>
void
porescale::parameters<T>::partitionVoxelGeometry_(void)
{

  // partition -- determine the voxels that belong to this PE, set up neighbor information, and send data to devices.

  // Special case for single GPU
  if (nPes_ == 1) {
    d_localVoxelGeometry_ = voxelGeometry_;
    return;
  }

  // Multi-GPU is WIP

}

//--- Explicit Type Instantiations ---//
template class porescale::parameters<double>;
template class porescale::parameters<float>;

