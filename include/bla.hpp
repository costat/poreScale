/* linear algebra header */
#ifndef _PORESCALE_BLA_H_
#define _PORESCALE_BLA_H_

#include "matrix.hpp"
#include "types.hpp"
#include "define.hpp"

#include <iostream>

namespace porescale {

//--- DENSE BLAS ---//
//--- L1 BLAS ---//

/** \brief Performs the operation
 *          result = ||A||_2
 *          where A is a vector, and result is a float of type T.
*/
template <typename T>
void norm(std::vector<T>& A, T& result);

/** \brief Performs the operation
 *          Y = alpha * X + Y
 *         where X and Y are vectors and alpha is a float of type T.
 */
template <typename T>
void axpy(T alpha, std::vector<T>& X, std::vector<T>& Y);

//--- SPARSE BLAS ---//
//--- L2 BLAS ---//

/** \brief Performs he operation
 *          C = alpha * A * B + beta C
 *          where A is a sparse matrix, B and C are vectors, and alpha and beta are floats of type T.
 */
template <typename T>
void sparseMatvec(T alpha, porescale::sparseMatrix<T>& A, std::vector<T>& B, T beta, std::vector<T>& C);


} // namespace porescale

#endif

