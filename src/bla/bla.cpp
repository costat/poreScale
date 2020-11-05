/**
 * \file
 * \author Timothy B. Costa
 * \brief Source for basic linear algebra routines.
 */

#include "bla.hpp"

//--- DENSE LINEAR ALGEBRA ---//
//--- L1 BLAS ---//
template <typename T>
void
porescale::norm<T>(
    porescale::vector& A,
    T&                 result
)
{

    return;
}

template <typename T>
void
porescale::axpy<T>(
    T                     norm,
    porescale::vector<T>& X,
    porescale::vector<T>& Y
)
{

    return;
}

//---SPARSE LINEAR ALGEBRA ---//
//--- L2 BLAS ---//
template <typename T>
void
porescale::sparseMatvec<T>(
    T                           alpha,
    porescale::sparseMatrix<T>& A,
    porescale::vector<T>&       B,
    T                           beta,
    porescale::vector<T>&       C
)
{

    return;
}
