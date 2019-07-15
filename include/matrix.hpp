/**
 * \file
 * \author Timothy B. Costa
 * \brief Matrix header file.
 */

#ifndef _PORESCALE_MATRIX_H_
#define _PORESCALE_MATRIX_H_

namespace porescale
{
    
/** \brief Abstract base matrix class
 * 
 */
template <typename T>
class matrix
{

};

/** \brief Sparse matrix derived class
 * 
 */
template <typename T>
class sparseMatrix : public matrix<T>
{

};

/** \brief Dense matrix derived class
 * 
 */
template <typename T>
class denseMatrix : public matrix<T>
{

};

template <typename T>
/** \brief Vector derived class */
class vector : public denseMatrix<T>
{

};

}

#endif
