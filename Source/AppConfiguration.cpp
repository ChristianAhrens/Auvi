#include "AppConfiguration.h"

namespace Auvi
{

AppConfiguration::AppConfiguration(const File& file)
	: JUCEAppBasics::AppConfigurationBase(file)
{

}

AppConfiguration::~AppConfiguration()
{
}

bool AppConfiguration::isValid()
{
	if (!JUCEAppBasics::AppConfigurationBase::isValid())
		return false;

	XmlElement* guiSectionElement = m_xml->getChildByName(AppConfiguration::getTagName(AppConfiguration::TagID::GUI));
	if (guiSectionElement)
	{
		XmlElement* visuSectionElement = guiSectionElement->getChildByName(AppConfiguration::getTagName(AppConfiguration::TagID::VISU));
		if (!visuSectionElement)
			return false;
	}
	else
		return false;

	XmlElement* devMgrSectionElement = m_xml->getChildByName(AppConfiguration::getTagName(AppConfiguration::TagID::DEVCFG));
	if (devMgrSectionElement)
	{
		//todo: further parsing neccessary?
	}
	else
		return false;

	return true;
}

}
