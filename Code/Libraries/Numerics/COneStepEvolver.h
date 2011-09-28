#ifndef C_ONESTEPEVOLVER_H
#define C_ONESTEPEVOLVER_H

#include <stdexcept>

#include "VectorImage.h"
#include "VectorField.h"

/** 
 * COneStepEvolver.h -- Base class that defines the general interface for 
 * numerical solvers to perform one update step. There is no testing
 * for numerical stability. The step is simply executed. Choosing an 
 * appropriate step-size is the resposibility of the caller. If implemented
 * by the derived class ComputeMaximalUpdateStep() can be used to get 
 * a suggested step size.
 *
 * Memory allocation is assumed to be external, so that it can be optimized
 * on an algorithm by algorithm case.
 *
 * Provided, so we can easily subsitute different solvers for the high
 * level solution algorithms
 *
 */

namespace CALATK
{

template <class T, unsigned int VImageDimension=3, class TSpace = T >
class COneStepEvolver {

public:

  /** Some useful typedefs */
  
  typedef VectorField< T, VImageDimension, TSpace > VectorFieldType;
  typedef VectorImage< T, VImageDimension, TSpace > VectorImageType;

  /********************************
   * Constructors and Destructors *
   ********************************/

  /**
   * Empty Constructor
   */
  COneStepEvolver();

  /**
   * Computes a maximial step size (implementation optional)
   *
   * @param v - vector field to be evolved by
   */
  virtual T ComputeMaximalUpdateStep( VectorFieldType* v  ) const;

  /**
   * Performs an update step, to be implemented by the derived class (pure virtual).
   *
   * @param v - vector field to be evolved by
   * @param In - initial image (may be scalar or vector values as for a map)
   * @param Inp1 - resulting evolved image
   * @param dt - time step
   */
  virtual void PerformStep( VectorFieldType* v, VectorImageType* In, VectorImageType* Inp1, T dt ) = 0;


  /**
   * Determines the number of desired threads for parallel implementations. 
   * Default is 1, i.e., no threading.
   *
   * @params uiThreads - number of threads
   */
  void SetNumberOfThreads( unsigned uiThreads );

  /**
   * Returns the set number of desired threads for parallel implementations. 
   *
   */
  unsigned int GetNumberOfThreads() const;


protected:

private:

  unsigned int m_uiNrOfThreads;

};

#include "COneStepEvolver.txx"

} // end namespace

#endif
