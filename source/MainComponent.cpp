/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : buttons_(LambdaButtonStrip::Horizontal)
{
	addAndMakeVisible(buttons_);

	buttons_.setButtonDefinitions({
		{ "close", { "Close", []() { JUCEApplicationBase::quit();  } } }
	});

    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);
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
}
