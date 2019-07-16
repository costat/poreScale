/**
 * \file
 * \author Timothy B. Costa
 * \brief Mesh definitions header file.
 */

#ifndef _PORESCALE_MESH_DEFINE_H_
#define _PORESCALE_MESH_DEFINE_H_

#include "define.hpp"

namespace porescale 
{

/** \brief Mesh vertex structure. */
template <typename T>
struct vertex
{

  /**< (x, y, z) coordinates of the vertex. */
  T coordinates[3] = { 0, 0, 0 };

};

/** \brief Mesh edge structure. */
template <typename T>
class edge
{

public:

  edge();
  edge( vertex<T>* v1, vertex<T>* v2 );

  ~edge();

  void init( vertex<T>* v1, vertex<T>* v2 );

  vertex<T>* vertices( psInt idx );
  T length(void) const;

private:

  void computeLength_(void);

  vertex<T>* vertices_[2];
  T          length_;

};

/** \brief Mesh face structure. */
template <typename T>
class face
{

public:

  face();
  face( psInt nEdges, edge<T>* e1,  ... );

  ~face();

   void init( psInt nEdges, edge<T>* e1,  ... );

  vertex<T>*  vertices( psInt idx );
  edge<T>*    edges( psInt idx );

  psInt     nVertices(void) const;
  psInt     nEdges(void) const;

  T           area(void) const;

private:

  void init_( edge<T>** edgesTmp );
  void computeArea_(void);

  vertex<T>** vertices_;
  edge<T>**   edges_;
  T           area_;

  psInt     nEdges_;
  psInt     nVertices_;

};

/** \brief Mesh cell structure. */
template <typename T>
class cell
{

};

/** \brief Mesh degree of freedom structure. */
template <typename T>
class degreeOfFreedom
{
  
};

}

#endif
