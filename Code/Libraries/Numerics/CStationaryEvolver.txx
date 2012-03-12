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

#ifndef C_STATIONARY_EVOLVER_TXX
#define C_STATIONARY_EVOLVER_TXX

template <class T, unsigned int VImageDimension >
CStationaryEvolver< T, VImageDimension >::CStationaryEvolver()
{
}

template <class T, unsigned int VImageDimension >
CStationaryEvolver< T, VImageDimension >::~CStationaryEvolver()
{
}

template <class T, unsigned int VImageDimension >
void CStationaryEvolver< T, VImageDimension >::SolveForward( VectorFieldType* pV, VectorImageType* pImIn, VectorImageType* pImOut, VectorImageType* pImTmp, T dT )
{
  // needs to make sure that it will be initialized for the given velocity field
  this->m_ptrOneStepEvolver->SetInitializeOneStepEvolverState( true );

  // FIXME: What should the multiplier be here, 1, 0.25?
  T dMaxTimeStep = this->m_ptrOneStepEvolver->ComputeMaximalUpdateStep( pV );
  assert( dMaxTimeStep>0 );

  T dCurrentTime = 0;

  // convencience variable to allow for easy toggling between the two storage variables while solving
  VectorImageType* pTmpResults[ 2 ];
  pTmpResults[ 0 ] = pImOut;
  pTmpResults[ 1 ] = pImTmp;
  
  bool bResultToggle = 0;

  // copy the initial image into the storage space for the result

  pTmpResults[ 0 ]->Copy( pImIn );

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
    pTmpResults[ 0 ]->Copy( pTmpResults[ 1 ] );
    }

}

#endif
