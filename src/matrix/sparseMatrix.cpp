/**
 * \file
 * \author Timothy B. Costa
 * \brief Source for sparseMatrix class.
 */

#include "matrix.hpp"

//--- Constructors ---//
template <typename T>
porescale::sparseMatrix<T>::sparseMatrix(void) :
    porescale::matrix<T>::matrix(), sparseFormat_(CSR),
    nnz_(0), colArray_(NULL),
    rowArray_(NULL), valueArray_(NULL) { };

template <typename T>
porescale::sparseMatrix<T>::sparseMatrix(parameters<T> * par) :
    porescale::matrix<T>::matrix(par), sparseFormat_(CSR),
    nnz_(0), colArray_(NULL),
    rowArray_(NULL), valueArray_(NULL) { };

//--- Destructor ---//
template <typename T>
porescale::sparseMatrix<T>::~sparseMatrix(void)
{
    if (this->allocated_)
    {
        delete[] colArray_;
        delete[] rowArray_;
        delete[] valueArray_;
    }
}

//--- Initiation and build ---//
template <typename T>
void
porescale::sparseMatrix<T>::init(porescale::parameters<T> * par)
{

}

template <typename T>
void
porescale::sparseMatrix<T>::buildZero(
    psInt rows,
    psInt columns,
    psInt nnz,
    psSparseFormat format
)
{
    this->setRows(rows);
    this->setColumns(columns);
    this->setNnz(nnz);

    allocateZero();
}

template <typename T>
void
porescale::sparseMatrix<T>::build(
    psInt   	     rows,
    psInt   	     columns,
    psInt   	     nnz,
    psInt 	   * colArray,
    psInt 	   * rowArray,
    T     	   * valueArray,
    psSparseFormat   format
)
{
    this->setRows(rows);
    this->setColumns(columns);
    this->setNnz(nnz);

    allocate();

    std::copy(colArray, colArray+nnz, colArray_);
    if (format == porescale::COO)
        std::copy(rowArray, rowArray+nnz, rowArray_);
    else if (format == porescale::CSR)
        std::copy(rowArray, rowArray+rows+1, rowArray_);
    std::copy(valueArray, valueArray+nnz, valueArray_);
}

//--- Accessors ---//
template <typename T>
psInt *
porescale::sparseMatrix<T>::columnArray(void) { return colArray_; }

template <typename T>
psInt *
porescale::sparseMatrix<T>::rowArray(void) { return rowArray_; }

template <typename T>
T *
porescale::sparseMatrix<T>::valueArray(void) { return valueArray_; }

//--- Sets ---//
template <typename T>
void
porescale::sparseMatrix<T>::setNnz(psInt nnzIn) { nnz_ = nnzIn; }

//--- Gets ---//
template <typename T>
porescale::psSparseFormat porescale::sparseMatrix<T>::sparseFormat(void) const { return sparseFormat_; }

template <typename T>
psInt porescale::sparseMatrix<T>::nnz(void) const { return nnz_; }

//--- Converts ---//

//--- Memory ---//
template <typename T>
void
porescale::sparseMatrix<T>::allocate(void)
{

    if (this->allocated_)
    {
        delete[] colArray_;
        delete[] rowArray_;
        delete[] valueArray_;
    }
    if (sparseFormat_ == CSR)
    {
        colArray_ = new psInt[nnz_];
        rowArray_ = new psInt[this->rows_+1];
        valueArray_ = new T[nnz_];
    }
    else if (sparseFormat_ == COO)
    {
        colArray_ = new psInt[nnz_];
        rowArray_ = new psInt[nnz_];
        valueArray_ = new T[nnz_];
    }

    this->allocated_ = true;
}

template <typename T>
void
porescale::sparseMatrix<T>::allocateZero(void)
{

    allocate();
    zero();
}

template <typename T>
void
porescale::sparseMatrix<T>::zero(void)
{
    if (sparseFormat_ == CSR)
    {
        for (int i = 0; i < nnz_; i++) colArray_[i] = 0;
        for (int i = 0; i < this->rows_+1; i++) rowArray_[i] = 0;
        for (int i = 0; i < nnz_; i++) valueArray_[i] = 0.0;
    }
    else if (sparseFormat_ == COO)
    {
        for (int i = 0; i < nnz_; i++) colArray_[i] = 0;
        for (int i = 0; i < nnz_; i++) rowArray_[i] = 0;
        for (int i = 0; i < nnz_; i++) valueArray_[i] = 0.0;
    }
}

//--- IO ---//
template <typename T>
void
readMTX(void)
{

}

template <typename T>
void
writeMTX(void)
{

}

//--- Explicit Instantiations ---//
template class porescale::sparseMatrix<float>;
template class porescale::sparseMatrix<double>;

