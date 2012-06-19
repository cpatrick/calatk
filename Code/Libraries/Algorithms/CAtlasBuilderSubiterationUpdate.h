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

#ifndef C_ATLAS_BUILDER_SUBITERATION_UPDATE_H
#define C_ATLAS_BUILDER_SUBITERATION_UPDATE_H

#include "CAtlasBuilder.h"
#include "CAtlasSubiterationUpdateObjectiveFunction.h"

namespace CALATK
{

template < class TState >

class CAtlasBuilderSubiterationUpdate : public CAtlasBuilder< TState >
{

public:
  /** Standard class typedefs. */
  typedef CAtlasBuilderSubiterationUpdate         Self;
  typedef CAtlasBuilder< TState >           Superclass;

  typedef typename TState::IndividualStateType IndividualStateType;

  typedef itk::SmartPointer< Self >         Pointer;
  typedef itk::SmartPointer< const Self >   ConstPointer;

  /* some useful typedefs */
  typedef typename TState::FloatType FloatType;
  typedef typename Superclass::VectorImageType VectorImageType;
  typedef typename Superclass::VectorFieldType VectorFieldType;

  typedef CAtlasObjectiveFunction< TState >  AtlasObjectiveFunctionType;
  typedef typename AtlasObjectiveFunctionType::IndividualObjectiveFunctionType IndividualObjectiveFunctionType;

  typedef VectorImageUtils< FloatType, TState::ImageDimension > VectorImageUtilsType;

  CAtlasBuilderSubiterationUpdate();
  virtual ~CAtlasBuilderSubiterationUpdate();

protected:

  virtual AtlasObjectiveFunctionType* CreateAtlasObjectiveFunction();
};

#include "CAtlasBuilderSubiterationUpdate.txx"

} // end namespace

#endif // C_ATLAS_BUILDER_SUBITERATION_UPDATE_H
