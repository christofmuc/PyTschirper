/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

#include "LambdaButtonStrip.h"
#include "PythonUtils.h"

class PyTschirpEditor : public Component,
	public ApplicationCommandTarget,
	private CodeDocument::Listener,
	private KeyListener
{
public:
	PyTschirpEditor(PyStdErrOutStreamRedirect &standardOuts);
	virtual ~PyTschirpEditor();

	virtual void resized() override;
	
	void loadDocument(std::string const &document);

	// Code document listener
	virtual void codeDocumentTextInserted(const String& newText, int insertIndex) override;
	virtual void codeDocumentTextDeleted(int startIndex, int endIndex) override;

	// Key listener, for custom shortcuts
	virtual bool keyPressed(const KeyPress& key, Component* originatingComponent) override;

	virtual ApplicationCommandTarget* getNextCommandTarget() override;
	virtual void getAllCommands(Array<CommandID>& commands) override;
	virtual void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) override;
	virtual bool perform(const InvocationInfo& info) override;

private:
	void executeDocument();
	void loadDocument();
	void saveDocument();
	void saveAsDocument();

	PyStdErrOutStreamRedirect &standardOuts_;
	std::unique_ptr<CodeEditorComponent> editor_;
	CodeDocument document_;
	LambdaButtonStrip buttons_;
	TextEditor currentError_, currentStdout_;
	StringArray errors_;

	ApplicationCommandManager commandManager_;
	String currentFilePath_;
};

