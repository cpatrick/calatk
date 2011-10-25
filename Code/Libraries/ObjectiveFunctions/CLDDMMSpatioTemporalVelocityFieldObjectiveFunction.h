#ifndef C_LDDMM_SPATIO_TEMPORAL_VELOCITY_FIELD_OBJECTIVE_FUNCTION_H
#define C_LDDMM_SPATIO_TEMPORAL_VELOCITY_FIELD_OBJECTIVE_FUNCTION_H

#include "CVelocityFieldObjectiveFunction.h"
#include "LDDMMUtils.h"
#include "CALATKCommon.h"

namespace CALATK
{

template <class T, unsigned int VImageDimension, class TState >
class CLDDMMSpatioTemporalVelocityFieldObjectiveFunction : public CVelocityFieldObjectiveFunction< T, VImageDimension, TState >
{
public:
  
  /** some useful typedefs */

  typedef CVelocityFieldObjectiveFunction< T, VImageDimension, TState > Superclass;
  typedef typename Superclass::VectorImageType VectorImageType;
  typedef typename Superclass::VectorFieldType VectorFieldType;

  typedef VectorFieldType* VectorFieldPointerType;
  typedef std::vector< VectorFieldPointerType >* VectorPointerToVectorFieldPointerType;

  typedef VectorImageType* VectorImagePointerType;
  typedef std::vector< VectorImagePointerType >* VectorPointerToVectorImagePointerType;

  typedef CImageManager< T, VImageDimension > ImageManagerType;
  typedef typename ImageManagerType::SImageInformation SImageInformation;
  typedef typename ImageManagerType::SubjectInformationType SubjectInformationType;

  CLDDMMSpatioTemporalVelocityFieldObjectiveFunction();
  virtual ~CLDDMMSpatioTemporalVelocityFieldObjectiveFunction();

  void InitializeState();
  void InitializeState( TState* pState );

  void GetMap( VectorFieldType* ptrMap, T dTime );

  SetMacro( NumberOfDiscretizationVolumesPerUnitTime, T );
  GetMacro( NumberOfDiscretizationVolumesPerUnitTime, T );

protected:

  struct STimePoint
  {
    bool bIsMeasurementPoint;
    T dTime;
    std::vector< VectorImagePointerType > vecMeasurementImages; // to support multiple images per time point
    std::vector< VectorFieldPointerType > vecMeasurementTransforms;
    std::vector< VectorImagePointerType > vecEstimatedImages;
  };

  void InitializeDataStructures();
  void InitializeDataStructuresFromState( TState* pState );

  void DeleteData();

  virtual void DeleteAuxiliaryStructures() = 0;
  virtual void CreateAuxiliaryStructures() = 0;

  virtual void DetermineTimePointData( std::vector< STimePoint >& vecTimePointData ) = 0;

  void CreateTimeDiscretization();
  void CreateTimeDiscretization( const std::vector< STimePoint >& vecTimePointData, std::vector< STimePoint >& vecTimeDiscretization, std::vector< T >& vecTimeIncrements, T dNumberOfDiscretizationVolumesPerUnitTime );

  void CreateNewStateStructures();
  void CreateNewGradientStructures();
  void ShallowCopyStateStructures( TState* pState );

  T m_NumberOfDiscretizationVolumesPerUnitTime;

  // bookkeeping structure, which keeps track of what measurements need to be compared to what estimated images

  std::vector< STimePoint > m_vecTimeDiscretization;
  std::vector< T > m_vecTimeIncrements;

};

#include "CLDDMMSpatioTemporalVelocityFieldObjectiveFunction.txx"

} // end namespace

#endif
