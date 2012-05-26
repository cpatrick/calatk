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

namespace CALATK
{

template < class TFloat, unsigned int VImageDimension >
CImageInformation< TFloat, VImageDimension >::CImageInformation()
  : m_TimePoint( 0 ),
    m_SubjectId( -1 ),
    m_UniqueId( -1 ),
    m_ImageHasBeenLoaded( false ),
    m_ScalesHaveBeenGenerated( false )
{

}

template < class TFloat, unsigned int VImageDimension >
CImageInformation< TFloat, VImageDimension >::~CImageInformation()
{

}

template < class TFloat, unsigned int VImageDimension >
void CImageInformation< TFloat, VImageDimension >::SetImageFileName( std::string imageFileName )
{
  m_ImageFileName = imageFileName;
}

template < class TFloat, unsigned int VImageDimension >
std::string CImageInformation< TFloat, VImageDimension >::GetImageFileName()
{
  return m_ImageFileName;
}

template < class TFloat, unsigned int VImageDimension >
void CImageInformation< TFloat, VImageDimension >::SetTransformationFileName( std::string transformationFileName )
{
#warning  TODO: check if it was set before
  m_TransformationFileName = transformationFileName;
}

template < class TFloat, unsigned int VImageDimension >
std::string CImageInformation< TFloat, VImageDimension >::GetTransformationFileName()
{
  return m_TransformationFileName;
}

template < class TFloat, unsigned int VImageDimension >
void CImageInformation< TFloat, VImageDimension >::SetTimePoint( FloatType timepoint )
{
  m_Timepoint = timepoint;
}

template < class TFloat, unsigned int VImageDimension >
typename CImageInformation< TFloat, VImageDimension >::FloatType
CImageInformation< TFloat, VImageDimension >::GetTimePoint()
{
  return m_Timepoint;
}

template < class TFloat, unsigned int VImageDimension >
void CImageInformation< TFloat, VImageDimension >::SetSubjectId( sid )
{
  m_SubjectId = sid;
}

template < class TFloat, unsigned int VImageDimension >
int CImageInformation< TFloat, VImageDimension >::GetSubjectId()
{
  return m_SubjectId;
}

template < class TFloat, unsigned int VImageDimension >
void CImageInformation< TFloat, VImageDimension >::SetUniqueId( int uid )
{
  m_UniqueId = uid;
}

template < class TFloat, unsigned int VImageDimension >
int CImageInformation< TFloat, VImageDimension >::GetUniqueId()
{
  return m_UniqueId;
}

} // end of namespace

