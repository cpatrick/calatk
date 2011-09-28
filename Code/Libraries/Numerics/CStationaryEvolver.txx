#ifndef C_STATIONARY_EVOLVER_TXX
#define C_STATIONARY_EVOLVER_TXX

template <class T, unsigned int VImageDimension, class TSpace >
CStationaryEvolver< T, VImageDimension, TSpace >::CStationaryEvolver()
{
}

template <class T, unsigned int VImageDimension, class TSpace >
CStationaryEvolver< T, VImageDimension, TSpace >::~CStationaryEvolver()
{
}

template <class T, unsigned int VImageDimension, class TSpace >
void CStationaryEvolver< T, VImageDimension, TSpace >::SolveForward( VectorFieldType* pV, VectorImageType* pImIn, VectorImageType* pImOut, VectorImageType* pImTmp, T dT )
{
  T dMaxTimeStep = this->m_ptrOneStepEvolver->ComputeMaximalUpdateStep( pV );
  
  T dCurrentTime = 0;

  // convencience variable to allow for easy toggling between the two storage variables while solving
  VectorImageType* pTmpResults[ 2 ];
  pTmpResults[ 0 ] = pImOut;
  pTmpResults[ 1 ] = pImTmp;
  
  bool bResultToggle = 0;

  // copy the initial image into the storage space for the result

  pTmpResults[ 0 ]->copy( pImIn );

  while ( dCurrentTime < dT )
    {
    T dDesiredTimeStep = std::min( dT-dCurrentTime, dMaxTimeStep );
    this->m_ptrOneStepEvolver->PerformStep( pV, pTmpResults[ bResultToggle ], pTmpResults[ !bResultToggle ], dDesiredTimeStep );
    bResultToggle = !bResultToggle;
    dCurrentTime += dDesiredTimeStep;
    }

  // if the result is in the first image we are done (because this is pImOut)
  // otherwise copy into pImOut

  if ( bResultToggle )
    {
    // need to copy, result is in pTmpResults[ 1 ]
    pTmpResults[ 0 ]->copy( pTmpResults[ 1 ] );
    }

}

#endif
