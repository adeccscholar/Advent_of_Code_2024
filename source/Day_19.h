#pragma once

// Project: Solutions for Advent of Code 2024
// file with Advent of Code 2024, 19th day : Linen Layout
// author: Volker Hillmann
// date:   19.12.2024
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.

#include "my_common_tools.h"

#include <string>
#include <string_view>
#include <ranges>
#include <print>

namespace nsDay19 {

   template <std::ranges::input_range range_ty>
   std::pair<std::string, std::string> RiddleDay19(range_ty const& values) {
      size_t result_1 = 0, result_2 = 0;

      std::println(std::cout, "the result for the 2nd part is {}", result_2);
      std::println(std::clog, "1st day finished");
      return { to_String(result_1), to_String(result_2) };
      }


} // end of namespace nsDay19
