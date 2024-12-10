#pragma once

// Project: Solutions for Advent of Code 2024
// file with Advent of Code, 9th day : Disk Fragmenter
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.


#include "my_common_tools.h"

#include <string>
#include <utility>
#include <ranges>

template <std::ranges::input_range range_ty>
std::pair<std::string, std::string> RiddleDay9(range_ty const& values) {
   size_t result_1 = 0, result_2 = 0;
   return { to_String(result_1), to_String(result_2) };
   }
