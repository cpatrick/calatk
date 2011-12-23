#ifndef __itkLongitudinalStatisticsFilter_h
#define __itkLongitudinalStatisticsFilter_h

#include "itkImageSource.h"
#include "itkConceptChecking.h"
#include "itkVectorImage.h"
#include "itkSimpleDataObjectDecorator.h"
#include "itkImageFileReader.h"

#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"

#include <itkDiffusionTensor3D.h>

#include <vector>
#include <string>
#include <algorithm>

#include <stdlib.h>

#include <vnl/algo/vnl_symmetric_eigensystem.h>
#include <vnl/vnl_matrix.h>
// Hongtu's includes

#include "globals.h"

#include "pointdef.h"
#include "surallfileM10Y06_MN_JP.h"
#include "geometry.h"

namespace itk
{

/** \class LongitudinalStatisticsFilter
 *  \brief This class takes creates a DWI atlas form a set of DWI images.
 *
 */

const unsigned int DIM = 3;

template < class MyRealType, class MyIntegerType >
bool sort_pred(const std::pair<MyRealType, MyIntegerType>& left, const std::pair<MyRealType, MyIntegerType>& right);

template <class MyRealType, class MyIntegerType>
class ITK_EXPORT LongitudinalStatisticsFilter :
  public ImageSource< VectorImage< MyRealType , DIM > >
{
public:

  /** Standard class typedefs. */
  typedef LongitudinalStatisticsFilter                Self;
  typedef ImageSource< VectorImage< MyRealType,  DIM > > Superclass;
  typedef SmartPointer<Self>                   Pointer;
  typedef SmartPointer<const Self>             ConstPointer;
  
  typedef VectorImage< MyRealType, DIM> VectorRealOutputImageType;
  typedef Image< MyRealType, DIM > ScalarRealOutputImageType;

  typedef VectorRealOutputImageType VectorImageType;
  typedef ScalarRealOutputImageType ScalarImageType;

  typedef typename VectorImageType::PixelType VectorType;

  typedef typename Superclass::Pointer    OutputImagePointer;
  typedef typename VectorRealOutputImageType::SpacingType SpacingType;
  typedef typename VectorRealOutputImageType::PointType   PointType;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(LongitudinalStatisticsFilter,ImageSource);

  /** Superclass typedefs. */
  typedef typename Superclass::OutputImageRegionType OutputImageRegionType;

  /** Some convenient typedefs. */

  typedef typename VectorRealOutputImageType::SizeType SizeType;

  typedef itk::DiffusionTensor3D< MyRealType > TensorType;
  typedef itk::Image< TensorType, DIM > TensorImageType;
  typedef itk::ImageFileReader< TensorImageType > TensorReaderType;

  typedef itk::ImageRegionConstIterator< TensorImageType > TensorIteratorType;
  typedef itk::ImageRegionIterator< VectorImageType > VectorIteratorType;

  typedef itk::Image< MyIntegerType, DIM > ScalarIntegerImageType;
  typedef itk::ImageFileReader< ScalarIntegerImageType > ScalarIntegerReaderType;
  typedef itk::ImageRegionIterator< ScalarIntegerImageType > ScalarIntegerIteratorType;

  typedef itk::ImageRegionIterator< ScalarImageType > ScalarIteratorType;

  // typedef ImageFileReader<DiffusionTensorType> TensorFileReaderType;

  /** Since a string is not a dataobject, we use the decorator to push
   *  it down the pipeline */ 
  typedef SimpleDataObjectDecorator< std::string*  > InputStringObjectType; 
  
  /** Determine the image dimension. */
  itkStaticConstMacro(ImageDimension, unsigned int, DIM );
  
  /** Set/Get the input of this process object.  */
  /* We have text files as input here for data that needs to be loaded */

  virtual void SetInput( const std::string* sCaseFile ); // TODO: Change

  /** Set the spacing (size of a pixel) of the image. 
   *  \sa GetSpacing() */
  itkSetMacro(Spacing,SpacingType);
  virtual void SetSpacing(const double* values);

  /** Get the spacing (size of a pixel) of the image. 
   * For ImageBase and Image, the default data spacing is unity. */
  itkGetConstReferenceMacro(Spacing,SpacingType);

  /** Set the origin of the image. 
   * \sa GetOrigin() */
  itkSetMacro(Origin,PointType);
  virtual void SetOrigin(const double* values);

  const InputStringObjectType* GetInput( void );

  /** Set verbose mode. */
  itkSetMacro( Verbose, bool);
  itkGetMacro( Verbose, bool);
  itkBooleanMacro( Verbose );

  itkSetMacro( DebugVerbose, bool );
  itkGetMacro( DebugVerbose, bool );
  itkBooleanMacro( DebugVerbose );

  itkSetMacro( CovaryWithGroup, bool );
  itkGetMacro( CovaryWithGroup, bool );
  itkBooleanMacro( CovaryWithGroup );

  itkSetMacro( CovaryWithGender, bool );
  itkGetMacro( CovaryWithGender, bool );
  itkBooleanMacro( CovaryWithGender );

  itkSetMacro( NoLogTransform, bool );
  itkGetMacro( NoLogTransform, bool );
  itkBooleanMacro( NoLogTransform );

  itkSetMacro( NoTimePointScaling, bool );
  itkGetMacro( NoTimePointScaling, bool );
  itkBooleanMacro( NoTimePointScaling );

  itkSetMacro( PathPrefix, std::string );
  itkGetMacro( PathPrefix, std::string );

  itkSetMacro( DesignMatrix, std::string );
  itkGetMacro( DesignMatrix, std::string );

  itkSetMacro( MaskImage, std::string );
  itkGetMacro( MaskImage, std::string );

  // not using itk marcros to preserve Hongtu's variable names
  void SetDimSPD( int v );
  int GetDimSPD();
  
  itkSetMacro( ConstraintMatrixFileName, std::string );
  itkGetMacro( ConstraintMatrixFileName, std::string );

  itkSetMacro( Seed, int );
  itkGetMacro( Seed, int );

  VectorRealOutputImageType* GetCovarianceOutputVolume();
  ScalarRealOutputImageType* GetRawPValueOutputVolume();
  ScalarRealOutputImageType* GetRSPValueOutputVolume();
  ScalarRealOutputImageType* GetSCOREValueOutputVolume();
  ScalarRealOutputImageType* GetWALDValueOutputVolume();

 /** Get the origin of the image.  */
 // itkGetConstReferenceMacro(Origin,PointType);

  
protected:
  LongitudinalStatisticsFilter();
  ~LongitudinalStatisticsFilter();

  typedef std::pair< MyRealType, unsigned int> myRealIntPair;

  typedef struct {
    std::map<unsigned int, unsigned int> subjectIdToIndexMap;
    std::vector<unsigned int> vSubjectId;
    std::vector<unsigned int> vGroup;
    std::vector<unsigned int> vGender;
    std::vector< std::vector<MyRealType> > vvScaleFactor;
    std::vector< std::vector<MyRealType> > vvAge;
    std::vector< std::vector<std::string> > vvFilenames;
    std::vector< std::vector< std::vector<MyRealType> > > vvAdditionalCovariates;
  } SCaseInformation;

  void ParseCaseFile();
  void DisplayCaseInformation();

  void PopulateStatisticsVariablesFromCaseInformationAndImageDimensions();

  void TimeSortCaseInformation( SCaseInformation &caseInformation );

  void PrintVector( const std::vector<MyRealType> & v );
  void PrintVector( const std::vector<std::string> & v );

  void ComputeStatisticsWithMask();
  void ComputeStatisticsWithoutMask();
  void TransferStatisticsResultsToOutputStructuresWithMask();
  void TransferStatisticsResultsToOutputStructuresWithoutMask();


  void LoadInputDataWithMask( int totalNumberOfImages, int dimensionSPD );
  void LoadInputDataWithoutMask( int totalNumberOfImages, int dimensionSPD );

  void LoadMatrix( std::vector<std::vector<MyRealType> >& matrix, std::string fileName );

  void LoadConstraintMatrix();  
  void LoadExternalDesignMatrix();

  void AllocateStatisticsDataStructures();
  void DeAllocateStatisticsDataStructures();

  void InitializeStatisticsDataStructures();

  void InitializeGlobals();
  void CopyMembersToGlobals();
  void PrintGlobals();

  virtual void GenerateOutputInformation();

  // non-threaded version to generate data
  void GenerateData();
  
  SizeType        m_Size;
  SpacingType     m_Spacing;
  PointType       m_Origin;

  virtual void PrintSelf(std::ostream& os, Indent indent) const;

private:

  LongitudinalStatisticsFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  bool m_Verbose;
  bool m_DebugVerbose;
  bool m_CovaryWithGroup;
  bool m_CovaryWithGender;
  bool m_NoLogTransform;
  bool m_NoTimePointScaling;

  std::string m_PathPrefix;
  std::string m_DesignMatrix;

  std::string m_MaskImage;

  int DimSPD;

  std::vector< std::vector<MyRealType> > m_ConstraintMatrix;
  std::vector< std::vector<MyRealType> > m_ExternalDesignMatrix;

  std::string m_ConstraintMatrixFileName;
  int m_Seed;

  typename VectorImageType::Pointer covarianceImage;
  typename ScalarImageType::Pointer rawPValueImage;
  typename ScalarImageType::Pointer sPValueImage;
  typename ScalarImageType::Pointer SCOREValueImage;
  typename ScalarImageType::Pointer WALDValueImage;
  
  typename ScalarIntegerReaderType::Pointer maskReader;
  typename TensorReaderType::Pointer tensorReader;

  SCaseInformation caseInformation;

  int numberOfPointsInMask;

  int SEEDY[12];
  int num_subjects;  // number of subjects
  int NOmaxTIMEs;    // maximum number of timepoints
  int num_cov;       // number of covariates
  int dimX, dimY, dimZ; // image dimensions
  int NopointSur;  // number of points within the mask
  int TotalImg;    // total number of images
  int noRow;       // number of rows for the linear constraints
  int obsforonemax; // QUESTION: what is this variable?
  int totalobs;

  int Ncov;
  int Nsample;
  int NcovVar;

  enum {MALE, FEMALE};

  double maxYvalue;

  // here come Hongtu's data structures

  int*      allDimTime;
  int*      CCID;
  float**   cov_beta;
  float**   ExactTime;
  int*      indxMi;
  int*      indxMi2;
  float**   INRR;
  float**   INRRnew;
  double**  INRRnewD;
  int*      Irank;
  float**   LCOVL;
  float*    LLBB;
  int*      imageMask;
  float**   midRR;
  float**   RR;
  float*    rr00;
  float*    simulationBias;
  float*    simulationRMS;
  float*    simulationSD;
  float*    tempLB;
  double**  TXX;

  float**   x0x;

  double**   designXX;
  double*    betaGEE;

  float*    Epsilon;
  float**   ImageData;
  float*    Scorevalue;
  float*    SmaxEmp;
  float*    Spvalue;
  float*    Srawpvalue;
  float*    tempoScorevalue;
  float***  tensors;
  float***  UUmu;
  float***  UUsgMu;
  float*    Waldtest;
  double*   Yresponse;

  // TODO: where do these variables come from?

  double** BrainTheta;
  float*** corrMatrix;
  double*  residualli;
  double** sum1;
  double** varGEE;
  float*** varMatrix;

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkLongitudinalStatisticsFilter.txx"
#endif

#endif
