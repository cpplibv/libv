// Project: libv.vm4imp, File: src/libv/vm4imp/importer.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <optional>
#include <string>


namespace libv {
namespace vm4 {

// -------------------------------------------------------------------------------------------------

class Model;
//class ImportResult;

//ImportResult import_ec(const char* data, size_t size);
//ImportResult import_ec(const std::filesystem::path& filePath);
//Model import(const char* data, size_t size);
//Model import(const std::filesystem::path& filePath);

std::optional<Model> import(const std::string& filePath);

// -------------------------------------------------------------------------------------------------

} // namespace vm4
} // namespace libv
