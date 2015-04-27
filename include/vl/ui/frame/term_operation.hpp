// File: term_operation.hpp, Created on 2015. március 29. 23:22, Author: Vader

#pragma once

#include <cstdint>

namespace vl {
namespace ui {

enum class TermOperation : uint8_t {
	Wait = 0,
	Force = 1,
	Default = 2
};

} //namespace ui
} //namespace vl