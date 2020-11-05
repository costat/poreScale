/**
 * \file
 * \author Timothy B. Costa
 * \brief Source for base solver class.
 */

#include "solve.hpp"

///// Solver base class /////

//--- Constructors ---//
template <typename T>
porescale::solver<T>::solver(void) : assembled_(false), verbose_(1) { }

template <typename T>
porescale::solver<T>::solver(porescale::parameters<T> * par) : assembled_(false), verbose_(1) { }


//--- Sets ---//
template <typename T>
void
porescale::solver<T>::setVerbose(psUInt verbose) { verbose_ = verbose; }

template <typename T>
void
porescale::solver<T>::setAssembled(bool assembled) { assembled_ = assembled; }

//--- Gets ---//
template <typename T>
bool
porescale::solver<T>::assembled(void) const { return assembled_; }

//--- Explicit Instantiations ---//
template class porescale::solver<float>;
template class porescale::solver<double>;

///// Iterative solver class /////
template <typename T>
porescale::iterativeSolver<T>::iterativeSolver(void) : solver<T>::solver(),
    checkResidual_(true), minIterations_(0), maxIterations_(100),
    relativeTolerance_(1e-4), absoluteTolerance_(1e-8),
    initialResidual_(-1), currentResidual_(-1)
{ };

template <typename T>
porescale::iterativeSolver<T>::iterativeSolver(parameters<T> * par) : solver<T>::solver(par),
    checkResidual_(true), minIterations_(0),
    initialResidual_(-1), currentResidual_(-1)
{
    maxIterations_ = par->solverMaxIterations();
    relativeTolerance_ = par->solverRelativeTolerance();
    absoluteTolerance_ = par->solverAbsoluteTolerance();
};

/** Gets */
template <typename T>
bool
porescale::iterativeSolver<T>::checkResidual(void) const { return checkResidual_; }

template <typename T>
psInt
porescale::iterativeSolver<T>::minIterations(void) const { return minIterations_; }

template <typename T>
psInt
porescale::iterativeSolver<T>::maxIterations(void) const { return maxIterations_; }

template <typename T>
psInt
porescale::iterativeSolver<T>::iterations(void) const { return iterations_; }

template <typename T>
T
porescale::iterativeSolver<T>::relativeTolerance(void) const { return relativeTolerance_; }

template <typename T>
T
porescale::iterativeSolver<T>::absoluteTolerance(void) const { return absoluteTolerance_; }

template <typename T>
T
porescale::iterativeSolver<T>::initialResidual(void) const { return initialResidual_; }

template <typename T>
T
porescale::iterativeSolver<T>::currentResidual(void) const { return currentResidual_; }

/** Sets */
template <typename T>
void
porescale::iterativeSolver<T>::setCheckResidual(bool checkRes) { checkResidual_ = checkRes; }

template <typename T>
void
porescale::iterativeSolver<T>::setMinIterations(psInt minIterations) { minIterations_ = minIterations; }

template <typename T>
void
porescale::iterativeSolver<T>::setMaxIterations(psInt maxIterations) { maxIterations_ = maxIterations; }

template <typename T>
void
porescale::iterativeSolver<T>::setRelativeTolerance(T relativeTolerance) { relativeTolerance_ = relativeTolerance; }

template< typename T>
void
porescale::iterativeSolver<T>::setAbsoluteTolerance(T absoluteTolerance) { absoluteTolerance_ = absoluteTolerance; }

//--- Setup and Solve ---//
template <typename T>
void
porescale::iterativeSolver<T>::setup(
    psUInt maxIt,
    T      absTolIn,
    T      relTolIn
)
{
    setMaxIterations(maxIt);
    setAbsoluteTolerance(absTolIn);
    setRelativeTolerance(relTolIn);
}

template <typename T>
void
porescale::iterativeSolver<T>::setPreconditioner(
    porescale::solver<T>& preconditioner
)
{
    this->preconditioner_ = &preconditioner;
}

template <typename T>
void
porescale::iterativeSolver<T>::solve(
    porescale::vector<T>&  rhs,
    porescale::vector<T> * sol
)
{
    if (this->preconditioner_ == nullptr)
        this->solveNoPreconditioner(rhs, sol);
    else
        this->solvePreconditioner(rhs, sol);
    return;
}

template <typename T>
void
porescale::iterativeSolver<T>::residualCheck(
    porescale::vector<T>& rhs,
    porescale::vector<T>* sol
)
{
    T one = 1.0;
    T zero = 0.0;

    if (residualVec_.size() != rhs.size())
        residualVec_.resize(rhs.size());

    porescale::sparseMatvec(-one, *this->matrix_, *sol, zero, residualVec_);
    porescale::axpy(one, rhs, residualVec_);
    porescale::norm(residualVec_, currentResidual_);
}

//--- Explicit Instantiations ---//
template class porescale::iterativeSolver<float>;
template class porescale::iterativeSolver<double>;

