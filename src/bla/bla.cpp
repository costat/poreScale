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
    std::vector<T>& A,
    T&              result
)
{

    result = std::transform_reduce(
                 std::execution::par_unseq, A.begin(), A.end(), 
                 A.begin(), 0.0
             );
    result = std::sqrt(result);

    return;
}

template <typename T>
void
porescale::axpy(
    T               alpha,
    std::vector<T>& X,
    std::vector<T>& Y
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
    std::vector<T>&             B,
    T                           beta,
    std::vector<T>&             C
)
{

    // Check matrix format
    if (A.sparseFormat() == porescale::COO) {

    } else if (A.sparseFormat() == porescale::CSR) {

    } else
        // error out
    return;
}

//--- Explicit Instantiations ---//
template void porescale::norm<float>(std::vector<float>&, float&);
template void porescale::norm<double>(std::vector<double>&, double&);
template void porescale::axpy<float>(float, std::vector<float>&, std::vector<float>&);
template void porescale::axpy<double>(double, std::vector<double>&, std::vector<double>&);
template void porescale::sparseMatvec<float>(float, porescale::sparseMatrix<float>&, std::vector<float>&, float, std::vector<float>&);
template void porescale::sparseMatvec<double>(double, porescale::sparseMatrix<double>&, std::vector<double>&, double, std::vector<double>&);
