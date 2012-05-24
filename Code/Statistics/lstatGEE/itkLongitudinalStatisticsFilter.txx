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

#ifndef __itkLongitudinalStatisticsFilter_txx
#define __itkLongitudinalStatisticsFilter_txx

#include "itkLongitudinalStatisticsFilter.h"
#include <itkNumericTraits.h>
#include "itkProgressReporter.h"

#include "surallfileM10Y06_MN_JP.h"

#define SUPPRESS_DETAILED_DEBUG

namespace itk
{

/** Constructor */
template <class MyRealType, class MyIntegerType>
LongitudinalStatisticsFilter<MyRealType, MyIntegerType>
::LongitudinalStatisticsFilter()
{
  this->SetNumberOfRequiredInputs(1);
  m_Size.Fill(0);
  m_Spacing.Fill(1.0);
  m_Origin.Fill(0.0);

  m_Verbose = true;
  m_DebugVerbose = true;
  m_Seed = 17;
  
  DimSPD = 6;

  m_CovaryWithGroup = false;
  m_CovaryWithGender = false;

  m_PathPrefix = "None";
  m_MaskImage = "None";
  m_ConstraintMatrixFileName = "None";
  m_DesignMatrix = "None";
  m_NoLogTransform = false;
  m_NoTimePointScaling = false;

  m_ConstraintMatrix.clear();

  numberOfPointsInMask = 0;

  // variables for Hongtu's code

  maxYvalue = 1;

  // TODO: make sure these variables are all properly defined

  num_subjects = 0;
  NOmaxTIMEs = 0;
  NopointSur = 0;
  num_cov = 0;
  noRow = 0; 

  Ncov = 0;
  Nsample = 0;
  NcovVar = 0;

  dimX = 0;
  dimY = 0;
  dimZ = 0;

  obsforonemax = 0;
  totalobs = 0;

  // initialize the globals

  this->InitializeGlobals();

  // initialize Hongtu's data structure

// here come Hongtu's data structures

  allDimTime = NULL;
  CCID = NULL;
  cov_beta = NULL;
  ExactTime = NULL;
  indxMi = NULL;
  indxMi2 = NULL;
  INRR = NULL;
  INRRnew = NULL;
  INRRnewD = NULL;
  Irank = NULL;
  LCOVL = NULL;
  LLBB = NULL;
  imageMask = NULL;
  midRR = NULL;
  RR = NULL;
  rr00 = NULL;
  simulationBias = NULL;
  simulationRMS = NULL;
  simulationSD = NULL;
  tempLB = NULL;
  TXX = NULL;

  x0x = NULL;

  designXX = NULL;
  betaGEE = NULL;

  Epsilon = NULL;
  ImageData = NULL;
  Scorevalue = NULL;
  SmaxEmp = NULL;
  Spvalue = NULL;
  Srawpvalue = NULL;
  tempoScorevalue = NULL;
  tensors = NULL;
  UUmu = NULL;
  UUsgMu = NULL;
  Waldtest = NULL;
  Yresponse = NULL;

  BrainTheta = NULL;
  corrMatrix = NULL;
  residualli = NULL;
  sum1 = NULL;
  varGEE = NULL;
  varMatrix = NULL;
  
}

/** Destructor */
template <class MyRealType, class MyIntegerType>
LongitudinalStatisticsFilter<MyRealType, MyIntegerType>
::~LongitudinalStatisticsFilter()
{

  // deallocate the memory that was explicitly requested

}

template <class MyRealType, class MyIntegerType>
void 
LongitudinalStatisticsFilter<MyRealType, MyIntegerType>
::InitializeGlobals()
{
  // initializes the global variables
  
  globals::congrval = 0;
  globals::tausval = 0;
  globals::Ncov = 0;
  globals::NopointSur = 0;
  globals::NOtraG = 0;
  globals::Xdimen = 0;
  globals::Ydimen = 0;
  globals::Zdimen = 0;
  globals::Nsample = 0;
  globals::NcovVar = 0;
  globals::DimSPD = 0; 
  globals::NOmaxTIMEs = 0;
  globals::htNoM = 0;
  globals::htNoR = 0;
  globals::DimYY= 0;  
  globals::NcovDim = 0;
  globals::DimXX = 0;

  globals::allDimTime = NULL;
  globals::outputDir = NULL;
}

template <class MyRealType, class MyIntegerType>
void 
LongitudinalStatisticsFilter<MyRealType, MyIntegerType>
::CopyMembersToGlobals()
{
  // copys the content of the member variables to the global variables
  // needs to be called before statistics functions are called

  globals::congrval = 0; // not used in this code
  globals::tausval = 0;  // not used in this code
  
  globals::Ncov = this->Ncov;
  globals::NopointSur = this->NopointSur;

  globals::NOtraG = 0; // number of triangles, not needed for this code

  globals::Xdimen = this->dimX;
  globals::Ydimen = this->dimY;
  globals::Zdimen = this->dimZ;

  globals::Nsample = this->Nsample;

  globals::NcovVar = this->NcovVar;
  globals::DimSPD = this->DimSPD;
 
  globals::NOmaxTIMEs = this->NOmaxTIMEs;

  globals::htNoM = 0; // number of families, not used in this code
  globals::htNoR = 0; // number of constraints, not used in this code

  globals::NcovDim = 0; // not used in this code

  globals::DimXX = 0; // not used in this code
  globals::DimYY = 0; // not used in this code

  globals::allDimTime = this->allDimTime;
  globals::outputDir = NULL; // not used in this code
  
  globals::TotalImg = TotalImg;
}

template <class MyRealType, class MyIntegerType>
void 
LongitudinalStatisticsFilter<MyRealType, MyIntegerType>
::PrintGlobals()
{
  // copys the content of the member variables to the global variables
  // needs to be called before statistics functions are called

  std::cout << "globals::congrval   = " << globals::congrval << std::endl;
  std::cout << "globals::tausval    = " << globals::tausval << std::endl;
  
  std::cout << "globals::Ncov       = " << globals::Ncov << std::endl;
  std::cout << "globals::NopointSur = " << globals::NopointSur << std::endl;

  std::cout << "globals::NOtraG     = " << globals::NOtraG << std::endl;

  std::cout << "globals::Xdimen     = " << globals::Xdimen << std::endl;
  std::cout << "globals::Ydimen     = " << globals::Ydimen << std::endl;
  std::cout << "globals::Zdimen     = " << globals::Zdimen << std::endl;

  std::cout << "globals::Nsample    = " << globals::Nsample << std::endl;

  std::cout << "globals::NcovVar    = " << globals::NcovVar << std::endl;
  std::cout << "globals::DimSPD     = " << globals::DimSPD << std::endl;
 
  std::cout << "globals::NOmaxTIMEs = " << globals::NOmaxTIMEs << std::endl;

  std::cout << "globals::htNoM      = " << globals::htNoM << std::endl;
  std::cout << "globals::htNoR      = " << globals::htNoR << std::endl;

  std::cout << "globals::NcovDim    = " << globals::NcovDim << std::endl;

  std::cout << "globals::DimXX       = " << globals::DimXX << std::endl;
  std::cout << "globals::DimYY       = " << globals::DimYY << std::endl;

  std::cout << "globals::allDimTime  = " << globals::allDimTime << std::endl;
  //std::cout << "globals::outputDir   = " << globals::outputDir << std::endl;
  
  std::cout << "globals::TotalImg    = " << globals::TotalImg << std::endl;

}

template <class MyRealType, class MyIntegerType>
void 
LongitudinalStatisticsFilter<MyRealType, MyIntegerType>
::SetDimSPD( int v )
{
  DimSPD = v;
}

template <class MyRealType, class MyIntegerType>
int
LongitudinalStatisticsFilter<MyRealType, MyIntegerType>
::GetDimSPD()
{
  return DimSPD;
}

/** Set the Input caselist */
template <class MyRealType, class MyIntegerType>
void 
LongitudinalStatisticsFilter<MyRealType, MyIntegerType>
::SetInput( const std::string* psCaseList )
{
  
  // strings are not dataobjects, so need to decorate it to push it down
  // the pipeline 
  typename InputStringObjectType::Pointer stringObject = 
    InputStringObjectType::New();
  stringObject->Set( const_cast< std::string*  >( psCaseList ) );
  
  // Process object is not const-correct so the const_cast is required here
  this->ProcessObject::SetNthInput(0,  stringObject  );
}

template <class MyRealType, class MyIntegerType>
const typename LongitudinalStatisticsFilter< MyRealType, MyIntegerType >::InputStringObjectType *
LongitudinalStatisticsFilter<MyRealType, MyIntegerType>
::GetInput(void) 
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }
  return static_cast<const InputStringObjectType * >
    (this->ProcessObject::GetInput(0) );
}

template <class MyRealType, class MyIntegerType>
void
LongitudinalStatisticsFilter<MyRealType, MyIntegerType>
::SetSpacing(const double* spacing)
{
  SpacingType s(spacing);
  this->SetSpacing( s );
}


template <class MyRealType, class MyIntegerType>
void
LongitudinalStatisticsFilter<MyRealType, MyIntegerType>
::SetOrigin(const double* origin)
{
  PointType p(origin);
  SetOrigin(p);
}


template <class MyRealType, class MyIntegerType>
void
LongitudinalStatisticsFilter<MyRealType, MyIntegerType>
::GenerateOutputInformation()
{
  
  // parsing of case file and loading of the input data needs to
  // happen here, so we can set the output dimensions appropriately

  ParseCaseFile();
  LoadConstraintMatrix();
  LoadExternalDesignMatrix();

  int totalNumberOfImages = 0;
  for ( unsigned int iI=0; iI<caseInformation.vSubjectId.size(); iI++ )
    {
    totalNumberOfImages += caseInformation.vvAge[iI].size();
    }

  if ( m_MaskImage.compare("None")!=0 )
    {
    if ( m_Verbose ) std::cout << "Loading input data with mask image." << std::endl;
    LoadInputDataWithMask( totalNumberOfImages, this->DimSPD );
    }
  else
    {
    if ( m_Verbose) std::cout << "Loading input data *without* mask image." << std::endl;
    LoadInputDataWithoutMask( totalNumberOfImages, this->DimSPD );
    }

  // requires that all input information is already read before
  // the statistics variables can be appropriately initialized 
  // needed to make sure we know what the size of the output should be
  PopulateStatisticsVariablesFromCaseInformationAndImageDimensions();

  // Get the input and output pointers 
  // Get from decorator
  VectorRealOutputImageType     *outputImage    = this->GetOutput();

  // Set output image params and Allocate image

  typename ScalarImageType::IndexType start = {{0,0,0}};
  typename ScalarImageType::RegionType region;


  region.SetIndex ( start );
  region.SetSize( m_Size );  // TODO: make sure this is set
			     // appropriately; maybe clone input info
			     // from one of the input files

  outputImage->SetRegions( region );

  // set spacing and origin

  outputImage->SetSpacing( m_Spacing );
  outputImage->SetOrigin(  m_Origin );
									
  // output is BETA image: link the information
  outputImage->SetNumberOfComponentsPerPixel( Ncov + 2*DimSPD );  
								  
  outputImage->Allocate();

  // allocate memory for all the other data structures

  // covariance

  // this is a symmetric matrix

  if ( Ncov>0 )
    {
    covarianceImage = VectorImageType::New();
    covarianceImage->SetOrigin( m_Origin );
    covarianceImage->SetSpacing( m_Spacing );
    covarianceImage->SetRegions( region );
    covarianceImage->SetNumberOfComponentsPerPixel( (Ncov+1)*Ncov/2 ); 
    covarianceImage->Allocate();

    std::cout << "allocating covariance image with " << (Ncov+1)*Ncov/2 << " components" << std::endl;
    }
  else
    {
    std::cout << "No covariates, covariance image is not allocated." << std::endl;
    }
  
  // rawpvalue

  rawPValueImage = ScalarImageType::New();
  rawPValueImage->SetOrigin( m_Origin );
  rawPValueImage->SetSpacing( m_Spacing );
  rawPValueImage->SetRegions( region );
  rawPValueImage->Allocate();

  // s pvalue

  sPValueImage = ScalarImageType::New();
  sPValueImage->SetOrigin( m_Origin );
  sPValueImage->SetSpacing( m_Spacing );
  sPValueImage->SetRegions( region );
  sPValueImage->Allocate();

  // SCORE value

  SCOREValueImage = ScalarImageType::New();
  SCOREValueImage->SetOrigin( m_Origin );
  SCOREValueImage->SetSpacing( m_Spacing );
  SCOREValueImage->SetRegions( region );
  SCOREValueImage->Allocate();

  // WALD value

  WALDValueImage = ScalarImageType::New();
  WALDValueImage->SetOrigin( m_Origin );
  WALDValueImage->SetSpacing( m_Spacing );
  WALDValueImage->SetRegions( region );
  WALDValueImage->Allocate();

}

template< class MyRealType, class MyIntegerType >
typename LongitudinalStatisticsFilter< MyRealType, MyIntegerType>::VectorRealOutputImageType*
LongitudinalStatisticsFilter< MyRealType, MyIntegerType >
::GetCovarianceOutputVolume()
{
  return covarianceImage;
}

template< class MyRealType, class MyIntegerType >
typename LongitudinalStatisticsFilter< MyRealType, MyIntegerType>::ScalarRealOutputImageType*
LongitudinalStatisticsFilter< MyRealType, MyIntegerType >
::GetRawPValueOutputVolume()
{
  return rawPValueImage;
}

template< class MyRealType, class MyIntegerType >
typename LongitudinalStatisticsFilter< MyRealType, MyIntegerType>::ScalarRealOutputImageType*
LongitudinalStatisticsFilter< MyRealType, MyIntegerType >
::GetRSPValueOutputVolume()
{
  return sPValueImage;
}

template< class MyRealType, class MyIntegerType >
typename LongitudinalStatisticsFilter< MyRealType, MyIntegerType>::ScalarRealOutputImageType*
LongitudinalStatisticsFilter< MyRealType, MyIntegerType >
::GetSCOREValueOutputVolume()
{
  return SCOREValueImage;
}

template< class MyRealType, class MyIntegerType >
typename LongitudinalStatisticsFilter< MyRealType, MyIntegerType>::ScalarRealOutputImageType*
LongitudinalStatisticsFilter< MyRealType, MyIntegerType >
::GetWALDValueOutputVolume()
{
  return WALDValueImage;
}


// data management

template< class MyRealType, class MyIntegerType >
void
LongitudinalStatisticsFilter< MyRealType, MyIntegerType >
::PrintVector(  const std::vector<MyRealType> & v )
{
  for ( unsigned int iI=0; iI<v.size(); iI++ )
    {
    std::cout << v[iI] << " ";
    }
  std::cout << std::endl;
}

template< class MyRealType, class MyIntegerType >
void
LongitudinalStatisticsFilter< MyRealType, MyIntegerType >
::PrintVector(  const std::vector<std::string> & v )
{
  for ( unsigned int iI=0; iI<v.size(); iI++ )
    {
    std::cout << v[iI] << std::endl;
    }
}

template< class MyRealType, class MyIntegerType >
void
LongitudinalStatisticsFilter< MyRealType, MyIntegerType >
::DisplayCaseInformation() 
{
  unsigned int iNumberOfSubjects = caseInformation.vGroup.size();

  std::cout << "Stored case information:" << std::endl;
  std::cout << "----------------------------------------------------------------" << std::endl << std::endl;

  for ( unsigned int iI=0; iI<iNumberOfSubjects; iI++ ) 
    {
    std::cout << "Subject id    = " << caseInformation.vSubjectId[iI] << ":" << std::endl;
    std::cout << "   Group      = " << caseInformation.vGroup[iI] << std::endl;
    std::cout << "   Gender     = " << caseInformation.vGender[iI] << " ";
    switch ( caseInformation.vGender[iI] )
      {
      case MALE:
	std::cout << "(MALE)";
	break;
      case FEMALE:
	std::cout << "(FEMALE)";
	break;
      default:
	std::cout << "(UNKNOWN)";
      }
    std::cout << std::endl;

    std::cout << "   Ages       = "; PrintVector( caseInformation.vvAge[iI] );
    std::cout << "   Scales     = "; PrintVector( caseInformation.vvScaleFactor[iI] );
    
    for ( unsigned int iJ=0; iJ<caseInformation.vvAdditionalCovariates[iI].size(); iJ++ )
      {
      std::cout << "   Covariates (tp=" << caseInformation.vvAge[iI][iJ] << ")= "; PrintVector( caseInformation.vvAdditionalCovariates[iI][iJ] );
      }

    std::cout << "   Associated filenames:" << std::endl;
    PrintVector( caseInformation.vvFilenames[iI] );
    std::cout << std::endl;

    }
}

template < class MyRealType, class MyIntegerType >
bool sort_pred(const std::pair< MyRealType, MyIntegerType >& left, const std::pair<MyRealType, MyIntegerType>& right)
{
  return left.first < right.first;
}


template < class MyRealType, class MyIntegerType >
void
LongitudinalStatisticsFilter< MyRealType, MyIntegerType >
::TimeSortCaseInformation( SCaseInformation& caseInformation ) 
{
  // takes the caseInformation structure and sorts it so that all
  // cases are ordered with increasing age (this is useful to pass
  // the information later into the longitudinal atlas builder

  // applies to vvScaleFactor, vvAge, vvFilenames, and
  // vvAdditionalCovariates

  // first make a copy

  unsigned int iNrOfResorts = 0;

  SCaseInformation copyOfCaseInformation = caseInformation;

  // now go thorugh all the subject

  for ( unsigned int iI=0; iI<caseInformation.vSubjectId.size(); iI++ )
    {

    // Find out how the age is sorted and resort everything according
    // to it. To do so we need to create pairs with a running index
    // which are being sorted with age as the key

    std::vector< myRealIntPair > vPairAgeAndIndex;
    for ( unsigned int iJ=0; iJ<caseInformation.vvAge[iI].size(); iJ++ )
      {
      vPairAgeAndIndex.push_back( myRealIntPair( caseInformation.vvAge[iI][iJ], iJ ) );
      }

    // now sort it

    std::sort(vPairAgeAndIndex.begin(), vPairAgeAndIndex.end(), sort_pred< MyRealType, MyIntegerType> );    

    // now we can resort

    for ( unsigned int iJ=0; iJ<caseInformation.vvAge[iI].size(); iJ++ )
      {
      unsigned int currentIndex = vPairAgeAndIndex[iJ].second;
      
      if ( iJ!=currentIndex ) iNrOfResorts++;

      caseInformation.vvScaleFactor[iI][iJ] = copyOfCaseInformation.vvScaleFactor[iI][currentIndex];
      caseInformation.vvAge[iI][iJ] = copyOfCaseInformation.vvAge[iI][currentIndex];
      caseInformation.vvFilenames[iI][iJ] = copyOfCaseInformation.vvFilenames[iI][currentIndex];
      caseInformation.vvAdditionalCovariates[iI][iJ] = copyOfCaseInformation.vvAdditionalCovariates[iI][currentIndex];
      }

    }

  if ( m_Verbose )
    {
    if ( iNrOfResorts>0 )
      {
      std::cout << "WARNING: Resorted " << iNrOfResorts << " datasets. Make sure design and constraint matrix still match up." << std::endl;
      }
    else
      {
      std::cout << "INFO: No resorting of the data was necessary." << std::endl;
      }
    }

}

template< class MyRealType, class MyIntegerType >
void
LongitudinalStatisticsFilter< MyRealType, MyIntegerType >
::ParseCaseFile() 
{

  // clear everything, just in case

  caseInformation.subjectIdToIndexMap.clear();
  caseInformation.vGroup.clear();
  caseInformation.vGender.clear();
  caseInformation.vSubjectId.clear();
  caseInformation.vvScaleFactor.clear();
  caseInformation.vvAge.clear();
  caseInformation.vvAdditionalCovariates.clear();
  caseInformation.vvFilenames.clear();
  
  // the case file is of the format

  // group gender volumeScaleFactor filename age subjectId cov0 cov1
  // # indicates a comment

  // also do check that the same subject id always has the same gender
  // and group id

  std::string sCaseFile = *(this->GetInput()->Get());

  if ( m_Verbose )
    std::cout << "Reading: " << sCaseFile << std::endl;

  std::ifstream in( sCaseFile.c_str() );

  unsigned int iCurrentIndex = 0;

  if ( in.is_open() )
    {

    std::string line;

     while ( getline ( in, line ) ) 
      {
      std::string::size_type iI = line.find_first_not_of ( " \t\n\v" );
      
      if ( iI != std::string::npos && line[iI] != '#'  )
	{
	// found something that needs to be processed, read it
	std::istringstream ins;

	unsigned int currentSubjectId;
	unsigned int currentGroup;
	unsigned int currentGender;

	std::string currentFileName;

	MyRealType currentScaleFactor;
	MyRealType currentAge;
	
	MyRealType currentAdditionalCovariate;

	ins.str( line );
	
	ins >> currentGroup;
	ins >> currentGender;
	ins >> currentScaleFactor;
	ins >> currentFileName;
	ins >> currentAge;
	ins >> currentSubjectId;

	// see if the subject already has an index, if not create a
	// new one

	unsigned int iSubjectIndex;

	std::map<unsigned int, unsigned int>::iterator iter = caseInformation.subjectIdToIndexMap.find( currentSubjectId );
	if ( iter != caseInformation.subjectIdToIndexMap.end() ) 
	  {
	  // end indicates that the element is not in the map
	  iSubjectIndex = iter->second;

	  if ( caseInformation.vGroup[iSubjectIndex] != currentGroup )
	    {
	    std::cout << "Group mismatch for line:"<<std::endl;
	    std::cout << "   " << line << std::endl;
	    }
	  
	  if ( caseInformation.vGender[iSubjectIndex] != currentGender )
	    {
	    std::cout << "Gender mismatch for line:"<<std::endl;
	    std::cout << "   " << line << std::endl;
	    }

	  caseInformation.vvFilenames[iSubjectIndex].push_back( currentFileName );
	  caseInformation.vvScaleFactor[iSubjectIndex].push_back( currentScaleFactor );
	  caseInformation.vvAge[iSubjectIndex].push_back( currentAge );

	  }
	else
	  {
	  // need to create new element
	  caseInformation.subjectIdToIndexMap[ currentSubjectId ] = iCurrentIndex;
	  iSubjectIndex = iCurrentIndex;
	  iCurrentIndex++;

	  caseInformation.vGroup.push_back( currentGroup );
	  caseInformation.vGender.push_back( currentGender );
	  caseInformation.vSubjectId.push_back( currentSubjectId );

	  std::vector<std::string> V;
	  V.push_back( currentFileName );
	  caseInformation.vvFilenames.push_back( V );

	  std::vector<MyRealType> W;
	  W.push_back( currentScaleFactor );
	  caseInformation.vvScaleFactor.push_back( W );

	  W.clear();
	  W.push_back( currentAge );
	  caseInformation.vvAge.push_back( W );
	  
	  std::vector< std::vector<MyRealType> > VV;
	  // initialize empty then add additional elements
	  caseInformation.vvAdditionalCovariates.push_back( VV );

	  }
	
	// now loop and see if we can find additional covariates

	std::vector<MyRealType> currentCovariates;
	while ( !ins.eof() )
	  {
	  ins >> currentAdditionalCovariate;
	  if ( !ins.fail() )
	    currentCovariates.push_back( currentAdditionalCovariate );
	  }
	//std::cout << "current covariates = "; PrintVector( currentCovariates );

	caseInformation.vvAdditionalCovariates[iSubjectIndex].push_back( currentCovariates );

	}
      }
    }
  else
    {
    std::cout << "ERROR: Could not open '" << sCaseFile << "' as input file. ABORT." << std::endl;
    exit( EXIT_FAILURE );
    }

  if ( !m_NoTimePointScaling )
    {
    if ( m_Verbose ) std::cout << "Scaling the time points." << std::endl;
    // find the largest time-point

    MyRealType largestTimeValue = caseInformation.vvAge[0][0];

    for ( unsigned iS=0; iS<caseInformation.vvAge.size(); iS++ )
      {
      for ( unsigned iT=0; iT<caseInformation.vvAge[iS].size(); iT++ )
	{
	if ( caseInformation.vvAge[iS][iT]>largestTimeValue )
	  {
	  largestTimeValue = caseInformation.vvAge[iS][iT];
	  }
	}
      }

    if ( m_Verbose ) std::cout << "Largest time value found = " << largestTimeValue << std::endl;
    
    // now do the scaling

     for ( unsigned iS=0; iS<caseInformation.vvAge.size(); iS++ )
      {
      for ( unsigned iT=0; iT<caseInformation.vvAge[iS].size(); iT++ )
	{
	caseInformation.vvAge[iS][iT]/=largestTimeValue;
	}
      }

    }

  // now sort them

  TimeSortCaseInformation( caseInformation );

  // output some statistical information

  DisplayCaseInformation();

}

template< class MyRealType, class MyIntegerType >
void
LongitudinalStatisticsFilter< MyRealType, MyIntegerType >
::PopulateStatisticsVariablesFromCaseInformationAndImageDimensions()
{
  // initialize the values for Hongtu's code

  num_subjects = caseInformation.vSubjectId.size();   // number of subjects
  
  // find the maximum number of timepoints
  NOmaxTIMEs = 0;
  for ( unsigned int iI=0; iI<caseInformation.vSubjectId.size(); iI++ )
    {
    // TODO: check if cast is proper
    NOmaxTIMEs = std::max( NOmaxTIMEs, (int)caseInformation.vvAge[iI].size() );
    }
  
  // get the number of covariates
  
  if ( m_DesignMatrix.compare("None") !=0 )
    {
    if ( !m_ExternalDesignMatrix.empty() ) 
      {
      num_cov = m_ExternalDesignMatrix[0].size();
      }
    else
      {
      num_cov = 0;
      }
    }
  else  // not specified get the info from the case file
    {
    
    // assume all the cases have the same number of covariates (TODO:
    // write check!)
    num_cov = caseInformation.vvAdditionalCovariates[0][0].size(); 
    if ( m_CovaryWithGroup ) num_cov++;
    if ( m_CovaryWithGender ) num_cov++;

    }
  
  // compute total number of images
  TotalImg = 0;
  
  for ( unsigned int iI=0; iI<caseInformation.vSubjectId.size(); iI++ )
    {
    TotalImg += caseInformation.vvFilenames[iI].size();
    }


  int q=1;//variance
  
  // Question/fixme: DimSPD is set externally, should be 6 for the tensor (correct?)

  Ncov=DimSPD*num_cov;
  NcovVar=DimSPD*q;
  Nsample=num_subjects;

  dimX = m_Size[0];
  dimY = m_Size[1];
  dimZ = m_Size[2];

  NopointSur = numberOfPointsInMask; // could simply directly save in
				     // this variable; want to keep
				     // codes separate though

  
  totalobs=TotalImg*DimSPD;

  if ( m_Verbose )
    std::cout << "totalobs = " << totalobs << std::endl;

  int mimax = 0;
  // find the case which has the most timepoints
  for ( unsigned int iI=0; iI<caseInformation.vSubjectId.size(); iI++ )
    {
    if ( (int)caseInformation.vvAge[iI].size()>mimax )
      mimax = (int)caseInformation.vvAge[iI].size();
    }

  obsforonemax=mimax*DimSPD;
  
  // the constraint matrix data structure first
  // contains the actual constraint matrix, followed by two 
  // vectors rr00, CCID, which are not clear to me

  noRow = 0;

  if ( m_Verbose )
    std::cout << "Number of columns of constraint matrix = " << m_ConstraintMatrix[0].size() << std::endl;

  if ( !m_ConstraintMatrix.empty() )
    {
    unsigned int nrOfColumnsForMatrix = m_ConstraintMatrix[0].size();
    for ( unsigned int iI=0; iI<m_ConstraintMatrix.size(); iI++ )
      {
      if ( m_ConstraintMatrix[iI].size() == nrOfColumnsForMatrix ) noRow++;
      }
    }

  if ( m_Verbose )
    std::cout << "noRow = " << noRow << std::endl;

}

template< class MyRealType, class MyIntegerType >
void
LongitudinalStatisticsFilter< MyRealType, MyIntegerType >
::LoadMatrix( std::vector< std::vector<MyRealType> > &matrix, std::string fileName )
{
  matrix.clear();

  if ( fileName.compare("None")!=0 )
    {
    if ( m_Verbose )
      std::cout << "Reading matrix from: " << fileName << std::endl;

    std::ifstream in( fileName.c_str() );

    if ( in.is_open() )
      {
       std::string line;

       while ( getline ( in, line ) ) 
	 {
	 std::string::size_type iI = line.find_first_not_of ( " \t\n\v" );
      
	 if ( iI != std::string::npos && line[iI] != '#'  )
	   {
	   // found something that needs to be processed, read and
	   // save it

	   std::vector<MyRealType> v;

	   std::istringstream ins;
	   MyRealType currentRealValue;
	   
	   ins.str( line );
	
	   //std::cout << "Reading current line: " << line << std::endl;

	   while ( !ins.eof() )
	     {
	     ins >> currentRealValue;
	     if ( !ins.fail() )
	       {
	       //std::cout << "Pushing back " << currentRealValue << std::endl;
	       v.push_back( currentRealValue );
	       }
	     }

	   matrix.push_back( v );

	   }
	 }// end while
      } // end if
    }
  else
    {
    if ( m_Verbose )
      std::cout << "No file for matrix defined." << std::endl;
    }

  // display the constraint matrix

  if ( m_Verbose )
    {
    std::cout << "Matrix = " << std::endl;
    for ( unsigned int iI=0; iI<matrix.size(); iI++ )
      {
      for ( unsigned int iJ=0; iJ<matrix[iI].size(); iJ++ )
	{
	std::cout << matrix[iI][iJ] << " ";
	}
      std::cout << std::endl;
      }
    }

  //if ( m_Verbose )
  //  std::cout << "Number of rows of the matrix = " << noRow << std::endl;
}

template< class MyRealType, class MyIntegerType >
void
LongitudinalStatisticsFilter< MyRealType, MyIntegerType >
::LoadExternalDesignMatrix()
{
  if ( m_Verbose )
    std::cout << "Reading the design matrix." << std::endl;
  LoadMatrix( m_ExternalDesignMatrix, m_DesignMatrix );
}

template< class MyRealType, class MyIntegerType >
void
LongitudinalStatisticsFilter< MyRealType, MyIntegerType >
::LoadConstraintMatrix()
{
  if ( m_Verbose )
    std::cout << "Reading the constraint matrix." << std::endl;
  LoadMatrix( m_ConstraintMatrix, m_ConstraintMatrixFileName );
}

template< class MyRealType, class MyIntegerType >
void
LongitudinalStatisticsFilter< MyRealType, MyIntegerType >
::LoadInputDataWithMask( int totalNumberOfImages, int dimensionSPD )
{

  // first read in mask image

  maskReader = ScalarIntegerReaderType::New();

  maskReader->SetFileName( m_MaskImage.c_str() );
    
  if ( m_Verbose )
    std::cout << "Reading mask image = " << m_MaskImage << " ...";

  maskReader->Update();

  if ( m_Verbose )
    std::cout << "done." << std::endl;

  // get the dimensions for hongtu's code 

  SizeType size = maskReader->GetOutput()->GetLargestPossibleRegion().GetSize();
  

  if ( m_Verbose )
    std::cout << "Image dimensions = " << size[0] << " " << size[1] << " " << size[2] << std::endl;

  // now check how many points are part of the mask, because we only
  // need to compute here

  ScalarIntegerIteratorType maskIterator( maskReader->GetOutput(), maskReader->GetOutput()->GetLargestPossibleRegion() );

  numberOfPointsInMask = 0;
  maskIterator.GoToBegin();
  while ( !maskIterator.IsAtEnd() )
    {
    MyIntegerType currentMaskValue = maskIterator.Get();
    if ( currentMaskValue>0 )
      numberOfPointsInMask++;
    ++maskIterator;
    }


  if ( m_Verbose )
    std::cout << "Allocating ImageData" << std::endl;
  ImageData = GEEUtilities::matrix_ht(1, numberOfPointsInMask, 1, totalNumberOfImages*dimensionSPD );


  // here we just read the tensor images and stick them immediately
  // into Hongtu's data structures
  
  tensorReader = TensorReaderType::New();

  unsigned int currentImageIndex = 0;

  for ( unsigned int iI=0; iI<caseInformation.vSubjectId.size(); iI++ )
    {
    // go over all the timepoints
    for ( unsigned int iT=0; iT<caseInformation.vvFilenames[iI].size(); iT++ )
      {
      if ( m_PathPrefix.compare("None")==0 )
	tensorReader->SetFileName( caseInformation.vvFilenames[iI][iT] );
      else
	tensorReader->SetFileName( m_PathPrefix + "/" + caseInformation.vvFilenames[iI][iT] );

      if ( m_Verbose )
	std::cout << "Reading in " << caseInformation.vvFilenames[iI][iT] << "...";

      tensorReader->Update();

      if ( m_Verbose ) std::cout << " done." << std::endl;

      // now stick the information into Hongtu's data structure

      ScalarIntegerIteratorType maskIterator( maskReader->GetOutput(), maskReader->GetOutput()->GetLargestPossibleRegion() );
      TensorIteratorType tensorIterator( tensorReader->GetOutput(), tensorReader->GetOutput()->GetLargestPossibleRegion() );

      maskIterator.GoToBegin();
      tensorIterator.GoToBegin();

      unsigned long currentIndex = 0;

      while ( !( maskIterator.IsAtEnd() || tensorIterator.IsAtEnd() ) )
	{

	TensorType currentTensor = tensorIterator.Get();
	MyIntegerType currentMaskValue = maskIterator.Get();

	// QUESTION: why is there a +1 for the index? (Taken from
	// Hongtu's code)

	if ( currentMaskValue>0 ) // inside the mask
	  {
	  //std::cout << "cI = " << currentIndex << "Tensor " << currentTensor << std::endl;

	  if ( m_NoLogTransform )
	    { // don't take the matrix logarithm
	    for ( unsigned int tensorComponentIndex=0; tensorComponentIndex<6; tensorComponentIndex++ )
	      {
	      ImageData[currentIndex+1][currentImageIndex*dimensionSPD+tensorComponentIndex+1] = currentTensor[ tensorComponentIndex ];
	      }
	    
	    }
	  else
	    {
	    // take the matrix logarithm
	    vnl_matrix<MyRealType> currentTensorMatrix(3,3,0);
	    currentTensorMatrix(0,0) = currentTensor[0];
	    currentTensorMatrix(0,1) = currentTensor[1];
	    currentTensorMatrix(0,2) = currentTensor[2];
	    currentTensorMatrix(1,0) = currentTensor[1];
	    currentTensorMatrix(1,1) = currentTensor[3];
	    currentTensorMatrix(1,2) = currentTensor[4];
	    currentTensorMatrix(2,0) = currentTensor[2];
	    currentTensorMatrix(2,1) = currentTensor[4];
	    currentTensorMatrix(2,2) = currentTensor[5];
	    
	    vnl_symmetric_eigensystem<MyRealType> eig( currentTensorMatrix );
	    vnl_diag_matrix<MyRealType> logEVs(3,0);
	    for ( unsigned int iE=0; iE<3; iE++ )
	      {
	      if ( eig.get_eigenvalue(iE)<vnl_math::float_eps )
		{
		logEVs(iE) = log( vnl_math::float_eps );
		}
	      else
		{
		logEVs(iE) = log( eig.get_eigenvalue(iE) );
		}
	      }
	    vnl_matrix<MyRealType> logM(3,3);
	    logM = eig.V*logEVs*eig.V.transpose();
	    vnl_vector<MyRealType> logTensorElements(6,0);
	    logTensorElements(0) = logM(0,0);
	    logTensorElements(1) = logM(0,1);
	    logTensorElements(2) = logM(0,2);
	    logTensorElements(3) = logM(1,1);
	    logTensorElements(4) = logM(1,2);
	    logTensorElements(5) = logM(2,2);
	    
	    for ( unsigned int tensorComponentIndex=0; tensorComponentIndex<6; tensorComponentIndex++ )
	      {
	      ImageData[currentIndex+1][currentImageIndex*dimensionSPD+tensorComponentIndex+1] = logTensorElements[ tensorComponentIndex ];
	      }
	    
	    }
	  
	  currentIndex++;
	  }

	++maskIterator;
	++tensorIterator;


	}
    
      currentImageIndex++;
    
      }
  
    }

  // set all the dimension infomration based on the tensor file

  this->SetSpacing( tensorReader->GetOutput()->GetSpacing() );
  m_Size = tensorReader->GetOutput()->GetLargestPossibleRegion().GetSize();
  m_Origin = tensorReader->GetOutput()->GetOrigin();

}

template< class MyRealType, class MyIntegerType >
void
LongitudinalStatisticsFilter< MyRealType, MyIntegerType >
::LoadInputDataWithoutMask( int totalNumberOfImages, int dimensionSPD )
{

  if ( m_Verbose )
    std::cout << "No mask image specified; computing for all points in the volume." << std::endl;

  // here we just read the tensor images and stick them immediately
  // into Hongtu's data structures
  
  typedef itk::ImageRegionConstIterator< TensorImageType > TensorIteratorType;

  tensorReader = TensorReaderType::New();

  unsigned int currentImageIndex = 0;

  for ( unsigned int iI=0; iI<caseInformation.vSubjectId.size(); iI++ )
    {
    // go over all the timepoints
    for ( unsigned int iT=0; iT<caseInformation.vvFilenames[iI].size(); iT++ )
      {
      if ( m_PathPrefix.compare("None")==0 )
	tensorReader->SetFileName( caseInformation.vvFilenames[iI][iT] );
      else
	tensorReader->SetFileName( m_PathPrefix + "/" + caseInformation.vvFilenames[iI][iT] );

      if ( m_Verbose )
	std::cout << "Reading in " << caseInformation.vvFilenames[iI][iT] << "...";

      tensorReader->Update();

      if ( currentImageIndex==0 ) 
	{
        // first read image, so look at the size and compute the
        // number of elements

	SizeType size = tensorReader->GetOutput()->GetLargestPossibleRegion().GetSize();

	numberOfPointsInMask = size[0]*size[1]*size[2]; // no mask,
							// use full image
	  
	// image data needs to be allocated here, so we do not need to keep
	// everything in memory twice

	if ( m_Verbose )
	  std::cout << "Allocating ImageData" << std::endl;
	ImageData = GEEUtilities::matrix_ht(1, numberOfPointsInMask, 1, totalNumberOfImages*dimensionSPD );

	}

      if ( m_Verbose ) std::cout << " done." << std::endl;

      // now stick the information into Hongtu's data structure

      TensorIteratorType tensorIterator( tensorReader->GetOutput(), tensorReader->GetOutput()->GetLargestPossibleRegion() );

      tensorIterator.GoToBegin();

      unsigned long currentIndex = 0;

      while ( !( tensorIterator.IsAtEnd() ) )
	{

	TensorType currentTensor = tensorIterator.Get();

	// need to compute the matrix logarithm if desired

	if ( m_NoLogTransform )
	  { // don't take the matrix logarithm
	  for ( unsigned int tensorComponentIndex=0; tensorComponentIndex<6; tensorComponentIndex++ )
	    {
	    ImageData[currentIndex+1][currentImageIndex*dimensionSPD+tensorComponentIndex+1] = currentTensor[ tensorComponentIndex ];
	    }
	  
	  }
	else
	  {
	  // take the matrix logarithm
	  vnl_matrix<MyRealType> currentTensorMatrix(3,3,0);
	  currentTensorMatrix(0,0) = currentTensor[0];
	  currentTensorMatrix(0,1) = currentTensor[1];
	  currentTensorMatrix(0,2) = currentTensor[2];
	  currentTensorMatrix(1,0) = currentTensor[1];
	  currentTensorMatrix(1,1) = currentTensor[3];
	  currentTensorMatrix(1,2) = currentTensor[4];
	  currentTensorMatrix(2,0) = currentTensor[2];
	  currentTensorMatrix(2,1) = currentTensor[4];
	  currentTensorMatrix(2,2) = currentTensor[5];

	  vnl_symmetric_eigensystem<MyRealType> eig( currentTensorMatrix );
	  vnl_diag_matrix<MyRealType> logEVs(3,0);
	  for ( unsigned int iE=0; iE<3; iE++ )
	    {
	    if ( eig.get_eigenvalue(iE)<vnl_math::float_eps )
	      {
	      logEVs(iE) = log( vnl_math::float_eps );
	      }
	    else
	      {
	      logEVs(iE) = log( eig.get_eigenvalue(iE) );
	      }
	    }
	  vnl_matrix<MyRealType> logM(3,3);
	  logM = eig.V*logEVs*eig.V.transpose();
	  vnl_vector<MyRealType> logTensorElements(6,0);
	  logTensorElements(0) = logM(0,0);
	  logTensorElements(1) = logM(0,1);
	  logTensorElements(2) = logM(0,2);
	  logTensorElements(3) = logM(1,1);
	  logTensorElements(4) = logM(1,2);
	  logTensorElements(5) = logM(2,2);

	  for ( unsigned int tensorComponentIndex=0; tensorComponentIndex<6; tensorComponentIndex++ )
	    {
	    ImageData[currentIndex+1][currentImageIndex*dimensionSPD+tensorComponentIndex+1] = logTensorElements[ tensorComponentIndex ];
	    }

	  }

	currentIndex++;

	++tensorIterator;

	}
    
      currentImageIndex++;
    
      }
  
    }

  // set all the dimension infomration based on the tensor file

  this->SetSpacing( tensorReader->GetOutput()->GetSpacing() );
  m_Size = tensorReader->GetOutput()->GetLargestPossibleRegion().GetSize();
  m_Origin = tensorReader->GetOutput()->GetOrigin();
  
}


template< class MyRealType, class MyIntegerType >
void
LongitudinalStatisticsFilter< MyRealType, MyIntegerType >
::AllocateStatisticsDataStructures()
{
  allDimTime=GEEUtilities::ivector_ht(1, num_subjects); /* reading the number of time points insides  HT */
  CCID=GEEUtilities::ivector_ht(0, DimSPD*num_cov); // define column for nonsingular R_1
  cov_beta=GEEUtilities::matrix_ht(0, num_cov*DimSPD-1, 0, num_cov*DimSPD-1);
  ExactTime=GEEUtilities::matrix_ht(1, num_subjects, 1, NOmaxTIMEs); 
  indxMi=GEEUtilities::ivector_ht(0, num_subjects); /* reading the number of time points insides  HT */
  indxMi2=GEEUtilities::ivector_ht(0, num_subjects); /* reading the number of time points insides  HT */
  INRR=GEEUtilities::matrix_ht(0, DimSPD*num_cov, 0, DimSPD*num_cov);
  INRRnew=GEEUtilities::matrix_ht(0, DimSPD*num_cov, 0, DimSPD*num_cov);
  INRRnewD=GEEUtilities::dmatrix_ht(1, DimSPD*num_cov, 1, DimSPD*num_cov);
  Irank=GEEUtilities::ivector_ht(0, DimSPD*num_cov);
  LCOVL=GEEUtilities::matrix_ht(0, DimSPD*num_cov-1, 0, DimSPD*num_cov-1);   // linear constraints covariance matrix
  LLBB=GEEUtilities::vector_ht(0, DimSPD*num_cov-1);   // linear constraints
  imageMask=GEEUtilities::ivector_ht(1, dimX*dimY*dimZ);
  midRR=GEEUtilities::matrix_ht(0, DimSPD*num_cov, 0, DimSPD*num_cov);
  RR=GEEUtilities::matrix_ht(0, DimSPD*num_cov-1, 0, DimSPD*num_cov-1); // linear constraints: Wald test
  rr00=GEEUtilities::vector_ht(0, DimSPD*num_cov);
  simulationBias=GEEUtilities::vector_ht(0, DimSPD*num_cov-1);
  simulationRMS=GEEUtilities::vector_ht(0, DimSPD*num_cov-1);
  simulationSD=GEEUtilities::vector_ht(0, DimSPD*num_cov-1);
  tempLB=GEEUtilities::vector_ht(0, DimSPD*num_cov);
  TXX=GEEUtilities::dmatrix_ht(1, DimSPD*num_cov, 1, DimSPD*num_cov);

  x0x= GEEUtilities::matrix_ht(0, TotalImg-1, 0, num_cov-1);

  designXX = GEEUtilities::dmatrix_ht(1, TotalImg*DimSPD, 1, Ncov);
  betaGEE= GEEUtilities::dvector_ht(1, DimSPD*num_cov+DimSPD*2);

  Epsilon = GEEUtilities::vector_ht(0, num_subjects);
  //ImageData = GEEUtilities::matrix(1, NopointSur, 1, TotalImg*DimSPD);
  ////allocated in load function!
  Scorevalue = GEEUtilities::vector_ht(1, NopointSur);
  SmaxEmp = GEEUtilities::vector_ht(1, Nresample);
  Spvalue = GEEUtilities::vector_ht(1, NopointSur);
  Srawpvalue = GEEUtilities::vector_ht(1, NopointSur);
  tempoScorevalue = GEEUtilities::vector_ht(0, NopointSur);
  tensors = GEEUtilities::fmatrix3_ht(0, DimSPD*dimZ-1, 0, dimX-1, 0, dimY-1);
  UUmu = GEEUtilities::fmatrix3_ht(0, NopointSur, 0, num_subjects, 0, noRow+1);
  UUsgMu = GEEUtilities::fmatrix3_ht(0, NopointSur, 0, noRow+1, 0, noRow+1);
  Waldtest = GEEUtilities::vector_ht(1,  NopointSur);
  Yresponse = GEEUtilities::dvector_ht(1, TotalImg*DimSPD);

  // TODO: where do these variables come from?

  BrainTheta = GEEUtilities::dmatrix_ht(1, NopointSur, 1, Ncov+2*DimSPD);
  corrMatrix = GEEUtilities::fmatrix3_ht(1, Nsample, 1, obsforonemax,1, obsforonemax);
  residualli = GEEUtilities::dvector_ht(1, totalobs);
  sum1 = GEEUtilities::dmatrix_ht(1, Ncov, 1, Ncov);
  varGEE = GEEUtilities::dmatrix_ht(1, Ncov, 1,Ncov);
  varMatrix = GEEUtilities::fmatrix3_ht(1, Nsample, 1, obsforonemax,1, obsforonemax);

  // check that everything is OK:

  if ( m_Verbose )
    {
    std::cout << "Initialize statistics variables:" << std::endl;
    std::cout << std::endl;
    std::cout << "num_subjects  = " << num_subjects << std::endl;
    std::cout << "DimSPD        = " << DimSPD << std::endl;
    std::cout << "num_cov       = " << num_cov << std::endl;
    std::cout << "NOmaxTIMEs    = " << NOmaxTIMEs << std::endl;
    std::cout << "dimX          = " << dimX << std::endl;     
    std::cout << "dimY          = " << dimY << std::endl;     
    std::cout << "dimZ          = " << dimZ << std::endl;     
    std::cout << "TotalImg      = " << TotalImg << std::endl;
    std::cout << "Ncov          = " << Ncov << std::endl;
    std::cout << "NopointSur    = " << NopointSur << std::endl;
    std::cout << "Nresample     = " << Nresample << std::endl;
    std::cout << "noRow         = " << noRow << std::endl;
    std::cout << "Nsample       = " << Nsample << std::endl;
    std::cout << "obsforonemax  = " << obsforonemax << std::endl;
    std::cout << "totalobs      = " << totalobs << std::endl;
    }

}

template< class MyRealType, class MyIntegerType >
void
LongitudinalStatisticsFilter< MyRealType, MyIntegerType >
::DeAllocateStatisticsDataStructures()
{
  GEEUtilities::free_ivector_ht(allDimTime, 1, num_subjects); /* reading the number of time points insides  HT */
  GEEUtilities::free_dvector_ht(betaGEE, 1, DimSPD*num_cov+DimSPD*2);
  GEEUtilities::free_dmatrix_ht(BrainTheta, 1, NopointSur, 1, Ncov+2*DimSPD);
  GEEUtilities::free_ivector_ht(CCID, 0, DimSPD*num_cov); //define column for nonsingular R_1
  GEEUtilities::free_fmatrix3_ht(corrMatrix, 1, Nsample, 1, obsforonemax,1, obsforonemax);
  GEEUtilities::free_matrix_ht(cov_beta, 0, num_cov*DimSPD-1, 0, num_cov*DimSPD-1);
  GEEUtilities::free_dmatrix_ht(designXX, 1, TotalImg*DimSPD, 1, Ncov);
  GEEUtilities::free_vector_ht(Epsilon, 0, num_subjects);
  GEEUtilities::free_matrix_ht(ExactTime, 1, num_subjects, 1, NOmaxTIMEs); 
  GEEUtilities::free_matrix_ht(ImageData, 1, NopointSur, 1,  TotalImg*DimSPD);
  GEEUtilities::free_ivector_ht(imageMask, 1, dimX*dimY*dimZ);
  GEEUtilities::free_ivector_ht(indxMi, 0, num_subjects); /* reading the number of time points insides  HT */
  GEEUtilities::free_ivector_ht(indxMi2, 0, num_subjects); /* reading the number of time points insides  HT */
  GEEUtilities::free_matrix_ht(INRR, 0, DimSPD*num_cov, 0, DimSPD*num_cov);
  GEEUtilities::free_matrix_ht(INRRnew, 0, DimSPD*num_cov, 0, DimSPD*num_cov);
  GEEUtilities::free_dmatrix_ht(INRRnewD, 1, DimSPD*num_cov, 1, DimSPD*num_cov);
  GEEUtilities::free_ivector_ht(Irank, 0, DimSPD*num_cov);
  GEEUtilities::free_matrix_ht(LCOVL, 0, DimSPD*num_cov-1, 0, DimSPD*num_cov-1);   // linear constraints covariance matrix
  GEEUtilities::free_vector_ht(LLBB, 0, DimSPD*num_cov-1);   // linear constraints
  //GEEUtilities::free_fmatrix3_ht(mask_tmp, 0, dimZ-1, 0, dimX-1, 0, dimY-1);
  GEEUtilities::free_matrix_ht(midRR, 0, DimSPD*num_cov, 0, DimSPD*num_cov);
  GEEUtilities::free_dvector_ht(residualli, 1, totalobs);
  GEEUtilities::free_matrix_ht(RR, 0, DimSPD*num_cov-1, 0, DimSPD*num_cov-1); // linear constraints: Wald test
  GEEUtilities::free_vector_ht(rr00, 0, DimSPD*num_cov);
  GEEUtilities::free_vector_ht(Scorevalue, 1, NopointSur);
  GEEUtilities::free_vector_ht(simulationBias, 0, DimSPD*num_cov-1);
  GEEUtilities::free_vector_ht(simulationRMS, 0, DimSPD*num_cov-1);
  GEEUtilities::free_vector_ht(simulationSD, 0, DimSPD*num_cov-1);
  GEEUtilities::free_vector_ht(SmaxEmp, 1, Nresample);
  GEEUtilities::free_vector_ht(Spvalue, 1, NopointSur);
  GEEUtilities::free_vector_ht(Srawpvalue, 1, NopointSur);
  GEEUtilities::free_dmatrix_ht(sum1, 1, Ncov, 1, Ncov);
  GEEUtilities::free_vector_ht(tempLB, 0, DimSPD*num_cov);
  GEEUtilities::free_vector_ht(tempoScorevalue, 0, NopointSur);
  GEEUtilities::free_dmatrix_ht(TXX, 1, Ncov, 1, Ncov);
   
  GEEUtilities::free_fmatrix3_ht(UUmu, 0, NopointSur, 0, num_subjects, 0, noRow+1);
  GEEUtilities::free_fmatrix3_ht(UUsgMu, 0, NopointSur, 0, noRow+1, 0, noRow+1);
  GEEUtilities::free_dmatrix_ht(varGEE, 1, Ncov, 1,Ncov);
  GEEUtilities::free_fmatrix3_ht(varMatrix, 1, Nsample, 1, obsforonemax,1, obsforonemax);
  GEEUtilities::free_vector_ht(Waldtest, 1, NopointSur);
  GEEUtilities::free_matrix_ht(x0x, 0, TotalImg-1, 0, num_cov-1);
  GEEUtilities::free_dvector_ht(Yresponse, 1, TotalImg*DimSPD);
}

template< class MyRealType, class MyIntegerType >
void
LongitudinalStatisticsFilter< MyRealType, MyIntegerType >
::InitializeStatisticsDataStructures()
{

  // when this function is called all statistics data structures have
  // already been allocated and the variables needed for their
  // allocation have been set; now fill up these dynamically allocated
  // datastructures with the given input information

  // initialize time point information
  
  std::cout << "allDimTime:" << std::endl;

  for ( unsigned int dii=1; dii<=(unsigned int)num_subjects; dii++ ) // time points
    {
    allDimTime[dii] = caseInformation.vvAge[dii-1].size();
    std::cout << allDimTime[dii] << " ";
    }

  std::cout << std::endl << "allDimTime: end." << std::endl;

  std::cout << "Exact time:" << std::endl;

  for( unsigned int dii=1; dii<=(unsigned int)num_subjects; dii++)
    {
    for( unsigned int djj=1; djj<=(unsigned int)allDimTime[dii]; djj++)
      { 
      ExactTime[dii][djj] = caseInformation.vvAge[dii-1][djj-1];
      if ( m_Verbose )
	std::cout << ExactTime[dii][djj] << " ";
      }
    }

  std::cout << "Exact time: end." << std::endl;

  if ( m_Verbose ) std::cout << std::endl;
  
  // QUESTION: Why is this treated as a special case? Can it not be
  // part of the loop?

  indxMi[0]=0; 
  indxMi2[0]=0; 
  indxMi[1]=allDimTime[1]; 
  indxMi2[1]=(int)(allDimTime[1]*(allDimTime[1]-1)*0.5); 
  
  if ( m_Verbose ) std::cout << "dii; indxMi[dii];  indxMi2[dii]:" << std::endl;

  for ( unsigned int dii=2; dii<=(unsigned int)num_subjects; dii++)
    {
    indxMi[dii]=allDimTime[dii]; 
    indxMi2[dii]=(int)(allDimTime[dii]*(allDimTime[dii]-1)*0.5);  
    indxMi[dii]+=indxMi[dii-1]; 
    indxMi2[dii]+=indxMi2[dii-1];
    
    if ( m_Verbose ) std::cout << dii << " " << indxMi[dii] << " " << indxMi2[dii] << std::endl;
    } 
  
    if ( m_Verbose ) std::cout << "dii; indxMi[dii];  indxMi2[dii]: end." << std::endl;

  if ( m_Verbose ) std::cout << "totalimg = " << TotalImg << "; num_cov = " << num_cov << std::endl;

  if ( m_DesignMatrix.compare("None") != 0 )
    {
    if ( m_Verbose )
      {
      std::cout << "Constructing covariates from the design matrix: " << m_DesignMatrix << std::endl;
      std::cout << "Make sure the ordering is consistent with the casefile (after auto-ordering): " << *(this->GetInput()->Get()) << std::endl;
      }

    // now fill in the matrix

    if ( !m_ExternalDesignMatrix.empty() )
      {

      if ( m_DebugVerbose )
	{
	std::cout << "TotalImg = " << TotalImg << "; num_cov = " << num_cov << std::endl;
	std::cout << "TIED = " << m_ExternalDesignMatrix.size() << "; num_covEDM = " << m_ExternalDesignMatrix[0].size() << std::endl;
	}
     
      

      for ( unsigned int iI=0; iI<m_ExternalDesignMatrix.size(); iI++ )
	{
	for ( unsigned int iJ=0; iJ<m_ExternalDesignMatrix[iI].size(); iJ++ )
	  {
	  x0x[iI][iJ] = m_ExternalDesignMatrix[iI][iJ];
	  }
	}
        

      }
    else
      {
      std::cout << "WARNING: Externally defined design matrix is EMPTY!" << std::endl;
      std::cout << "Could not initialize the design matrix." << std::endl;
      }

    }
  else  // no external design matrix defined, initialize based on case file
    {

    // initialize the design matrix; correct? This is for the covariates
    // QUESTION: xox???
    
    // every image has multiple time-points so we need to translate this
    // to the linear organization of Hongtu's datastructure
    
    unsigned int currentImageCounter = 0;  // translates to Hongtu's
    // linear index
    
    for ( unsigned int iSubj=0; iSubj<caseInformation.vSubjectId.size(); iSubj++ )
      {
      for ( unsigned int iTimePoint=0; iTimePoint<caseInformation.vvAge[iSubj].size(); iTimePoint++ )
	{
	
	unsigned covOffset = 0;
	if ( m_CovaryWithGroup )
	  {
	  x0x[currentImageCounter][0] = caseInformation.vGroup[iSubj];
	  covOffset++;
	  }
	
	if ( m_CovaryWithGender )
	  {
	  x0x[currentImageCounter][covOffset] = caseInformation.vGender[iSubj];
	  covOffset++;
	  }
	
	for( unsigned int djj=covOffset; djj<(unsigned int)num_cov; djj++)
	  {
	  x0x[currentImageCounter][djj] = caseInformation.vvAdditionalCovariates[iSubj][iTimePoint][djj-covOffset];
	  }
	
	currentImageCounter++;
	}
      }
    }


  //Hongtu Zhu  March 17. 2010   
  // We need to standardize each column of covariate matrix 
  //Hongtu Zhu 

/*    float x0xMean, x0xVar;  

    for ( unsigned int iJ=0; iJ<m_ExternalDesignMatrix[0].size(); iJ++ )
	  {
          x0xMean=0.0; 
          x0xVar=0.0; 
          for ( unsigned int iI=0; iI<m_ExternalDesignMatrix.size(); iI++ )
	   {
		   x0xMean+=x0x[iI][iJ];
                   x0xVar+=x0x[iI][iJ]*x0x[iI][iJ]; 
	  }
         x0xMean=x0xMean/(1.0*m_ExternalDesignMatrix.size());  
         x0xVar=x0xVar/(1.0*m_ExternalDesignMatrix.size())-x0xMean*x0xMean;  
         x0xVar=sqrt(x0xVar);     
         if(x0xVar>0) 
          for ( unsigned int iI=0; iI<m_ExternalDesignMatrix.size(); iI++ )
		  x0x[iI][iJ]=(x0x[iI][iJ]-x0xMean)/x0xVar;
	} 
*/  
       
  //Hongtu Zhu  March 17. 2010   


  // end of the design matrix initialization

  // linear constraints

  if ( m_Verbose )
    std::cout << "number of rows for the linear constraints = " << noRow << std::endl;

  if(noRow>DimSPD*num_cov-1)
    noRow=DimSPD*num_cov-1;
	
  for( int ii=0; ii<=DimSPD*num_cov-1; ii++)
    {
    for( int jj=0; jj<=DimSPD*num_cov-1; jj++)
      {
      RR[ii][jj]=0.0;
      }
    rr00[ii]=0.0;
    }

  if ( m_Verbose )
    std::cout << "TotalImg = " << TotalImg << ", dim=[" << DimSPD*dimZ << " " << dimX << " " << dimY << "]" << std::endl;

  // Linear Matrix LL with NoRow and Ncov; Constraint Vector; columns
  // that makes R_1 nonsingular; QUESTION?

  for ( int dii=0; dii<DimSPD*num_cov; dii++)
    {
    for( int djj=0; djj<DimSPD*num_cov; djj++)
      {
      INRR[dii][djj]=0.0;
      midRR[dii][djj]=0.0;
      }
    }

  // QUESTION: linearMAGE.txt?? has to be the constraint matrix
  // tranfer the information from the matrix we already read previously
  
  // check the dimension of the constraint matrix

  if ( !m_ConstraintMatrix.empty() )
    {
    if ( (int)m_ConstraintMatrix.size()!=(noRow*2+1) || (int)m_ConstraintMatrix[0].size()!=DimSPD*num_cov )
      {
      std::cout << "ERROR: constraint matrix should be of dimension: " << (noRow*2+1) << " x " << DimSPD*num_cov;
      std::cout << "; but is " << m_ConstraintMatrix.size() << " x " << m_ConstraintMatrix[0].size() << std::endl;
      }
    }

  if ( m_DebugVerbose )
    std::cout << "RR:" << std::endl;

  for( int dii=0; dii<noRow; dii++)
    {
    for( int djj=0; djj<DimSPD*num_cov; djj++)
      {
      RR[dii][djj] = m_ConstraintMatrix[dii][djj];
      if ( m_Verbose )
	std::cout << RR[dii][djj] << " ";
      }
    std::cout << std::endl;
    } 
	
  if ( m_DebugVerbose )
    std::cout << std::endl << "RR:end" << std::endl;

  if ( m_DebugVerbose )
    std::cout << "rr00:" << std::endl;

  for( int dii=0; dii<noRow; dii++)
    {
    rr00[dii] = m_ConstraintMatrix[noRow+dii][0];
    if ( m_Verbose )
      std::cout << rr00[dii] << std::endl;
    }

  if ( m_DebugVerbose )
    std::cout << "rr00:end" << std::endl;

  for( int dii=0; dii<noRow; dii++)
    {
    // QUESTION: Is the (int) cast ok here? What should be in the matrix
    CCID[dii]= (int)m_ConstraintMatrix[2*noRow][dii];//(int)m_ConstraintMatrix[2*noRow+dii][0];
    CCID[dii]=CCID[dii]-1;
    midRR[0][CCID[dii]]=1;
    }

  if ( m_DebugVerbose )
    {

    std::cout << "midRR:" << std::endl;
    for ( int dii=0; dii<noRow; dii++ )
      {
      std::cout << midRR[0][dii] << " ";
      }
    std::cout << std::endl;
    std::cout << "midRR:end" << std::endl;

    std::cout << "CCID(before):" << std::endl;
    for ( int dii=0; dii<noRow; dii++ )
      {
      std::cout << CCID[dii] << " ";
      }
    std::cout << std::endl;
    std::cout << "CCID(before):end" << std::endl;

    }

  for( int dii=0, djj=noRow; dii<DimSPD*num_cov; dii++)
    if( midRR[0][dii]<1.0 )
      {
      CCID[djj]=dii;
      djj++;
      }

  if ( m_DebugVerbose )
    {
    std::cout << "CCID(after):" << std::endl;
    for ( int dii=0; dii<noRow; dii++ )
      {
      std::cout << CCID[dii] << " ";
      }
    std::cout << std::endl;
    std::cout << "CCID(after):end" << std::endl;
    }

  for( int dii=0; dii<noRow; dii++)
    for( int djj=0; djj<noRow; djj++)
      INRR[dii][djj]=RR[dii][CCID[djj]];

  // display it

  if ( m_DebugVerbose )
    {

    std::cout << "INRR matrix: " << std::endl;
  
    for( int dii=0; dii<noRow; dii++)
      {
      for( int djj=0; djj<noRow; djj++)
	std::cout << INRR[dii][djj] << " ";
      std::cout << std::endl;
      }
    
    std::cout << "INRR. End" << std::endl;
    }

  ivYS(INRR, 0, noRow-1);
  
  for( int dii=0; dii<noRow; dii++)
    for( int djj=0; djj<DimSPD*num_cov; djj++)
      {
      midRR[dii][djj]=0.0;
      for( int dkk=0; dkk<noRow; dkk++)
	midRR[dii][djj]-=INRR[dii][dkk]*RR[dkk][djj];
      }
  
  for( int dii=0; dii<noRow; dii++)
    for( int djj=0; djj<noRow; djj++)
      midRR[dii][CCID[djj]]=INRR[dii][djj];
  
  for( int dii=noRow; dii<DimSPD*num_cov; dii++)
    midRR[dii][CCID[dii]]=1.0;
  
  //this generates midRR=[R_1^{-1}, -R_1^{-1}R_2]
  for( int dii=0; dii<DimSPD*num_cov; dii++)
    for( int djj=0; djj<DimSPD*num_cov; djj++)
      INRR[dii][djj]=midRR[dii][djj];
  
  for( int dii=0; dii<DimSPD*num_cov; dii++)
    for( int djj=0; djj<DimSPD*num_cov; djj++)
      INRRnew[dii][djj]=0.0;
  
  for( int dii=0; dii<noRow; dii++)
    for( int djj=0; djj<DimSPD*num_cov; djj++)
      INRRnew[dii][djj]=RR[dii][djj];
  
  for( int dii=noRow; dii<DimSPD*num_cov; dii++)
    INRRnew[dii][CCID[dii]]=1.0;
  
  for( int dii=1; dii<=DimSPD*num_cov; dii++)
    for( int djj=1; djj<=DimSPD*num_cov; djj++)
      INRRnewD[dii][djj]=INRRnew[dii-1][djj-1];
  
  //float ht=DnewIVrank(INRRnewD, Irank, DimSPD*num_cov);
  DnewIVrank(INRRnewD, Irank, DimSPD*num_cov);
	
  if ( m_DebugVerbose )
    {
    
    std::cout << "INRRnewD:" << std::endl;
    
    for( int dii=1; dii<=DimSPD*num_cov; dii++)
      {
      for( int djj=1; djj<=DimSPD*num_cov; djj++)
	std::cout << INRRnewD[dii][djj] << " ";
      std::cout << std::endl;
      }
    
    std::cout << "INRRnewD:end." << std::endl;

    }

  // create design matrix here
  
  for( int dii=1; dii<=TotalImg*DimSPD; dii++)
    for( int dkk=1; dkk<=Ncov; dkk++)
      designXX[dii][dkk]=0.0;

  for( int dii=1; dii<=TotalImg; dii++)
    for( int dkk=1; dkk<=DimSPD; dkk++)
      for( int djj=1; djj<=num_cov; djj++)
	designXX[(dii-1)*DimSPD+dkk][(dkk-1)*num_cov+djj]=x0x[dii-1][djj-1]*1.0;

  if ( m_Verbose )
    {
    std::cout << "Design matrix:" << std::endl;
    for( int dii=1; dii<=TotalImg; dii++)
      {
      for( int dkk=1; dkk<=DimSPD; dkk++)
	{
	for( int djj=1; djj<=num_cov; djj++)
	  {
	  std::cout << designXX[(dii-1)*DimSPD+dkk][(dkk-1)*num_cov+djj] << " ";
	  }
	std::cout << std::endl;
	}
      }
    }

   for( int dii=1; dii<=Ncov; dii++)
     for( int djj=1; djj<=Ncov; djj++)
       TXX[dii][djj]=0.0;

   for( int dii=1; dii<=TotalImg*DimSPD; dii++)
     for( int djj=1; djj<=Ncov; djj++)
       for( int dkk=1; dkk<=Ncov; dkk++)
	 TXX[djj][dkk]+=designXX[dii][djj]*designXX[dii][dkk];

   invv(TXX, Ncov);

   std::cout << "after input the images 2" << std::endl;

   for( int djj=1; djj<=NopointSur; djj++)
     {
     Spvalue[djj]=0.0;
     Srawpvalue[djj]=0.0;
     Waldtest[djj]=0.0;
     }

}


template< class MyRealType, class MyIntegerType >
void
LongitudinalStatisticsFilter< MyRealType, MyIntegerType >
::ComputeStatisticsWithMask()
{


   // here also the output is created; need to iterate only over the
   // mask, the tensors have already been stored in Hongtu's data structures
  
   // define the iterators and start iterating

   ScalarIntegerIteratorType maskIterator( maskReader->GetOutput(), maskReader->GetOutput()->GetLargestPossibleRegion() );
   
   VectorIteratorType covarianceIterator;
   VectorType zeroCovarianceVector;
   VectorType covarianceVector;

   if ( Ncov>0 ) // if we have covariates
     {
     covarianceIterator = VectorIteratorType( covarianceImage, covarianceImage->GetLargestPossibleRegion() );
     covarianceIterator.GoToBegin();
     zeroCovarianceVector.SetSize( (Ncov+1)*Ncov/2 );
     zeroCovarianceVector.Fill( 0 );
     
     covarianceVector.SetSize( (Ncov+1)*Ncov/2 );
     }
   
   // TODO: covariance storage needs to be dimension (Ncov+1)*(Ncov)2
   // make this a true covariance image, so we can visualize it as tensors

   // TODO: implement a version which does not use the mask iterator
   // if computation is to be performed across the complete domain

   maskIterator.GoToBegin();

   unsigned long pointINDX = 1;  // index to make the connection to
				    // Hongtu's datastructures
   int counttemp1=0;
   double tempValue;
   double tempMean;

   unsigned long index = 0;

   // Hongtu Miccai --> does all of this really go here?, do I keep my
   // original part further below?

   maxYvalue=0.0;

   for ( unsigned int iM=0; iM<(unsigned int)NopointSur; iM++ )
     {
     for( int i=0; i<TotalImg; i++)
       {
       for( int dii=1; dii<=DimSPD; dii++)
	 {
	 if(ImageData[pointINDX][i*DimSPD+dii]>maxYvalue)
	   maxYvalue=ImageData[pointINDX][i*DimSPD+dii];
 //        std::cout << " I am here: maxYvalue " << maxYvalue << std::endl;
	 }
       }/* for */
     pointINDX++;
     }


   std::cout << " I am here: maxYvalue " << maxYvalue << std::endl;

   int flag = 0;
   int noiter = 0;

   int p=num_cov;//4 PARAMETERS?????
   int q=1;//variance

   unsigned long nrOfEstimatesComputed = 0;

   pointINDX = 1;

   while ( !( maskIterator.IsAtEnd() || covarianceIterator.IsAtEnd() ) )
     {
     MyIntegerType currentMaskValue = maskIterator.Get();
     
     if ( Ncov>0 )
       covarianceVector = zeroCovarianceVector;
     
     if ( currentMaskValue>0 ) // inside the mask
       {
       
       flag=1;
       //attention: You need to give tensors to Y
       tempValue=0.0;  tempMean=0.0;
       for( int i=0; i<TotalImg; i++)
	 {
	 for( int dii=1; dii<=DimSPD; dii++)
	   {
	   Yresponse[i*DimSPD+dii]= ImageData[pointINDX][i*DimSPD+dii]*100.0/maxYvalue;
           if(dii==1){
	     tempValue+=Yresponse[i*DimSPD+dii]*Yresponse[i*DimSPD+dii];
	     tempMean+=Yresponse[i*DimSPD+dii];
	      } 
	    }
	 }
     
       
       tempValue=tempValue/(TotalImg*1.0);  // DimPSD?
       tempMean=tempMean/(TotalImg*1.0);    // DimPSD?
       
       if(sqrt(tempValue-tempMean*tempMean)<=0.001) 
	 flag=0;
       
       if(flag>=1)
	 {
		 
	 nrOfEstimatesComputed++; 

   GEEestimatesHongtu(betaGEE,
                                  residualli,
                                  designXX,
                                  TXX,
                                  Yresponse,
                                  Nsample,
                                  Ncov,
                                  TotalImg,
                                  DimSPD,
                                  allDimTime,
                                  p,
                                  q,
                                  varMatrix,
                                  ExactTime,
                                  indxMi,
                                  indxMi2);


   CalVarGEEHongtu(designXX, residualli, DimSPD, allDimTime, p , betaGEE,  varMatrix,  varGEE, Nsample, Ncov );

   Waldtest[pointINDX]=0.0;
   Waldtest[pointINDX] = WaldHongtu(noRow, betaGEE, varGEE, RR, rr00, Ncov);

   if(pointINDX%1000==0)
         std::cout << "Waldtest = " << Waldtest[pointINDX] << std::endl;
 
         unsigned int covarianceIndex = 0;
	 for( int dkk=1; dkk<=Ncov; dkk++) 
	   {
	   for( int dii=dkk; dii<=Ncov; dii++)
	     { 
	     tempValue=(float)varGEE[dkk][dii]; 
	     covarianceVector[ covarianceIndex ] = tempValue;
	     covarianceIndex++;
	     } 
	   }           
	 for( int dkk=1; dkk<=Ncov+2*DimSPD; dkk++)
	   {
	   BrainTheta[pointINDX][dkk]=betaGEE[dkk];
 	   } 

   Scorevalue[pointINDX] = SCOREbetaGEEHongtu(noRow,
                                                          pointINDX,
                                                          Yresponse,
                                                          designXX,
                                                          BrainTheta[pointINDX],
                                                          INRRnewD,
                                                          RR,
                                                          rr00,
                                                          Scorevalue,
                                                          UUmu[pointINDX - 1],
                                                          UUsgMu[pointINDX - 1],
                                                          allDimTime,
                                                          DimSPD,
                                                          varMatrix,
                                                          Nsample,
                                                          Ncov);

 	 }
       else  
	 {
	 Scorevalue[pointINDX]=0.0;
	 Waldtest[pointINDX]=0.0;
	 for( int i=0; i<num_subjects; i++)
	   {
	   for( int j=0; j<=noRow; j++)
	     {
	     UUmu[pointINDX-1][i][j]=0.0;   //Feb21
	     }
	   }

	 for( int i=0; i<=noRow; i++)
	   {
	   for( int j=0; j<=noRow; j++)
	     {
	     UUsgMu[pointINDX-1][i][j]=0.0;
	     if(i==j)
	       UUsgMu[pointINDX-1][i][i]=1.0;   //Feb21
	     }
	   }
	 } // end else; if (flag>=1)
       if (Scorevalue[pointINDX]==0.0)
	 counttemp1+=1;

       pointINDX++;
       }

     // now store the covariance value in the image

     if ( Ncov>0 )
       {
       covarianceIterator.Set( covarianceVector );
       ++covarianceIterator;
       }

     index++;
     ++maskIterator;

     } // end loop over mask and tensor image


   std::cout << std::endl << "pointINDX = " << pointINDX << ", ";
   std::cout << "NopointSur = " << NopointSur << ", ";
   std::cout << "index = " << index << ", ";
   std::cout << "counttemp1 = " << counttemp1 << std::endl;


   // TODO: we also need to iterate over all the outputs and stick the
   // results in; TODO: allocate and define these datastructures

   noiter=1;
   do
     {
     SmaxEmp[noiter]=0.0;
     
     for( int djj=0; djj<num_subjects; djj++)
       {
       if(s_xuni()>0.5)
	 Epsilon[djj]=1.0;
       else
	 Epsilon[djj]=-1.0;
       }

     for( int dii=0; dii<NopointSur; dii++)
       {
       tempoScorevalue[dii]=resampleMUYS(noRow, num_cov*DimSPD, num_subjects, 0,  Epsilon, UUmu[dii], UUsgMu[dii]);
       
       if(tempoScorevalue[dii]>=Scorevalue[dii+1])
	 Srawpvalue[dii+1]=Srawpvalue[dii+1]+1.0;
       
       if(tempoScorevalue[dii]>SmaxEmp[noiter])
	 SmaxEmp[noiter]=tempoScorevalue[dii];

       }
     
     std::cout << noiter << std::endl;
     noiter++;
     
     } while(noiter<=Nresample);

   std::cout << std::endl << "NopointSur = " << NopointSur << std::endl;

   for( int dii=1; dii<=NopointSur; dii++)
     {
     Srawpvalue[dii]=Srawpvalue[dii]/(Nresample*1.0);
     if(Srawpvalue[dii]<3.0)
       if ( m_DebugVerbose )
	  std::cout << "dii = " << dii << " " << Srawpvalue[dii]*1.0 << std::endl;
     if(Srawpvalue[dii]>0)
       Srawpvalue[dii]=-log10(Srawpvalue[dii]*1.0);
     else
       {
       Srawpvalue[dii]=3.0;
       }
     }
   
   noiter=Nresample;
   sort2YS(noiter, SmaxEmp);
   
   std::string  summary="SummaryResFtest.txt";
   FILE *fp1 = fopen(summary.c_str(), "w"); //*.img 

   fprintf(fp1, "Resampling distributions\n"); 

   for( int dii=1; dii<=Nresample; dii++) 
     fprintf(fp1, "%14lf\n", SmaxEmp[dii]); 
   fclose(fp1);

   
   int counttemp=0;
   
   // QUESTION: What does this command do?

   for( int dii=1; dii<=NopointSur; dii++)
     if(Srawpvalue[dii]==3.0)
       counttemp+=1;

   std::cout << std::endl << " counttemp = " << counttemp; 
   CalPvalueYS(NopointSur, SmaxEmp,  Scorevalue, Spvalue); 
   if ( m_Verbose )
     std::cout << "Number of estimates computed = " << nrOfEstimatesComputed << std::endl;

}


template< class MyRealType, class MyIntegerType >
void
LongitudinalStatisticsFilter< MyRealType, MyIntegerType >
::ComputeStatisticsWithoutMask()
{


   // here also the output is created; need to iterate only over the
   // mask, the tensors have already been stored in Hongtu's data structures
  
   // define the iterators and start iterating

  VectorIteratorType covarianceIterator;
  VectorType zeroCovarianceVector;
  VectorType covarianceVector;

  if ( Ncov>0 ) // if we have covariates
    {
    covarianceIterator = VectorIteratorType( covarianceImage, covarianceImage->GetLargestPossibleRegion() );
    covarianceIterator.GoToBegin();
    zeroCovarianceVector.SetSize( (Ncov+1)*Ncov/2 );
    zeroCovarianceVector.Fill( 0 );

    covarianceVector.SetSize( (Ncov+1)*Ncov/2 );
    }


   // TODO: covariance storage needs to be dimension (Ncov+1)*(Ncov)2
   // make this a true covariance image, so we can visualize it as tensors

   // TODO: implement a version which does not use the mask iterator
   // if computation is to be performed across the complete domain


   unsigned long pointINDX = 1;  // index to make the connection to
				    // Hongtu's datastructures
   int counttemp1=0;
   double tempValue;

   unsigned long index = 0;

   int flag = 0;
   int noiter = 0;

   int p=num_cov;//4 PARAMETERS?????
   int q=1;//variance


   while ( !( covarianceIterator.IsAtEnd() ) )
     {
     if ( Ncov )
       covarianceVector = zeroCovarianceVector;

 
       std::cout << "pointINDX  I am here= " << pointINDX << std::endl;

     flag=1;
     //attention: You need to give tensors to Y
     tempValue=0.0;
     for( int i=0; i<TotalImg; i++)
       {
       for( int dii=1; dii<=DimSPD; dii++)
	 {
	 Yresponse[i*DimSPD+dii]= ImageData[pointINDX][i*DimSPD+dii];
	 // fprintf(dat1, "%lf\n", Yresponse[i*DimSPD+dii]); 
	 // Yresponse[i*DimSPD+dii]=gasdev();
	 tempValue+=Yresponse[i*DimSPD+dii]*Yresponse[i*DimSPD+dii];
	 }
       }
     // scanf("%d", &dii);
     // printf("2. %lf\n", tempValue);
     
     if(fabs(tempValue)<=0.0)
       flag=0;
     
     if(flag>=1)
       {
       // for(i=0; i<TotalImg; i++)
       // for(dii=1; dii<=DimSPD; dii++)
       // fprintf(fp1, "%f \n", Yresponse[i*DimSPD+dii]);
       
       GEEestimatesHongtu(betaGEE,   residualli, designXX, TXX, Yresponse, Nsample, Ncov, TotalImg, DimSPD, allDimTime, p, q, varMatrix, ExactTime,   indxMi,   indxMi2);
       
       //exit(-1); // for debug only: REMOVE

       // scanf("%d", &dii);
       // printf("4. \n");
       
       CalVarGEEHongtu(designXX, residualli, DimSPD, allDimTime, p , betaGEE,  varMatrix,  varGEE, Nsample, Ncov );
       
       Waldtest[pointINDX]=WaldHongtu(noRow, betaGEE, varGEE, RR, rr00, Ncov );
       
       // QUESTION: We store everything in a covariance image, so we
       // do not need to keep track of these coordinates. Correct?
       
       //tempValue=ii+1.0;   
       //fwrite(&tempValue, sizeof(float), 1, datVAR); 
       //tempValue=jj+1.0;   
       //fwrite(&tempValue, sizeof(float), 1, datVAR); 
       //tempValue=kk+1.0;   
       //fwrite(&tempValue, sizeof(float), 1, datVAR); 
       
       unsigned int covarianceIndex = 0;
       
       for( int dkk=1; dkk<=Ncov; dkk++) 
	 {
	 for( int dii=dkk; dii<=Ncov; dii++)
	   { 
	   tempValue=(float)varGEE[dkk][dii]; 
	   //fwrite(&tempValue, sizeof(float), 1, datVAR);
	   covarianceVector[ covarianceIndex ] = tempValue; 
	   covarianceIndex++;
	   }
	 // scanf("%d", &dii);
	 }
       
       for( int dkk=1; dkk<=Ncov+2*DimSPD; dkk++)
	 {
	 BrainTheta[pointINDX][dkk]=betaGEE[dkk];
	 // fprintf(fp1, "beta=%f \n", betaGEE[dkk]);
	 }
       // for(dkk=1; dkk<=Ncov; dkk++)
       // fprintf(fp1, "var=%lf \n", sqrt(varGEE[dkk][dkk]));
       
       Scorevalue[pointINDX]=SCOREbetaGEEHongtu(noRow, pointINDX, Yresponse, designXX, BrainTheta[pointINDX],  INRRnewD, RR, rr00, Scorevalue, UUmu[pointINDX-1], UUsgMu[pointINDX-1], allDimTime, DimSPD, varMatrix, Nsample, Ncov );
       // printf("%f %f\n", Waldtest[pointINDX], Scorevalue[pointINDX]);
       // fprintf(fp1, "%f  %f\n", Waldtest[pointINDX], Scorevalue[pointINDX]);
       // scanf("%d", &dii); 
       // fclose(fp1); 
       }
     else // if (flag>=1)
       {
       Scorevalue[pointINDX]=0.0;
       Waldtest[pointINDX]=0.0;
       for( int i=0; i<num_subjects; i++)
	 {
	 for( int j=0; j<=noRow; j++)
	   {
	   UUmu[pointINDX-1][i][j]=0.0;   //Feb21
	   }
	 }
       
       for( int i=0; i<=noRow; i++)
	 {
	 for( int j=0; j<=noRow; j++)
	   {
	   UUsgMu[pointINDX-1][i][j]=0.0;
	   if(i==j)
	     UUsgMu[pointINDX-1][i][i]=1.0;   //Feb21
	   }
	 }
       } // end else; if (flag>=1)
     if (Scorevalue[pointINDX]==0.0)
       counttemp1+=1;
     
     pointINDX++;

     // now store the covariance value in the image

     if ( Ncov>0 )
       {
       covarianceIterator.Set( covarianceVector );
       ++covarianceIterator;
       }

     index++;


     } // end loop over all voxels


   std::cout << std::endl << "pointINDX = " << pointINDX << ", ";
   std::cout << "NopointSur = " << NopointSur << ", ";
   std::cout << "index = " << index << ", ";
   std::cout << "counttemp1 = " << counttemp1 << std::endl;


   // TODO: we also need to iterate over all the outputs and stick the
   // results in; TODO: allocate and define these datastructures

   noiter=1;
   do
     {
     SmaxEmp[noiter]=0.0;
     
     for( int djj=0; djj<num_subjects; djj++)
       {
       if(s_xuni()>0.5)
	 Epsilon[djj]=1.0;
       else
	 Epsilon[djj]=-1.0;
       }

     for( int dii=0; dii<NopointSur; dii++)
       {
       tempoScorevalue[dii]=resampleMUYS(noRow, num_cov*DimSPD, num_subjects, 0,  Epsilon, UUmu[dii], UUsgMu[dii]);
       
       if(tempoScorevalue[dii]>=Scorevalue[dii+1])
	 Srawpvalue[dii+1]=Srawpvalue[dii+1]+1.0;
       
       if(tempoScorevalue[dii]>SmaxEmp[noiter])
	 SmaxEmp[noiter]=tempoScorevalue[dii];

       }
     
     std::cout << noiter << std::endl;
     noiter++;
     
     } while(noiter<=Nresample);

   std::cout << std::endl << "NopointSur = " << NopointSur << std::endl;

   for( int dii=1; dii<=NopointSur; dii++)
     {
     Srawpvalue[dii]=Srawpvalue[dii]/(Nresample*1.0);
     
     if(Srawpvalue[dii]<3.0)
       if ( m_DebugVerbose )
	 std::cout << "dii = " << dii << " " << Srawpvalue[dii]*1.0 << std::endl;
     //printf("dii=%d  %f\n", dii, Srawpvalue[dii]); 

     if(Srawpvalue[dii]>0)
       Srawpvalue[dii]=-log10(Srawpvalue[dii]*1.0);
     else
       {
       Srawpvalue[dii]=3.0;
       }
     }
   
   noiter=Nresample;
   sort2YS(noiter, SmaxEmp);
   
   std::string  summary="SummaryResFtest.txt";
   FILE *fp1 = fopen(summary.c_str(), "w"); //*.img 

   fprintf(fp1, "Resampling distributions\n"); 

   for( int dii=1; dii<=Nresample; dii++) 
     fprintf(fp1, "%14lf\n", SmaxEmp[dii]); 
   fclose(fp1);

   
   int counttemp=0;
   
   // QUESTION: What does this command do?

   for( int dii=1; dii<=NopointSur; dii++)
     if(Srawpvalue[dii]==3.0)
       counttemp+=1;

   //printf("\n counttemp=%d",counttemp);
   std::cout << std::endl << " counttemp = " << counttemp;

   // QUESTION: What happens here?

   CalPvalueYS(NopointSur, SmaxEmp,  Scorevalue, Spvalue);

   //printf("\n voxels=%d, NopointSur=%d",voxels,NopointSur);
   //std::cout << std::endl << " voxels = " << voxels << ", NopointSur = " << NopointSur;

}

template <class MyRealType, class MyIntegerType>
void 
LongitudinalStatisticsFilter<MyRealType, MyIntegerType>
::TransferStatisticsResultsToOutputStructuresWithMask()
{
   // store the raw p-values
   // store the s p-values
   // store the SCORE values
   // store the WALD values
   // store the BETA values -- this will be the output

   ScalarIntegerIteratorType maskIterator( maskReader->GetOutput(), maskReader->GetOutput()->GetLargestPossibleRegion() );   
   ScalarIteratorType rawPValueIterator( rawPValueImage, rawPValueImage->GetLargestPossibleRegion() );
   ScalarIteratorType sPValueIterator( sPValueImage, sPValueImage->GetLargestPossibleRegion() );
   ScalarIteratorType SCOREValueIterator( SCOREValueImage, SCOREValueImage->GetLargestPossibleRegion() );
   ScalarIteratorType WALDValueIterator( WALDValueImage, WALDValueImage->GetLargestPossibleRegion() );

   // todo, maybe make this the actual ouput, because this are the coefficients
   VectorIteratorType BETAIterator( this->GetOutput(), this->GetOutput()->GetLargestPossibleRegion() );

   maskIterator.GoToBegin();
   rawPValueIterator.GoToBegin();
   sPValueIterator.GoToBegin();
   SCOREValueIterator.GoToBegin();
   WALDValueIterator.GoToBegin();
   BETAIterator.GoToBegin();

   unsigned int currentVoxelIndex = 1; // QUESTION: needs to start at 1, why??

   VectorType zeroBetaVector;
   zeroBetaVector.SetSize( Ncov + 2*DimSPD );
   zeroBetaVector.Fill( 0 );


   VectorType currentBetaVector;
   currentBetaVector.SetSize( Ncov + 2*DimSPD );

   while ( ! ( rawPValueIterator.IsAtEnd() || 
	       maskIterator.IsAtEnd() ||
	       sPValueIterator.IsAtEnd() ||
	       SCOREValueIterator.IsAtEnd() ||
	       WALDValueIterator.IsAtEnd() ||
	       BETAIterator.IsAtEnd() ) )
     {
     if ( maskIterator.Get()>0 )
       {
       // inside the mask
       rawPValueIterator.Set( Srawpvalue[ currentVoxelIndex ] );
       sPValueIterator.Set( Spvalue[ currentVoxelIndex ] );
       SCOREValueIterator.Set( Scorevalue[ currentVoxelIndex ] );
       WALDValueIterator.Set( Waldtest[ currentVoxelIndex ] );

       for ( int dii=1; dii<=Ncov+2*DimSPD; dii++ )
	 {
	 currentBetaVector[dii-1] = (MyRealType)maxYvalue/100.0*BrainTheta[ currentVoxelIndex ][ dii ];
	 }
       BETAIterator.Set( currentBetaVector );

       currentVoxelIndex++;
       }
     else
       {
       // outside the mask; write a zero
       rawPValueIterator.Set( 0 );
       sPValueIterator.Set( 0 );
       SCOREValueIterator.Set( 0 );
       WALDValueIterator.Set( 0 );
       BETAIterator.Set( zeroBetaVector );
       }
     ++maskIterator;
     ++rawPValueIterator;
     ++sPValueIterator;
     ++SCOREValueIterator;
     ++WALDValueIterator;
     ++BETAIterator;
     }

}

template <class MyRealType, class MyIntegerType>
void 
LongitudinalStatisticsFilter<MyRealType, MyIntegerType>
::TransferStatisticsResultsToOutputStructuresWithoutMask()
{
   // store the raw p-values
   // store the s p-values
   // store the SCORE values
   // store the WALD values
   // store the BETA values -- this will be the output

  // everything will be written out; assumes there is no mask (simply
  // for testing, for all practical applications a mask should be
  // available and should be used)

   ScalarIteratorType rawPValueIterator( rawPValueImage, rawPValueImage->GetLargestPossibleRegion() );
   ScalarIteratorType sPValueIterator( sPValueImage, sPValueImage->GetLargestPossibleRegion() );
   ScalarIteratorType SCOREValueIterator( SCOREValueImage, SCOREValueImage->GetLargestPossibleRegion() );
   ScalarIteratorType WALDValueIterator( WALDValueImage, WALDValueImage->GetLargestPossibleRegion() );

   // todo, maybe make this the actual ouput, because this are the coefficients
   VectorIteratorType BETAIterator( this->GetOutput(), this->GetOutput()->GetLargestPossibleRegion() );

   rawPValueIterator.GoToBegin();
   sPValueIterator.GoToBegin();
   SCOREValueIterator.GoToBegin();
   WALDValueIterator.GoToBegin();
   BETAIterator.GoToBegin();

   VectorType currentBetaVector;
   currentBetaVector.SetSize( Ncov + 2*DimSPD );

   unsigned int currentVoxelIndex = 1; // QUESTION: needs to start at 1, why??

   while ( ! ( rawPValueIterator.IsAtEnd() || 
	       sPValueIterator.IsAtEnd() ||
	       SCOREValueIterator.IsAtEnd() ||
	       WALDValueIterator.IsAtEnd() ||
	       BETAIterator.IsAtEnd() ) )
     {
     rawPValueIterator.Set( Srawpvalue[ currentVoxelIndex ] );
     sPValueIterator.Set( Spvalue[ currentVoxelIndex ] );
     SCOREValueIterator.Set( Scorevalue[ currentVoxelIndex ] );
     WALDValueIterator.Set( Waldtest[ currentVoxelIndex ] );
     
     for ( int dii=1; dii<=Ncov+2*DimSPD; dii++ )
       {
       currentBetaVector[dii-1] = BrainTheta[ currentVoxelIndex ][ dii ];
       }
     BETAIterator.Set( currentBetaVector );
     
     currentVoxelIndex++;

     ++rawPValueIterator;
     ++sPValueIterator;
     ++SCOREValueIterator;
     ++WALDValueIterator;
     ++BETAIterator;
     }

}

//----------------------------------------------------------------------------

/** Update */
template< class MyRealType, class MyIntegerType >
void
LongitudinalStatisticsFilter< MyRealType, MyIntegerType >
::GenerateData()
{
  itkDebugMacro(<< "LongitudinalStatisticsFilter::Update() called");

  // generateOutputInformation has been called at this point, so we
  // can allocate the statistics structures

  AllocateStatisticsDataStructures();
  InitializeStatisticsDataStructures();
  CopyMembersToGlobals();
  PrintGlobals();

  // initializing the seeds

  setSEED(SEEDY, m_Seed);
  init1(SEEDY);

  if ( m_Verbose ) std::cout << "Computing statistics." << std::endl;

  if ( m_MaskImage.compare("None")!=0 )
    {
    // has a mask image
    ComputeStatisticsWithMask();
    TransferStatisticsResultsToOutputStructuresWithMask();
    }
  else
    {
    ComputeStatisticsWithoutMask();
    TransferStatisticsResultsToOutputStructuresWithoutMask();
    }

   DeAllocateStatisticsDataStructures();

} // end update function  

template <class MyRealType, class MyIntegerType>
void 
LongitudinalStatisticsFilter<MyRealType, MyIntegerType>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Size : " << m_Size << std::endl;
  os << indent << "Origin: " << m_Origin << std::endl;
  os << indent << "Spacing: " << m_Spacing << std::endl;
}


} // end namespace itk

#endif
