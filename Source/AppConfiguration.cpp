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

	XmlElement* devMgrSectionElement = m_xml->getChildByName(AppConfiguration::TagNames::DEVCFG);
	if (devMgrSectionElement)
	{
		//todo: further parsing neccessary?
	}
	else
		return false;

	return true;
}

bool AppConfiguration::flush()
{
	if (!m_xml)
		return false;

	//DBG("#####################################");
	//DBG(m_xml->getTagName());
	//forEachXmlChildElement(*m_xml, childElement)
	//{
	//	DBG("- " + childElement->getTagName());
	//	forEachXmlChildElement(*childElement, childchildElement)
	//	{
	//		DBG("-- " + childchildElement->getTagName());
	//		forEachXmlChildElement(*childchildElement, childchildchildElement)
	//		{
	//			DBG("--- " + childchildchildElement->getTagName());
	//		}
	//	}
	//}
	//DBG("#####################################");

	if (!m_xml->writeTo(*m_file.get()))
		jassertfalse;

	return true;
}

std::unique_ptr<XmlElement> AppConfiguration::getConfigState(StringRef tagName)
{
	if (m_xml)
	{
		XmlElement *tagNameElement = m_xml->getChildByName(tagName);
		if (tagNameElement)
			return std::make_unique<XmlElement>(*tagNameElement);
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
		XmlElement *existingChildElement = m_xml->getChildByName(stateXml->getTagName());

		if(!existingChildElement)
			m_xml->addChildElement(new XmlElement(*stateXml));
		else
			m_xml->replaceChildElement(existingChildElement, new XmlElement(*stateXml));

		return true;
	}
		
	return false;
}

}