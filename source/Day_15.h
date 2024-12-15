#pragma once

// Project: Solutions for Advent of Code 2024
// file with Advent of Code 2024, 15t day : Warehouse Woes
// author: Volker Hillmann
// date:   15.12.2024
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.


#include "my_common_tools.h"

#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <regex>
#include <ranges>
#include <print>

using namespace std::string_literals;

template <std::ranges::input_range range_ty>
inline std::pair<std::string, std::string> RiddleDay15(range_ty const& values) {
   size_t result_1 = 0, result_2 = 0;
   std::println(std::cout, "the result for the 1st part is {}", result_1);
   std::println(std::cout, "the result for the 2nd part is {}", result_2);
   return { to_String(result_1), to_String(result_2) };
   }
