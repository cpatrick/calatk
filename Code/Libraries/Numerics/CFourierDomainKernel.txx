#ifndef C_FOURIER_DOMAIN_KERNEL_TXX
#define C_FOURIER_DOMAIN_KERNEL_TXX

template <class T, unsigned int VImageDimension >
CFourierDomainKernel< T, VImageDimension >::CFourierDomainKernel()
{
  fftwData2D = NULL;
  fftwData3D = NULL;
}

template <class T, unsigned int VImageDimension >
CFourierDomainKernel< T, VImageDimension >::~CFourierDomainKernel()
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
}

template <class T, unsigned int VImageDimension >
void CFourierDomainKernel< T, VImageDimension >::AllocateFFTDataStructures2D( unsigned int szX, unsigned int szY )
{
  // Set up the fftw data
  unsigned int numElts = szX*szY;
  fftwData2D = new fftwData2D();
  
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
  fftwData3D = new fftwData3D();
  
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
void CFourierDomainKernel< T, VImageDimension >::AllocateFFTDataStructures( VectorImage* pVecIm )
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
      throw std::runtime_erro( "Cannot allocate FFT data structure of desired dimension." );
    }
}

CONTINUE HERE
TODO: Add the actual convolution/deconvolution functionality
TODO: Make this so we can apply it to scalar and vector images
TODO: Hook up to Helmholtz operator

#endif
