#ifndef C_STATE_IMAGE_DOMAIN_H
#define C_STATE_IMAGE_DOMAIN_H

#include "CState.h"
#include "VectorImage.h"

/**
 * Class which introduces upsampling of a state based on image dimensions
 *
 */

namespace CALATK
{

template <class T, unsigned int VImageDimension, class TResampler >
class CStateImageDomain : public CState< T, VImageDimension, TResampler >
{
public:

  /** Some useful typedefs */
  typedef VectorImage< T, VImageDimension > VectorImageType;
  typedef CStateImageDomain< T, VImageDimension, TResampler > TState;

  CStateImageDomain() {};
  virtual ~CStateImageDomain() {};

  virtual TState* CreateUpsampledStateAndAllocateMemory( const VectorImageType* pGraftImage ) const = 0;

protected:
private:
};

} // end namespace

#endif
