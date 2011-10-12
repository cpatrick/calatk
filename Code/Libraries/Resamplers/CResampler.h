#ifndef C_RESAMPLER_H
#define C_RESAMPLER_H

#include "VectorImage.h"

namespace CALATK
{

template <class T, unsigned int VImageDimension=3 >
class CResampler
{
public:

  /** Some useful typedefs */
  typedef VectorImage< T, VImageDimension > VectorImageType;

  CResampler();
  virtual ~CResampler();

  virtual void Downsample( VectorImageType* ptrImIn, VectorImageType* ptrImOut ) = 0;
  virtual void Upsample( VectorImageType* ptrImIn, VectorImageType* ptrImOut ) = 0;

protected:
  
private:
};

#include "CResampler.txx"

} // end namespace

#endif
