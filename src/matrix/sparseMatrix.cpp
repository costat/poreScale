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
porescale::sparseMatrix<T>::~sparseMatrix(void) {}

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
    this->setSparseFormat(format);

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

    std::copy(colArray, colArray+nnz, colArray_.data());
    if (format == porescale::COO)
        std::copy(rowArray, rowArray+nnz, rowArray_.data());
    else if (format == porescale::CSR)
        std::copy(rowArray, rowArray+rows+1, rowArray_.data());
    std::copy(valueArray, valueArray+nnz, valueArray_.data());
}

//--- Accessors ---//
template <typename T>
psInt *
porescale::sparseMatrix<T>::columnArray(void) { return colArray_.data(); }

template <typename T>
psInt *
porescale::sparseMatrix<T>::rowArray(void) { return rowArray_.data(); }

template <typename T>
T *
porescale::sparseMatrix<T>::valueArray(void) { return valueArray_.data(); }

//--- Sets ---//
template <typename T>
void
porescale::sparseMatrix<T>::setNnz(psInt nnzIn) { nnz_ = nnzIn; }

template <typename T>
void
porescale::sparseMatrix<T>::setSparseFormat(psSparseFormat format) { sparseFormat_ = format; }

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

    if (sparseFormat_ == CSR)
    {
        colArray_.resize(nnz_);
        rowArray_.resize(this->rows_+1);
        valueArray_.resize(nnz_);
    }
    else if (sparseFormat_ == COO)
    {
        colArray_.resize(nnz_);
        rowArray_.resize(nnz_);
        valueArray_.resize(nnz_);
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
porescale::sparseMatrix<T>::readMTX(
    std::string& matrixFile
)
{
    psUInt tmpRows_, tmpCols_, tmpEntries_;

    std::ifstream matIn(matrixFile.c_str());

    // parse through comments
    while (matIn.peek() == '%') matIn.ignore(2048, '\n');

    // grab dimension
    matIn >> tmpRows_ >> tmpCols_ >> tmpEntries_;
    this->setRows(tmpRows_);
    this->setColumns(tmpCols_);
    this->setNnz(tmpEntries_);
    diagIdx_.resize(tmpRows_);
    rowArray_.resize(tmpEntries_);
    colArray_.resize(tmpEntries_);
    valueArray_.resize(tmpEntries_);

    for (int i = 0; i < nnz(); i++)
    {
        matIn >> rowArray_[i] >> colArray_[i] >> valueArray_[i];
        rowArray_[i]--;
        colArray_[i]--;
    }
    matIn.close();

   // Sort COO


    return;
}

template <typename T>
void
porescale::sparseMatrix<T>::writeMTX(
    std::string& matrixFile
)
{
    std::ofstream outstream;
    outstream.open(matrixFile);

    outstream << "%%MatrixMarket coordinate real\n";
    outstream << "%---------------------------------------------------\n";
    outstream << "%---------------------------------------------------\n";
    outstream << this->rows() << " " << this->columns() << " " << this->nnz() << "\n";

    if (this->sparseFormat_ == COO)
    {
        for (int i = 0; i < this->nnz_; i++)
        {
            outstream << (this->rowArray_[i]+1) << " " << (this->colArray_[i]+1) << " " << this->valueArray_[i] << "\n";
        }
    }
    else if (this->sparseFormat() == CSR)
    {
        int rowStart, rowEnd;
        for (int i = 0; i < this->rows(); i++)
        {
            rowStart = this->rowArray_[i];
            rowEnd = this->rowArray_[i+1];
            for (int idx = rowStart; idx < rowEnd; idx++)
            {
                outstream << (i+1) << " " << (this->colArray_[idx]+1) << " " << this->valueArray_[idx] << "\n";
            }
        }
    }
    else
    {
        outstream << "PS ERROR : Unsupported matrix format in writeMtx.\n";
    }
    outstream.close();

}

//--- Explicit Instantiations ---//
template class porescale::sparseMatrix<float>;
template class porescale::sparseMatrix<double>;

