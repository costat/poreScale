/**
 * \file
 * \author Timothy B. Costa
 * \brief Source for matrix base class.
 */

#include "matrix.hpp"

//--- Constructors ---//
template <typename T>
porescale::matrix<T>::matrix(void) : myPe_(0), nPes_(0),
    globalRows_(0), localRows_(0), globalColumns_(0), localColumns_(0),
    firstRow_(0), firstColumn_(0), allocated_(false) { };

template <typename T>
porescale::matrix<T>::matrix(porescale::parameters<T> * par) :
    globalRows_(0), localRows_(0), globalColumns_(0), localColumns_(0),
    firstRow_(0), firstColumn_(0), allocated_(false)
{
    myPe_ = par->myPe();
    nPes_ = par->nPes();
}

//--- Init ---//
template <typename T>
void
porescale::matrix<T>::init(parameters<T> * par)
{
    myPe_   = par->myPe();
    nPes_ = par->nPes();
}

//--- Sets ---//
template <typename T>
void
porescale::matrix<T>::setGlobalRows(psInt gRows) { globalRows_ = gRows; }

template <typename T>
void
porescale::matrix<T>::setLocalRows(psInt lRows) { localRows_ = lRows; }

template <typename T>
void
porescale::matrix<T>::setGlobalColumns(psInt gColumns) { globalColumns_ = gColumns; }

template <typename T>
void
porescale::matrix<T>::setLocalColumns(psInt lColumns) { localColumns_ = lColumns; }

//--- Gets ---//
template <typename T>
psInt
porescale::matrix<T>::myPe(void) const { return myPe_; }

template <typename T>
psInt
porescale::matrix<T>::nPes(void) const { return nPes_; }

template <typename T>
psInt
porescale::matrix<T>::globalRows(void) const { return globalRows_; }

template <typename T>
psInt
porescale::matrix<T>::localRows(void) const { return localRows_; }

template <typename T>
psInt
porescale::matrix<T>::globalColumns(void) const { return globalColumns_; }

template <typename T>
psInt
porescale::matrix<T>::localColumns(void) const { return localColumns_; }

//--- Explicit Instantiations ---//
template class porescale::matrix<float>;
template class porescale::matrix<double>;

