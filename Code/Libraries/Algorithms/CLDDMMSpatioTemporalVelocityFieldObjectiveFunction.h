#ifndef C_LDDMM_SPATIO_TEMPORAL_VELOCITY_FIELD_OBJECTIVE_FUNCTION_H
#define C_LDDMM_SPATIO_TEMPORAL_VELOCITY_FIELD_OBJECTIVE_FUNCTION_H

#include "CVelocityFieldObjectiveFunction.h"
#include "CALATKCommon.h"

namespace CALATK
{

template <class T, class TState, unsigned int VImageDimension=3 >
class CLDDMMSpatioTemporalVelocityFieldObjectiveFunction : public CVelocityFieldObjectiveFunction< T, TState, VImageDimension >
{
public:
  
  /** some useful typedefs */

  typedef CVelocityFieldObjectiveFunction< T, TState, VImageDimension > Superclass;
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
  ~CLDDMMSpatioTemporalVelocityFieldObjectiveFunction();

  T GetCurrentEnergy();
  void ComputeGradient();

  void GetMap( VectorFieldType* ptrMap, T dTime );
  void GetImage( VectorImageType* ptrIm, T dTime );

  void InitializeState();
  void InitializeDataStructures();
  void InitializeDataStructuresFromState( TState* pState );

  SetMacro( NumberOfDiscretizationVolumesPerUnitTime, T );
  GetMacro( NumberOfDiscretizationVolumesPerUnitTime, T );

protected:

  void DeleteData();
  void CreateTimeDiscretization( std::vector< T > &vecTimePoints, std::vector< T > &vecTimeIncrements, std::vector< T > &vecMeasurementTimePoints, T dNumberOfDiscretizationVolumesPerUnitTime );

private:

  VectorFieldPointerType m_ptrMapIn; // map for the numerical solution
  VectorFieldPointerType m_ptrMapOut; // map for the numerical solution
  VectorFieldPointerType m_ptrMapTmp; // map for the numerical solution

  VectorFieldPointerType m_ptrTmpVelocityField; // generic temoorary velocity field
  VectorFieldPointerType m_ptrTmpGradient; // to store the temporary gradient
  VectorImagePointerType m_ptrI0; // initial image
  VectorImagePointerType m_ptrCurrentLambdaEnd; // current value of the adjoint, gets successively updated for multiple time points

  VectorImagePointerType m_ptrDeterminantOfJacobian;

  VectorPointerToVectorImagePointerType m_ptrI; // image
  VectorPointerToVectorImagePointerType m_ptrLambda; // adjoint

  T m_NumberOfDiscretizationVolumesPerUnitTime;
  std::vector< T > m_vecMeasurementTimepoints;
  unsigned int m_uiTotalNumberOfDiscretizationSteps;
  std::vector< T > m_vecTimeIncrements;
  std::vector< T > m_vecTimePoints;
};

#include "CLDDMMSpatioTemporalVelocityFieldObjectiveFunction.txx"

} // end namespace

#endif
