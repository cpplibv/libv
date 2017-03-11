// File:   core.hpp Author: Vader Created on 2015. december 14., 3:45

#pragma once

#include <functional>

namespace libv {
namespace frame {

// -------------------------------------------------------------------------------------------------

struct Frame;

void coreExec(const std::function<void()>&);
void coreExec(std::function<void()>&&);

void activateFrame(Frame* frame);
void deactivateFrame(Frame* frame);
void registerFrame(Frame* frame);
void unregisterFrame(Frame* frame);

// -------------------------------------------------------------------------------------------------

} // namespace frame
} // namespace libv