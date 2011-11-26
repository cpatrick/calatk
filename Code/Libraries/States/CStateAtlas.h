#ifndef C_STATE_ATLAS_H
#define C_STATE_ATLAS_H

namespace CALATK
{
/**
  * Implements the state for atlas building.
  * This state is simply a collection of individual states of the individual registrations
  * between an image and the atlas image. Templated over the state of these registrations.
  */
class CStateAtlas : public CStateImageDomain< typename TState::T, typename TState::VImageDimension, typename TState::TResampler >
{
 public:
  /** some useful typedefs */
  typedef typename TState::T T;
  typedef typename TState::VImageDimension VImageDimension;
  typedef typename TState::TResampler TResampler;
  typedef CStateImageDomain< TState, VImageDimension, TResampler > Superclass;
  typedef Superclass SuperclassTState;

  /**
   * Empty constructor
   */
  CStateAtlas();

  /**
   * copy constructor, creation of the image for the first time, need to allocate memory
   */
  CStateAtlas( const CStateAtlas & c );

  /**
   * Destructor, this class will involve dynamic memory allocation, so needs a destructor
   */
  ~CStateAtlas();

  /**
   * Allow for upsampling of the state
   */
  SuperclassTState* CreateUpsampledStateAndAllocateMemory( const VectorImageType* pGraftImage ) const; 

  /// declare operators to be able to do some computations with this state, which are needed in the numerical solvers

  /**
   * assignment, memory already allocated, may need to be destroyed and re-allocated
   */
  CStateAtlas & operator=( const CStateAtlas & p );

  CStateAtlas & operator+=(const CStateAtlas & p );

  CStateAtlas & operator-=(const CStateAtlas & p );

  CStateAtlas & operator*=(const T & p );

  CStateAtlas operator+(const CStateAtlas & p ) const;

  CStateAtlas operator-(const CStateAtlas & p ) const;

  CStateAtlas operator*(const T & p ) const;
  
  /**
   * @brief Computes the square norm of the state. To be used for example in a line search method
   * to establish sufficient descrease of an objective function
   *
   * @return Returns the squared norm. For the atlas, this is the sum of the squared norms of all
   * the components.
   */
  T SquaredNorm();

protected:
  void ClearDataStructure();

  // holds the state vectors of the individual registration algorithms
  std::vector< TState > m_vecIndividualStates;

private:

};

#include "CStateAtlas.txx"

} // end namespace

#endif
