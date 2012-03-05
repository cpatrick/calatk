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

#ifndef C_FOURIER_DOMAIN_KERNEL_TXX
#define C_FOURIER_DOMAIN_KERNEL_TXX

template <class T, unsigned int VImageDimension >
CFourierDomainKernel< T, VImageDimension >::CFourierDomainKernel():
  m_MemoryWasAllocated( false )
{
  fftwData = NULL;
}

template <class T, unsigned int VImageDimension >
void CFourierDomainKernel< T, VImageDimension >::DeleteData()
{
  if ( fftwData != NULL )
    {

    // clean up fftw data
    CFFTDataType<T>::FFTDestroyPlan( fftwData->fwd );
    CFFTDataType<T>::FFTDestroyPlan( fftwData->bck );
    fftw_free( fftwData->in );
    fftw_free( fftwData->out );

    delete fftwData;
    fftwData = NULL;
    }

   this->m_MemoryWasAllocated = false;
   this->m_KernelsNeedToBeComputed = true;

}

template <class T, unsigned int VImageDimension >
void CFourierDomainKernel< T, VImageDimension >::DeallocateMemory()
{
  fftw_cleanup();
  this->DeleteData();
}

template <class T, unsigned int VImageDimension >
CFourierDomainKernel< T, VImageDimension >::~CFourierDomainKernel()
{
  fftw_cleanup();
  DeleteData();
}

template <class T, unsigned int VImageDimension >
void CFourierDomainKernel< T, VImageDimension >::AllocateFFTDataStructures1D( unsigned int szX )
{
  typedef typename CFFTDataType<T>::FFTComplexType FFTComplexType;

  // Set up the fftw data
  unsigned int numElts = szX;
  fftwData = new CFFTDataType<T>();

  fftwData->in = (T*) fftw_malloc( sizeof(T) * numElts);
  fftwData->out = (FFTComplexType*) fftw_malloc(sizeof(FFTComplexType) * numElts);

  fftwData->fwd = CFFTDataType<T>::FFT_plan_dft_r2c_1d(szX, fftwData->in, fftwData->out, FFTW_ESTIMATE);
  fftwData->bck = CFFTDataType<T>::FFT_plan_dft_c2r_1d(szX, fftwData->out, fftwData->in, FFTW_ESTIMATE);
}

template <class T, unsigned int VImageDimension >
void CFourierDomainKernel< T, VImageDimension >::AllocateFFTDataStructures2D( unsigned int szX, unsigned int szY )
{
  typedef typename CFFTDataType<T>::FFTComplexType FFTComplexType;

  // Set up the fftw data
  unsigned int numElts = szX*szY;
  fftwData = new CFFTDataType<T>();

  fftwData->in = (T*) fftw_malloc( sizeof(T) * numElts );
  fftwData->out = (FFTComplexType*) fftw_malloc( sizeof(FFTComplexType) * numElts );

  fftwData->fwd = CFFTDataType<T>::FFT_plan_dft_r2c_2d(szX, szY, fftwData->in, fftwData->out, FFTW_ESTIMATE);
  fftwData->bck = CFFTDataType<T>::FFT_plan_dft_c2r_2d(szX, szY, fftwData->out, fftwData->in, FFTW_ESTIMATE);

}

template <class T, unsigned int VImageDimension >
void CFourierDomainKernel< T, VImageDimension >::AllocateFFTDataStructures3D( unsigned int szX, unsigned int szY, unsigned int szZ )
{
  typedef typename CFFTDataType<T>::FFTComplexType FFTComplexType;

 // Set up the fftw data
  unsigned int numElts = szX*szY*szZ;
  fftwData = new CFFTDataType<T>();
  
  fftwData->in = (T*) fftw_malloc( sizeof(T) * numElts );
  fftwData->out = (FFTComplexType*) fftw_malloc( sizeof(FFTComplexType) * numElts);

  fftwData->fwd = CFFTDataType<T>::FFT_plan_dft_r2c_3d(szX, szY, szZ, fftwData->in, fftwData->out, FFTW_ESTIMATE);
  fftwData->bck = CFFTDataType<T>::FFT_plan_dft_c2r_3d(szX, szY, szZ, fftwData->out, fftwData->in, FFTW_ESTIMATE);
}

template <class T, unsigned int VImageDimension >
void CFourierDomainKernel< T, VImageDimension >::AllocateFFTDataStructures( VectorImageType* pVecIm )
{
  unsigned int szX = pVecIm->getSizeX();
  unsigned int szY = pVecIm->getSizeY();
  unsigned int szZ = pVecIm->getSizeZ();

  switch ( VImageDimension )
    {
    case 1:
      AllocateFFTDataStructures1D( szX );
    case 2:
      AllocateFFTDataStructures2D( szX, szY );
      break;
    case 3:
      AllocateFFTDataStructures3D( szX, szY, szZ );
      break;
    default:
      throw std::runtime_error( "Cannot allocate FFT data structure of desired dimension." );
    }
}

template <class T, unsigned int VImageDimension >
T CFourierDomainKernel< T, VImageDimension >::GetFFromIndex( unsigned int iI, unsigned int iM, T dx )
{
  // results in the frequency f = w/(2*pi)

  T f;
  T LM = iM*dx;

  // offset to deal with even and odd index lengths
  unsigned int MOff;

  if ( iM % 2 == 0 )
    {
    MOff = 0;
    }
  else
    {
    MOff = 1;
    }

  if ( iI >= (iM+MOff)/2 )
    {
    f = ( (int)iI-(int)iM )/LM;
    }
  else
    {
    f = iI/LM;
    }

  return f;
}

template <class T, unsigned int VImageDimension >
void CFourierDomainKernel< T, VImageDimension >::ConvolveInFourierDomain( VectorImageType1D* pVecImage, VectorImageType1D* pL )
{
  assert( pL->getDim() == 1 );

  unsigned int szX = pVecImage->getSizeX();

  unsigned int dim = pVecImage->getDim();

// doing the loop the slow way because fftw
// requires row-major formatting

  for ( unsigned int x = 0; x < szX; ++x )
    {
    // add to fftw matrix
    fftwData->in[x] = pVecImage->getValue(x, 0);
    }

  //
  // do fourier domain operations
  //

  // tranform forward
  CFFTDataType<T>::FFTExecute( fftwData->fwd );

  // TODO: Assumption here is that we have a self-adjoint operator
  // TODO: need to support complex cases and do it really with the actual adjoint
  // multiply by L^2
  for ( unsigned int x = 0; x < szX; ++x )
    {
    T lVal = pL->getValue(x,0);

    fftwData->out[x][0] = fftwData->out[x][0] * lVal;
    fftwData->out[x][1] = fftwData->out[x][1] * lVal;
    }

  // transform backward
  CFFTDataType<T>::FFTExecute( fftwData->bck );

  //
  // convolve to get result and convert back to our format
  //

  for ( unsigned int x = 0; x < szX; ++x )
    {

    // scale the fft results and calculate gradient
    T val = (fftwData->in[x])/szX;

    pVecImage->setValue(x,0,val);
    }

}

template <class T, unsigned int VImageDimension >
void CFourierDomainKernel< T, VImageDimension >::ConvolveInFourierDomain( VectorImageType2D* pVecImage, VectorImageType2D* pL )
{

  assert( pL->getDim() == 1 );

  unsigned int szX = pVecImage->getSizeX();
  unsigned int szY = pVecImage->getSizeY();

  unsigned int dim = pVecImage->getDim();

// doing the loop the slow way because fftw
// requires row-major formatting

  for ( unsigned int d = 0; d < dim; ++d )
    {
    for ( unsigned int x = 0; x < szX; ++x ) 
      {  
      for ( unsigned int y = 0; y < szY; ++y ) 
        { 
        // add to fftw matrix
        unsigned int index = y + (szY * x);
        fftwData->in[index] = pVecImage->getValue(x,y,d);
        }
      }
      
    //
    // do fourier domain operations
    //
  
    // tranform forward
    CFFTDataType<T>::FFTExecute( fftwData->fwd );
  
    // TODO: Assumption here is that we have a self-adjoint operator
    // TODO: need to support complex cases and do it really with the actual adjoint
    // multiply by L^2
    for ( unsigned int x = 0; x < szX; ++x ) 
      {
      for ( unsigned int y = 0; y < szY/2+1; ++y ) 
        {
      
        unsigned int index = y + ((szY/2+1) * x);
        T lVal = pL->getValue(x,y,0);
      
        fftwData->out[index][0] = fftwData->out[index][0] * lVal;
        fftwData->out[index][1] = fftwData->out[index][1] * lVal;
        }
      }
  
    // transform backward
    CFFTDataType<T>::FFTExecute( fftwData->bck );

    //
    // convolve to get result and convert back to our format
    //
  
    for ( unsigned int x = 0; x < szX; ++x ) 
      {
      for ( unsigned int y = 0; y < szY; ++y ) 
        {
      
        unsigned int index = y + (szY * x);
      
        // scale the fft results and calculate gradient
        T val = (fftwData->in[index])/(szX*szY);
      
        pVecImage->setValue(x,y,d,val);
        }
      }

    } // loop over dimension
}

template <class T, unsigned int VImageDimension >
void CFourierDomainKernel< T, VImageDimension >::ConvolveInFourierDomain( VectorImageType3D* pVecImage, VectorImageType3D* pL )
{

  assert( pL->getDim() == 1 );

  unsigned int szX = pVecImage->getSizeX();
  unsigned int szY = pVecImage->getSizeY();
  unsigned int szZ = pVecImage->getSizeZ();

  unsigned int dim = pVecImage->getDim();

  // doing the loop the slow way because fftw
  // requires row-major formatting

  for ( unsigned int d = 0; d < dim; ++d )
    {
    
    for ( unsigned int x = 0; x < szX; ++x ) 
      {    
      for ( unsigned int y = 0; y < szY; ++y ) 
        { 
        for ( unsigned int z = 0; z < szZ; ++z ) 
          {
          // add to fftw matrix
          unsigned int index = z + szZ * (y + szY*x);
          fftwData->in[index] = pVecImage->getValue(x,y,z,d);
          }
        }
      }
    
    
    //
    // do fourier domain operations
    //
    
    // tranform forward
    CFFTDataType< T >::FFTExecute( fftwData->fwd );

    // multiply by L^2
  
    for ( unsigned int x = 0; x < szX; ++x ) 
      {
      for ( unsigned int y = 0; y < szY; ++y ) 
        {
        for ( unsigned int z = 0; z < szZ/2+1; ++z ) 
          {
          unsigned int index = z + (szZ/2+1) * (y + szY*x);
          T lVal = pL->getValue(x,y,z,0);
        
          fftwData->out[index][0] = fftwData->out[index][0] * lVal;
          fftwData->out[index][1] = fftwData->out[index][1] * lVal;

          }
        }
      }
      
    // transform backward
    CFFTDataType< T >::FFTExecute( fftwData->bck );
  
    //
    // convolve to get result and convert back to our format
    //
    
    for ( unsigned int x = 0; x < szX; ++x ) 
      {
      for ( unsigned int y = 0; y < szY; ++y ) 
        {
        for (unsigned int z = 0; z < szZ; ++z ) 
          {
          unsigned int index = z + szZ * (y + szY*x);
        
          T val = fftwData->in[index]/(szX*szY*szZ);
        
          pVecImage->setValue(x,y,z,d, val);
        
          }
        }
      }

    } // end loop over dimension

}

template <class T, unsigned int VImageDimension >
void CFourierDomainKernel< T, VImageDimension >::AllocateMemoryAndComputeKernelsIfNeeded( VectorImageType* pVecImage )
{
  if ( !this->m_MemoryWasAllocated )
    {
    this->AllocateMemoryForKernelAndInverseKernel( pVecImage );
    AllocateFFTDataStructures( pVecImage );
    this->m_MemoryWasAllocated = true;
    }

  if ( this->m_KernelsNeedToBeComputed )
    {
    this->ComputeKernelAndInverseKernel( pVecImage );
    ConfirmKernelsWereComputed();
    }

  assert( this->m_ptrL.GetPointer() != NULL );

  if ( pVecImage->getSizeX() != this->m_ptrL->getSizeX() ||
       pVecImage->getSizeY() != this->m_ptrL->getSizeY() ||
       pVecImage->getSizeZ() != this->m_ptrL->getSizeZ() )
    {
    throw std::runtime_error( "Kernel incompatible with velocity field size.");
    }

  assert( this->m_ptrLInv.GetPointer() != NULL );
  
  if ( pVecImage->getSizeX() != this->m_ptrLInv->getSizeX() ||
       pVecImage->getSizeY() != this->m_ptrLInv->getSizeY() ||
       pVecImage->getSizeZ() != this->m_ptrLInv->getSizeZ() )
    {
    throw std::runtime_error( "Kernel incompatible with velocity field size.");
    }
}

template <class T, unsigned int VImageDimension >
void CFourierDomainKernel< T, VImageDimension >::ConvolveWithKernel( VectorImageType* pVecImage )
{
  AllocateMemoryAndComputeKernelsIfNeeded( pVecImage );
  ConvolveInFourierDomain( pVecImage, this->m_ptrL );
}

template <class T, unsigned int VImageDimension >
void CFourierDomainKernel< T, VImageDimension >::ConvolveWithInverseKernel( VectorImageType* pVecImage )
{
  AllocateMemoryAndComputeKernelsIfNeeded( pVecImage );
  ConvolveInFourierDomain( pVecImage, this->m_ptrLInv );
}

template <class T, unsigned int VImageDimension >
void CFourierDomainKernel< T, VImageDimension >::ConfirmKernelsWereComputed()
{
  this->m_KernelsNeedToBeComputed = false;
}

#endif
