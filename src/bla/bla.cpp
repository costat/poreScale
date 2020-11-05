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
porescale::norm(
    porescale::vector<T>& A,
    T&                    result
)
{

    result = std::transform_reduce(
                 std::execution::par_unseq, A.begin(), A.end(),
                 [=](T A) { return A * A; },
                 0.0, std::plus()
             );
    result = std::sqrt(result);

    return;
}

template <typename T>
void
porescale::axpy(
    T                     alpha,
    porescale::vector<T>& X,
    porescale::vector<T>& Y
)
{

    std::transform(
        std::execution::par_unseq, X.begin(), X.end(), Y.begin(), Y.begin(),
        [=](T X, T Y) { return Y + alpha * X; }
    );

    return;

}

//---SPARSE LINEAR ALGEBRA ---//
//--- L2 BLAS ---//
template <typename T>
void
porescale::sparseMatvec(
    T                           alpha,
    porescale::sparseMatrix<T>& A,
    porescale::vector<T>&       B,
    T                           beta,
    porescale::vector<T>&       C
)
{

    // Check matrix format
    if (A.sparseFormat == porescale::COO) {

    } else if (A.sparseFormat == porescale::CSR) {

    } else
        // error out
    return;
}
