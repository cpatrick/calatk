#ifndef C_RESAMPLER_H
#define C_RESAMPLER_H

#include "VectorImage.h"
#include "CGaussianKernel.h"
#include "CALATKCommon.h"

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

  virtual void Downsample( const VectorImageType* ptrImIn, VectorImageType* ptrImOut ) = 0;
  virtual void Upsample( const VectorImageType* ptrImIn, VectorImageType* ptrImOut ) = 0;

  SetMacro( Sigma, T );
  GetMacro( Sigma, T );

protected:

  CGaussianKernel< T, VImageDimension > m_GaussianKernel;

private:

  T m_Sigma;

};

#include "CResampler.txx"

} // end namespace

#endif
