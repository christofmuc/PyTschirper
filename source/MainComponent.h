/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

#include "PythonUtils.h"
#include "PyTschirpEditor.h"

#include <pybind11/embed.h>

namespace py = pybind11;

class MainComponent   : public Component
{
public:
    MainComponent();
    ~MainComponent();

    void resized() override;

private:
	py::scoped_interpreter guard_;
	PyStdErrOutStreamRedirect pyOutputRedirect_;	
	PyTschirpEditor editor_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
