#pragma once

#include <pybind11/pybind11.h>

namespace pms_utils::bindings::python::atom {

void _register(pybind11::module &_module);

}