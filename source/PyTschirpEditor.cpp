#include "PyTschirpEditor.h"

//#include "Logger.h"

#include <pybind11/embed.h>
#include <memory>

namespace py = pybind11;

PyTschirpEditor::PyTschirpEditor(PyStdErrOutStreamRedirect &standardOuts) : standardOuts_(standardOuts), buttons_(201, LambdaButtonStrip::Direction::Horizontal)
{
	editor_ = std::make_unique<CodeEditorComponent>(document_, nullptr);
	editor_->addKeyListener(this);
	addAndMakeVisible(editor_.get());
	LambdaButtonStrip::TButtonMap buttons = {
		{ "load", { 0, "Load...", [this]() {
			loadDocument();
		}}},
		{ "save", { 1, "Save...", [this]() {
			saveDocument();
		}}},
		{ "saveAs", { 2, "Save as...", []() {}}},
		{ "run", { 3, "Run (ALT+RETURN)", [this]() {
		executeDocument();
		}}},
		{ "about", { 4, "About", []() {} }},
		{ "close", { 5, "Close", []() {
			JUCEApplicationBase::quit();
		} }}
	};
	buttons_.setButtonDefinitions(buttons);
	addAndMakeVisible(buttons_);
	addAndMakeVisible(currentError_);
	addAndMakeVisible(currentStdout_);
	currentError_.setReadOnly(true);
	document_.addListener(this);

	// Setup hot keys
	commandManager_.registerAllCommandsForTarget(&buttons_);
	auto keyMap = commandManager_.getKeyMappings();
	keyMap->addKeyPress(201, KeyPress(79, ModifierKeys::ctrlModifier, 'o'));
	addKeyListener(commandManager_.getKeyMappings());
	editor_->setCommandManager(&commandManager_);
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

void PyTschirpEditor::loadDocument()
{
	FileChooser chooser("Please select the python file to load...",
		File::getSpecialLocation(File::userHomeDirectory),
		"*.py");

	if (chooser.browseForFileToOpen())
	{
		File pythonFile(chooser.getResult());
		if (!pythonFile.existsAsFile()) {
			return;
		}
		currentFilePath_ = pythonFile.getFullPathName();
		auto fileText = pythonFile.loadFileAsString();
		editor_->loadContent(fileText);
	}
}

void PyTschirpEditor::saveDocument()
{
	if (currentFilePath_.isNotEmpty()) {
		File pythonFile(currentFilePath_);
		if (pythonFile.existsAsFile() && pythonFile.hasWriteAccess()) {
			pythonFile.deleteFile();
		}
		FileOutputStream out(pythonFile);
		if (out.openedOk()) {
			out.writeText(document_.getAllContent(), false, false, nullptr);
		}
	}
	else {
		saveAsDocument();
	}
}

void PyTschirpEditor::saveAsDocument()
{

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

juce::ApplicationCommandTarget* PyTschirpEditor::getNextCommandTarget()
{
	// Delegate to the lambda button strip
	return &buttons_;
}

void PyTschirpEditor::getAllCommands(Array<CommandID>& commands)
{
	// Editor itself has no commands, this is only used to delegate commands the CodeEditor does not handle to the lambda button strip
}

void PyTschirpEditor::getCommandInfo(CommandID commandID, ApplicationCommandInfo& result)
{
	// None, as no commands are registered here
}

bool PyTschirpEditor::perform(const InvocationInfo& info)
{
	// Always false, as no commands are registered here
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

