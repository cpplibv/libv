// File: Importer.hpp, Created on 2015. január 17. 7:33, Author: Vader

#pragma once

// std
#include <optional>
#include <string>
#include <experimental/filesystem>
namespace std { namespace filesystem = experimental::filesystem; } /*FILESYSTEM_SUPPORT*/


namespace libv {
namespace vm4 {

// -------------------------------------------------------------------------------------------------

class Model;
class ImportResult;

//ImportResult import_ec(const char* data, size_t size);
//ImportResult import_ec(const std::filesystem::path& filePath);
//Model import(const char* data, size_t size);
//Model import(const std::filesystem::path& filePath);

std::optional<Model> import(const std::string& filePath);

// -------------------------------------------------------------------------------------------------

} // namespace vm4
} // namespace libv
