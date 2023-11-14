/*
  ==============================================================================

	DeckGUI.cpp
	Created: 13 Mar 2020 6:44:48pm
	Author:  asikf

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player,
	AudioFormatManager& formatManagerToUse,
	AudioThumbnailCache& cacheToUse
) : player(_player),
waveformDisplay(formatManagerToUse, cacheToUse)
{

	// make visible play, stop, forward and back buttons
	addAndMakeVisible(playButton);
	addAndMakeVisible(stopButton);
	addAndMakeVisible(backButton);
	addAndMakeVisible(forwardButton);

	// make visible sliders
	addAndMakeVisible(volSlider);
	addAndMakeVisible(speedSlider);
	addAndMakeVisible(posSlider);

	// make visible waveform display
	addAndMakeVisible(waveformDisplay);



	// button listeners
	playButton.addListener(this);
	stopButton.addListener(this);
	backButton.addListener(this);
	forwardButton.addListener(this);


	// slider listeners
	volSlider.addListener(this);
	speedSlider.addListener(this);
	posSlider.addListener(this);


	// volume slider settings
	volSlider.setRange(0.0, 1.0);
	volSlider.setValue(0.3);
	volSlider.setNumDecimalPlacesToDisplay(2);


	// speed slider settings
	speedSlider.setRange(0.0, 100.0);
	speedSlider.setNumDecimalPlacesToDisplay(0);
	speedSlider.setValue(10.0);
	posSlider.setRange(0.0, 1.0);
	posSlider.setNumDecimalPlacesToDisplay(2);






	startTimer(500);

	


}

DeckGUI::~DeckGUI()
{
	stopTimer();
}

void DeckGUI::paint(Graphics& g)
{

	double rowH = getHeight() / 8;
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));   // clear the background

	g.setColour(Colours::blanchedalmond);
	g.drawRect(getLocalBounds(), 1);   // draw an outline around the component
	Image background = ImageCache::getFromMemory(BinaryData::dja_jpg, BinaryData::dja_jpgSize);
	g.drawImageAt(background, 0, 0);

	g.setColour(Colours::white);
	g.setFont(14.0f);
	g.drawText("DeckGUI", getLocalBounds(),
		Justification::centred, true); 


	// modify style for sliders
	volSlider.setSliderStyle(Slider::SliderStyle::LinearBar);
	volSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
	speedSlider.setSliderStyle(Slider::SliderStyle::LinearBar);
	speedSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
	posSlider.setSliderStyle(Slider::SliderStyle::LinearBar);
	posSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);

	g.drawText("Volume", 0, rowH, getWidth() / 3, rowH, juce::Justification::centred, true); // Draw volume label
	g.drawText("Speed", getWidth() / 3, rowH, getWidth() / 3, rowH, juce::Justification::centred, true); // Draw speed label
	g.drawText("Position", getWidth() / 3 * 2, rowH, getWidth() / 3, rowH, juce::Justification::centred, true); // Draw position label

	// set colours of buttons
	playButton.setColour(TextButton::buttonColourId, Colours::sienna);
	stopButton.setColour(TextButton::buttonColourId, Colours::red);
	backButton.setColour(TextButton::buttonColourId, Colours::goldenrod);
	forwardButton.setColour(TextButton::buttonColourId, Colours::darkgoldenrod);
	volSlider.setColour(juce::Slider::backgroundColourId, Colours::green);

	// modify look and feel
	getLookAndFeel().setColour(juce::Slider::backgroundColourId, juce::Colours::red);

}

void DeckGUI::resized()
{
	double rowH = getHeight() / 8;

	// positions of buttons
	playButton.setBounds(0, rowH * 7, getWidth() / 4, rowH);
	stopButton.setBounds(getWidth() / 4, rowH * 7, getWidth() / 4, rowH);
	backButton.setBounds(getWidth() / 4 * 2, rowH * 7, getWidth() / 4, rowH);
	forwardButton.setBounds(getWidth() / 4 * 3, rowH * 7, getWidth() / 4, rowH);

	// positions of sliders
	volSlider.setBounds(0, rowH * 2, getWidth() / 3, rowH);
	speedSlider.setBounds(getWidth() / 3, rowH * 2, getWidth() / 3, rowH);
	posSlider.setBounds(getWidth() / 3 * 2, rowH * 2, getWidth() / 3, rowH);


	// position of waveform
	waveformDisplay.setBounds(0, rowH * 5, getWidth(), rowH * 2);


}

void DeckGUI::buttonClicked(Button* button) // operation after button clicked
{
	if (button == &playButton)
	{
		std::cout << "Play button was clicked " << std::endl;
		player->start(); // start the track
	}
	if (button == &stopButton)
	{
		std::cout << "Stop button was clicked " << std::endl;
		player->stop(); // stop/ pause the track

	}
	if (button == &loadButton) // load the track
	{
		FileChooser chooser{ "Select a file..." };
		if (chooser.browseForFileToOpen())
		{
			loadFile(juce::URL{ chooser.getResult() });
			waveformDisplay.loadURL(URL{ chooser.getResult() });

		}
	}


	double add = 0.04;
	if (button == &forwardButton)
	{

		player->setPositionRelative(player->getPositionRelative() + add); // add 0.04 to the current position

	}

	if (button == &backButton)
	{

		player->setPositionRelative(player->getPositionRelative() - add); // deduct 0.04 from the current position

	}
}

void DeckGUI::sliderValueChanged(Slider* slider)
{
	if (slider == &volSlider)
	{
		player->setGain(slider->getValue());  // adjust volume
	}

	if (slider == &speedSlider)
	{
		player->setSpeed(slider->getValue()); // adjust speed
	}

	if (slider == &posSlider)
	{
		player->setPositionRelative(slider->getValue()); // adjust position
	}

}

bool DeckGUI::isInterestedInFileDrag(const StringArray& files)
{
	std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
	return true;
}

void DeckGUI::filesDropped(const StringArray& files, int x, int y)
{
	std::cout << "DeckGUI::filesDropped" << std::endl;
	if (files.size() == 1)
	{
		loadFile(juce::URL{ juce::File{files[0]} });
	}
}

void DeckGUI::timerCallback()
{
	waveformDisplay.setPositionRelative(
		player->getPositionRelative());
}


void DeckGUI::loadFile(juce::URL audioURL) 
{
	DBG("DeckGUI::loadFile called");
	player->loadURL(audioURL);
	waveformDisplay.loadURL(audioURL);
}

