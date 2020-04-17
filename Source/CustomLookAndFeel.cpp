#include "CustomLookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel()
{
	setColour(ColourScheme::windowBackground, Colours::darkgrey);
	setColour(ColourScheme::widgetBackground, Colours::grey);
	setColour(ColourScheme::menuBackground, Colours::darkgrey);
	setColour(ColourScheme::outline, Colours::lightgrey);
	setColour(ColourScheme::defaultText, Colours::lightgrey);
	setColour(ColourScheme::defaultFill, Colours::grey);
	setColour(ColourScheme::highlightedText, Colours::white);
	setColour(ColourScheme::highlightedFill, Colours::grey);
	setColour(ColourScheme::menuText, Colours::dimgrey);

	setColour(ResizableWindow::ColourIds::backgroundColourId, Colours::darkgrey);

	setColour(TextEditor::ColourIds::backgroundColourId, Colours::darkgrey);
	setColour(TextEditor::ColourIds::textColourId, Colours::dimgrey);
	setColour(TextEditor::ColourIds::highlightColourId, Colours::white);
	setColour(TextEditor::ColourIds::highlightedTextColourId, Colours::lightgrey);
	setColour(TextEditor::ColourIds::outlineColourId, Colours::lightgrey);
	setColour(TextEditor::ColourIds::focusedOutlineColourId, Colours::lightgrey);
	setColour(TextEditor::ColourIds::shadowColourId, Colours::grey);

	setColour(ComboBox::ColourIds::arrowColourId, Colours::lightgrey);
	setColour(ComboBox::ColourIds::backgroundColourId, Colours::darkgrey);
	setColour(ComboBox::ColourIds::buttonColourId, Colours::grey);
	setColour(ComboBox::ColourIds::focusedOutlineColourId, Colours::darkgrey);
	setColour(ComboBox::ColourIds::outlineColourId, Colours::lightgrey);
	setColour(ComboBox::ColourIds::textColourId, Colours::lightgrey);
    
    setColour(PopupMenu::ColourIds::backgroundColourId, Colours::darkgrey);
    setColour(PopupMenu::ColourIds::textColourId, Colours::lightgrey);
    setColour(PopupMenu::ColourIds::headerTextColourId, Colours::lightgrey);
    setColour(PopupMenu::ColourIds::highlightedBackgroundColourId, Colours::white);
    setColour(PopupMenu::ColourIds::highlightedTextColourId, Colours::lightgrey);

	setColour(TextButton::ColourIds::buttonColourId, Colours::grey);
	setColour(TextButton::ColourIds::buttonOnColourId, Colours::lightgrey);
	setColour(TextButton::ColourIds::textColourOffId, Colours::lightgrey);
	setColour(TextButton::ColourIds::textColourOnId, Colours::dimgrey);

	setColour(DrawableButton::ColourIds::backgroundColourId, Colours::grey);
	setColour(DrawableButton::ColourIds::backgroundOnColourId, Colours::lightgrey);
	setColour(DrawableButton::ColourIds::textColourId, Colours::lightgrey);
	setColour(DrawableButton::ColourIds::textColourOnId, Colours::dimgrey);
}

CustomLookAndFeel::~CustomLookAndFeel()
{
}
