#ifndef C_RESAMPLER_LINEAR_H
#define C_RESAMPLER_LINEAR_H

#include "CResampler.h"

namespace CALATK
{

template <class T, unsigned int VImageDimension=3 >
class CResamplerLinear : public CResampler< T, VImageDimension >
{
public:

  /** Some useful typedefs */
  typedef CResampler< T, VImageDimension > Superclass;
  typedef typename Superclass::VectorImageType VectorImageType;

  CResamplerLinear();
  virtual ~CResamplerLinear();

  void Downsample( const VectorImageType* ptrImIn, VectorImageType* ptrImOut );
  void Upsample( const VectorImageType* ptrImIn, VectorImageType* ptrImOut );

protected:
private:
};

#include "CResamplerLinear.txx"

} // end namespace

#endif
