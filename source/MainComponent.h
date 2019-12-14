/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

#include "LambdaButtonStrip.h"
#include "MidiLogView.h"

class MainComponent   : public Component
{
public:
    MainComponent();
    ~MainComponent();

    void resized() override;

private:
	MidiLogView logView_;
	LambdaButtonStrip buttons_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
