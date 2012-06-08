/*
*
*  Copyright 2011, 2012 by the CALATK development team
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
 * CFFTDataType
 *
 * A class that holds the necessary fftw data structures for the LDDMM Fourier domain operations
 */

template < class TFloat >
class CFFTDataType;

template < >
class CFFTDataType< float >
{
public:
  /** generic typedef for the fftw complex type */
  typedef fftwf_complex FFTComplexType;
  typedef fftwf_plan FFTPlanType;

  /** static function to excute the plan */
  static void FFTExecute( fftwf_plan_s* fftplan )
  {
    fftwf_execute( fftplan );
  };

  /** static function to destroy the plan */
  static void FFTDestroyPlan( fftwf_plan_s* fftplan )
  {
    fftwf_destroy_plan( fftplan );
  };

  /** static function to free memory */
  static void FFTFreeIn( float * data )
  {
    fftwf_free( data );
  }

  /** static function to free memory */
  static void FFTFreeOut( fftwf_complex* data )
  {
    fftwf_free( data );
  }

  /** static function to cleanup */
  static void FFTCleanup()
  {
    fftwf_cleanup();
  }

  /** static function to make the plan */
  static FFTPlanType FFT_plan_dft_r2c_1d( int szX, float* in, FFTComplexType* out, unsigned int uiAction )
  {
    return fftwf_plan_dft_r2c_1d( szX, in, out, uiAction );
  };

  /** static function to make the plan */
  static FFTPlanType FFT_plan_dft_r2c_2d( int szX, int szY, float* in, FFTComplexType* out, unsigned int uiAction )
  {
    return fftwf_plan_dft_r2c_2d( szX, szY, in, out, uiAction );
  };

  /** static function to make the plan */
  static FFTPlanType FFT_plan_dft_r2c_3d( int szX, int szY, int szZ, float* in, FFTComplexType* out, unsigned int uiAction )
  {
    return fftwf_plan_dft_r2c_3d( szX, szY, szZ, in, out, uiAction );
  };

  /** static function to make the plan */
  static FFTPlanType FFT_plan_dft_c2r_1d( int szX, FFTComplexType* out, float* in, unsigned int uiAction )
  {
    return fftwf_plan_dft_c2r_1d( szX, out, in, uiAction );
  };

  /** static function to make the plan */
  static FFTPlanType FFT_plan_dft_c2r_2d( int szX, int szY, FFTComplexType* out, float* in, unsigned int uiAction )
  {
    return fftwf_plan_dft_c2r_2d( szX, szY, out, in, uiAction );
  };

  /** static function to make the plan */
  static FFTPlanType FFT_plan_dft_c2r_3d( int szX, int szY, int szZ, FFTComplexType* out, float* in, unsigned int uiAction )
  {
    return fftwf_plan_dft_c2r_3d( szX, szY, szZ, out, in, uiAction );
  };

  /** Array of input doubles to be transformed */
  float* in;
  /** Array of output double-pairs to hold the result */
  fftwf_complex* out;

  /** FFTW plan object for the forward transformation */
  fftwf_plan fwd;
  /** FFTW plan object for the backward transformation */
  fftwf_plan bck;
};

template < >
class CFFTDataType< double >
{
public:
  /** generic typedef for the fftw complex type */
  typedef fftw_complex FFTComplexType;
  typedef fftw_plan FFTPlanType;

  /** static function to excute the plan */
  static void FFTExecute( fftw_plan_s* fftplan )
  {
    fftw_execute( fftplan );
  };

  /** static function to destroy the plan */
  static void FFTDestroyPlan( fftw_plan_s* fftplan )
  {
    fftw_destroy_plan( fftplan );
  };

  /** static function to free memory */
  static void FFTFreeIn( double * data )
  {
    fftw_free( data );
  }

  /** static function to free memory */
  static void FFTFreeOut( fftw_complex* data )
  {
    fftw_free( data );
  }

  /** static function to cleanup */
  static void FFTCleanup()
  {
    fftw_cleanup();
  }

  /** static function to make the plan */
  static FFTPlanType FFT_plan_dft_r2c_1d( int szX, double* in, FFTComplexType* out, unsigned int uiAction )
  {
    return fftw_plan_dft_r2c_1d( szX, in, out, uiAction );
  };

  /** static function to make the plan */
  static FFTPlanType FFT_plan_dft_r2c_2d( int szX, int szY, double* in, FFTComplexType* out, unsigned int uiAction )
  {
    return fftw_plan_dft_r2c_2d( szX, szY, in, out, uiAction );
  };

  /** static function to make the plan */
  static FFTPlanType FFT_plan_dft_r2c_3d( int szX, int szY, int szZ, double* in, FFTComplexType* out, unsigned int uiAction )
  {
    return fftw_plan_dft_r2c_3d( szX, szY, szZ, in, out, uiAction );
  };

  /** static function to make the plan */
  static FFTPlanType FFT_plan_dft_c2r_1d( int szX, FFTComplexType* out, double* in, unsigned int uiAction )
  {
    return fftw_plan_dft_c2r_1d( szX, out, in, uiAction );
  };

  /** static function to make the plan */
  static FFTPlanType FFT_plan_dft_c2r_2d( int szX, int szY, FFTComplexType* out, double* in, unsigned int uiAction )
  {
    return fftw_plan_dft_c2r_2d( szX, szY, out, in, uiAction );
  };

  /** static function to make the plan */
  static FFTPlanType FFT_plan_dft_c2r_3d( int szX, int szY, int szZ, FFTComplexType* out, double* in, unsigned int uiAction )
  {
    return fftw_plan_dft_c2r_3d( szX, szY, szZ, out, in, uiAction );
  };

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
  /** Standard class typedefs. */
  typedef CFourierDomainKernel            Self;
  typedef CKernel< T, VImageDimension >   Superclass;
  typedef itk::SmartPointer< Self >       Pointer;
  typedef itk::SmartPointer< const Self > ConstPointer;

  /* some typedefs */
  typedef typename Superclass::VectorImageType   VectorImageType;
  typedef typename Superclass::VectorImageType1D VectorImageType1D;
  typedef typename Superclass::VectorImageType2D VectorImageType2D;
  typedef typename Superclass::VectorImageType3D VectorImageType3D;

  CFourierDomainKernel();
  virtual ~CFourierDomainKernel();

  virtual void ConvolveWithKernel( VectorImageType * ptrVecImage );
  virtual void ConvolveWithInverseKernel( VectorImageType * ptrVecImage );

  virtual void DeallocateMemory();

protected:

  /** 
   * Converts from a linear index to the frequency f.
   * Properly handles even and odd index lengths (to be used with the FFT)
   */
  static T GetFrequencyFromIndex( unsigned int iI, unsigned int iM, T dx );
  
  void ConfirmKernelsWereComputed();

  void ConvolveInFourierDomain( VectorImageType1D* pVecImage, VectorImageType1D* pL );
  void ConvolveInFourierDomain( VectorImageType2D* pVecImage, VectorImageType2D* pL );
  void ConvolveInFourierDomain( VectorImageType3D* pVecImage, VectorImageType3D* pL );

  void AllocateFFTDataStructures( const VectorImageType* pVecIm );
  void AllocateMemoryAndComputeKernelsIfNeeded( const VectorImageType* pVecImage );

private:
  void DeleteData();

  void AllocateFFTDataStructures1D( unsigned int szX );
  void AllocateFFTDataStructures2D( unsigned int szX, unsigned int szY );
  void AllocateFFTDataStructures3D( unsigned int szX, unsigned int szY, unsigned int szZ );

  CFFTDataType<T>* fftwData;

  bool m_MemoryWasAllocated;
};

}

#endif
