/**
 * \file
 * \author Timothy B. Costa
 * \brief Matrix header file.
 */

#ifndef _PORESCALE_MATRIX_H_
#define _PORESCALE_MATRIX_H_

#include "parameters.hpp"
#include "types.hpp"

namespace porescale
{
    
/** \brief Abstract base matrix class
 * 
 */
template <typename T>
class matrix
{
public:
    /** \brief Default constructor. */
    matrix(void);

    /** \brief Construct from parameters. */
    matrix(parameters<T> * par);

    /** \brief Init from parameters. */
    void init(parameters<T> * par);

    // Sets
    void setGlobalRows(psInt gRows);
    void setLocalRows(psInt lRows);
    void setGlobalColumns(psInt gColumns);
    void setLocalColumns(psInt lColumns);

    // Gets
    psInt rank(void) const;
    psInt nRanks(void) const;
    psInt globalRows(void) const;
    psInt localRows(void) const;
    psInt globalColumns(void) const;
    psInt localColumns(void) const;

    virtual void allocate(void) = 0;

protected:

    psInt rank_;
    psInt nRanks_; 

    psInt globalRows_;
    psInt localRows_;
    psInt globalColumns_;
    psInt localColumns_;

    psInt firstRow_;
    psInt firstColumn_;

    bool allocated_;

};

/** \brief Sparse matrix derived class
 * 
 */
template <typename T>
class sparseMatrix : public matrix<T>
{
public:
    /** \brief Default constructor. */
    sparseMatrix(void);
    /** \brief Construct from parameters. */
    sparseMatrix(parameters<T> * par);

    /** \brief Initialize as copy of another matrix. */
    void init(parameters<T> * par);

    /** \brief Initialize from 3-arrays. */

    // Sets
    void          setNnz(psInt nnzIn);

    // Gets
    psSparseFormat sparseFormat(void) const;
    psInt          nnz(void) const;

    // Converts

    virtual void allocate(void);

protected:

    psSparseFormat sparseFormat_;
    psInt          nnz_;


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
