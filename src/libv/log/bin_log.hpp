// File: Log.hpp, Created on 2014. december 1. 12:50, Author: Vader

#pragma once

// libv
#include <libv/utility/span.hpp>
// std
#include <cstdint>
#include <utility>
#include <array>
#include <cstring>
#include <string_view>
#include <variant>




namespace libv {
namespace log {

// -------------------------------------------------------------------------------------------------



// -------------------------------------------------------------------------------------------------

} // namespace log
} // namespace libv



// =================================================================================================

// NOTE: Idea based on https://www.youtube.com/watch?v=FyJI4Z6jD4w

char* output;

template <typename... Args>
void serialize(int32_t id, const Args&... args) {
    // ...
    char* os = output; // Important optimization!
    std::memcpy(os, &id, sizeof(id)); os += sizeof(id);
    ((std::memcpy(os, &args, sizeof(args)), os += sizeof(args)), ...);
    output = os; // Important optimization!
}

// -------------------

int32_t gen_id() {
    static int32_t id = 0;
    return id++;
}

struct LogDataNode;

LogDataNode*& log_data_head() {
    static LogDataNode* head = nullptr;
    return head;
}

class LogStatementMetadata;

struct LogDataNode {
    int32_t id = 0;
    const LogDataNode* next;
    const LogStatementMetadata* data;

    LogDataNode(const LogStatementMetadata* data) :
        id{gen_id()},
        next{std::exchange(log_data_head(), this)},
        data{data} {
    }
};

// -------------------

struct Int{};
struct Float{};
/*
struct Enum{
    libv::span<std::pair<int, std::string_view> table;
};
*/

using TypeDescriptor = std::variant<Int, Float>;
using TypeDescriptors = libv::span<const TypeDescriptor>;

template <typename T>
struct GetTypeDescriptor;
template <>
struct GetTypeDescriptor<int> {
    static constexpr TypeDescriptor value{Int{}};
};
template <>
struct GetTypeDescriptor<float> {
    static constexpr TypeDescriptor value{Float{}};
};

// -------------------

struct LogMacroData {
    int32_t line; // level, function, file
    std::string_view fmt;
};

struct LogStatementMetadata {
    LogMacroData macro_data;
    TypeDescriptors descriptors;
};

template <typename F, typename... Args>
const LogStatementMetadata* get_meta_data_ptr() {
    static constexpr std::array type_descriptors{GetTypeDescriptor<Args>::value...};
    static constexpr LogStatementMetadata meta_data{F{}(), TypeDescriptors{type_descriptors}};
    return &meta_data;
}

template <typename F, typename... Args>
inline LogDataNode meta_data_node{get_meta_data_ptr<F, Args...>()};

template <typename F, typename... Args>
void log(const Args&... args) {
    serialize(meta_data_node<F, Args...>.id, args...);
}

// -------------------

template <size_t size>
struct Explanation { // Explanation == "lazy" eval format string to return as error from a function
    LogDataNode* node{};
    std::array<std::byte, size> data{};
};
// Expected<int, Explanation<4>> foo(int x);
// auto x = foo(0);
// if (!x)
//      LOG("bla bla {error}", x.error())

// -------------------

auto foo(int x, int y) {
    { struct {constexpr LogMacroData operator()() const noexcept {
        return LogMacroData{__LINE__, "Hello {} {}" // FMT stuff here: line, function, file, fmt, level
        };}} metadata; log<decltype(metadata)>(x, y); // Real args here
    }

    { struct {constexpr LogMacroData operator()() const noexcept {
        return LogMacroData{__LINE__, "World {} {}" // FMT stuff here: line, function, file, fmt, level
        };}} metadata; log<decltype(metadata)>(x, 1.0f); // Real args here
    }

    return x + y;
}

auto foo2(int x, int y) {
    { struct {constexpr LogMacroData operator()() const noexcept {
        return LogMacroData{__LINE__, "Hello {} {}" // FMT stuff here: line, function, file, fmt, level
        };}} metadata; log<decltype(metadata)>(1000000, 1200000); // Real args here
    }

    { struct {constexpr LogMacroData operator()() const noexcept {
        return LogMacroData{__LINE__, "World {} {}" // FMT stuff here: line, function, file, fmt, level
        };}} metadata; log<decltype(metadata)>(x, 1.0f); // Real args here
    }

    return x + y;
}

// -------------------------------------------------------------------------------------------------
