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
    nnz_(0), sorted_(0), colArray_(NULL),
    rowArray_(NULL), valueArray_(NULL) { };

template <typename T>
porescale::sparseMatrix<T>::sparseMatrix(parameters<T> * par) :
    porescale::matrix<T>::matrix(par), sparseFormat_(CSR),
    nnz_(0), sorted_(0), colArray_(NULL),
    rowArray_(NULL), valueArray_(NULL) { };

//--- Destructor ---//
template <typename T>
porescale::sparseMatrix<T>::~sparseMatrix(void) {}

//--- Initiation and build ---//
template <typename T>
void
porescale::sparseMatrix<T>::init(porescale::parameters<T> * par)
{ }

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
    this->setSparseFormat(format);

    allocate();

    std::copy(std::execution::par_unseq, colArray, colArray+colArray_.size(), colArray_.data());
    std::copy(std::execution::par_unseq, rowArray, rowArray+rowArray_.size(), rowArray_.data());
    std::copy(std::execution::par_unseq, valueArray, valueArray+valueArray_.size(), valueArray_.data());
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

template <typename T>
void
porescale::sparseMatrix<T>::setSorted(bool sorted) { sorted_ = sorted; }

//--- Gets ---//
template <typename T>
porescale::psSparseFormat porescale::sparseMatrix<T>::sparseFormat(void) const { return sparseFormat_; }

template <typename T>
psInt porescale::sparseMatrix<T>::nnz(void) const { return nnz_; }

template <typename T>
bool porescale::sparseMatrix<T>::sorted(void) const {return sorted_; }

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
    std::fill(std::execution::par_unseq, colArray_.begin(), colArray_.end(), 0);
    std::fill(std::execution::par_unseq, rowArray_.begin(), rowArray_.end(), 0);
    std::fill(std::execution::par_unseq, valueArray_.begin(), valueArray_.end(), 0);
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
    sortCOO();

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

// Manipulation
template <typename T>
void
porescale::sparseMatrix<T>::sortCOO()
{
    if (sparseFormat() != COO) return;

    std::vector<porescale::arrayCOO<T>> tempCOO(nnz());

    for (int i = 0; i < nnz(); i++)
    {
        tempCOO[i].i_index = rowArray_[i];
        tempCOO[i].j_index = colArray_[i];
        tempCOO[i].value = valueArray_[i];
    }

    sortCOObyIbyJ<T> sortStruct;
    std::sort(std::execution::par_unseq, tempCOO.begin(), tempCOO.end(), sortStruct);

    for (int i = 0; i < nnz(); i++)
    {
        rowArray_[i] = tempCOO[i].i_index;
        colArray_[i] = tempCOO[i].j_index;
        valueArray_[i] = tempCOO[i].value;
    }

    this->sorted_ = 1;

    return;
}

//--- Explicit Instantiations ---//
template class porescale::sparseMatrix<float>;
template class porescale::sparseMatrix<double>;

