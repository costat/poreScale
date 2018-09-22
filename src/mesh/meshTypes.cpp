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

