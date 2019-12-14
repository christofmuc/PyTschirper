#include "PyTschirpEditor.h"

#include "StreamLogger.h"

#include <pybind11/embed.h>
#include <memory>

namespace py = pybind11;

PyTschirpEditor::PyTschirpEditor(PyStdErrOutStreamRedirect &standardOuts) : standardOuts_(standardOuts), buttons_(201, LambdaButtonStrip::Direction::Horizontal), grabbedFocus_(false)
{
	initPython();

	editor_ = std::make_unique<CodeEditorComponent>(document_, nullptr);
	editor_->addKeyListener(this);
	addAndMakeVisible(editor_.get());
	LambdaButtonStrip::TButtonMap buttons = {
		{ "load", { 0, "Open (CTRL-O)", [this]() {
			loadDocument();
		}, 0x4F /* O */, ModifierKeys::ctrlModifier}},
		{ "save", { 1, "Save (CTRL-S)", [this]() {
			saveDocument();
		}, 0x53 /* S */, ModifierKeys::ctrlModifier}},
		{ "saveAs", { 2, "Save as (CTRL-A)", [this]() {
			saveAsDocument();
		}, 0x41 /* A */, ModifierKeys::ctrlModifier}},
		{ "run", { 3, "Run (CTRL-ENTER)", [this]() {
			executeDocument();
		}, 0x0D /* ENTER */, ModifierKeys::ctrlModifier}},
		{ "about", { 4, "About", []() {}, -1, 0}},
		{ "close", { 5, "Close (CTRL-W)", []() {
			JUCEApplicationBase::quit();
		}, 0x57 /* W */, ModifierKeys::ctrlModifier}}
	};
	buttons_.setButtonDefinitions(buttons);
	addAndMakeVisible(buttons_);
	addAndMakeVisible(helpText_);
	addAndMakeVisible(stdErrLabel_);
	addAndMakeVisible(currentError_);
	addAndMakeVisible(stdOutLabel_);
	addAndMakeVisible(currentStdout_);
	stdErrLabel_.setText("stderr:", dontSendNotification);
	currentError_.setReadOnly(true);
	currentError_.setMultiLine(true, false);
	stdOutLabel_.setText("stdout:", dontSendNotification);
	currentStdout_.setReadOnly(true);
	currentStdout_.setMultiLine(true, false);
	document_.addListener(this);

	helpText_.setReadOnly(true);
	helpText_.setMultiLine(true, false);
	helpText_.setText("Welcome to the PyTschirp demo program. Below is a python script editor which already imported pytschirp.\n"
		"\n"
		"Type some commands like 'r = Rev2()' and press CTRL-ENTER to execute the script");

	// Setup hot keys
	commandManager_.registerAllCommandsForTarget(&buttons_);
	addKeyListener(commandManager_.getKeyMappings());
	editor_->setCommandManager(&commandManager_);

	editor_->setWantsKeyboardFocus(true);
	startTimer(100);
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
	helpText_.setBounds(area.removeFromTop(60).withTrimmedLeft(20).withTrimmedRight(20));

	auto outputArea = area.removeFromBottom(area.getHeight() / 3).reduced(20);

	auto stdErr = outputArea.removeFromLeft(outputArea.getWidth() / 2).withTrimmedRight(10);
	stdErrLabel_.setBounds(stdErr.removeFromTop(20));
	currentError_.setBounds(stdErr);

	auto stdOut = outputArea.withTrimmedLeft(10);
	stdOutLabel_.setBounds(stdOut.removeFromTop(20));
	currentStdout_.setBounds(stdOut);

	editor_->setBounds(area.reduced(20));
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
	FileChooser chooser("Save as...",
		File::getSpecialLocation(File::userHomeDirectory),
		"*.py");

	if (chooser.browseForFileToSave(true))
	{
		File chosenFile(chooser.getResult());
		currentFilePath_ = chosenFile.getFullPathName();
		saveDocument();
	}
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

void PyTschirpEditor::timerCallback()
{
	if (editor_->isShowing()) {
		editor_->grabKeyboardFocus();
		grabbedFocus_ = true;
		stopTimer();
	}
}

void PyTschirpEditor::initPython()
{
	try {
		auto pytschirp = py::module::import("pytschirp");
		py::globals()["Rev2"] = pytschirp.attr("Rev2");
		py::globals()["Rev2Patch"] = pytschirp.attr("Rev2Patch");
	}
	catch (std::exception &e) {
		jassert(false);
		StreamLogger::instance() << e.what() << std::endl;
	}
}

void PyTschirpEditor::executeDocument()
{
	String pythonCode = document_.getAllContent();
	try {
		standardOuts_.clear();
		py::exec(pythonCode.toStdString(), py::globals());

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

