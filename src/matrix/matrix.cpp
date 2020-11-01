/**
 * \file
 * \author Timothy B. Costa
 * \brief Source for matrix base class.
 */

#include "matrix.hpp"

//--- Constructors ---//
template <typename T>
porescale::matrix<T>::matrix(void) :
    rows_(0), columns_(0),
    allocated_(false) { };

template <typename T>
porescale::matrix<T>::matrix(porescale::parameters<T> * par) :
    rows_(0), columns_(0),
    allocated_(false)
{ }

//--- Init ---//
template <typename T>
void
porescale::matrix<T>::init(parameters<T> * par)
{ }

//--- Sets ---//
template <typename T>
void
porescale::matrix<T>::setRows(psInt rows) { rows_ = rows; }

template <typename T>
void
porescale::matrix<T>::setColumns(psInt columns) { columns_ = columns; }

//--- Gets ---//
template <typename T>
psInt
porescale::matrix<T>::rows(void) const { return rows_; }

template <typename T>
psInt
porescale::matrix<T>::columns(void) const { return columns_; }

//--- Explicit Instantiations ---//
template class porescale::matrix<float>;
template class porescale::matrix<double>;

