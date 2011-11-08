#ifndef C_RESAMPLER_H
#define C_RESAMPLER_H

#include "VectorImage.h"
#include "CGaussianKernel.h"
#include "CALATKCommon.h"

#include "CProcessBase.h"
#include "JSONParameterUtils.h"

namespace CALATK
{

template <class T, unsigned int VImageDimension=3 >
class CResampler : public CProcessBase
{
public:

  /** Some useful typedefs */
  typedef VectorImage< T, VImageDimension > VectorImageType;
  typedef CProcessBase Superclass;

  CResampler();
  virtual ~CResampler();

  virtual void Downsample( const VectorImageType* ptrImIn, VectorImageType* ptrImOut ) = 0;
  virtual void Upsample( const VectorImageType* ptrImIn, VectorImageType* ptrImOut ) = 0;

  virtual void SetSigma( T dSigma );
  SetJSONMacro( Sigma, T );
  GetMacro( Sigma, T );

  virtual void SetAutoConfiguration( Json::Value& ConfValue );

protected:

  CGaussianKernel< T, VImageDimension > m_GaussianKernel;
  T m_Sigma;

private:

  const T DefaultSigma;
  
  bool m_ExternallySetSigma;

};

#include "CResampler.txx"

} // end namespace

#endif
