/**
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
void LDDMMUtils< T, VImageDimension >::DetermineTimeSeriesTimePointData( ImageManagerType* ptrImageManager, unsigned int uiSubjectIndex, std::vector< STimePoint >& vecTimePointData )
{
  // get the subject ids
  std::vector< unsigned int > vecSubjectIndices;
  ptrImageManager->GetAvailableSubjectIndices( vecSubjectIndices );

  unsigned int uiNumberOfDifferentSubjects = vecSubjectIndices.size();

  if ( uiNumberOfDifferentSubjects != 1 )
    {
    throw std::runtime_error( "DetermineTimeSeriesTimePointData: Currently only supports one subject at a time." );
    return;
    }

  std::vector< T > vecMeasurementTimepoints;
  // make sure we have at least two timepoints
  ptrImageManager->GetTimepointsForSubjectIndex( vecMeasurementTimepoints, vecSubjectIndices[ uiSubjectIndex ] );
  assert( vecMeasurementTimepoints.size() > 1 );

  std::cout << "Measurement timepoints = " << vecMeasurementTimepoints << std::endl;

  // get the full time-course information for the subject
  SubjectInformationType* pSubjectInfo;
  ptrImageManager->GetImagesWithSubjectIndex( pSubjectInfo, vecSubjectIndices[ uiSubjectIndex ] );

  // clear the time-point information vector

  vecTimePointData.clear();

  // now enter all the time-point information based on the subject information

  // go through all the timepoints and enter them into the vecTimeDiscretization structure

  typename SubjectInformationType::iterator iter;

  // TODO: just make the image manager return this type of data-structure by default

  T dLastTimePoint = 0;
  bool bFirstValue = true;

  for ( iter = pSubjectInfo->begin(); iter != pSubjectInfo->end(); ++iter )
    {

    if ( !bFirstValue && ( dLastTimePoint == (*iter)->timepoint ) )
      {
      // if there is a multiple measurement
      unsigned int uiLast = vecTimePointData.size()-1;
      vecTimePointData[ uiLast ].vecMeasurementImages.push_back( (*iter)->pIm );
      vecTimePointData[ uiLast ].vecMeasurementTransforms.push_back( (*iter)->pTransform );
      }
    else
      {
      // fill the time-point stucture
      STimePoint timePoint;
      timePoint.bIsMeasurementPoint = true; // all of them are measurements
      timePoint.dTime = (*iter)->timepoint;
      timePoint.vecMeasurementImages.push_back( (*iter)->pIm );
      timePoint.vecMeasurementTransforms.push_back( (*iter)->pTransform );

      vecTimePointData.push_back( timePoint );

      }

      dLastTimePoint = (*iter)->timepoint;
      bFirstValue = false;

    }

}

#endif
