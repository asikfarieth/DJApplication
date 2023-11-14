/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 10 Mar 2022 8:10:26pm
    Author:  asikf

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================

PlaylistComponent::PlaylistComponent(DeckGUI* _deckGUILEFT,
    DeckGUI* _deckGUIRIGHT,
    DJAudioPlayer* _player3
) : deckGUILEFT(_deckGUILEFT),
deckGUIRIGHT(_deckGUIRIGHT),
player3(_player3)
{
    // Make visibile tablecomponent/ playlist
    addAndMakeVisible(Playlist);

    // Make visible the 'Add to playlist' Button
    addAndMakeVisible(playlistButton);

    // Make visible add to left/ add to right buttons
    addAndMakeVisible(addToLeft);
    addAndMakeVisible(addToRight);

    // Make visible searchbox and search label
    addAndMakeVisible(searchBox);
    addAndMakeVisible(searchLabel);

    // Set label of search box to Search
    searchLabel.setText("Search", dontSendNotification);

    // Attach label to searchbox component
    searchLabel.attachToComponent(&searchBox, true);

    // Add headers to the playlist with ID's 1 and 2 respectively
    Playlist.getHeader().addColumn("Tracks", 1, 1);
    Playlist.getHeader().addColumn("Length", 2, 1);
    Playlist.setModel(this);
    


    // Listeners for buttons addToLeft, addToRight and searchbox
    addToLeft.addListener(this);
    addToRight.addListener(this);
    searchBox.addListener(this);

    // Grabs the value of the search box once return key is pressed
    searchBox.onReturnKey = [this] { searchPlaylist(searchBox.getText()); };

    // Listener for playlist button
    playlistButton.addListener(this);
    

    
}

PlaylistComponent::~PlaylistComponent()
{


}

void PlaylistComponent::paint (Graphics& g)
{
    
    double rowH = getHeight() / 8;
    g.fillAll (Colours::rosybrown);   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component


    g.setColour (Colours::white); // set colour to white
    g.setFont (16.0f); // set font size
    g.drawText("Type above and press enter to search playlist", 0, rowH * 7, getWidth(), rowH, juce::Justification::centred, true); // instruction for search box
    playlistButton.setColour(TextButton::buttonColourId, Colours::darkorange); // set colour of add to playlist button
   
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    double rowH = getHeight() / 8;

    // position of playlist and the buttons
    Playlist.setBounds(0, 0, getWidth(), getHeight()/2);
    addToLeft.setBounds(0, rowH * 4, getWidth()/2, rowH);
    addToRight.setBounds(getWidth()/2, rowH * 4, getWidth()/2, rowH);

    // position of headers
    Playlist.getHeader().setColumnWidth(1, 12.8 * getWidth() / 20);
    Playlist.getHeader().setColumnWidth(2, 5 * getWidth() / 20);

    // position of add to playlist button and searchbox
    playlistButton.setBounds(0, rowH * 5, getWidth(), rowH);
    searchBox.setBounds(0, rowH * 6, getWidth(), rowH);
}

// get the number of rows in the vector of tracks
int PlaylistComponent::getNumRows() {

    return tracks.size();
}


// paint the background of each track
void  PlaylistComponent::paintRowBackground(Graphics& g,
    int rowNumber,
    int width,
    int height,
    bool rowIsSelected) {

    if (rowIsSelected) {
        g.fillAll(Colours::orange);

    }
    else {
        g.fillAll(Colours::darkgrey);
    }

}


// paint each cell
void PlaylistComponent::paintCell(Graphics& g,
                                  int rowNumber,
                                  int columnId,
                                  int width,
                                  int height,
                                  bool rowIsSelected
                                 )
{
    if (rowNumber < getNumRows())
    {
        if (columnId == 1)
        {
            g.drawText(tracks[rowNumber].title,
                2,
                0,
                width - 4,
                height,
                Justification::centred,
                true
            );
        }
        if (columnId == 2)
        {
            g.drawText(tracks[rowNumber].length,
                2,
                0,
                width - 4,
                height,
                Justification::centred,
                true
            );
        }
    }
}


// functions to run if playlist buttons are clicked

void PlaylistComponent::buttonClicked(Button* button)
{
    if (button == &playlistButton) // run add to playlist function
    {
       
        addToPlaylist();
        Playlist.updateContent();
    }

    else if (button == &addToLeft) // load track to the left deck
    {
        
        loadTrack(deckGUILEFT);
    }
    else if (button == &addToRight) // load track to the right deck
    {
        
        loadTrack(deckGUIRIGHT);
    }


 
}

// load tracks into the deck
void PlaylistComponent::loadTrack(DeckGUI* deckGUI)
{
    int selectedRowIndex =  Playlist.getSelectedRow(); // grab the index of the selected row
    if (selectedRowIndex != -1) // check if the row is selected
    {
      
        deckGUI->loadFile(tracks[selectedRowIndex].URL); // load the track of the selected deck via deckGUI
    }
    else
    {
        AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, // alert user to select track if there is no row selected
            "ALERT",
            "Please select a track before adding to deck",
            "OK",
            nullptr
        );
    }
}


// add tracks to playlist
void PlaylistComponent::addToPlaylist()
{
   
    FileChooser chooser{ "Select one or more files" }; // initialize file chooser
    if (chooser.browseForMultipleFilesToOpen()) // check if the chooser has multiple files to open
    {
        for (const File& track : chooser.getResults()) // create object of each track with track name and length
        {
            String fileName{ track.getFileNameWithoutExtension() };
                TrackEntry newTrack{ track }; // track entry object
                URL audioURL{ track }; // URL of track
                newTrack.length = convertLengthToMin(audioURL); // convert length to minutes and store it in to newTrack object
                tracks.push_back(newTrack); // push each object into vector of tracks
           
        }
    }
}

// grab the index of the search query

int PlaylistComponent::TrackIndex(juce::String searchQuery)
{
    auto elem = [searchQuery](const TrackEntry& obj) {return obj.title.contains(searchQuery); };
    auto itr = find_if(tracks.begin(), tracks.end(), elem); // iterate through the string to check for searchQuery
    auto index = -1; // initialize index to -1

    if (itr != tracks.end()) 
    {
        index = distance(tracks.begin(), itr); // assign index to the string 
    }

    return index; // return index
}

// search the playlist
void PlaylistComponent::searchPlaylist(juce::String searchQuery)
{

    if (searchQuery == "") // if there is no search query, deselect all the rows
    {
        Playlist.deselectAllRows();

    }
    else // search query present
    {
        int rowIndex = TrackIndex(searchQuery); // grab the index of the row based on search query
        Playlist.selectRow(rowIndex); // select the row
    }
}



String PlaylistComponent::convertLengthToMin(URL audioURL) // convert audio's length to minutes
{
    player3->loadURL(audioURL); // load the URL of the track
    double seconds = player3->getLengthInSeconds();
    String mins{ convertTime(seconds) };
    return mins;
}


String PlaylistComponent::convertTime(double seconds) // convert length into minutes and seconds
{
    
    String mins = std::to_string(int(std::round(seconds)) / 60); // convert into string minutes
    String secs=  std::to_string(int(std::round(seconds)) % 60); // convert into string seconds

    return String{ mins + ":" + secs }; // return the string in the form of mins:seconds
}





