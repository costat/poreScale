/**
 * \file
 * \author Timothy B. Costa
 * \brief Source for base solver class.
 */

#include "solve.hpp"

///// Solver base class /////

//--- Constructors ---//
template <typename T>
porescale::solver<T>::solver(void) : built_(0) { }

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
porescale::iterativeSolver<T>::iterativeSolver(parameters<T> * par) : solver<T>::solver(),
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

//--- Explicit Instantiations ---//
template class porescale::iterativeSolver<float>;
template class porescale::iterativeSolver<double>;