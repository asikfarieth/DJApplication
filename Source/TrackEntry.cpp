#include "TrackEntry.h"
#include <filesystem>


TrackEntry::TrackEntry(juce::File _file) : file(_file), // Track entry constructor7
title(_file.getFileNameWithoutExtension()),
URL(juce::URL{ _file })
{
}

bool TrackEntry::operator==(const juce::String& newTrack) const // Check if the title matches with the track selected
{
    if (title == newTrack) {
        return 1;
    }
    else {
        return 0;
    }
    
}