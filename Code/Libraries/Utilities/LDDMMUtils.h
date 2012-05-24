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

#ifndef LDDMM_UTILS_H
#define LDDMM_UTILS_H

#include "CALATKCommon.h"
#include "CImageManager.h"
#include "VectorImage.h"
#include "VectorField.h"
#include "VectorImageUtils.h"
#include "VectorFieldUtils.h"
#include "CEvolver.h"

#include <stdexcept>
#include <cmath>

// DEBUG
#include <iostream>

namespace CALATK
{

/**
 * LDDMMUtils.h - Utilities for the LDDMM algorithm
 *
 * This class provides a number of static utility functions that are used
 * during the LDDMM algorithm.  These functions are wrappers that allow the 
 * algorithm to make calls independed of whether 2D or 3D images are used.
 */

template < class T, unsigned int VImageDimension = 3 >
class LDDMMUtils 
{

public:

  typedef VectorImage< T, VImageDimension > VectorImageType;
  typedef VectorField< T, VImageDimension > VectorFieldType;

  typedef typename VectorFieldType::Pointer             VectorFieldPointerType;
  typedef std::vector< VectorFieldPointerType > *       VectorPointerToVectorFieldPointerType;
  typedef const std::vector< VectorFieldPointerType >*  ConstVectorPointerToVectorFieldPointerType;

  typedef CTimePoint< T, VectorImageType, VectorFieldType > STimePoint;
  typedef CImageManager< T, VImageDimension > ImageManagerType;
  typedef typename ImageManagerType::ImageInformation ImageInformation;
  typedef typename ImageManagerType::SubjectInformationType SubjectInformationType;

  typedef CEvolver< T, VImageDimension > EvolverType;


  /*********************
   * Wrapper Functions *
   *********************/
  
  /**
   * Function that sets input vector field to the identity map (2D/3D)
   *
   * @param fld - the vector field to be motified in place
   */
  static void identityMap(VectorFieldType* fld);
  
  /**
   * Function that applys a map to an image (2D/3D)
   *
   * @param map - the map to apply to im
   * @param imIn - the image to be pulled
   * @param imOut - return variable for the resulting image
   */
  static void applyMap( const VectorFieldType* map, const VectorImageType* imIn, VectorImageType* imOut);
  
  /**
   * Function that computes the determinant of the jacobian for a vector field. (2D/3D)
   *
   * @param fld - the vector field to be processed
   * @param imOut - image that will hold the determinant of Jacobian
   */
  static void computeDeterminantOfJacobian(const VectorFieldType* fld, VectorImageType* imOut);
  
  /**
    * Function that creates a time discretization given a set of measurement timepoints
    *
    * @param vecTimePointData - vector which holds the timepoints of the measurements
    * @param vecTimeDiscretization - return vector containing the computed time discretization
    * @param vecTimeIncrements - return vector containing the time increments of the discretization
    * @param dNumberOfDiscretizationVolumesPerUnitTime - number of desired time discretization steps per unit time
    */
  static void CreateTimeDiscretization( const std::vector< STimePoint >& vecTimePointData, std::vector< STimePoint >& vecTimeDiscretization, std::vector< T >& vecTimeIncrements, T dNumberOfDiscretizationVolumesPerUnitTime );

  /**
    * Function that teases out the timepoints for a particular subject from an image manager
    * and returns a vector of this time point information which can be used to compute a time-discretization for LDDMM
    *
    * @param pImageMangager - pointer to the image manager
    * @param uiSubjectIndex - index of subject of which the timepoints should be extracted
    * @param vecTimePointData - resulting vector of timepoint data
    * @return number of measurements in the time series
    */
  static unsigned int DetermineTimeSeriesTimePointData( ImageManagerType* pImageManager, unsigned int uiSubjectIndex, std::vector< STimePoint >& vecTimePointData );

  /**
    * Function which computes a map between two time-points given a spatio-temporal velocity field.
    * Here, the assumption is that dTimeFrom<dTimeTo.
    *
    * @return ptrMap - resulting map
    * @param dTimeFrom - beginning time-point
    * @param dTimeTo - ending time-point
    * @param vecTimeDiscretization - vector which holds the temporal discretization information (t's)
    * @param ptrSpatioTemporalVelocityField - pointer to the spatio-temporal velocity field
    * @param ptrEvolver - pointer to the evolver which is used to advect the map (during the computations) to obtain the result
    */
  static void GetMapFromToFromSpatioTemporalVelocityField(
      VectorFieldType* ptrMap,
      T dTimeFrom,
      T dTimeTo,
      const std::vector< STimePoint >& vecTimeDiscretization,
      ConstVectorPointerToVectorFieldPointerType ptrSpatioTemporalVelocityField,
      EvolverType* ptrEvolver );

};

#include "LDDMMUtils.txx"

} // end namespace

#endif
