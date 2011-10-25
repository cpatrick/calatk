#ifndef C_LDDMM_GEOMETRIC_METAMORPHOSIS_OBJECTIVE_FUNCTION_H
#define C_LDDMM_GEOMETRIC_METAMORPHOSIS_OBJECTIVE_FUNCTION_H

namespace CALATK
{

template <class T, unsigned int VImageDimension, class TState >
class CLDDMMGeometricMetamorphosisObjectiveFunction : public CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< T, VImageDimension, TState >
{
public:

  /** Some useful typedefs */

  typedef CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< T, VImageDimension, TState > Superclass;
  typedef typename Superclass::VectorImageType VectorImageType;
  typedef typename Superclass::VectorFieldType VectorFieldType;

  CLDDMMGeometricMetamorphosisObjectiveFunction();
  virtual ~CLDDMMGeometricMetamorphosisObjectiveFunction();

  void GetImage( VectorImageType* ptrIm, T dTime );

  T GetCurrentEnergy();
  void ComputeGradient();

  SetMacro( Sigma1Sqr, T );
  GetMacro( Sigma1Sqr, T );

  SetMacro( Sigma2Sqr, T );
  GetMacro( Sigma2Sqr, T );

  SetMacro( W, T );
  GetMacro( W, T );

protected:

 /** Some useful protected typedefs */

  typedef typename Superclass::STimePoint STimePoint;
  typedef typename Superclass::VectorFieldPointerType VectorFieldPointerType;
  typedef typename Superclass::VectorImagePointerType VectorImagePointerType;
  typedef typename Superclass::VectorPointerToVectorFieldPointerType VectorPointerToVectorFieldPointerType;
  typedef typename Superclass::VectorPointerToVectorImagePointerType VectorPointerToVectorImagePointerType;
  
  typedef typename Superclass::ImageManagerType ImageManagerType;
  typedef typename Superclass::SImageInformation SImageInformation;
  typedef typename Superclass::SubjectInformationType SubjectInformationType;

  void ComputeImagesForward();
  void ComputeAdjointBackward();

  void DetermineTimePointData( std::vector< STimePoint >& vecTimePointData );

  void DeleteAuxiliaryStructures();
  void CreateAuxiliaryStructures();

  unsigned int DetermineTimeIndexOfTimePoint1();

private:

  unsigned int m_uiTimeIndexOfTimePoint1;

  SImageInformation* pImInfo0;
  SImageInformation* pImInfo1;

  SImageInformation* pMaskInfo0;
  SImageInformation* pMaskInfo1;

  // just convenience pointers to make writing the algorithms easier
  // memory pointed to is taken care of by the imageManager (it there a way to make them const?)
  VectorImagePointerType ptrT0;
  VectorImagePointerType ptrT2;
  VectorImagePointerType ptrI0;
  VectorImagePointerType ptrI1;
  VectorImagePointerType ptrEstI1;
  VectorImagePointerType ptrEstIT1;
  VectorImagePointerType ptrEstIT2;

  VectorFieldPointerType m_ptrMapIn; // map for the numerical solution
  VectorFieldPointerType m_ptrMapOut; // map for the numerical solution
  VectorFieldPointerType m_ptrMapTmp; // map for the numerical solution

  // temporary storage memory managed by this class
  VectorImagePointerType m_ptrT2M1;
  VectorImagePointerType m_ptrIT1M1;

  VectorPointerToVectorImagePointerType m_ptrI;
  VectorPointerToVectorImagePointerType m_ptrT;

  VectorPointerToVectorImagePointerType m_ptrLambda;
  VectorPointerToVectorImagePointerType m_ptrLambdaT;

  VectorImagePointerType m_ptrCurrentAdjointDifference;
  VectorImagePointerType m_ptrCurrentAdjointDifferenceT;

  VectorImagePointerType m_ptrCurrentLambdaEnd;
  VectorImagePointerType m_ptrCurrentLambdaTEnd;

  VectorImagePointerType m_ptrI1Comp;
  VectorImagePointerType m_ptrEstI1Comp;

  VectorImagePointerType m_ptrDeterminantOfJacobian;

  VectorFieldPointerType m_ptrTmpVelocityField;
  VectorFieldPointerType m_ptrTmpGradient;

  T m_Sigma1Sqr; // 1/m_Sigma1Sqr is the multiplier for the data attachment term, for the background deformation
  T m_Sigma2Sqr; // 1/m_Sigma2Sqr is the multiplier for the data attachment term, for the foreground deformation

  T m_W; // is the weighting between the foreground and the background velocity fields.

};

#include "CLDDMMGeometricMetamorphosisObjectiveFunction.txx"

} // end namespace

#endif
