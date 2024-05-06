#pragma once

#include <string>
#include <tuple>
#include <vector>

/// @brief List all errors
using Errors = std::vector<std::string>;

/// @brief RegExp data; fields: 1) is_bad; 2) reg_exp string
using RegExpData = std::tuple<bool, std::string>;

/// @brief RegExp list (stored raw as strings)
using RegExpList = std::vector<RegExpData>;

/// @brief Test data chunk (raw)
using TestDataItem = std::vector<char>;

/// @brief List all data chunks
using TestData = std::vector<TestDataItem>;

constexpr size_t default_scale_regexp = 20000U; ///< Regexp iterations count

constexpr size_t default_scale_test_data = 10U; ///< Data chunks iteration count
