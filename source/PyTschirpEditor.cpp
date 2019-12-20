#include "PyTschirpEditor.h"

#include "StreamLogger.h"
#include "MidiController.h"

#include <pybind11/embed.h>
#include <memory>

namespace py = pybind11;

PyTschirpEditor::PyTschirpEditor(PyStdErrOutStreamRedirect &standardOuts) : standardOuts_(standardOuts), buttons_(201, LambdaButtonStrip::Direction::Horizontal), grabbedFocus_(false)
{
	initPython();

	editor_ = std::make_unique<CodeEditorComponent>(document_, nullptr);
	addAndMakeVisible(editor_.get());
	LambdaButtonStrip::TButtonMap buttons = {
		{ "load", { 0, "Open (CTRL-O)", [this]() {
			loadDocument();
			editor_->grabKeyboardFocus();
		}, 0x4F /* O */, ModifierKeys::ctrlModifier}},
		{ "save", { 1, "Save (CTRL-S)", [this]() {
			saveDocument();
		}, 0x53 /* S */, ModifierKeys::ctrlModifier}},
		{ "saveAs", { 2, "Save as (CTRL-A)", [this]() {
			saveAsDocument();
		}, 0x41 /* A */, ModifierKeys::ctrlModifier}},
		{ "run", { 3, "Run All (CTRL-ENTER)", [this]() {
			executeDocument();
		}, 0x0D /* ENTER */, ModifierKeys::ctrlModifier}},
		{ "runSel", { 4, "Run Sel (ALT-ENTER)", [this]() {
			executeSelection();
		}, 0x0D /* ENTER */, ModifierKeys::altModifier}},
		{ "about", { 5, "About", [this]() {
			aboutBox();
		}, -1, 0}},
		{ "close", { 6, "Close (CTRL-W)", []() {
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

	addAndMakeVisible(logView_);

	// Setup hot keys
	commandManager_.registerAllCommandsForTarget(&buttons_);
	addKeyListener(commandManager_.getKeyMappings());
	editor_->setCommandManager(&commandManager_);

	editor_->setWantsKeyboardFocus(true);
	startTimer(100);
}

PyTschirpEditor::~PyTschirpEditor()
{
	document_.removeListener(this);
}

void PyTschirpEditor::resized()
{
	Rectangle<int> area(getLocalBounds());

	auto left = area.removeFromLeft(area.getWidth() / 2);
	auto right = area;

	logView_.setBounds(right.removeFromBottom(area.getHeight() * 1 / 3));

	buttons_.setBounds(left.removeFromTop(60).reduced(20));
	helpText_.setBounds(left.removeFromTop(60).withTrimmedLeft(20).withTrimmedRight(20));

	auto outputArea = right;

	auto stdErr = outputArea.removeFromBottom(outputArea.getHeight() / 2);
	stdErrLabel_.setBounds(stdErr.removeFromTop(20));
	currentError_.setBounds(stdErr);

	auto stdOut = outputArea;
	stdOutLabel_.setBounds(stdOut.removeFromTop(20));
	currentStdout_.setBounds(stdOut);

	editor_->setBounds(left.reduced(20));
}

void PyTschirpEditor::loadDocument(std::string const &document)
{
	editor_->loadContent(document);
}

void PyTschirpEditor::loadDocument()
{
	File defaultExampleLocation = File::getSpecialLocation(File::userDocumentsDirectory).getChildFile("PyTschirperExamples");
	FileChooser chooser("Please select the python file to load...",
		defaultExampleLocation,
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

void PyTschirpEditor::aboutBox()
{
	String message = "This software is copyright 2019 by Christof Ruch\n"
		"Released under dual license, by default under AGPL-3.0, but an MIT licensed version is available on request by the author\n"
		"\n"
		"This software is provided 'as-is,' without any express or implied warranty.In no event shall the author be held liable for any damages arising from the use of this software.\n"
		"\n"
		"Other licenses:\n"
		"This software is build using JUCE, who might want to track your IP address. See https://github.com/WeAreROLI/JUCE/blob/develop/LICENSE.md for details.\n"
		"We also use pybind11, which is Copyright (c) 2016 Wenzel Jakob <wenzel.jakob@epfl.ch>, All rights reserved. See https://github.com/pybind/pybind11.\n"
		"The boost library is used for parts of this software, see https://www.boost.org/.\n"
		"The installer provided also contains the Microsoft Visual Studio 2017 Redistributable Package.\n"
		;
	AlertWindow::showMessageBox(AlertWindow::InfoIcon, "About", message, "Close");
}

void PyTschirpEditor::codeDocumentTextInserted(const String& newText, int insertIndex)
{
}

void PyTschirpEditor::codeDocumentTextDeleted(int startIndex, int endIndex)
{
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

		auto p = pytschirp.attr("midiControllerInstance")();
		auto *midiController = p.cast<midikraft::MidiController *>();
		midiController->setMidiLogFunction([this](MidiMessage const &message, String const &source, bool isOut) {
			logView_.addMessageToList(message, source, isOut);
		});
	}
	catch (std::exception &e) {
		jassert(false);
		StreamLogger::instance() << e.what() << std::endl;
	}
}

void PyTschirpEditor::executeDocument()
{
	String pythonCode = document_.getAllContent();
	executeString(pythonCode);
}

void PyTschirpEditor::executeSelection()
{
	String selectedText = document_.getTextBetween(editor_->getSelectionStart(), editor_->getSelectionEnd());
	executeString(selectedText);
}

void PyTschirpEditor::executeString(String const &string) {
	try {
		standardOuts_.clear();
		py::exec(string.toStdString(), py::globals());

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
