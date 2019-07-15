/* solvers header */
#ifndef _PORESCALE_SOLVE_H_
#define _PORESCALE_SOLVE_H_

#include "parameters.hpp"

// system includes
#include <vector>

namespace porescale
{
  /** \brief Solver class used by model solve functions.
   * 
   */
  template <typename T>
  class solver
  {
  public:
    /** \brief Default constructor. */
    solver(void);
    /** \brief Construct from parameters. */
    solver(parameters<T> * par);

    /** \brief Init from parameters. */
    virtual psErr init(parameters<T> * par) = 0;

    /** \brief Abstract solver build function. */
    virtual psErr build(void) = 0;

  protected:

    bool built_;                /**< Flag determining if solver has been built. */

    parameters<T> * par_;       /**< Pointer to parameters object. */

  };

  /** \brief Iterative solver derived class.
   * 
   */
  template <typename T>
  class iterativeSolver : public solver<T>
  {
  public:
    /** \brief Default constructor. */
    iterativeSolver(void);
    /** \brief Construct from parameters. */
    iterativeSolver(parameters<T> * par);

    /** \brief Init from parameters. */
    virtual psErr init(parameters<T> * par) = 0;

    /** \brief Abstract solver build function. */
    virtual psErr build(void) = 0;

    /** Gets */
    bool    checkResidual(void) const;
    psInt minIterations(void) const;
    psInt maxIterations(void) const;
    T       relativeTolerance(void) const;
    T       absoluteTolerance(void) const;
    T       initialResidual(void) const;
    T       currentResidual(void) const;

    /** Sets */
    psErr setCheckResidual(bool checkRes);
    psErr setMinIterations(psInt minIterations);
    psErr setMaxIterations(psInt maxIterations);
    psErr setRelativeTolerance(T relativeTolerance);
    psErr setAbsoluteTolerance(T absoluteTolerance);

  protected:
    bool    checkResidual_;
    psInt minIterations_;
    psInt maxIterations_;
    T       relativeTolerance_;
    T       absoluteTolerance_;
    T       initialResidual_;
    T       currentResidual_;

  };

  /** \brief Krylov solver derived class.
   * 
   */
  template <typename T>
  class krylovSolver : public iterativeSolver<T>
  {
  public:
    /** \brief Default constructor. */
    krylovSolver(void);
    /** \brief Construct from parameters. */
    krylovSolver(parameters<T> * par);

    /** \brief Init from parameters. */
    virtual psErr init(parameters<T> * par) = 0;

    /** \brief Abstract solver build function. */
    virtual psErr build(void) = 0;
  };

  /** \brief CG solver derived class.
   * 
   */
  template <typename T>
  class CGSolver : public krylovSolver<T>
  {
  public:
    /** \brief Default constructor. */
    CGSolver(void);
    /** \brief Construct from parameters. */
    CGSolver(parameters<T> * par);

    /** \brief Init from parameters. */
    virtual psErr init(parameters<T> * par);

    /** \brief Solver build function. */
    virtual psErr build(void);

  };

  /** \brief biCGStab solver derived class
   * 
   */
  template <typename T>
  class biCGStabSolver : public krylovSolver<T>
  {

  };

  /** \brief Multigrid Solver derived class
   * 
   */
  template <typename T>
  class multigridSolver : public iterativeSolver<T>
  {
    
  };

  /** \brief AMG Solver derived class.
   * 
   */
  template <typename T>
  class AMGSolver : public multigridSolver<T>
  {

  };

  /** \brief Geometric multigrid derived class.
   * 
   */
  template <typename T>
  class geometricMultigridSolver : public multigridSolver<T>
  {

  };

  /** \brief Smoother derived class. 
   * 
   */
  template <typename T>
  class smoother : public iterativeSolver<T>
  {

  };

  /** \brief Preconditioner derived class.
   * 
   */
  template <typename T>
  class preconditioner : public solver<T>
  {

  };

  /** \brief Direct solver derived class.
   * 
   */
  template <typename T>
  class directSolver : public solver<T>
  {

  };

}

#endif
