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
  /** Array of input doubles to be transformed for X field component */
  double* inX;
  /** Array of output double-pairs to hold the result for the transformed X component */
  fftw_complex* outX;
  /** Array of input doubles to be transformed for Y field component */
  double* inY;
  /** Array of output double-pairs to hold the result for the transformed Y component */
  fftw_complex* outY;
  
  /** FFTW plan object for the forward X transformation */
  fftw_plan fwdX;
  /** FFTW plan object for the backward X transformation */
  fftw_plan bckX;
  /** FFTW plan object for the forward Y transformation */
  fftw_plan fwdY;
  /** FFTW plan object for the backward Y transformation */
  fftw_plan bckY;

};

/**
 * fftwData3D
 *
 * A struct that holds the necessary fftw data structures for the LDDMM
 * fourier domain operations (3D)
 */
struct fftwData3DType 
{

  /** Array of input doubles to be transformed for X field component */
  double* inX;
  /** Array of output double-pairs to hold the result for the transformed X component */
  fftw_complex* outX;
  /** Array of input doubles to be transformed for Y field component */
  double* inY;
  /** Array of output double-pairs to hold the result for the transformed Y component */
  fftw_complex* outY;
  /** Array of input doubles to be transformed for Z field component */
  double* inZ;
  /** Array of output double-pairs to hold the result for the transformed Z component */
  fftw_complex* outZ;
  
  /** FFTW plan object for the forward X transformation */
  fftw_plan fwdX;
  /** FFTW plan object for the backward X transformation */
  fftw_plan bckX;
  /** FFTW plan object for the forward Y transformation */
  fftw_plan fwdY;
  /** FFTW plan object for the backward Y transformation */
  fftw_plan bckY;
  /** FFTW plan object for the forward Z transformation */
  fftw_plan fwdZ;
  /** FFTW plan object for the backward Z transformation */
  fftw_plan bckZ;

};

template <class T, unsigned int VImageDimension=3 >
class CFourierDomainKernel : public CKernel< T, VImageDimension >
{
public:

  /** some typedefs */
  
  typedef CKernel< T, VImageDimension > Superclass;
  typedef typename Superclass::VectorImageType VectorImageType;
  typedef typename Superclass::VectorFieldType VectorFieldType;

  CFourierDomainKernel();
  ~CFourierDomainKernel();

  void ConvolveWithKernel( VectorFieldType* pVecField );
  void ConvolveWithInverseKernel( VectorFieldType* pVecField );

protected:

  void ConvolveInFourierDomain( VectorFieldType* pVecField, VectorImageType* pL );
  void AllocateFFTDataStructures( VectorImageType* pVecIm );

  VectorImageType *m_ptrL;
  VectorImageType *m_ptrLInv;

private:

  void AllocateFFTDataStructures2D( unsigned int szX, unsigned int szY );
  void AllocateFFTDataStructures3D( unsigned int szX, unsigned int szY, unsigned int szZ );

  void DeleteData();

  void ConvolveInFourierDomain2D( VectorFieldType* pVecField, VectorImageType* pL );
  void ConvolveInFourierDomain3D( VectorFieldType* pVecField, VectorImageType* pL );

  fftwData2DType* fftwData2D;
  fftwData3DType* fftwData3D;
};

#include "CFourierDomainKernel.txx"

}

#endif
