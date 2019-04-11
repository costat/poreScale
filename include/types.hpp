/**
 * \file
 * \author Timothy B. Costa
 * \brief poreScale types
 */

#ifndef _PORESCALE_TYPES_H_
#define _PORESCALE_TYPES_H_

namespace porescale
{

#ifdef _OLD_MESH_TYPES_                                 // Old structs used for mesh in HGF. Replaced in meshTypes.{c/h}pp.
/** \brief Mesh vertex struct
 *
 */
template <typename T>
struct vertex
{
  T        coords[3];                                  	/**< Array of vertex coordinates. coords[0] gives the x coordinate, coords[1] gives the y coordinate, and coords[2] gives the z coordinate. */
  psInt_t gnum;                                       	/**< In specifying the global node number for this vertex. */
};

/** \brief Mesh edge struct
 *
 */
struct edge
{
  psInt_t vns[2];                                     	/**< Array giving the local vertex numbers connected by the edge. */
  psInt_t gnum;						/**< In specifying the global edge number for this edge. */
  psInt_t neighbor;                                   	/**< For 2d problems, in specifying the global cell number for the cell sharing this edge. -1 for boundary edge. */
  psInt_t bctype;                                     	/**< For 2d problems, in specifying the boundary type for the edge. 0: interior, 1: dirichlet, 2: neumann. */
};

/** \brief Mesh face struct
 *
 */
struct face
{
  psInt_t vns[4];                                     /**< Array giving the local vertex numbers connected by the face. */
  psInt_t gnum;                                       /**< In specifying the global face number for this face. */
  psInt_t neighbor;                                   /**< In specifying the global cell number for the cell sharing this face. -1 for a boundary face. */
  psInt_t bctype;                                     /**< In specifying the boundary type for the face. 0: interior, 1: dirichlet, 2: neumann. */
};

/** \brief Hexahedral or quadrilateral cell struct 
 *
 */
template <typename T>
struct qCell
{
  struct edge      edg[12];                            /**< Array of edge structs detailing the edges in the cell. For a 2d problem, edg[0:3] desribes all edges. */
  struct face      fac[6];                             /**< Array of face structs detailing the faces in the cell. Not used in 2d problems. */
  struct vertex<T> vtx[8];                             /**< Array of vertex structs detailing the vertices in the cell. For a 2d problem, vtx[0:3] describes all vertices. */
  T                dx;                                 /**< Length of the cell (x direction). */
  T                dy;                                 /**< Width of the cell (y direction). */
  T                dz;                                 /**< Height of the cell (z direction). */
};

/** \brief Struct describing a degree of freedom in a structured model.
 *
 */
template <typename T>
struct degreeOfFreedom
{
  psInt_t doftype;                                    /**< In specifiying the type of degree of freedom: 0 for interior, 1 for edge, 2 for face. */
  T       coords[3];                                  /**< Array of coordinates of the degree of freedom. coords[0] gives the x coordinate, coords[1] gives the y coordinate, and coords[2] gives the z coordinate. */
  psInt_t cell_numbers[2];                            /**< Array of mesh cell numbers containing the degree of freedom. */
  psInt_t neighbors[6];                               /**< Array listing the global number of neighboring degrees of freedom, i.e. DOFs which interact with this DOF in the model. */
};
#endif

/** \brief Struct for coordinate sparse data format sorting.
 *
 */
template <typename T>
struct arrayCOO
{
  psInt_t i_index;                                    /**< In determining the row of the array entry. */
  psInt_t j_index;                                    /**< In determining the column of the array entry. */
  T        value;                                      /**< Double precision value of the array entry. */
};

/** \brief Struct for tracking boundary node information.
 *
 */
template <typename T>
struct boundaryNode
{
  psInt_t type;              /**< Boundary type. */
  T        value;             /**< Boundary value. */
};

/** \brief Enum for selecting inflow boundary condition.
 *
 */
enum PORESCALE_INFLOW
{
  PORESCALE_INFLOW_PARABOLIC,
  PORESCALE_INFLOW_CONSTANT
};

}

#endif
