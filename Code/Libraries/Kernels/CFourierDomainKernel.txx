#ifndef C_FOURIER_DOMAIN_KERNEL_TXX
#define C_FOURIER_DOMAIN_KERNEL_TXX

template <class T, unsigned int VImageDimension >
CFourierDomainKernel< T, VImageDimension >::CFourierDomainKernel()
{
  fftwData2D = NULL;
  fftwData3D = NULL;
}

template <class T, unsigned int VImageDimension >
void CFourierDomainKernel< T, VImageDimension >::DeleteData()
{
  if ( fftwData2D != NULL ) 
    {

    // clean up fftw data
    fftw_destroy_plan(fftwData2D->fwd);
    fftw_destroy_plan(fftwData2D->bck);
    fftw_free(fftwData2D->in);
    fftw_free(fftwData2D->out);

    delete fftwData2D;
    fftwData2D = NULL;
    }

  if ( fftwData3D != NULL ) 
    {

    // clean up fftw data
    fftw_destroy_plan(fftwData3D->fwd);
    fftw_destroy_plan(fftwData3D->bck);
    fftw_free(fftwData3D->in);
    fftw_free(fftwData3D->out);

    delete fftwData3D;
    fftwData3D = NULL;
    }

   this->m_MemoryWasAllocated = false;
   this->m_KernelsNeedToBeComputed = true;

}

template <class T, unsigned int VImageDimension >
void CFourierDomainKernel< T, VImageDimension >::DeallocateMemory()
{
  Superclass::DeallocateMemory();
  DeleteData();
}

template <class T, unsigned int VImageDimension >
CFourierDomainKernel< T, VImageDimension >::~CFourierDomainKernel()
{
  DeleteData();
}

template <class T, unsigned int VImageDimension >
void CFourierDomainKernel< T, VImageDimension >::AllocateFFTDataStructures2D( unsigned int szX, unsigned int szY )
{
  // Set up the fftw data
  unsigned int numElts = szX*szY;
  fftwData2D = new fftwData2DType();
  
  fftwData2D->in = (double*) fftw_malloc(sizeof(double) * numElts);
  fftwData2D->out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * numElts);
  
  fftwData2D->fwd = fftw_plan_dft_r2c_2d(szX, szY, fftwData2D->in, fftwData2D->out, FFTW_ESTIMATE);
  fftwData2D->bck = fftw_plan_dft_c2r_2d(szX, szY, fftwData2D->out, fftwData2D->in, FFTW_ESTIMATE);

}

template <class T, unsigned int VImageDimension >
void CFourierDomainKernel< T, VImageDimension >::AllocateFFTDataStructures3D( unsigned int szX, unsigned int szY, unsigned int szZ )
{

 // Set up the fftw data
  unsigned int numElts = szX*szY*szZ;
  fftwData3D = new fftwData3DType();
  
  fftwData3D->in = (double*) fftw_malloc(sizeof(double) * numElts);
  fftwData3D->out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * numElts);

  fftwData3D->fwd = fftw_plan_dft_r2c_3d(szX, szY, szZ, fftwData3D->in, fftwData3D->out, FFTW_ESTIMATE);
  fftwData3D->bck = fftw_plan_dft_c2r_3d(szX, szY, szZ, fftwData3D->out, fftwData3D->in, FFTW_ESTIMATE);
}

template <class T, unsigned int VImageDimension >
void CFourierDomainKernel< T, VImageDimension >::AllocateFFTDataStructures( VectorImageType* pVecIm )
{
  unsigned int szX = pVecIm->getSizeX();
  unsigned int szY = pVecIm->getSizeY();
  unsigned int szZ = pVecIm->getSizeZ();

  switch ( VImageDimension )
    {
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
T CFourierDomainKernel< T, VImageDimension >::GetKFromIndex( unsigned int iI, unsigned int iM, T dx )
{
  // results in an angular frequency w; f = 2*pi*w

  T w;
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

  if ( iI >= (iM+MOff)/LM )
    {
    w = ( iI-iM )/LM;
    }
  else
    {
    w = iI/LM;
    }

  return w;
}

template <class T, unsigned int VImageDimension >
void CFourierDomainKernel< T, VImageDimension >::ConvolveInFourierDomain2D( VectorImageType* pVecImage, VectorImageType* pL )
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
        fftwData2D->in[index] = pVecImage->getValue(x,y,d);
        }
      }
      
    //
    // do fourier domain operations
    //
  
    // tranform forward
    fftw_execute(fftwData2D->fwd);
  
    // TODO: Assumption here is that we have a self-adjoint operator
    // TODO: need to support complex cases and do it really with the actual adjoint
    // multiply by L^2
    for ( unsigned int x = 0; x < szX; ++x ) 
      {
      for ( unsigned int y = 0; y < szY/2+1; ++y ) 
        {
      
        unsigned int index = y + ((szY/2+1) * x);
        T lVal = pL->getValue(x,y,0);
      
        fftwData2D->out[index][0] = fftwData2D->out[index][0] * lVal;
        fftwData2D->out[index][1] = fftwData2D->out[index][1] * lVal;
        }
      }
  
    // transform backward
    fftw_execute( fftwData2D->bck );

    //
    // convolve to get result and convert back to our format
    //
  
    for ( unsigned int x = 0; x < szX; ++x ) 
      {
      for ( unsigned int y = 0; y < szY; ++y ) 
        {
      
        unsigned int index = y + (szY * x);
      
        // scale the fft results and calculate gradient
        T val = (fftwData2D->in[index])/(szX*szY);
      
        pVecImage->setValue(x,y,d,val);
        }
      }

    } // loop over dimension
}

template <class T, unsigned int VImageDimension >
void CFourierDomainKernel< T, VImageDimension >::ConvolveInFourierDomain3D( VectorImageType* pVecImage, VectorImageType* pL )
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
          fftwData3D->in[index] = pVecImage->getValue(x,y,z,d);
          }
        }
      }
    
    
    //
    // do fourier domain operations
    //
    
    // tranform forward
    fftw_execute(fftwData3D->fwd);

    // multiply by L^2
  
    for ( unsigned int x = 0; x < szX; ++x ) 
      {
      for ( unsigned int y = 0; y < szY; ++y ) 
        {
        for ( unsigned int z = 0; z < szZ/2+1; ++z ) 
          {
          unsigned int index = z + (szZ/2+1) * (y + szY*x);
          T lVal = pL->getValue(x,y,z,0);
        
          fftwData3D->out[index][0] = fftwData3D->out[index][0] * lVal;
          fftwData3D->out[index][1] = fftwData3D->out[index][1] * lVal;

          }
        }
      }
      
    // transform backward
    fftw_execute(fftwData3D->bck);
  
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
        
          T val = fftwData3D->in[index]/(szX*szY*szZ);
        
          pVecImage->setValue(x,y,z,d, val);
        
          }
        }
      }

    } // end loop over dimension

}

template <class T, unsigned int VImageDimension >
void CFourierDomainKernel< T, VImageDimension >::ConvolveInFourierDomain( VectorImageType* pVecImage, VectorImageType* pL )
{
  switch ( VImageDimension )
    {
    case 2:
      ConvolveInFourierDomain2D( pVecImage, pL );
      break;
    case 3:
      ConvolveInFourierDomain3D( pVecImage, pL );
      break;
    default:
      throw std::runtime_error("Cannot convolve image of such a dimension." );
    }
}

template <class T, unsigned int VImageDimension >
void CFourierDomainKernel< T, VImageDimension >::AllocateMemoryAndComputeKernelsIfNeeded( VectorImageType* pVecImage )
{
  if ( !this->m_MemoryWasAllocated )
    {
    this->AllocateMemoryForKernelAndInverseKernel( pVecImage );
    AllocateFFTDataStructures( pVecImage );
    ConfirmMemoryWasAllocated();
    }

  if ( this->m_KernelsNeedToBeComputed )
    {
    this->ComputeKernelAndInverseKernel( pVecImage );
    ConfirmKernelsWereComputed();
    }

  assert( this->m_ptrL != NULL );

  if ( pVecImage->getSizeX() != this->m_ptrL->getSizeX() ||
       pVecImage->getSizeY() != this->m_ptrL->getSizeY() ||
       pVecImage->getSizeZ() != this->m_ptrL->getSizeZ() )
    {
    throw std::runtime_error( "Kernel incompatible with velocity field size.");
    }

  assert( this->m_ptrLInv != NULL );
  
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
void CFourierDomainKernel< T, VImageDimension >::ConfirmMemoryWasAllocated()
{
  this->m_MemoryWasAllocated = true;
}

template <class T, unsigned int VImageDimension >
void CFourierDomainKernel< T, VImageDimension >::ConfirmKernelsWereComputed()
{
  this->m_KernelsNeedToBeComputed = false;
}

#endif
