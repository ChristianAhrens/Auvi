#pragma once

#include <JuceHeader.h>

namespace Auvi
{

class AppConfiguration
{
public:
	class XmlConfigurableElement
	{
	public:
        virtual ~XmlConfigurableElement(){};
        
		virtual std::unique_ptr<XmlElement> createStateXml() = 0;
		virtual bool setStateXml(XmlElement* stateXml) = 0;

		void triggerConfigurationUpdate()
		{
			AppConfiguration::getInstance().triggerListenersUpdate();
		};
	};

	class Listener
	{
	public:
        virtual ~Listener(){};
        
		virtual void performConfigurationDump() = 0;
	};

public:
    enum TagID
    {
        GUI,
        VISU,
        DEVCFG,
		VISUMAP,
		VISUUSEDB,
    };
    static String getTagName(TagID ID)
    {
        switch(ID)
        {
        case GUI:
            return "GraphicalUserInterface";
        case VISU:
            return "Visualizers";
        case DEVCFG:
            return "DEVICESETUP";
		case VISUMAP:
			return "VisualizerMapping";
		case VISUUSEDB:
			return "UseValuesInDB";
		default:
			return "INVALID";
        }
	};

public:
	AppConfiguration(const File &file);
	~AppConfiguration();

	static AppConfiguration& getInstance() noexcept;

	bool isValid();

	void addListener(AppConfiguration::Listener* l);
	void triggerListenersUpdate();

	std::unique_ptr<XmlElement> getConfigState(StringRef tagName);
	bool setConfigState(std::unique_ptr<XmlElement> stateXml);

private:
	bool initializeFromDisk();
	bool exists();
	bool create();
	bool flush();
	void debugPrintXmlTree();

	std::unique_ptr<File>		m_file;
	std::unique_ptr<XmlElement>	m_xml{ nullptr };

	std::vector<Listener*>		m_listeners;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppConfiguration)
};

}
