/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "PythonUtils.h"

namespace py = pybind11;

PyStdErrOutStreamRedirect::PyStdErrOutStreamRedirect()
{
	auto sysm = py::module::import("sys");
	_stdout = sysm.attr("stdout");
	_stderr = sysm.attr("stderr");
	auto stringio = py::module::import("io").attr("StringIO");
	_stdout_buffer = stringio();  // Other file like object can be used here as well, such as objects created by pybind11
	_stderr_buffer = stringio();
	sysm.attr("stdout") = _stdout_buffer;
	sysm.attr("stderr") = _stderr_buffer;
}

PyStdErrOutStreamRedirect::~PyStdErrOutStreamRedirect()
{
	auto sysm = py::module::import("sys");
	sysm.attr("stdout") = _stdout;
	sysm.attr("stderr") = _stderr;
}

std::string PyStdErrOutStreamRedirect::stdoutString()
{
	_stdout_buffer.attr("seek")(0);
	return py::str(_stdout_buffer.attr("read")());
}

std::string PyStdErrOutStreamRedirect::stderrString()
{
	_stderr_buffer.attr("seek")(0);
	return py::str(_stderr_buffer.attr("read")());
}

