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
    globalNnz_(0), localNnz_(0), colArray_(NULL), 
    rowArray_(NULL), valueArray_(NULL) { };

template <typename T>
porescale::sparseMatrix<T>::sparseMatrix(parameters<T> * par) :
    porescale::matrix<T>::matrix(par), sparseFormat_(CSR), 
    globalNnz_(0), localNnz_(0), colArray_(NULL), 
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
    this->rank_ = par->rank();
    this->nRanks_ = par->nRanks();
}

template <typename T>
void
porescale::sparseMatrix<T>::buildZero( 
    psInt localRows,      psInt globalRows, 
    psInt localColumns,   psInt globalColumns,
    psInt localNnz,       psInt globalNnz,
    psSparseFormat format
)
{
    this->setLocalRows(localRows);
    this->setGlobalRows(globalRows);
    this->setLocalColumns(localColumns);
    this->setGlobalColumns(globalColumns);
    this->setLocalNnz(localNnz);
    this->setGlobalNnz(globalNnz);

    allocate();
}

template <typename T>
void
porescale::sparseMatrix<T>::build( 
    psInt   localRows,      psInt            globalRows, 
    psInt   localColumns,   psInt            globalColumns,
    psInt   localNnz,       psInt            globalNnz,
    psInt * colArray,       psInt          * rowArray,
    T     * valueArray,     psSparseFormat   format
)
{
    this->setLocalRows(localRows);
    this->setGlobalRows(globalRows);
    this->setLocalColumns(localColumns);
    this->setGlobalColumns(globalColumns);
    this->setLocalNnz(localNnz);
    this->setGlobalNnz(globalNnz);

    allocate();



}

//--- Accessors ---//

//--- Sets ---//
template <typename T>
void
porescale::sparseMatrix<T>::setLocalNnz(psInt nnzIn) { localNnz_ = nnzIn; }

template <typename T>
void
porescale::sparseMatrix<T>::setGlobalNnz(psInt nnzIn) { globalNnz_ = nnzIn; }

//--- Gets ---//
template <typename T>
porescale::psSparseFormat porescale::sparseMatrix<T>::sparseFormat(void) const { return sparseFormat_; }

template <typename T>
psInt porescale::sparseMatrix<T>::globalNnz(void) const { return globalNnz_; }

template <typename T>
psInt porescale::sparseMatrix<T>::localNnz(void) const { return localNnz_; }

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
        colArray_ = new psInt[localNnz_];
        rowArray_ = new psInt[this->localRows_+1];
        valueArray_ = new T[localNnz_];
    }
    else if (sparseFormat_ == COO)
    {
        colArray_ = new psInt[localNnz_];
        rowArray_ = new psInt[localNnz_];
        valueArray_ = new T[localNnz_];
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
porescale::sparseMatrix<T>::copyHostToDevice(void)
{
    
}

template <typename T>
void
porescale::sparseMatrix<T>::copyDeviceToHost(void)
{

}

template <typename T>
void
porescale::sparseMatrix<T>::zero(void)
{
    if (sparseFormat_ == CSR)
    {
        for (int i = 0; i < localNnz_; i++) colArray_[i] = 0;
        for (int i = 0; i < this->localRows_+1; i++) rowArray_[i] = 0;
        for (int i = 0; i < localNnz_; i++) valueArray_[i] = 0.0;
    }
    else if (sparseFormat_ == COO)
    {
        for (int i = 0; i < localNnz_; i++) colArray_[i] = 0;
        for (int i = 0; i < localNnz_; i++) rowArray_[i] = 0;
        for (int i = 0; i < localNnz_; i++) valueArray_[i] = 0.0;
    }
}

//--- IO ---//

//--- Explicit Instantiations ---//
template class porescale::sparseMatrix<float>;
template class porescale::sparseMatrix<double>;