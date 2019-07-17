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
    /** \brief Set the global number of rows. */
    void setGlobalRows(psInt gRows);
    /** \brief Set the local number of rows. */
    void setLocalRows(psInt lRows);
    /** \brief Set the global number of columns. */
    void setGlobalColumns(psInt gColumns);
    /** \brief Set the local number of columns. */
    void setLocalColumns(psInt lColumns);

    // Gets
    /** \brief Return the current rank. */
    psInt rank(void) const;
    /** \brief Return the global number of ranks. */
    psInt nRanks(void) const;
    /** \brief Return the global number of rows. */
    psInt globalRows(void) const;
    /** \brief Returns the local number of rows. */
    psInt localRows(void) const;
    /** \brief Return the global number of columns. */
    psInt globalColumns(void) const;
    /** \brief Return the local number of columns. */
    psInt localColumns(void) const;

    // Converts

    // Memory
    /** \brief Allocates memory based on number of nonzeros and sets all entries to 0. 
     *  If memory was already allocated, data is wiped and set to 0.
     */
    virtual void allocate(void) = 0;
    /** \brief Update device data from host. */
    virtual void copyHostToDevice(void) = 0;
    /** \brief Update host data from device. */
    virtual void copyDeviceToHost(void) = 0;
    /** \brief Zero all matrix data. */
    virtual void zero(void) = 0;

protected:

    psInt rank_;                /**< Local rank. */ 
    psInt nRanks_;              /**< Global number of ranks. */

    psInt globalRows_;          /**< Global number of rows. */
    psInt localRows_;           /**< Local number of rows */
    psInt globalColumns_;       /**< Global number of columns. */
    psInt localColumns_;        /**< Local number of columns. */

    psInt firstRow_;            /**< Row index of first row in this rank. */
    psInt firstColumn_;         /**< Column index of first column in this rank. */

    bool allocated_;            /**< Flag indicating if matrix memory has been allocated. */

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

    /** \brief Build from input arrays. */

    // Accessors

    // Sets
    /** \brief Set the global number of nonzeros. */
    void          setGlobalNnz(psInt nnzIn);
    /** \brief Set the local number of nonzeros. */
    void          setLocalNnz(psInt nnzIn);

    // Gets
    /** \brief Return the sparse format. */
    psSparseFormat sparseFormat(void) const;
    /** \brief Return the global number of nonzeros. */
    psInt          globalNnz(void) const;
    /** \brief Return the local number of nonzeros. */
    psInt          localNnz(void) const;

    // Converts

    // Memory
    /** \brief Allocates memory based on number of nonzeros and sets all entries to 0. 
     *  If memory was already allocated, data is wiped and set to 0.
     */
    virtual void allocate(void);
    /** \brief Copy memory to device. */
    virtual void copyHostToDevice(void);
    /** \brief Copy memory back to host. */
    virtual void copyDeviceToHost(void);
    /** \brief Zero all elements of the matrix. */
    virtual void zero(void);

    // IO
    /** Read in MTX file and distribute. */
    void readMTX(void);
    /** Coalesce to 1 rank and write MTX file. */
    void writeMTX(void);
    /** Read parallel data in poreScale format.  */
    void parRead(void);
    /** Parallel write to poreScale format files. */
    void parWrite(void);

protected:

    psSparseFormat sparseFormat_;   /**< Tracks the sparse matrix format. */
    psInt          globalNnz_;      /**< Global number of nonzeros. */
    psInt          localNnz_;       /**< Local number of nonzeros. */

    // host data
    psInt * colArray_;              /**< Host column array. */
    psInt * rowArray_;              /**< Host row array. */
    T     * valueArray_;            /**< Host value array. */

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
