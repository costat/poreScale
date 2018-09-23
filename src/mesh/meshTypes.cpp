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
    psInt_t             nEdges,
    porescale::edge<T>* e1, 
    ...
)
{

    // Set edges 
    nEdges_ = nEdges;
    edges_ = new edge<T>*[nEdges];

    va_list argPtr;
    va_start( argPtr, e1 );

    edges_[0] = e1;
    for (int i = 1; i < nEdges; i++) {
        edges_[i] = va_arg( argPtr, porescale::edge<T>* );
    }
    va_end( argPtr );

    // Find unique nodes and set vertices
    std::unordered_set< porescale::vertex<T>* > uset;

    uset.insert( edges_[0]->vertices(0) );
    uset.insert( edges_[0]->vertices(1) );

    nVertices_ = 2;

    for (int i = 1; i < nEdges; i++) {
        for (int j = 0; j < 2; j++ ) {
            if (uset.find( edges_[i]->vertices(j) ) == uset.end()) {
                uset.insert( edges_[i]->vertices(j) );
                nVertices_++;
            }
        }
    }

    vertices_ = new porescale::vertex<T>*[nVertices_];

    psInt_t idx = 0;
    for (const auto& elem: uset) {
        vertices_[idx] = elem;
        idx++;
    }

    // Compute dimensions

}

//--- Public member functions ---//
template <typename T>
psErr_t
porescale::face<T>::init(
    psInt_t             nEdges,
    porescale::edge<T>* e1, 
    ...
)
{

    // Set edges 
    nEdges_ = nEdges;
    edges_ = new edge<T>*[nEdges];

    va_list argPtr;
    va_start( argPtr, e1 );

    edges_[0] = e1;
    for (int i = 1; i < nEdges; i++) {
        edges_[i] = va_arg( argPtr, porescale::edge<T>* );
    }
    va_end( argPtr );

    // Find unique nodes and set vertices
    std::unordered_set< porescale::vertex<T>* > uset;

    uset.insert( edges_[0]->vertices(0) );
    uset.insert( edges_[0]->vertices(1) );

    nVertices_ = 2;

    for (int i = 1; i < nEdges; i++) {
        for (int j = 0; j < 2; j++ ) {
            if (uset.find( edges_[i]->vertices(j) ) == uset.end()) {
                uset.insert( edges_[i]->vertices(j) );
                nVertices_++;
            }
        }
    }

    vertices_ = new porescale::vertex<T>*[nVertices_];

    psInt_t idx = 0;
    for (const auto& elem: uset) {
        vertices_[idx] = elem;
        idx++;
    }

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