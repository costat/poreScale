/**
 * \file
 * \author Timothy B. Costa
 * \brief Matrix header file.
 */

#ifndef _PORESCALE_MATRIX_H_
#define _PORESCALE_MATRIX_H_

#include "parameters.hpp"
#include "types.hpp"
#include "define.hpp"

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
    /** \brief Set the number of rows. */
    void setRows(psInt gRows);
    /** \brief Set the number of columns. */
    void setColumns(psInt gColumns);

    // Gets
    /** \brief Return the number of rows. */
    psInt rows(void) const;
    /** \brief Return the number of columns. */
    psInt columns(void) const;

    // Convert

    // Memory
    /** \brief Allocates memory based on number of nonzeros.
     *         If already allocated, deletes and re-allocates.
     */
    virtual void allocate(void) = 0;
    /** \brief Allocates memory based on number of nonzeros and sets all entries to 0.
     *         if already allocated, deletes and re-allocates.
     */
    virtual void allocateZero(void) = 0;
    /** \brief Zero all matrix data. */
    virtual void zero(void) = 0;

protected:
    psInt rows_;          	/**< Global number of rows. */
    psInt columns_;       	/**< Global number of columns. */

    bool allocated_;            /**< Flag indicating if matrix memory has been allocated. */
    bool built_;                /**< Flag indicating if the matrix has been built. */

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

    /** \brief Destructor */
    ~sparseMatrix(void);

    /** \brief Initialize as copy of another matrix. */
    void init(parameters<T> * par);

    /** \brief Build with 0'd values. */
    void buildZero( psInt rows,
                    psInt columns,
                    psInt nnz,
                    psSparseFormat format );

    /** \brief Build from master pe and distribute. */
    void build( psInt   rows,   	psInt   columns,
                psInt   nnz,    	psInt * colArray,
                psInt * rowArray,       T     * valueArray,
                psSparseFormat format );

    // Accessors
    /** \brief Pointer to column array. */
    psInt * columnArray(void);
    /** \brief Pointer to row array. */
    psInt * rowArray(void);
    /** \brief Pointer to value array. */
    T * valueArray(void);

    // Sets
    /** \brief Set the number of nonzeros. */
    void          setNnz(psInt nnzIn);

    /** \brief Set the sparse format of the matrix. */
    void          setSparseFormat(psSparseFormat format);

    // Gets
    /** \brief Return the sparse format. */
    psSparseFormat sparseFormat(void) const;
    /** \brief Return the number of nonzeros. */
    psInt          nnz(void) const;

    // Converts

    // Memory
    /** \brief Allocates memory based on number of nonzeros.
     *         If memory was already allocated, deletes and re-allocates.
     */
    virtual void allocate(void);
    /** \brief Allocates memory based on number of nonzeros and sets all entries to 0.
     *         If already allocated, deletes and re-allocates.
     */
    virtual void allocateZero(void);

    /** \brief Zero all elements of the matrix. */
    virtual void zero(void);

    // IO
    /** Read in MTX file and distribute. */
    void readMTX(std::string& matrixFile);
    /** Coalesce and write MTX file. */
    void writeMTX(void);

protected:

    psSparseFormat sparseFormat_;   /**< Tracks the sparse matrix format. */
    psInt          nnz_;      	    /**< Number of nonzeros. */

    // host data
    std::vector<psInt> colArray_;   /**< Column array. */
    std::vector<psInt> rowArray_;   /**< Row array. */
    std::vector<psInt> diagIdx_;    /**< Store indices of diagonal elements. */
    std::vector<T>     valueArray_; /**< Value array. */

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
