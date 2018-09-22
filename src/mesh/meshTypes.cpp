/**
 * \file
 * \author Timothy B. Costa
 * \brief Mesh types header file.
 */

#include "meshTypes.hpp"

//////////////// EDGE //////////////////

//--- Constructors and Destructors ---//
template <typename T>
porescale::edge<T>::edge() : length_(0.0)
{
    vertices_[0] = NULL;
    vertices_[1] = NULL;
}

template <typename T>
porescale::edge<T>::edge( 
    porescale::vertex<T>* v1, 
    porescale::vertex<T>* v2
)
{
    vertices_[0] = v1;
    vertices_[1] = v2;
    computeLength_();
}

//--- Public member functions ---//
template <typename T>
psErr_t
porescale::edge<T>::init(
    porescale::vertex<T>* v1,
    porescale::vertex<T>* v2
)
{
    vertices_[0] = v1;
    vertices_[1] = v2;
    computeLength_();
}

template <typename T>
porescale::vertex<T>*
porescale::edge<T>::vertices(
    psInt_t idx
)
{
    return vertices_[idx];
}

template <typename T>
T 
porescale::edge<T>::length(void) const { return length_; }

//--- Private member functions ---//
template <typename T>
psErr_t
porescale::edge<T>::computeLength_(void)
{
    porescale::vertex<T> *v1 = vertices_[0];
    porescale::vertex<T> *v2 = vertices_[1];

    T tmp;
    length_ = 0.0;
    for (int i = 0; i < 3; i++) 
    {
        tmp = (v1->coordinates[i] - v2->coordinates[i]);
        length_ += tmp*tmp;
    }

    length_ = sqrt(length_);
    
}

//--- Explicit type instantiations ---//
template class porescale::edge<float>;
template class porescale::edge<double>;


//////////////// FACE //////////////////

//--- Constructors and Destructors ---//
template <typename T>
porescale::face<T>::face() : vertices_(NULL), edges_(NULL), 
                             length_(0.0), width_(0.0), nEdges_(0), nVertices_(0) { }

template <typename T>
porescale::face<T>::face( 
    porescale::edge<T>* e1, 
    porescale::edge<T>* e2, 
    porescale::edge<T>* e3, 
    ...
)
{

    // Get edge count

    // Set edges, nEdges and nVertices

    // Find unique nodes and set vertices

    // Compute dimensions

}

//--- Public member functions ---//
template <typename T>
psErr_t
porescale::face<T>::init(
    porescale::edge<T>* e1, 
    porescale::edge<T>* e2, 
    porescale::edge<T>* e3, 
    ...
)
{

    // Get edge count

    // Set edges

    // Find unique nodes and set vertices

    // Compute dimensions

}

template <typename T>
porescale::vertex<T>*
porescale::face<T>::vertices(
    psInt_t idx
)
{
    return vertices_[idx];
}

template <typename T>
porescale::edge<T>*
porescale::face<T>::edges(
    psInt_t idx
)
{
    return edges_[idx];
}

template <typename T>
psInt_t porescale::face<T>::nVertices(void) const { return nVertices_; }

template <typename T>
psInt_t porescale::face<T>::nEdges(void) const { return nEdges_; }

template <typename T>
T porescale::face<T>::length(void) const { return length_; }

template <typename T>
T porescale::face<T>::width(void) const { return width_; }

//--- Private member functions ---//


//--- Explicit type instantiations ---//
template class porescale::face<float>;
template class porescale::face<double>;