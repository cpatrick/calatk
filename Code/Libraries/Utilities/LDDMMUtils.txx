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

#ifndef LDDMM_UTILS_TXX
#define LDDMM_UTILS_TXX

#ifdef _MSC_VER
#include <float.h>  // for _isnan() on VC++
#define isnan(x) _isnan(x)  // VC++ uses _isnan() instead of isnan()
#else
#define isnan(x) std::isnan(x)
//#include <math.h>  // for isnan() everywhere else
#endif

///////////////////////
// Wrapper Functions //
///////////////////////

//
// identityMap 
//
template < class T, unsigned int VImageDimension >
void LDDMMUtils< T, VImageDimension >::identityMap(VectorFieldType* fld)
{
  VectorFieldUtils<T, VImageDimension >::identityMap(fld);
}

//
// applyMap
//
template < class T, unsigned int VImageDimension >
void LDDMMUtils< T, VImageDimension >::applyMap( const VectorFieldType* map, const VectorImageType* imIn, VectorImageType* imOut) {

  VectorFieldUtils< T, VImageDimension >::applyMap(map, imIn, imOut);

}

//
// computeDeterminantOfJacobian
//
template < class T, unsigned int VImageDimension >
void LDDMMUtils< T, VImageDimension >::computeDeterminantOfJacobian( const VectorFieldType* fld, VectorImageType* imOut)
{
  
  VectorFieldUtils< T, VImageDimension >::computeDeterminantOfJacobian( fld, imOut );
  
}

//
// createTimeDiscretization
//
template < class T, unsigned int VImageDimension >
void LDDMMUtils< T, VImageDimension >::CreateTimeDiscretization( const std::vector< STimePoint >& vecTimePointData, std::vector< STimePoint >& vecTimeDiscretization, std::vector< T >& vecTimeIncrements, T dNumberOfDiscretizationVolumesPerUnitTime )
{
  vecTimeDiscretization.clear();
  vecTimeIncrements.clear();

  const T dTolerance = 1e-10; // discretization tolerance, so we don't end up with a time-step of this size

  T dDesiredTimeStep = 1.0/dNumberOfDiscretizationVolumesPerUnitTime;

  // go through all the timepoints and enter them into the vecTimeDiscretization structure

  typename std::vector< STimePoint >::const_iterator iter;

  T dLastTimePoint = 0;

  for ( iter = vecTimePointData.begin(); iter != vecTimePointData.end(); ++iter )
    {

    std::cout << "Time point = " << iter->dTime << std::endl;

    if ( vecTimeDiscretization.empty() )
      {
      // this is the first value so let's enter it
      STimePoint currentTimePoint = *iter;
      currentTimePoint.bIsMeasurementPoint = true;
      vecTimeDiscretization.push_back( currentTimePoint );
      dLastTimePoint = currentTimePoint.dTime;
      }
    else
      {
      // this is a different timepoint. Need to create discretization in between if too far away from last time point

      while ( iter->dTime - dLastTimePoint > dDesiredTimeStep + dTolerance )
        {
        dLastTimePoint += dDesiredTimeStep;
        STimePoint timePoint;
        timePoint.bIsMeasurementPoint = false;
        timePoint.dTime = dLastTimePoint;

        vecTimeDiscretization.push_back( timePoint );
        }

        // now it should be small enough, so enter the image information here
        //T deltaT = iter->dTime - dLastTimePoint;
        assert(  iter->dTime - dLastTimePoint <= dDesiredTimeStep + dTolerance );

        STimePoint currentTimePoint = *iter;
        currentTimePoint.bIsMeasurementPoint = true;
        vecTimeDiscretization.push_back( currentTimePoint );

        dLastTimePoint = currentTimePoint.dTime;

      } // end of else (not the first measurement)

    } // end loop over all the subject data

  // go through the time discretization and determine the time-increments

  typename std::vector< STimePoint >::iterator iterTimeDiscretization;
  iterTimeDiscretization = vecTimeDiscretization.begin();

  T dTimeNM1 = iterTimeDiscretization->dTime;

  for ( ++iterTimeDiscretization; iterTimeDiscretization != vecTimeDiscretization.end(); ++iterTimeDiscretization )
    {
    T dTimeN = iterTimeDiscretization->dTime;
    vecTimeIncrements.push_back( dTimeN - dTimeNM1 );
    dTimeNM1 = dTimeN;
    }

  std::cout << "vecTimeIncrements = " << vecTimeIncrements << std::endl;

}


//
// Determining the timepoint data from data of the image manager
//
template < class T, unsigned int VImageDimension >
unsigned int LDDMMUtils< T, VImageDimension >::DetermineTimeSeriesTimePointData( ImageManagerType* ptrImageManager, int subjectIndex, std::vector< STimePoint >& vecTimePointData )
{
  subjectIndex = ptrImageManager->GetFirstSubjectIndexIfNegative( subjectIndex );

  unsigned int uiNumberOfMeasurements = 0;

  std::vector< T > vecMeasurementTimePoints;
  // make sure we have at least two timepoints
  ptrImageManager->GetTimePointsForSubjectIndex( vecMeasurementTimePoints, subjectIndex );
  assert( vecMeasurementTimePoints.size() > 1 );

  std::cout << "Measurement timepoints = " << vecMeasurementTimePoints << std::endl;

  // get the full time-course information for the subject
  std::vector< TimeSeriesDataPointType > timeSeries;
  ptrImageManager->GetTimeSeriesWithSubjectIndex( timeSeries, subjectIndex );

  // clear the time-point information vector
  vecTimePointData.clear();

  // now enter all the time-point information based on the subject information

  // go through all the timepoints and enter them into the vecTimeDiscretization structure

  typename std::vector< TimeSeriesDataPointType >::iterator iter;

  // TODO: just make the image manager return this type of data-structure by default

  T dLastTimePoint = 0;
  bool bFirstValue = true;

  for ( iter = timeSeries.begin(); iter != timeSeries.end(); ++iter )
    {

    uiNumberOfMeasurements++;

    if ( !bFirstValue && ( dLastTimePoint == iter->GetTimePoint() ) )
      {
      // if there is a multiple measurement
      unsigned int uiLast = vecTimePointData.size()-1;
      vecTimePointData[ uiLast ].vecMeasurementImages.push_back( iter->GetImage() );
      vecTimePointData[ uiLast ].vecMeasurementTransforms.push_back( iter->GetTransform() );
      }
    else
      {
      // fill the time-point stucture
      STimePoint timePoint;
      timePoint.bIsMeasurementPoint = true; // all of them are measurements
      timePoint.dTime = iter->GetTimePoint();
      timePoint.vecMeasurementImages.push_back( iter->GetImage() );
      timePoint.vecMeasurementTransforms.push_back( iter->GetTransform() );

      vecTimePointData.push_back( timePoint );

      }

      dLastTimePoint = iter->GetTimePoint();
      bFirstValue = false;

    }

  return uiNumberOfMeasurements;

}


template < class T, unsigned int VImageDimension >
void LDDMMUtils< T, VImageDimension >::GetMapFromToFromSpatioTemporalVelocityField(
    VectorFieldType* ptrMap,
    T dTimeFrom,
    T dTimeTo,
    const std::vector< STimePoint >& vecTimeDiscretization,
    ConstVectorPointerToVectorFieldPointerType ptrSpatioTemporalVelocityField,
    EvolverType* ptrEvolver )
{
  if ( dTimeFrom <= dTimeTo )
    {
      GetMapFromToFromSpatioTemporalVelocityFieldForward( ptrMap, dTimeFrom, dTimeTo, vecTimeDiscretization, ptrSpatioTemporalVelocityField, ptrEvolver );
    }
  else
    {
      GetMapFromToFromSpatioTemporalVelocityFieldBackward( ptrMap, dTimeFrom, dTimeTo, vecTimeDiscretization, ptrSpatioTemporalVelocityField, ptrEvolver );
    }
}

//
// Computing the map between two timepoints in the forward direction
//
template < class T, unsigned int VImageDimension >
void LDDMMUtils< T, VImageDimension >::GetMapFromToFromSpatioTemporalVelocityFieldForward(
    VectorFieldType* ptrMap,
    T dTimeFrom,
    T dTimeTo,
    const std::vector< STimePoint >& vecTimeDiscretization,
    ConstVectorPointerToVectorFieldPointerType ptrSpatioTemporalVelocityField,
    EvolverType* ptrEvolver )
{
  assert( dTimeTo >= dTimeFrom );
  std::cout << "Computing map from " << dTimeFrom << " to " << dTimeTo << std::endl;

  if ( dTimeFrom < vecTimeDiscretization[0].dTime || dTimeTo > vecTimeDiscretization.back().dTime )
    {
    throw std::runtime_error("Requested map outside of valid time range.");
    return;
    }

  VectorFieldType* ptrMapOut = ptrMap;

  // create two additional maps to hold the solution
  typename VectorFieldType::Pointer ptrMapIn = new VectorFieldType( ptrMap );
  typename VectorFieldType::Pointer ptrMapTmp = new VectorFieldType( ptrMap );

  // get the map between two time points
  LDDMMUtils< T, VImageDimension >::identityMap( ptrMapIn );


  T dCurrentTime = vecTimeDiscretization[0].dTime;
  unsigned int uiStart = 0;

  // we may need to fast forward to the beginning time point

  if ( dCurrentTime < dTimeFrom )
    {

    for ( unsigned int iI = 0; iI < vecTimeDiscretization.size()-1; ++iI )
      {
      T dCurrentDT = vecTimeDiscretization[ iI+1 ].dTime - vecTimeDiscretization[ iI ].dTime;
      if ( dCurrentTime + dCurrentDT > dTimeFrom )
        {
        // evolve for an increment
        std::cout << "partially evolve for " << dTimeFrom - dCurrentTime << std::endl;
        ptrEvolver->SolveForward( (*ptrSpatioTemporalVelocityField)[ iI ], ptrMapIn, ptrMapOut, ptrMapTmp, dTimeFrom-dCurrentTime );
        // for next step, copy
        ptrMapIn->Copy( ptrMapOut );
        uiStart = iI+1;
        dCurrentTime += dCurrentDT;
        break;
        }
      else
        {
        // just skip ahead
        dCurrentTime += dCurrentDT;
        uiStart = iI + 1;
        }
      if ( dCurrentTime >= dTimeFrom )
        {
        break;
        }
      }
    }

  std::cout << "fast forwarded to " << dCurrentTime << std::endl;
  std::cout << "starting from index " << uiStart << std::endl;

  // now we can move ahead

  for ( unsigned int iI = uiStart; iI < vecTimeDiscretization.size()-1; ++iI )
    {
    T dCurrentDT = vecTimeDiscretization[ iI+1 ].dTime - vecTimeDiscretization[ iI ].dTime;
    if ( dCurrentTime + dCurrentDT < dTimeTo )
      {
      std::cout << "evolved for " << dCurrentDT << std::endl;
      ptrEvolver->SolveForward( (*ptrSpatioTemporalVelocityField)[ iI ], ptrMapIn, ptrMapOut, ptrMapTmp, dCurrentDT );
      dCurrentTime += dCurrentDT;
      }
    else
      {
      std::cout << "finally partially evolved for " << dTimeTo-dCurrentTime << std::endl;
      ptrEvolver->SolveForward( (*ptrSpatioTemporalVelocityField)[ iI ], ptrMapIn, ptrMapOut, ptrMapTmp, dTimeTo-dCurrentTime );
      dCurrentTime = dTimeTo;
      break;
      }
    // for next step, copy
    ptrMapIn->Copy( ptrMapOut );
    }
}

//
// Computing the map between two timepoints in the backward direction
//
template < class T, unsigned int VImageDimension >
void LDDMMUtils< T, VImageDimension >::GetMapFromToFromSpatioTemporalVelocityFieldBackward(
    VectorFieldType* ptrMap,
    T dTimeFrom,
    T dTimeTo,
    const std::vector< STimePoint >& vecTimeDiscretization,
    ConstVectorPointerToVectorFieldPointerType ptrSpatioTemporalVelocityField,
    EvolverType* ptrEvolver )
{
  assert( dTimeTo <= dTimeFrom );
  std::cout << "Computing map from " << dTimeFrom << " to " << dTimeTo << std::endl;

  if ( dTimeFrom < vecTimeDiscretization[0].dTime || dTimeTo > vecTimeDiscretization.back().dTime )
    {
    throw std::runtime_error("Requested map outside of valid time range.");
    return;
    }

  VectorFieldType* ptrMapOut = ptrMap;

  // create two additional maps to hold the solution
  typename VectorFieldType::Pointer ptrMapIn = new VectorFieldType( ptrMap );
  typename VectorFieldType::Pointer ptrMapTmp = new VectorFieldType( ptrMap );
  typename VectorFieldType::Pointer ptrNegativeVelocity = new VectorFieldType( ptrMap );

  // get the map between two time points
  LDDMMUtils< T, VImageDimension >::identityMap( ptrMapIn );

  T dCurrentTime = vecTimeDiscretization.back().dTime;
  unsigned int uiStart = 0;

  // we may need to fast backward to the beginning time point

  if ( dCurrentTime > dTimeFrom )
    {

    for ( int iI = vecTimeDiscretization.size()-2; iI>=0; --iI )
      {
      T dCurrentDT = vecTimeDiscretization[ iI+1 ].dTime - vecTimeDiscretization[ iI ].dTime;
      if ( dCurrentTime - dCurrentDT < dTimeFrom )
        {
        // evolve for an increment
        std::cout << "partially evolve for " << dCurrentTime - dTimeFrom << std::endl;
        ptrNegativeVelocity->Copy( (*ptrSpatioTemporalVelocityField)[ iI ] );
        ptrNegativeVelocity->MultiplyByConstant( -1.0 );
        ptrEvolver->SolveForward( ptrNegativeVelocity, ptrMapIn, ptrMapOut, ptrMapTmp, dCurrentTime - dTimeFrom );
        // for next step, copy
        ptrMapIn->Copy( ptrMapOut );
        uiStart = iI-1;
        dCurrentTime -= dCurrentDT;
        break;
        }
      else
        {
        // just skip ahead
        dCurrentTime -= dCurrentDT;
        uiStart = iI - 1;
        }
      if ( dCurrentTime <= dTimeFrom )
        {
        break;
        }
      }
    }


  std::cout << "fast backwarded to " << dCurrentTime << std::endl;
  std::cout << "starting from index " << uiStart << std::endl;

  // now we can move backward

  for ( int iI = static_cast< int >( uiStart ); iI >= 0; --iI )
    {
    T dCurrentDT = vecTimeDiscretization[ iI+1 ].dTime - vecTimeDiscretization[ iI ].dTime;
    if ( dCurrentTime - dCurrentDT > dTimeTo )
      {
      std::cout << "evolved for " << dCurrentDT << std::endl;
      ptrNegativeVelocity->Copy( (*ptrSpatioTemporalVelocityField)[ iI ] );
      ptrNegativeVelocity->MultiplyByConstant( -1.0 );
      ptrEvolver->SolveForward( ptrNegativeVelocity, ptrMapIn, ptrMapOut, ptrMapTmp, dCurrentDT );
      dCurrentTime -= dCurrentDT;
      }
    else
      {
      std::cout << "finally partially evolved for " << dCurrentTime - dTimeTo << std::endl;
      ptrNegativeVelocity->Copy( (*ptrSpatioTemporalVelocityField)[ iI ] );
      ptrNegativeVelocity->MultiplyByConstant( -1.0 );
      ptrEvolver->SolveForward( ptrNegativeVelocity, ptrMapIn, ptrMapOut, ptrMapTmp, dCurrentTime-dTimeTo );
      dCurrentTime = dTimeTo;
      break;
      }
    // for next step, copy
    ptrMapIn->Copy( ptrMapOut );
    }

}


#endif
