/**
 * \file
 * \author Timothy B. Costa
 * \brief poreScale parameters
 */

#ifndef _PORESCALE_PARAMETERS_H_
#define _PORESCALE_PARAMETERS_H_

#include <mpi.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "define.hpp"

namespace porescale
{

/** \brief Struct holding a variety of problem information.
 *
 */
template <typename T>
class parameters
{

public:
  /** \brief Default constructor. */
  parameters(void);
  /** \brief Constructor with initialization from problem path. */
  parameters(std::string& problemPath);

  ~parameters(void);

  /** \brief Initialize from problem path. */
  psErr_t init(std::string& problemPath);

  /** \brief Returns problem dimension. */
  psInt_t      dimension(void) const;
  /** \brief Returns length of the domain. */
  T            length(void) const;
  /** \brief Returns width of the domain. */
  T            width(void) const;
  /** \brief Returns height of the domain. */
  T            height(void) const;
  /** \brief Returns maximum inflow value. */
  T            inflowMax(void) const;
  /** \brief Returns pointer to voxelGeometry. */
  psUInt8_t *  voxelGeometry(void);
  /** \brief Returns x mesh dimension. */
  psInt_t      nx(void) const;
  /** \brief Returns y mesh dimension. */
  psInt_t      ny(void) const;
  /** \brief Returns z mesh dimension. */
  psInt_t      nz(void) const;
  /** \brief Returns max iterations allowed in iterative solvers. */
  psInt_t      solverMaxIterations(void) const;
  /** \brief Returns absolute tolerance for the iterative solver. */
  T            solverAbsoluteTolerance(void) const;
  /** \brief Returns relative tolerance for the iterative solver. */
  T            solverRelativeTolerance(void) const;
  /** \brief Returns verbose level for the iterative solver. */
  psInt_t      solverVerbose(void) const;
  /** \brief Returns reference to the problem path. */
  std::string& problemPath(void);
  /** \brief Returns number of global ranks. */
  psInt_t      nRanks(void) const;
  /** \brief Returns rank of current process. */
  psInt_t      rank(void) const;

  /** \brief Prints problem parameters to console. */
  void printParameters(void);

private:

  /** \brief Initializes a parameters struct from data in a problem directory.
   * 
   * @param[in] problemPath - path to problem folder containing Parameters.dat and Geometry.dat files.
   *
   */
  psErr_t initParameters_( std::string& problemPath );

  /** \brief Loads parameters into a parameters struct from Parameters.dat file.
   *
   * @param[in] problemPath - path to Parameters.dat.
   */
  psErr_t loadParameters_( std::string& problemPath );

  /** \brief Reads voxel geometry from Geometry.dat file.
   *
   * @param[in] problemPath - path to Geomtry.dat.
   */
  psErr_t importVoxelGeometry_( std::string& problemPath );

  /** \brief Function to partition voxel data across MPI ranks. */
  psErr_t partitionVoxelGeometry_(void);

  // Physical information
  psInt_t dimension_;                      /**< Specifies if the problem is 2d or 3d. */
  T       length_;                         /**< Specifies the length of the domain (x-direction). */
  T       width_;                          /**< Specifies the width of the domain (y-direction). */
  T       height_;                         /**< Specifies the height of the domain (z-direction). */
  T       inflowMax_;                      /**< Specifies the maximum inflow velocity. Defaults to 1 */

  // Mesh information 
  psUInt_t  *  localGeometryIndex_;        /**< Vector storing indices of voxelGeometry belonging to current rank. */
  psUInt8_t *  voxelGeometry_;             /**< Vector storing a voxel geometry read from the Geometry.dat input file. */
  psInt_t      nx_;                        /**< Specifies the x mesh dimension. */
  psInt_t      ny_;                        /**< Specifies the y mesh dimension. */
  psInt_t      nz_;                        /**< Specifies the z mesh dimension. */

  // Solver controls
  psInt_t solverMaxIterations_;            /**< Specifies the maximum iterations allowed in iterative solvers. */
  T       solverAbsoluteTolerance_;        /**< Specifies the absolute error tolerance for iterative solvers. */
  T       solverRelativeTolerance_;        /**< Specifies the relative error tolerance for iterative solvers. */
  psInt_t solverVerbose_;                  /**< Specifies the level of console output produced by iterative solvers. */

  // Save problem folder
  std::string problemPath_;                /**< Path to folder containing Geometry.dat and Parameters.dat input files. */

  // MPI information
  psInt_t nRanks_;   		           /**< Number of MPI ranks. */
  psInt_t rank_;                           /**< Current MPI rank. */
};

}

#endif
