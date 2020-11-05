/* solvers header */
#ifndef _PORESCALE_SOLVE_H_
#define _PORESCALE_SOLVE_H_

#include "parameters.hpp"
#include "types.hpp"
#include "define.hpp"
#include "matrix.hpp"

#include <execution>

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
    solver(porescale::parameters<T> * par);

    /** \brief Init from parameters. */
    virtual void init(parameters<T> * par) = 0;

    /** \brief Abstract solver build function. */
    virtual void assemble(void) = 0;

    /** \brief Set the solver matrix. */
    virtual void setMatrix(porescale::sparseMatrix<T>& matrix) = 0;

    /** \brief Set verbose level. */
    void setVerbose(psUInt);

    /** \brief Set if solver is assembled. */
    void setAssembled(bool);

    /** \brief Return true if the solver has been assembled. Else return 0. */
    bool assembled(void) const;

    /** \brief Pure virtual solve function. Defined in derived solvers. */
    virtual void solve(porescale::vector<T>& rhs, porescale::vector<T>& sol) = 0;

  protected:

    bool assembled_;		/**< Flag determining if solver has been assembled. */
    psUInt verbose_;		/**< How verbose should solver printing be. */

    /** \brief Pointer to the solver matrix. */
    porescale::sparseMatrix<T> *matrix_;
    /** \brief Pointer to preconditioner. */
    porescale::solver<T> *preconditioner_;

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
    virtual void init(parameters<T> * par) = 0;

    /** Gets */
    bool    checkResidual(void) const;			/** Check if residual check is enabled. */
    psInt   minIterations(void) const;			/** Return minimum iterations. */
    psInt   maxIterations(void) const;			/** Return maximum iterations. */
    psInt   iterations(void) const;			/** Iterations in last solve. */
    T       relativeTolerance(void) const;		/** Return solver relative tolerance. */
    T       absoluteTolerance(void) const;		/** Return solver absolute tolerance. */
    T       initialResidual(void) const;		/** Return initial residual. */
    T       currentResidual(void) const;		/** Return current residual. */

    /** Sets */
    void setCheckResidual(bool checkRes);		/** Set residual check. */
    void setMinIterations(psInt minIterations);		/** Set minimum iterations. */
    void setMaxIterations(psInt maxIterations);		/** Set maximum iterations. */
    void setRelativeTolerance(T relativeTolerance);	/** Set relative tolerance. */
    void setAbsoluteTolerance(T absoluteTolerance);	/** Set absolute tolerance. */

    /** \brief Set the solver matrix. */
    virtual void setMatrix(porescale::sparseMatrix<T>& matrix) = 0;

    /** Setup the stopping criteria. */
    void setup(psUInt maxIt, T absTolIn, T relTolIn);

    /** Solve the system. */
    void solve(porescale::vector<T>& rhs, porescale::vector<T>* sol);

    /** Set the preconditioner. */
    void setPreconditioner(porescale::solver<T>& preconditioner);

  protected:
    bool    checkResidual_;
    psInt   minIterations_;
    psInt   maxIterations_;
    psInt   iterations_;
    T       relativeTolerance_;
    T       absoluteTolerance_;
    T       initialResidual_;
    T       currentResidual_;

    /** \brief Pure abstract function to solve with no preconditioner. */
    virtual void solveNoPreconditioner(porescale::vector<T>& rhs, porescale::vector<T>* sol) = 0;
    /** \brief Pure abstract function to solve with preconditioner. */
    virtual void solvePreconditioner(porescale::vector<T>& rhs, porescale::vector<T>* sol) = 0;

    /** \brief Compute the residual. */
    void residualCheck(porescale::vector<T>& rhs, porescale::vector<T>* sol);
     /** \brief Scratch vector for storing residual calculations. */
    // need to define vector porescale::vector<T> residualVec_;

  };

  /** \brief CG solver derived class.
   *
   */
  template <typename T>
  class CGSolver : public iterativeSolver<T>
  {
  public:
    /** \brief Default constructor. */
    CGSolver(void);
    /** \brief Construct from parameters. */
    CGSolver(parameters<T> * par);

    /** \brief Init from parameters. */
    virtual void init(parameters<T> * par);

    /** \brief Solver build function. */
    virtual void build(void);

    /** \brief Sets pointer to matrix in linear system to be solved. */
    void setMatrix(porescale::sparseMatrix<T>& matrix);

    /** \brief Disabled assemble function. No assemble necessary for CG. */
    void assemble(void);

    private:
    /** \brief Scratch vectors used in CG iterations. */
    std::vector<T> r_, w_, p_, tmp_;
    /** \brief Function to solve with preconditioner. Called by inherited solve function. */
    void solvePreconditioner(porescale::vector<T>& rhs, porescale::vector<T> *sol);
    /** \brief Function to solve with no preconditioner. Called by inherited solve function. */
    void solveNoPreconditioner(porescale::vector<T>& rhs, porescale::vector<T> *sol);

  };

  /** \brief biCGStab solver derived class
   *
   */
  template <typename T>
  class biCGStabSolver : public iterativeSolver<T>
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
