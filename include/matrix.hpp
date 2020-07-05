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
    /** \brief Set global index of first row in local matrix. */
    void setFirstRow(psInt firstRow);
    /** \brief Set global index of first column in local matrix. */
    void setFirstColumn(psInt firstColumn);
    /** \brief Set index of pe containing north neighboring submatrix. */
    void setNorthNeighbor(psInt northNeighbor);
    /** \brief Set index of pe containing west neighboring submatrix. */
    void setWestNeighbor(psInt westNeighbor);
    /** \brief Set index of pe containing south neighboring submatrix. */
    void setSouthNeighbor(psInt southNeighbor);
    /** \brief Set index of pe containing east neighboring submatrix. */
    void setEastNeighbor(psInt eastNeighbor);

    // Gets
    /** \brief Return the current pe. */
    psInt myPe(void) const;
    /** \brief Return the global number of pes. */
    psInt nPes(void) const;
    /** \brief Return the global number of rows. */
    psInt globalRows(void) const;
    /** \brief Returns the local number of rows. */
    psInt localRows(void) const;
    /** \brief Return the global number of columns. */
    psInt globalColumns(void) const;
    /** \brief Return the local number of columns. */
    psInt localColumns(void) const;
    /** \brief Return the global index of the first row in local matrix. */
    psInt firstRow(void) const;
    /** \brief Return the global index of the first column in local matrix. */
    psInt firstColumn(void) const;
    /** \brief Return the index of pe containing north neighboring submatrix. */
    psInt northNeighbor(void) const;
    /** \brief Return the index of pe containing west neighboring submatrix. */
    psInt westNeighbor(void) const;
    /** \brief Return the index of pe containing south neighboring submatrix. */
    psInt southNeighbor(void) const;
    /** \brief Return the index of pe containing east neighboring submatrix. */
    psInt eastNeighbor(void) const;

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
    /** \brief Update device data from host. */
    virtual void copyHostToDevice(void) = 0;
    /** \brief Update host data from device. */
    virtual void copyDeviceToHost(void) = 0;
    /** \brief Zero all matrix data. */
    virtual void zero(void) = 0;

protected:
    psInt myPe_;                /**< Local pe. */
    psInt nPes_;              /**< Global number of pes. */

    psInt globalRows_;          /**< Global number of rows. */
    psInt localRows_;           /**< Local number of rows */
    psInt globalColumns_;       /**< Global number of columns. */
    psInt localColumns_;        /**< Local number of columns. */

    psInt firstRow_;            /**< Global index of first row in this pe. */
    psInt firstColumn_;         /**< Global index of first column in this pe. */

    psInt northNeighbor_;       /**< Pe containing north neighboring submatrix. */
    psInt westNeighbor_;        /**< Pe containing west neighboring submatrix. */
    psInt southNeighbor_;       /**< Pe containing east neighboring submatrix. */
    psInt eastNeighbor_;        /**< Pe containing west neighboring submatrix. */

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
    void buildZero( psInt localRows,      psInt globalRows,
                    psInt localColumns,   psInt globalColumns,
                    psInt localNnz,       psInt globalNnz,
                    psSparseFormat format );

    /** \brief Build from input arrays distributed on pes. */
    void buildPar( psInt   localRows,    psInt   globalRows,
                   psInt   localColumns, psInt   globalColumns,
                   psInt   localNnz,     psInt   globalNnz,
                   psInt * colArray,     psInt * rowArray,
                   T     * valueArray,   psSparseFormat format );

    /** \brief Build from master pe and distribute. */
    void buildSeq( psInt   globalRows,   psInt   globalColumns,
                   psInt   globalNnz,    psInt * colArray,
                   psInt * rowArray,     T     * valueArray,
                   psSparseFormat format );

    // Accessors
    /** \brief Pointer to column array. */
    psInt * columnArray(void);
    /** \brief Pointer to row array. */
    psInt * rowArray(void);
    /** \brief Pointer to value array. */
    T * valueArray(void);

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
    /** \brief Allocates memory based on number of nonzeros.
     *         If memory was already allocated, deletes and re-allocates.
     */
    virtual void allocate(void);
    /** \brief Allocates memory based on number of nonzeros and sets all entries to 0.
     *         If already allocated, deletes and re-allocates.
     */
    virtual void allocateZero(void);
    /** \brief Copy memory to device. */
    virtual void copyHostToDevice(void);
    /** \brief Copy memory back to host. */
    virtual void copyDeviceToHost(void);
    /** \brief Zero all elements of the matrix. */
    virtual void zero(void);

    // IO
    /** Read in MTX file and distribute. */
    void readMTX(void);
    /** Coalesce and write MTX file. */
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
