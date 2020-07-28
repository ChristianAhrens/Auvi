#pragma once

#include <JuceHeader.h>

#include "../submodules/JUCE-AppBasics/Source/AppConfigurationBase.h"

namespace Auvi
{

class AppConfiguration : public JUCEAppBasics::AppConfigurationBase
{

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
	~AppConfiguration() override;

	bool isValid() override;

private:

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppConfiguration)
};

}
