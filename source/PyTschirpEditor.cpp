#include "PyTschirpEditor.h"

//#include "Logger.h"

#include <pybind11/embed.h>
#include <memory>

namespace py = pybind11;

PyTschirpEditor::PyTschirpEditor(PyStdErrOutStreamRedirect &standardOuts) : standardOuts_(standardOuts)
{
	editor_ = std::make_unique<CodeEditorComponent>(document_, nullptr);
	editor_->addKeyListener(this);
	addAndMakeVisible(editor_.get());
	LambdaButtonStrip::TButtonMap buttons = {
		{ "sendToSynth", { "Compile and send (ALT+RETURN)", [this]() {
		executeDocument();
	}}}
	};
	buttons_.setButtonDefinitions(buttons);
	addAndMakeVisible(buttons_);
	addAndMakeVisible(currentError_);
	addAndMakeVisible(currentStdout_);
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
	auto outputArea = area.removeFromBottom(area.getHeight() / 3).reduced(20);
	currentError_.setBounds(outputArea.removeFromLeft(outputArea.getWidth()/2));
	currentStdout_.setBounds(outputArea);
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
		// Alt+Return: Immediate execute
		executeDocument();
		return true;
	}
	return false;
}

void PyTschirpEditor::executeDocument()
{
	String pythonCode = document_.getAllContent();
	try {
		standardOuts_.clear();
		py::exec(pythonCode.toStdString());

		if (standardOuts_.stderrString().empty()) {
			currentError_.setText("Success", dontSendNotification);
		}
		else {
			currentError_.setText(standardOuts_.stderrString(), dontSendNotification);
		}
		currentStdout_.setText(standardOuts_.stdoutString(), dontSendNotification);
	}
	catch (std::exception &e) {
		currentError_.setText(e.what(), dontSendNotification);
		currentStdout_.setText(standardOuts_.stdoutString(), dontSendNotification);
	}
}

