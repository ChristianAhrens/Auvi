#include "AppConfiguration.h"

namespace Auvi
{

AppConfiguration::AppConfiguration(const File& file)
	: m_file(std::make_unique<File>(file))
{
	if (!exists() && !create())
		jassertfalse;

	initializeFromDisk();
}

AppConfiguration::~AppConfiguration()
{
}

bool AppConfiguration::exists()
{
	return m_file->exists();
}

bool AppConfiguration::create()
{
	if (exists())
		return true;

	if (!m_file->create().wasOk())
		return false;

	return true;
}

bool AppConfiguration::initializeFromDisk()
{
	m_xml = juce::parseXML(*m_file.get());

	if (m_xml && m_xml->hasTagName(JUCEApplication::getInstance()->getApplicationName()))
	{
		return true;
	}
	else
	{
		m_xml = std::make_unique<XmlElement>(JUCEApplication::getInstance()->getApplicationName());
	}
		
	return false;
}

bool AppConfiguration::isValid()
{
	if (!m_xml)
		return false;

	if (!m_xml->hasTagName(JUCEApplication::getInstance()->getApplicationName()))
		return false;

	XmlElement* guiSectionElement = m_xml->getChildByName(AppConfiguration::TagNames::GUI);
	if (guiSectionElement)
	{
		XmlElement* visuSectionElement = guiSectionElement->getChildByName(AppConfiguration::TagNames::VISU);
		if (!visuSectionElement)
			return false;
	}
	else
		return false;

	//XmlElement* devMgrSectionElement = m_xml->getChildByName(AppConfiguration::TagNames::DEVMGR);
	//if (devMgrSectionElement)
	//{
	//
	//}
	//else
	//	return false;

	return true;
}

bool AppConfiguration::flush()
{
	if (!m_xml)
		return false;

	return m_xml->writeTo(*m_file.get());
}

std::unique_ptr<XmlElement> AppConfiguration::getConfigState(StringRef tagName)
{
	if (m_xml)
	{
		XmlElement *tagNameElement = m_xml->getNextElementWithTagName(tagName);
		if (tagNameElement)
			return std::unique_ptr<XmlElement>(tagNameElement);
		else
			return nullptr;
	}
	else
		return nullptr;
}

bool AppConfiguration::setConfigState(std::unique_ptr<XmlElement> stateXml)
{
	if (stateXml && m_xml)
	{
		XmlElement* memcpy = new XmlElement(*stateXml);
		m_xml->addChildElement(memcpy);
		return true;
	}
		
	return false;
}

}