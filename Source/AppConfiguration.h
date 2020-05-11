#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

namespace Auvi
{

class AppConfiguration
{
public:
	struct TagNames {
		constexpr static const char *GUI = { "GraphicalUserInterface" };
		constexpr static const char *VISU = { "Visualizers" };
		constexpr static const char* DEVCFG = { "DEVICESETUP" };
	};

public:
	AppConfiguration(const File &file 
		= File::getSpecialLocation(File::SpecialLocationType::userApplicationDataDirectory).getFullPathName() + "/" 
		+ JUCEApplication::getInstance()->getApplicationName() + "/" 
		+ JUCEApplication::getInstance()->getApplicationName() + ".config");
	~AppConfiguration();

	bool exists();
	bool create();
	bool initializeFromDisk();
	bool isValid();
	bool flush();

	std::unique_ptr<XmlElement> getConfigState(StringRef tagName);
	bool setConfigState(std::unique_ptr<XmlElement> stateXml);

	//==============================================================================


private:
	std::unique_ptr<File>		m_file;
	std::unique_ptr<XmlElement>	m_xml{ nullptr };

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppConfiguration)
};

}