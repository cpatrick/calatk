#ifndef C_FOURIER_DOMAIN_KERNEL_H
#define C_FOURIER_DOMAIN_KERNEL_H

#include "CKernel.h"
#include <fftw3.h>

namespace CALATK
{

/**
 * fftwData2D
 *
 * A struct that holds the necessary fftw data structures for the LDDMM
 * fourier domain operations (2D)
 */
struct fftwData2DType 
{
  /** Array of input doubles to be transformed */
  double* in;
  /** Array of output double-pairs to hold the result */
  fftw_complex* out;
  
  /** FFTW plan object for the forward transformation */
  fftw_plan fwd;
  /** FFTW plan object for the backward transformation */
  fftw_plan bck;
};

/**
 * fftwData3D
 *
 * A struct that holds the necessary fftw data structures for the LDDMM
 * fourier domain operations (3D)
 */
struct fftwData3DType 
{

  /** Array of input doubles to be transformed */
  double* in;
  /** Array of output double-pairs to hold the result */
  fftw_complex* out;
  
  /** FFTW plan object for the forward transformation */
  fftw_plan fwd;
  /** FFTW plan object for the backward transformation */
  fftw_plan bck;
};

template <class T, unsigned int VImageDimension=3 >
class CFourierDomainKernel : public CKernel< T, VImageDimension >
{
public:

  /** some typedefs */
  
  typedef CKernel< T, VImageDimension > Superclass;
  typedef typename Superclass::VectorImageType VectorImageType;

  CFourierDomainKernel();
  ~CFourierDomainKernel();

  void ConvolveWithKernel( VectorImageType* pVecImage );
  void ConvolveWithInverseKernel( VectorImageType* pVecImage );

  virtual void DeallocateMemory();

protected:

  /** 
   * Converts from a linear index to the angular frequency omega 
   * Properly handles even and odd index lengths (to be used with the FFT)
   */
  static T GetKFromIndex( unsigned int iI, unsigned int iM, T dx );
  
  void ConfirmKernelsWereComputed();
  void ConfirmMemoryWasAllocated();

  void ConvolveInFourierDomain( VectorImageType* pVecImage, VectorImageType* pL );

  void AllocateFFTDataStructures( VectorImageType* pVecIm );
  void AllocateMemoryAndComputeKernelsIfNeeded( VectorImageType* pVecImage );

private:

  void AllocateFFTDataStructures2D( unsigned int szX, unsigned int szY );
  void AllocateFFTDataStructures3D( unsigned int szX, unsigned int szY, unsigned int szZ );

  void DeleteData();

  void ConvolveInFourierDomain2D( VectorImageType* pVecImage, VectorImageType* pL );
  void ConvolveInFourierDomain3D( VectorImageType* pVecImage, VectorImageType* pL );

  fftwData2DType* fftwData2D;
  fftwData3DType* fftwData3D;
};

#include "CFourierDomainKernel.txx"

}

#endif
