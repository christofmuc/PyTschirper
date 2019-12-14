/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

#include "LambdaButtonStrip.h"
#include "PythonUtils.h"

class PyTschirpEditor : public Component,
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

private:
	void executeDocument();

	PyStdErrOutStreamRedirect &standardOuts_;
	std::unique_ptr<CodeEditorComponent> editor_;
	CodeDocument document_;
	LambdaButtonStrip buttons_;
	TextEditor currentError_, currentStdout_;
	StringArray errors_;
};


