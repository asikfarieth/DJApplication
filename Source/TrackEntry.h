
#pragma once
#include <JuceHeader.h>

class TrackEntry // Create track entry objects
{
public:
    TrackEntry(juce::File _file);
    File file;
    URL URL;
    String title;
    String length;
    bool operator==(const juce::String& newTrack) const; // return true if the other object is the same - operator overload
};