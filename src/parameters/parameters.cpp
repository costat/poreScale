/**
 * \file
 * \author Timothy B. Costa
 * \brief Parameters source.
 */

#include "parameters.hpp"
#include "parmetis.h"

//--- Constructors ---//
template <typename T>
porescale::parameters<T>::parameters(void) : dimension_(0), length_(0), width_(0), height_(0),
                                                             inflowMax_(1.0), voxelGeometry_(NULL),
                                                             nx_(0), ny_(0), nz_(0), 
                                                             solverMaxIterations_(100),
                                                             solverAbsoluteTolerance_(1e-4),
                                                             solverRelativeTolerance_(1e-4),
                                                             solverVerbose_(1),
                                                             nRanks_(0), rank_(0) {}

template <typename T>
porescale::parameters<T>::parameters( std::string& problemPath ) : dimension_(0), length_(0), width_(0), height_(0),
                                                             inflowMax_(1.0), voxelGeometry_(NULL),
                                                             nx_(0), ny_(0), nz_(0), 
                                                             solverMaxIterations_(100),
                                                             solverAbsoluteTolerance_(1e-4),
                                                             solverRelativeTolerance_(1e-4),
                                                             solverVerbose_(1),
                                                             nRanks_(0), rank_(0)
{
  initParameters_( problemPath );
}

//--- Destructor ---//
template <typename T>
porescale::parameters<T>::~parameters(void)
{
  if (localGeometryIndex_ != NULL) delete [] localGeometryIndex_;
  if (voxelGeometry_ != NULL) delete [] voxelGeometry_;
}

//--- Public Member Functions ---//

template <typename T>
psErr_t
porescale::parameters<T>::init(
  std::string& problemPath
)
{
  return initParameters_(problemPath);
}


template <typename T>
void
porescale::parameters<T>::printParameters(void)
{
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
psErr_t
porescale::parameters<T>::initParameters_(
  std::string& problemPath
)
{

  int MPIinit = 0;
  MPI_Initialized( &MPIinit );
  if (!MPIinit) MPI_Init(NULL, NULL);

  MPI_Comm_size( PORESCALE_COMM, &nRanks_ );
  MPI_Comm_rank( PORESCALE_COMM, &rank_ );

  problemPath_ = problemPath;
  std::string Parameters = problemPath_ + "Parameters.dat";
  std::string Geometry = problemPath_ + "Geometry.dat";

  psErr_t err;

  err = loadParameters_(Parameters);
  err = importVoxelGeometry_(Geometry);

  MPI_Barrier(PORESCALE_COMM);

  err = partitionVoxelGeometry_();

  MPI_Barrier(PORESCALE_COMM);

  return err;

}

template <typename T>
psErr_t
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


  //--- Some error checks ---//

  //--- length ---//
  if (length_ == 0) {
    return PORESCALE_UNSUCCESSFUL;
  }

  //--- width ---//
  if (width_ == 0) {
    return PORESCALE_UNSUCCESSFUL;
  }

  //--- solver_max_iterations ---//
  if (solverMaxIterations_ == 0) {
    return PORESCALE_UNSUCCESSFUL;
  }

  //--- solver_absolute_tolerance ---//
  if (solverAbsoluteTolerance_ == 0) {
    return PORESCALE_UNSUCCESSFUL;
  }

   //--- solver_relative_tolerance ---//
  if (solverRelativeTolerance_ == 0) {
    return PORESCALE_UNSUCCESSFUL;
  }

  return PORESCALE_SUCCESSFUL;

}

template <typename T>
psErr_t
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
  if (nx_ == 0) {
    return PORESCALE_UNSUCCESSFUL;
  }

  // grab ny
  if (ifs.good())
  {
    std::getline(ifs, line);
  }
  std::istringstream iy(line);
  iy >> str >> ny_;
  if (ny_ == 0) {
    return PORESCALE_UNSUCCESSFUL;
  }

  // grab nz
  if (ifs.good())
  {
    std::getline(ifs, line);
  }
  std::istringstream iz(line);
  iz >> str >> nz_;

  if (!nz_) dimension_ = 2;
  else dimension_ = 3;

  if (!nx_ || !ny_) {
    return PORESCALE_UNSUCCESSFUL;
  }

  psInt_t nzFactor = (!nz_) ? 1 : nz_;
  voxelGeometry_ = new psUInt8_t[nx_ * ny_ * nzFactor];

  std::string hold;
  // read remaining lines of geometry into parameters file
  psUInt_t inputIter = 0;
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

  // check consistency between array read in and dimensions in file
  if (inputIter != nx_ * ny_ * nzFactor - 1) return PORESCALE_UNSUCCESSFUL;
  else return PORESCALE_SUCCESSFUL;

}

template <typename T>
psErr_t
porescale::parameters<T>::partitionVoxelGeometry_(void)
{

  // parmetis flags
  idx_t ncon        = 0;
  idx_t nparts      = this->nRanks();
  idx_t wgtflag     = 0;
  idx_t numflag     = 0;
  idx_t edgecut;

  // parmetis arrays
  idx_t   * vwgt    = NULL;                       // no weights
  idx_t   * adjwgt  = NULL;                       // no weights
  double  * tpwgts  = NULL;                       // no weights
  double  * ubvec   = NULL;                       // no weights
  idx_t   * options = new idx_t[3];
  
  options[0] = 0;

  // create CSR graph representation of voxelGeometry_
  idx_t * part;
  idx_t * vtxdist;
  idx_t * xadj;
  idx_t * adjncy;

  // WIP  here //

  MPI_Comm PM_Comm = PORESCALE_COMM;

  // partition
  int err = ParMETIS_V3_PartKway( vtxdist, 
                                  xadj,
                                  adjncy,
                                  vwgt,
                                  adjwgt,
                                 &wgtflag,
                                 &numflag,
                                 &ncon,
                                 &nparts,
                                  tpwgts,
                                  ubvec,
                                  options,
                                 &edgecut,
                                  part,
                                 &PM_Comm
                                );
                            
  delete [] options;

  return PORESCALE_UNSUCCESSFUL;
}

//--- Explicit Type Instantiations ---//
template class porescale::parameters<double>;
template class porescale::parameters<float>;
