#include "PyTschirpEditor.h"

//#include "Logger.h"

PyTschirpEditor::PyTschirpEditor() 
{
	editor_.reset(new CodeEditorComponent(document_, nullptr));
	editor_->addKeyListener(this);
	addAndMakeVisible(editor_.get());
	LambdaButtonStrip::TButtonMap buttons = {
		{ "sendToSynth", { "Compile and send (ALT+RETURN)", [this]() {
		//compileAndSend();
	}}}
	};
	buttons_.setButtonDefinitions(buttons);
	addAndMakeVisible(buttons_);
	addAndMakeVisible(currentError_);
	currentError_.setReadOnly(true);
	document_.addListener(this);
}

PyTschirpEditor::~PyTschirpEditor()
{
	editor_->removeKeyListener(this);
	document_.removeListener(this);
}

void PyTschirpEditor::resized()
{
	Rectangle<int> area(getLocalBounds());
	buttons_.setBounds(area.removeFromTop(60).reduced(20));
	currentError_.setBounds(area.removeFromBottom(area.getHeight() / 3).reduced(20));
	editor_->setBounds(area.withTrimmedLeft(20).withTrimmedRight(20));
}

void PyTschirpEditor::loadDocument(std::string const &document)
{
	editor_->loadContent(document);
}

void PyTschirpEditor::codeDocumentTextInserted(const String& newText, int insertIndex)
{
}

void PyTschirpEditor::codeDocumentTextDeleted(int startIndex, int endIndex)
{
}

bool PyTschirpEditor::keyPressed(const KeyPress& key, Component* originatingComponent)
{
	if (key.getKeyCode() == KeyPress::returnKey && key.getModifiers().isAltDown()) {
		// Alt+Return: Send to Synth
		//compileAndSend();
		return true;
	}
	return false;
}

