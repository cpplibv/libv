// File:   resource.hpp Author: Vader Created on 2016. szeptember 2., 13:51

#pragma once

// libv
#include <libv/view.hpp>
// std
#include <memory>

// -------------------------------------------------------------------------------------------------

namespace libv {
namespace ui {

template <typename T>
using ResourceView = libv::view_ptr<T>;

class File;

class UIFont2D;
class UIShader;
class UIProgram;
class UITexture2D;

//using File_view = std::shared_ptr<File>;
using File_view = ResourceView<File>;

using UIFont2D_view = ResourceView<UIFont2D>;
using UIShader_view = ResourceView<UIShader>;
using UIProgram_view = ResourceView<UIProgram>;
using UITexture2D_view = ResourceView<UITexture2D>;

} // namespace ui
} // namespace libv
