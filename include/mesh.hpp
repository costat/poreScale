/**
 * \file
 * \author Timothy B. Costa
 * \brief Mesh header file.
 */

#ifndef _PORESCALE_MESH_H_
#define _PORESCALE_MESH_H_

#include "parameters.hpp"
#include "define.hpp"
#include "meshTypes.hpp"

namespace porescale 
{

/** \brief Abstract base mesh class. All mesh classes are derived from this class. */
template <typename T>
class mesh
{

public:

  /** \brief Default constructor. */
  mesh(void);
  /** \brief Constructor with pointer to parameters. */
  mesh(parameters<T> * par);

  /** \brief Destructor. */
  ~mesh(void);

  /** \brief Abstract mesh build function. */
  virtual psErr build(void)        = 0;

  /** \brief Abstract mesh sanity check. */
  virtual psErr checkSanity(void)  = 0;

  /** \brief Abstract function to write VTK file. */
  virtual psErr writeVTK(void)     = 0;

  /** \brief Returns flag determining if mesh has been built. */
  bool built(void) const;

protected:

  bool built_;                /**< Flag determining if mesh has been built. */

  parameters<T> * par_;       /**< Pointer to parameters object. */

};

/** \brief Voxel mesh class. */
template <typename T>
class voxel : public mesh<T>
{

public:

  /** \brief Default constructor. */
  voxel(void);
  /** \brief Constructor with pointer to parameters. */
  voxel(parameters<T> * par);

  /** \brief Destructor. */
  ~voxel(void);

  /** \brief Function to build a quad mesh from voxel data. */
  virtual psErr build(void);

  /** \brief Function to check mesh sanity. */
  virtual psErr checkSanity(void);

  /** \brief Function write mesh info to VTK file for visual inspection. */
  virtual psErr writeVTK(void);

private:

  /** \brief Builds mesh for 2d problem. */
  psErr build2d_(void);
  /** \brief Builds mesh for 3d problem. */
  psErr build3d_(void);

};

}
#endif
