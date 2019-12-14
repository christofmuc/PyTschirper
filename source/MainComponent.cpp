/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : buttons_(LambdaButtonStrip::Horizontal), editor_(pyOutputRedirect_)
{
	addAndMakeVisible(editor_);
	addAndMakeVisible(buttons_);
	addAndMakeVisible(logView_);

	buttons_.setButtonDefinitions({
		{ "close", { "Close", []() { JUCEApplicationBase::quit();  } } }
	});

    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 800);
}

MainComponent::~MainComponent()
{
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
	auto area = getLocalBounds();	
	buttons_.setBounds(area.removeFromBottom(80));
	editor_.setBounds(area.removeFromTop(500));
	logView_.setBounds(area);
}
