/*
*
*  Copyright 2011 by the CALATK development team
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

#ifndef CALATK_MACROS_H
#define CALATK_MACROS_H

/**
  * Defines general purpose macros for CALATK
  *
  */

/** Set built-in type.  Creates member Set"name"() (e.g., SetVisibility()); */
#define SetMacro(name,type)                     \
  virtual void Set##name (const type _arg)      \
  {                                             \
    this->m_##name = _arg;                                              \
    this->m_ExternallySet##name = true;                                 \
  }                                                                     \
  virtual void SetToDefault##name ()                                    \
  {                                                                     \
    this->m_##name = this->Default##name;                               \
    this->m_ExternallySet##name = false;                                \
  }                                                                     \
  virtual void SetJSON##name(const type _arg)                           \
  {                                                                     \
    if ( this->GetExternallySet##name() )                               \
      {                                                                 \
      std::cout << "Variable " << #name << " appears to have been set before to " << this->m_##name << ". Ignoring new value." << std::endl; \
      }                                                                 \
    else                                                                \
      {                                                                 \
      this->m_##name = _arg;                                            \
      }                                                                 \
  }                                                                     \

#define SetJSONMacro(name,type)                                         \
  virtual void SetToDefault##name ()                                    \
  {                                                                     \
    this->m_##name = this->Default##name;                               \
    this->m_ExternallySet##name = false;                                \
  }                                                                     \
  virtual void SetJSON##name(const type _arg)                           \
  {                                                                     \
    if ( this->GetExternallySet##name() )                               \
      {                                                                 \
      std::cout << "Variable " << #name << " appears to have been set before to " << this->m_##name << ". Ignoring new value." << std::endl; \
      }                                                                 \
    else                                                                \
      {                                                                 \
      this->m_##name = _arg;                                            \
      }                                                                 \
  }


/** Get built-in type.  Creates member Get"name"() (e.g., GetVisibility()); */
#define GetMacro(name,type) \
  virtual type Get##name () \
  {                         \
    return this->m_##name;  \
  }                                \
  virtual type GetDefault##name () \
  {                                \
    return this->Default##name;    \
  }                                \
  virtual type GetExternalOrDefault##name () \
  {                                          \
  if ( this->m_ExternallySet##name )         \
    {                                        \
    return this->m_##name;                   \
    }                                        \
  else                                       \
    {                                        \
  return this->Default##name;                \
    }                                        \
  }                                          \
  virtual bool GetExternallySet##name () \
  {                                      \
  return this->m_ExternallySet##name;    \
  }                                      \

/** Master macro to deal with setting parameters through JSON in the classes */

#define SetJSONFromKeyDouble( currentConfigurationIn, currentConfigurationOut, VariableName ) \
  this->SetJSON##VariableName( this->m_CombinedJSONConfig->GetFromKey( currentConfigurationIn, #VariableName, this->GetExternalOrDefault##VariableName() ).asDouble() ); \
  currentConfigurationOut[ #VariableName ] = currentConfigurationIn[ #VariableName ]; \

#define SetJSONFromKeyUInt( currentConfigurationIn, currentConfigurationOut, VariableName ) \
  this->SetJSON##VariableName( this->m_CombinedJSONConfig->GetFromKey( currentConfigurationIn, #VariableName, this->GetExternalOrDefault##VariableName() ).asUInt() ); \
  currentConfigurationOut[ #VariableName ] = currentConfigurationIn[ #VariableName ]; \

#define SetJSONFromKeyInt( currentConfigurationIn, currentConfigurationOut, VariableName ) \
  this->SetJSON##VariableName( this->m_CombinedJSONConfig->GetFromKey( currentConfigurationIn, #VariableName, this->GetExternalOrDefault##VariableName() ).asInt() ); \
  currentConfigurationOut[ #VariableName ] = currentConfigurationIn[ #VariableName ]; \

#define SetJSONFromKeyBool( currentConfigurationIn, currentConfigurationOut, VariableName ) \
  this->SetJSON##VariableName( this->m_CombinedJSONConfig->GetFromKey( currentConfigurationIn, #VariableName, this->GetExternalOrDefault##VariableName() ).asBool() ); \
  currentConfigurationOut[ #VariableName ] = currentConfigurationIn[ #VariableName ]; \

#define SetJSONFromKeyString( currentConfigurationIn, currentConfigurationOut, VariableName ) \
  this->SetJSON##VariableName( this->m_CombinedJSONConfig->GetFromKey( currentConfigurationIn, #VariableName, this->GetExternalOrDefault##VariableName() ).asString() ); \
  currentConfigurationOut[ #VariableName ] = currentConfigurationIn[ #VariableName ]; \

#define SetJSONFromKeyVector( currentConfigurationIn, currentConfigurationOut, VariableName ) \
  this->SetJSON##VariableName( this->m_CombinedJSONConfig->GetFromKeyAsVector( currentConfigurationIn, #VariableName, this->GetExternalOrDefault##VariableName() ) ); \
  currentConfigurationOut[ #VariableName ] = currentConfigurationIn[ #VariableName ]; \

#define SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, VariableName, HelpString) \
  this->m_CombinedJSONConfig->SetHelpForKey( currentConfigurationIn, #VariableName, HelpString ); \
  this->m_CleanedJSONConfig->SetHelpForKey( currentConfigurationOut, #VariableName, HelpString ); \

#define SetJSONHelpForRootKey( VariableName, HelpString ) \
  this->m_CombinedJSONConfig->SetHelpForKey( #VariableName, HelpString ); \
  this->m_CleanedJSONConfig->SetHelpForKey( #VariableName, HelpString ); \

#define SetJSONHelpForKeyBefore( currentConfigurationIn, currentConfigurationOut, VariableName, HelpString) \
  this->m_CombinedJSONConfig->SetHelpForKey( currentConfigurationIn, #VariableName, HelpString, Json::commentBefore ); \
  this->m_CleanedJSONConfig->SetHelpForKey( currentConfigurationOut, #VariableName, HelpString, Json::commentBefore ); \

#define SetJSONHelpForRootKeyBefore( VariableName, HelpString ) \
  this->m_CombinedJSONConfig->SetHelpForKey( #VariableName, HelpString, Json::commentBefore ); \
  this->m_CleanedJSONConfig->SetHelpForKey( #VariableName, HelpString, Json::commentBefore ); \

/** multi-dimensional macro */
#define DoItND(type,ImageDimension,argc,argv) \
  switch ( ImageDimension ) \
  {    \
  case 1: \
    return DoIt<type,1>( argc, argv ); \
    break; \
  case 2: \
    return DoIt<type,2>( argc, argv ); \
    break; \
  case 3: \
    return DoIt<type,3>( argc, argv ); \
    break; \
  default: \
    std::cerr << "Unsupported image dimension = " << ImageDimension << std::endl; \
  } \

/** multi-dimensional datatype macro */
#define DoItNDWithType(sFloatingPointType,ImageDimension,argc,argv) \
    std::cout << "Using command-line specified floating point type" << std::endl; \
    std::for_each( sFloatingPointType.begin(), sFloatingPointType.end(), ::tolower); \
    if ( sFloatingPointType.compare( "float" )==0 ) \
    { \
      std::cout << "Using float datatype for all computations" << std::endl; \
      DoItND( float, uiImageDimension, argc, argv ); \
    } \
    else if ( sFloatingPointType.compare( "double" )==0 ) \
    { \
      std::cout << "Using double datatype for all computations" << std::endl; \
      DoItND( double, uiImageDimension, argc, argv ); \
    } \
    else \
    { \
      std::cerr << "Unsupported requested datatype " << sFloatingPointType << ". ABORT." << std::endl; \
      return EXIT_FAILURE; \
    } \

/** min and max macros */
#ifndef MAX
  #define MAX(x,y) ((x)>(y)?(x):(y))
  #define MIN(x,y) ((x)<(y)?(x):(y))
#endif

/**
 * Testing macros
 */
#define AssertEqual(_v1, _v2, msg)\
  if (_v1 != _v2) {\
    std::cerr << msg << " v1: " << _v1 << " v2: " << _v2 << std::endl;\
    return EXIT_FAILURE;\
  }

#define AssertEqualFloat(_v1, _v2, msg)\
  if (_v1 > _v2 + 0.0001 || _v1 < _v2 - 0.0001) {\
    std::cerr << msg << " v1: " << _v1 << " v2: " << _v2 << std::endl;\
    return EXIT_FAILURE;\
  }

#define AssertTrue(arg, msg)\
  if (!(arg)) {\
    std::cerr << msg << std::endl;\
    return EXIT_FAILURE;\
  }

#endif // CALATK_MACROS_H
