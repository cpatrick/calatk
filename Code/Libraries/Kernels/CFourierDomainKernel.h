/*
*
*  Copyright 2011 by the CALATK development team
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*
*
*/

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

  /* some typedefs */
  
  typedef CKernel< T, VImageDimension > Superclass;
  typedef typename Superclass::VectorImageType VectorImageType;
  typedef typename Superclass::VectorImageType2D VectorImageType2D;
  typedef typename Superclass::VectorImageType3D VectorImageType3D;

  CFourierDomainKernel();
  ~CFourierDomainKernel();

  void ConvolveWithKernel( VectorImageType* pVecImage );
  void ConvolveWithInverseKernel( VectorImageType* pVecImage );

  virtual void DeallocateMemory();

protected:

  /** 
   * Converts from a linear index to the frequency f
   * Properly handles even and odd index lengths (to be used with the FFT)
   */
  static T GetFFromIndex( unsigned int iI, unsigned int iM, T dx );
  
  void ConfirmKernelsWereComputed();
  void ConfirmMemoryWasAllocated();

  void ConvolveInFourierDomain( VectorImageType2D* pVecImage, VectorImageType2D* pL );
  void ConvolveInFourierDomain( VectorImageType3D* pVecImage, VectorImageType3D* pL );

  void AllocateFFTDataStructures( VectorImageType* pVecIm );
  void AllocateMemoryAndComputeKernelsIfNeeded( VectorImageType* pVecImage );

private:

  void AllocateFFTDataStructures2D( unsigned int szX, unsigned int szY );
  void AllocateFFTDataStructures3D( unsigned int szX, unsigned int szY, unsigned int szZ );

  void DeleteData();

  fftwData2DType* fftwData2D;
  fftwData3DType* fftwData3D;
};

#include "CFourierDomainKernel.txx"

}

#endif
