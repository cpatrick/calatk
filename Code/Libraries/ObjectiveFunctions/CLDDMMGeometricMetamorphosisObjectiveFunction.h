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

#ifndef C_LDDMM_GEOMETRIC_METAMORPHOSIS_OBJECTIVE_FUNCTION_H
#define C_LDDMM_GEOMETRIC_METAMORPHOSIS_OBJECTIVE_FUNCTION_H

#include "CLDDMMSpatioTemporalVelocityFieldObjectiveFunction.h"
#include "CKernel.h"

namespace CALATK
{

template < class TState >
class CLDDMMGeometricMetamorphosisObjectiveFunction
    : public CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< TState >
{
public:
  /** Standard class typedefs. */
  typedef CLDDMMGeometricMetamorphosisObjectiveFunction                 Self;
  typedef CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< TState >  Superclass;
  typedef itk::SmartPointer< Self >                                     Pointer;
  typedef itk::SmartPointer< const Self >                               ConstPointer;

  /* Some useful typedefs */
  typedef typename TState::FloatType T;

  typedef typename Superclass::CEnergyValues CEnergyValues;

  typedef typename Superclass::VectorImageType VectorImageType;
  typedef typename Superclass::VectorFieldType VectorFieldType;

  typedef typename Superclass::EvolverType     EvolverType;
  typedef typename Superclass::KernelType      KernelType;


  CLDDMMGeometricMetamorphosisObjectiveFunction();
  virtual ~CLDDMMGeometricMetamorphosisObjectiveFunction();

  void GetImageT( VectorImageType* ptrIm, T dTime );
  void GetImage( VectorImageType* ptrIm, T dTime );
  void GetInitialImage( VectorImageType* ptrIm );
  const VectorImageType* GetPointerToInitialImage() const;
  void GetMomentum( VectorImageType* ptrMomentum, T dTime );
  void GetInitialMomentum( VectorImageType* ptrMomentum );

  CEnergyValues GetCurrentEnergy();
  void ComputeGradient();

  void ComputeInitialUnsmoothedVelocityGradient( VectorFieldType* ptrInitialUnsmoothedVelocityGradient, unsigned int uiKernelNumber = 0 );

  SetMacro( Sigma1Sqr, T );
  GetMacro( Sigma1Sqr, T );

  SetMacro( Sigma2Sqr, T );
  GetMacro( Sigma2Sqr, T );

  SetMacro( W, T );
  GetMacro( W, T );

  void SetMaskKernelPointer( KernelType * pKernel );
  KernelType * GetMaskKernelPointer();

  void OutputStateInformation( unsigned int uiIter, std::string outputPrefix="" );

  virtual void SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned );

protected:

 /** Some useful protected typedefs */

  typedef typename Superclass::STimePoint STimePoint;
  typedef typename Superclass::VectorFieldPointerType VectorFieldPointerType;
  typedef typename Superclass::VectorImagePointerType VectorImagePointerType;
  typedef typename Superclass::VectorPointerToVectorFieldPointerType VectorPointerToVectorFieldPointerType;
  typedef typename Superclass::VectorPointerToVectorImagePointerType VectorPointerToVectorImagePointerType;
  
  typedef typename Superclass::ImageManagerType ImageManagerType;
  typedef typename Superclass::ImageInformation ImageInformation;
  typedef typename Superclass::SubjectInformationType SubjectInformationType;

  void ComputeImagesForward();
  void ComputeAdjointBackward();
  void ComputeInvertedMask( const VectorImageType* ptrImIn, VectorImageType* ptrImOut);
  void ComputeCompositedImage( const VectorImageType* ptrImIn, const VectorImageType* ptrInvMask1, const VectorImageType* ptrInvMask2, VectorImageType* ptrImComp );

  void DetermineTimePointData( std::vector< STimePoint >& vecTimePointData );

  void DeleteAuxiliaryStructures();
  void CreateAuxiliaryStructures();

  unsigned int DetermineTimeIndexOfTimePoint1();

private:

  unsigned int m_uiTimeIndexOfTimePoint1;

  ImageInformation* m_ptrImageInformation0;
  ImageInformation* m_ptrImageInformation1;

  ImageInformation* m_ptrMaskInformation0;
  ImageInformation* m_ptrMaskInformation1;

  // just convenience pointers to make writing the algorithms easier
  // memory pointed to is taken care of by the imageManager (it there a way to make them const?)
  VectorImagePointerType ptrT0;
  VectorImagePointerType ptrT2;
  VectorImagePointerType ptrI0;
  VectorImagePointerType ptrI1;
  VectorImagePointerType ptrEstI1;
  VectorImagePointerType ptrEstT1;
  VectorImagePointerType ptrEstT2;

  VectorFieldPointerType m_ptrMapIn; // map for the numerical solution
  VectorFieldPointerType m_ptrMapOut; // map for the numerical solution
  VectorFieldPointerType m_ptrMapTmp; // map for the numerical solution

  // temporary storage memory managed by this class
  VectorImagePointerType m_ptrT2M1;
  VectorImagePointerType m_ptrEstT1M1;

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

  const T DefaultSigma1Sqr;
  const T DefaultSigma2Sqr;
  const T DefaultW;

  bool m_ExternallySetSigma1Sqr;
  bool m_ExternallySetSigma2Sqr;
  bool m_ExternallySetW;

  typename KernelType::Pointer m_ptrMaskKernel;
};

#include "CLDDMMGeometricMetamorphosisObjectiveFunction.txx"

} // end namespace

#endif
