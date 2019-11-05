/*
  ==============================================================================

    ProcessorData.cpp
    Created: 5 Nov 2019 5:17:20pm
    Author:  Christian Ahrens

  ==============================================================================
*/

#include "AbstractProcessorData.h"

AbstractProcessorData::AbstractProcessorData()
{
    m_type = Invalid;
}

AbstractProcessorData::~AbstractProcessorData()
{

}

AbstractProcessorData::Type AbstractProcessorData::GetDataType()
{
    return m_type;
}
