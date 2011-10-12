#ifndef C_FOURIER_DOMAIN_KERNEL_TXX
#define C_FOURIER_DOMAIN_KERNEL_TXX

template <class T, unsigned int VImageDimension >
CFourierDomainKernel< T, VImageDimension >::CFourierDomainKernel()
{
  fftwData2D = NULL;
  fftwData3D = NULL;

  m_ptrL = NULL;
  m_ptrLInv = NULL;
}

template <class T, unsigned int VImageDimension >
void CFourierDomainKernel< T, VImageDimension >::DeleteData()
{
  if ( fftwData2D != NULL ) 
    {

    // clean up fftw data
    fftw_destroy_plan(fftwData2D->fwdX);
    fftw_destroy_plan(fftwData2D->fwdY);
    fftw_destroy_plan(fftwData2D->bckX);
    fftw_destroy_plan(fftwData2D->bckY);
    fftw_free(fftwData2D->inX);
    fftw_free(fftwData2D->inY);
    fftw_free(fftwData2D->outX);
    fftw_free(fftwData2D->outY);

    delete fftwData2D;
    fftwData2D = NULL;
    }

  if ( fftwData3D != NULL ) 
    {

    // clean up fftw data
    fftw_destroy_plan(fftwData3D->fwdX);
    fftw_destroy_plan(fftwData3D->fwdY);
    fftw_destroy_plan(fftwData3D->fwdZ);
    fftw_destroy_plan(fftwData3D->bckX);
    fftw_destroy_plan(fftwData3D->bckY);
    fftw_destroy_plan(fftwData3D->bckZ);
    fftw_free(fftwData3D->inX);
    fftw_free(fftwData3D->inY);
    fftw_free(fftwData3D->inZ);
    fftw_free(fftwData3D->outX);
    fftw_free(fftwData3D->outY);
    fftw_free(fftwData3D->outZ);

    delete fftwData3D;
    fftwData3D = NULL;
    }

  if ( m_ptrL != NULL ) 
    {
    delete m_ptrL;
    m_ptrL = NULL;
    }

  if ( m_ptrLInv != NULL ) 
    {
    delete m_ptrLInv;
    m_ptrLInv = NULL;
    }

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
  
  fftwData2D->inX = (double*) fftw_malloc(sizeof(double) * numElts);
  fftwData2D->outX = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * numElts);
  fftwData2D->inY = (double*) fftw_malloc(sizeof(double) * numElts);
  fftwData2D->outY = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * numElts);
  
  fftwData2D->fwdX = fftw_plan_dft_r2c_2d(szX, szY, fftwData2D->inX, fftwData2D->outX, FFTW_ESTIMATE);
  fftwData2D->fwdY = fftw_plan_dft_r2c_2d(szX, szY, fftwData2D->inY, fftwData2D->outY, FFTW_ESTIMATE);
  fftwData2D->bckX = fftw_plan_dft_c2r_2d(szX, szY, fftwData2D->outX, fftwData2D->inX, FFTW_ESTIMATE);
  fftwData2D->bckY = fftw_plan_dft_c2r_2d(szX, szY, fftwData2D->outY, fftwData2D->inY, FFTW_ESTIMATE);

}

template <class T, unsigned int VImageDimension >
void CFourierDomainKernel< T, VImageDimension >::AllocateFFTDataStructures3D( unsigned int szX, unsigned int szY, unsigned int szZ )
{

 // Set up the fftw data
  unsigned int numElts = szX*szY*szZ;
  fftwData3D = new fftwData3DType();
  
  fftwData3D->inX = (double*) fftw_malloc(sizeof(double) * numElts);
  fftwData3D->outX = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * numElts);
  fftwData3D->inY = (double*) fftw_malloc(sizeof(double) * numElts);
  fftwData3D->outY = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * numElts);
  fftwData3D->inZ = (double*) fftw_malloc(sizeof(double) * numElts);
  fftwData3D->outZ = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * numElts);
  
  fftwData3D->fwdX = fftw_plan_dft_r2c_3d(szX, szY, szZ, fftwData3D->inX, fftwData3D->outX, FFTW_ESTIMATE);
  fftwData3D->fwdY = fftw_plan_dft_r2c_3d(szX, szY, szZ, fftwData3D->inY, fftwData3D->outY, FFTW_ESTIMATE);
  fftwData3D->fwdZ = fftw_plan_dft_r2c_3d(szX, szY, szZ, fftwData3D->inZ, fftwData3D->outZ, FFTW_ESTIMATE);
  fftwData3D->bckX = fftw_plan_dft_c2r_3d(szX, szY, szZ, fftwData3D->outX, fftwData3D->inX, FFTW_ESTIMATE);
  fftwData3D->bckY = fftw_plan_dft_c2r_3d(szX, szY, szZ, fftwData3D->outY, fftwData3D->inY, FFTW_ESTIMATE);
  fftwData3D->bckZ = fftw_plan_dft_c2r_3d(szX, szY, szZ, fftwData3D->outZ, fftwData3D->inZ, FFTW_ESTIMATE);

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
void CFourierDomainKernel< T, VImageDimension >::ConvolveInFourierDomain2D( VectorFieldType* pVecField, VectorImageType* pL )
{

  assert( pL->getDim() == 1 );

  unsigned int szX = pVecField->getSizeX();
  unsigned int szY = pVecField->getSizeY();

// doing the loop the slow way because fftw
// requires row-major formatting

  for ( unsigned int x = 0; x < szX; ++x ) 
    {  
    for ( unsigned int y = 0; y < szY; ++y ) 
      { 
      // add to fftw matrix
      unsigned int index = y + (szY * x);
      fftwData2D->inX[index] = pVecField->getX(x,y);
      fftwData2D->inY[index] = pVecField->getY(x,y);
      }
    }
      
  //
  // do fourier domain operations
  //
  
  // tranform forward
  fftw_execute(fftwData2D->fwdX);
  fftw_execute(fftwData2D->fwdY);
  
  // TODO: Assumption here is that we have a self-adjoint operator
  // TODO: need to support complex cases and do it really with the actual adjoint
  // multiply by L^2
  for ( unsigned int x = 0; x < szX; ++x ) 
    {
    for ( unsigned int y = 0; y < szY/2+1; ++y ) 
      {
      
      unsigned int index = y + ((szY/2+1) * x);
      T lVal = pL->getValue(x,y,0);
      
      fftwData2D->outX[index][0] = fftwData2D->outX[index][0] * lVal;
      fftwData2D->outX[index][1] = fftwData2D->outX[index][1] * lVal;
      fftwData2D->outY[index][0] = fftwData2D->outY[index][0] * lVal;
      fftwData2D->outY[index][1] = fftwData2D->outY[index][1] * lVal;
      
      }
    }
  
  // transform backward
  fftw_execute( fftwData2D->bckX );
  fftw_execute( fftwData2D->bckY );
  
  //
  // convolve to get result and convert back to our format
  //
  
  for ( unsigned int x = 0; x < szX; ++x ) 
    {
    for ( unsigned int y = 0; y < szY; ++y ) 
      {
      
      unsigned int index = y + (szY * x);
      
      // scale the fft results and calculate gradient
      T valX = (fftwData2D->inX[index])/(szX*szY);
      T valY = (fftwData2D->inY[index])/(szX*szY);
      
      pVecField->setX(x,y, valX);
      pVecField->setY(x,y, valY);
      }
    }
}

template <class T, unsigned int VImageDimension >
void CFourierDomainKernel< T, VImageDimension >::ConvolveInFourierDomain3D( VectorFieldType* pVecField, VectorImageType* pL )
{

  assert( pL->getDim() == 1 );

  unsigned int szX = pVecField->getSizeX();
  unsigned int szY = pVecField->getSizeY();
  unsigned int szZ = pVecField->getSizeZ();

  // doing the loop the slow way because fftw
  // requires row-major formatting

  for ( unsigned int x = 0; x < szX; ++x ) 
    {    
    for ( unsigned int y = 0; y < szY; ++y ) 
      { 
      for ( unsigned int z = 0; z < szZ; ++z ) 
        {
        // add to fftw matrix
        unsigned int index = z + szZ * (y + szY*x);
        fftwData3D->inX[index] = pVecField->getX(x,y,z);
        fftwData3D->inY[index] = pVecField->getY(x,y,z);
        fftwData3D->inZ[index] = pVecField->getZ(x,y,z);
        }
      }
    }
  
  
  //
  // do fourier domain operations
  //
  
  // tranform forward
  fftw_execute(fftwData3D->fwdX);
  fftw_execute(fftwData3D->fwdY);
  fftw_execute(fftwData3D->fwdZ);
  
  // multiply by L^2
  
  for ( unsigned int x = 0; x < szX; ++x ) 
    {
    for ( unsigned int y = 0; y < szY; ++y ) 
      {
      for ( unsigned int z = 0; z < szZ/2+1; ++z ) 
        {
        unsigned int index = z + (szZ/2+1) * (y + szY*x);
        T lVal = pL->getValue(x,y,z,0);
        
        fftwData3D->outX[index][0] = fftwData3D->outX[index][0] * lVal;
        fftwData3D->outX[index][1] = fftwData3D->outX[index][1] * lVal;
        fftwData3D->outY[index][0] = fftwData3D->outY[index][0] * lVal;
        fftwData3D->outY[index][1] = fftwData3D->outY[index][1] * lVal;
        fftwData3D->outZ[index][0] = fftwData3D->outZ[index][0] * lVal;
        fftwData3D->outZ[index][1] = fftwData3D->outZ[index][1] * lVal;
        }
      }
    }
      
  // transform backward
  fftw_execute(fftwData3D->bckX);
  fftw_execute(fftwData3D->bckY);
  fftw_execute(fftwData3D->bckZ);
  
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
        
        T valX = fftwData3D->inX[index]/(szX*szY*szZ);
        T valY = fftwData3D->inY[index]/(szX*szY*szZ);
        T valZ = fftwData3D->inZ[index]/(szX*szY*szZ);
        
        pVecField->setX(x,y,z, valX);
        pVecField->setY(x,y,z, valY);
        pVecField->setZ(x,y,z, valZ);
        
        }
      }
    }
}

template <class T, unsigned int VImageDimension >
void CFourierDomainKernel< T, VImageDimension >::ConvolveInFourierDomain( VectorFieldType* pVecField, VectorImageType* pL )
{
  switch ( VImageDimension )
    {
    case 2:
      ConvolveInFourierDomain2D( pVecField, pL );
      break;
    case 3:
      ConvolveInFourierDomain3D( pVecField, pL );
      break;
    default:
      throw std::runtime_error("Cannot convolve image of such a dimension." );
    }
}

template <class T, unsigned int VImageDimension >
void CFourierDomainKernel< T, VImageDimension >::ConvolveWithKernel( VectorFieldType* pVecField )
{
  if ( !this->m_KernelsWereCreated )
    {
    this->CreateKernelAndInverseKernel( pVecField );
    ConfirmKernelsWereCreated();
    AllocateFFTDataStructures( pVecField );
    }
  else 
    {
    assert( m_ptrL != NULL );
    if ( pVecField->getSizeX() != m_ptrL->getSizeX() ||
         pVecField->getSizeY() != m_ptrL->getSizeY() ||
         pVecField->getSizeZ() != m_ptrL->getSizeZ() )
      {
      throw std::runtime_error( "Kernel incompatible with velocity field size.");
      }
    }

  assert( m_ptrL != NULL );
  ConvolveInFourierDomain( pVecField, m_ptrL );
}

template <class T, unsigned int VImageDimension >
void CFourierDomainKernel< T, VImageDimension >::ConvolveWithInverseKernel( VectorFieldType* pVecField )
{

  if ( !this->m_KernelsWereCreated )
    {
    this->CreateKernelAndInverseKernel( pVecField );
    ConfirmKernelsWereCreated();
    AllocateFFTDataStructures( pVecField );
    }
  else
    {
    assert( m_ptrLInv != NULL );
    if ( pVecField->getSizeX() != m_ptrLInv->getSizeX() ||
         pVecField->getSizeY() != m_ptrLInv->getSizeY() ||
         pVecField->getSizeZ() != m_ptrLInv->getSizeZ() )
      {
      throw std::runtime_error( "Kernel incompatible with velocity field size.");
      }
    }

  assert( m_ptrLInv != NULL );
  ConvolveInFourierDomain( pVecField, m_ptrLInv );
}

template <class T, unsigned int VImageDimension >
void CFourierDomainKernel< T, VImageDimension >::ConfirmKernelsWereCreated()
{
  this->m_KernelsWereCreated = true;
}



#endif
