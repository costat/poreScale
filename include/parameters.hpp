/**
 * \file
 * \author Timothy B. Costa
 * \brief poreScale parameters
 */

#ifndef _PORESCALE_PARAMETERS_H_
#define _PORESCALE_PARAMETERS_H_

#include <nvshmem.h>
#include <nvshmemx.h>
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
  void init(std::string& problemPath);

  /** \brief Returns problem dimension. */
  psInt      dimension(void) const;
  /** \brief Returns length of the domain. */
  T            length(void) const;
  /** \brief Returns width of the domain. */
  T            width(void) const;
  /** \brief Returns height of the domain. */
  T            height(void) const;
  /** \brief Returns maximum inflow value. */
  T            inflowMax(void) const;
  /** \brief Returns pointer to voxelGeometry. */
  psUInt8 *  voxelGeometry(void);
  /** \brief Returns x mesh dimension. */
  psInt      nx(void) const;
  /** \brief Returns y mesh dimension. */
  psInt      ny(void) const;
  /** \brief Returns z mesh dimension. */
  psInt      nz(void) const;
  /** \brief Returns max iterations allowed in iterative solvers. */
  psInt      solverMaxIterations(void) const;
  /** \brief Returns absolute tolerance for the iterative solver. */
  T            solverAbsoluteTolerance(void) const;
  /** \brief Returns relative tolerance for the iterative solver. */
  T            solverRelativeTolerance(void) const;
  /** \brief Returns verbose level for the iterative solver. */
  psInt      solverVerbose(void) const;
  /** \brief Returns reference to the problem path. */
  std::string& problemPath(void);
  /** \brief Returns id of current pe. */
  psInt      myPe(void) const;
  /** \brief Returns number of processing elements. */
  psInt      nPes(void) const;

  /** \brief Prints problem parameters to console. */
  void printParameters(void);

private:

  /** \brief Initializes a parameters struct from data in a problem directory.
   *
   * @param[in] problemPath - path to problem folder containing Parameters.dat and Geometry.dat files.
   *
   */
  void initParameters_( std::string& problemPath );

  /** \brief Loads parameters into a parameters struct from Parameters.dat file.
   *
   * @param[in] problemPath - path to Parameters.dat.
   */
  void loadParameters_( std::string& problemPath );

  /** \brief Reads voxel geometry from Geometry.dat file.
   *
   * @param[in] problemPath - path to Geomtry.dat.
   */
  void importVoxelGeometry_( std::string& problemPath );

  /** \brief Function to partition voxel data across PEs. */
  void partitionVoxelGeometry_(void);

  // Physical information
  psInt dimension_;                    /**< Specifies if the problem is 2d or 3d. */
  T     length_;                       /**< Specifies the length of the domain (x-direction). */
  T     width_;                        /**< Specifies the width of the domain (y-direction). */
  T     height_;                       /**< Specifies the height of the domain (z-direction). */
  T     inflowMax_;                    /**< Specifies the maximum inflow velocity. Defaults to 1 */

  // Mesh information
  psUInt  *  localGeometryIndex_;        /**< Vector storing indices of voxelGeometry belonging to current PE. */
  psUInt8 *  voxelGeometry_;             /**< Vector storing a voxel geometry read from the Geometry.dat input file. */
  psInt      nx_;                        /**< Specifies the x mesh dimension. */
  psInt      ny_;                        /**< Specifies the y mesh dimension. */
  psInt      nz_;                        /**< Specifies the z mesh dimension. */

  // Solver controls
  psInt solverMaxIterations_;            /**< Specifies the maximum iterations allowed in iterative solvers. */
  T     solverAbsoluteTolerance_;        /**< Specifies the absolute error tolerance for iterative solvers. */
  T     solverRelativeTolerance_;        /**< Specifies the relative error tolerance for iterative solvers. */
  psInt solverVerbose_;                  /**< Specifies the level of console output produced by iterative solvers. */

  // Save problem folder
  std::string problemPath_;              /**< Path to folder containing Geometry.dat and Parameters.dat input files. */

  // NVSHMEM information
  psInt nPes_;				 /**< Number of NVSHMEM Processing Elements. */
  psInt myPe_;				 /**< Return ID of current PE. */

};

}

#endif
