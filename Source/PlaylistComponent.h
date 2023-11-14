/*
  ==============================================================================

    PlaylistComponent.h
    Created: 10 Mar 2022 8:10:26pm
    Author:  asikf

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include "DeckGUI.h"
#include "DJAudioPlayer.h"
#include <algorithm>
#include <string>
#include <fstream>
#include "TrackEntry.h"



//==============================================================================
/*
*/

// initliaze playlist class, inherit from component, tablelist model, button listener and textedit listener
class PlaylistComponent : public Component,
    public TableListBoxModel,
    public Button::Listener,
    public TextEditor::Listener
{
public:
    PlaylistComponent(DeckGUI* _deckGUILEFT, 
        DeckGUI* _deckGUIRIGHT,
        DJAudioPlayer* _player3); // initialize constructor
    ~PlaylistComponent() override; 

    void paint (Graphics&) override;  // initialize paint
    void resized() override; // initliaze resized function

    int getNumRows() override; // initialize getNumRows function
    void  paintRowBackground(Graphics&, 
        int rowNumber, 
        int width, 
        int height, 
        bool rowIsSelected) override; // initialize paintRowBackground function

   void paintCell(Graphics&, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override; // initialize paintCell function
   void buttonClicked(Button* button) override; // initialize button clicked function

private:
    TableListBox tableComponent; // tableComponent object

    std::vector<TrackEntry> tracks; // vector of track entries
    TableListBox Playlist; // playlist object
    DeckGUI* deckGUILEFT; // initialize left deck
    DeckGUI* deckGUIRIGHT; // initialize right deck
    TextButton playlistButton{ "ADD TO PLAYLIST" }; // initialize add to playlist button
    TextButton addToLeft{ "ADD TO LEFT" }; // initialize addtoleft button
    TextButton addToRight{ "ADD TO RIGHT" }; // initialize addtoright button
    TextEditor searchBox; // initialize searchbox
    Label searchLabel; // initialize search label
   
    std::vector<std::string> trackTitles; // initialize vector of track titles
    DJAudioPlayer* player3; // initialize player3 which will be used to process meta data
    String convertLengthToMin(URL audioURL); // initialize function to convert audio length to mins
    
   
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
        void addToPlaylist();

    void loadTrack(DeckGUI* deckGUI); // initialize load track function which will be used to load tracks on the decks
    String convertTime(double seconds); // convert time in seconds to mins:secs format
    void searchPlaylist(juce::String searchQuery); // initualize searchPlaylist function to select rows that have been searched
    int TrackIndex(juce::String searchQuery); // grab the index of the track based on the search query
   
};

