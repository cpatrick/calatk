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
  ~CLDDMMSpatioTemporalVelocityFieldObjectiveFunction();

  T GetCurrentEnergy();
  void ComputeGradient();

  SetMacro( SigmaSqr, T );
  GetMacro( SigmaSqr, T );

  void GetMap( VectorFieldType* ptrMap, T dTime );
  void GetImage( VectorImageType* ptrIm, T dTime );

  void InitializeState();
  void InitializeDataStructures();
  void InitializeDataStructuresFromState( TState* pState );

  SetMacro( NumberOfDiscretizationVolumesPerUnitTime, T );
  GetMacro( NumberOfDiscretizationVolumesPerUnitTime, T );

protected:

  struct STimePoint
  {
    bool bIsMeasurementPoint;
    T dTime;
    std::vector< VectorImagePointerType > vecMeasurementImages; // to support multiple images per time point
    std::vector< VectorFieldPointerType > vecMeasurementTransforms;
    VectorImagePointerType ptrEstimatedImage;
  };

  void DeleteData();
  void CreateTimeDiscretization();
  void CreateTimeDiscretization( SubjectInformationType* pSubjectInfo, std::vector< STimePoint >& vecTimeDiscretization, std::vector< T >& vecTimeIncrements, T dNumberOfDiscretizationVolumesPerUnitTime );
  void CreateNewStateStructures();
  void ShallowCopyStateStructures( TState* pState );
  void CreateGradientAndAuxiliaryStructures();

  void ComputeImagesForward();
  void ComputeAdjointBackward();

private:

  VectorFieldPointerType m_ptrMapIn; // map for the numerical solution
  VectorFieldPointerType m_ptrMapOut; // map for the numerical solution
  VectorFieldPointerType m_ptrMapTmp; // map for the numerical solution

  VectorFieldPointerType m_ptrTmpVelocityField; // generic temoorary velocity field
  VectorFieldPointerType m_ptrTmpGradient; // to store the temporary gradient
  VectorImagePointerType m_ptrI0; // initial image
  VectorImagePointerType m_ptrCurrentLambdaEnd; // current value of the adjoint, gets successively updated for multiple time points
  VectorImagePointerType m_ptrCurrentAdjointDifference;

  VectorImagePointerType m_ptrDeterminantOfJacobian;

  VectorPointerToVectorImagePointerType m_ptrI; // image
  VectorPointerToVectorImagePointerType m_ptrLambda; // adjoint

  T m_NumberOfDiscretizationVolumesPerUnitTime;

  std::vector< T > m_vecMeasurementTimepoints;

  // bookkeeping structure, which keeps track of what measurements need to be compared to what estimated images

  std::vector< STimePoint > m_vecTimeDiscretization;

  std::vector< T > m_vecTimeIncrements;

  T m_SigmaSqr; // 1/m_SigmaSqr is the multiplier for the data attachment term

};

#include "CLDDMMSpatioTemporalVelocityFieldObjectiveFunction.txx"

} // end namespace

#endif
