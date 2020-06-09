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

AppConfiguration& AppConfiguration::getInstance() noexcept
{
	static AppConfiguration s_config{	File::getSpecialLocation(File::SpecialLocationType::userApplicationDataDirectory).getFullPathName() + "/"
										+ JUCEApplication::getInstance()->getApplicationName() + "/"
										+ JUCEApplication::getInstance()->getApplicationName() + ".config" };

	return s_config;
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

bool AppConfiguration::flush()
{
	if (!m_xml)
		return false;

#ifdef DEBUG
	debugPrintXmlTree();
#endif

	if (!m_xml->writeTo(*m_file.get()))
		jassertfalse;

	return true;
}

void AppConfiguration::addListener(AppConfiguration::Listener* l)
{
	m_listeners.push_back(l);
}

void AppConfiguration::triggerListenersUpdate()
{
	for (auto l : m_listeners)
		l->performConfigurationDump();

	flush();
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

void AppConfiguration::debugPrintXmlTree()
{
	DBG("#####################################");
	DBG("###### saving xml tree to file ######");
	DBG("#####################################");
	DBG(m_xml->getTagName());
	forEachXmlChildElement(*m_xml, childElement)
	{
		auto childDbgString = String("- " + childElement->getTagName());
		for (int i = 0; i < childElement->getNumAttributes(); ++i)
			childDbgString += " " + childElement->getAttributeName(i) + "=" + childElement->getAttributeValue(i);
		DBG(childDbgString);

		forEachXmlChildElement(*childElement, childchildElement)
		{
			auto childchildDbgString = String("-- " + childchildElement->getTagName());
			for (int i = 0; i < childchildElement->getNumAttributes(); ++i)
				childchildDbgString += " " + childchildElement->getAttributeName(i) + "=" + childchildElement->getAttributeValue(i);
			DBG(childchildDbgString);

			forEachXmlChildElement(*childchildElement, childchildchildElement)
			{
				auto childchildchildDbgString = String("--- " + childchildchildElement->getTagName());
				for (int i = 0; i < childchildchildElement->getNumAttributes(); ++i)
					childchildchildDbgString += " " + childchildchildElement->getAttributeName(i) + "=" + childchildchildElement->getAttributeValue(i);
				DBG(childchildchildDbgString);

				forEachXmlChildElement(*childchildchildElement, childchildchildchildElement)
				{
					auto childchildchildchildDbgString = String("---- " + childchildchildchildElement->getTagName());
					for (int i = 0; i < childchildchildchildElement->getNumAttributes(); ++i)
						childchildchildchildDbgString += " " + childchildchildchildElement->getAttributeName(i) + "=" + childchildchildchildElement->getAttributeValue(i);
					DBG(childchildchildchildDbgString);

					forEachXmlChildElement(*childchildchildchildElement, childchildchildchildchildElement)
					{
						auto childchildchildchildchildDbgString = String("----- " + childchildchildchildchildElement->getTagName());
						for (int i = 0; i < childchildchildchildchildElement->getNumAttributes(); ++i)
							childchildchildchildchildDbgString += " " + childchildchildchildchildElement->getAttributeName(i) + "=" + childchildchildchildchildElement->getAttributeValue(i) + " value=" + childchildchildchildchildElement->getAllSubText();
						DBG(childchildchildchildchildDbgString);
					}
				}
			}
		}
	}
	DBG("#####################################\n");
}

}
