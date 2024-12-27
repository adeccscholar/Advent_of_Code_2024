#pragma once

// Project: Solutions for Advent of Code 2024
// file with Advent of Code 2024, 1st day : Historian Hysteria
// author: Volker Hillmann
// date:   01.12.2024, changed 1t 18.12.2024 to reuse the read funktion
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.

/*

Using std::ranges, providing a template for reading pairs of 
integers within a pipe operation from a std::ranges::input_range. 
Using algorithms for sorting and computation, and using 
std::views::zip to combine data streams as input for algorithms.

The template functions ParsePair and Difference are so general 
that they can be provided in a header-only library and reused. 

Extend the function parsePair with new template parameter to read 
pairs of integer with a dynamic length and split char.
*/

#include "my_common_tools.h"

#include <string>
#include <utility>
#include <vector>
#include <stdexcept>
#include <regex>
#include <ranges>
#include <span>
#include <print>

namespace nsDay01 {

[[nodiscard]] std::string escapeChar(char c) {
   static const char specialChars[15] = R"([\^$.|?*+(){}])"; // list of chars which need escaped
   std::span<const char> c_span(specialChars, sizeof(specialChars) - 1);
   if ( auto it = std::ranges::find(c_span, c ); it != c_span.end()) return "\\"s + std::string(1, c); // add escape
   return std::string(1, c); 
   }

template <my_integral_ty ty, char SPLIT = ' ', size_t SIZE = 6>
std::pair<ty, ty> parsePair(std::string_view const& para) {
   static std::regex pattern("\\s*(0|([1-9]\\d{0,"s + to_String(SIZE - 1) + "}))\\s*"s + escapeChar(SPLIT) + "\\s*(0|([1-9]\\d{0,"s + to_String(SIZE - 1) + "}))\\s*");
   std::smatch matches;   
   std::string input = { para.begin(), para.end() };
   if (std::regex_match(input, matches, pattern)) {
      return { toInt<ty>(matches[1].str()), toInt<ty>(matches[3].str()) };
      }
   else {
      throw std::invalid_argument("unexpected format, couldn't extract numbers.");
      }
   }


template <my_integral_ty ty>
ty Difference(ty const& a, ty const& b) {
   return b > a ? b - a : a - b;
   }

template <std::ranges::input_range range_ty>
std::pair<std::string, std::string> RiddleDay1(range_ty const& values) {
   auto result = values | std::ranges::views::filter([](auto const& p) { return p.size() > 0; })
      | std::ranges::views::transform(parsePair<size_t, ' ', 5>)
      | std::ranges::to<std::vector>();

   auto first_vals = result |  std::views::elements<0> | std::ranges::to<std::vector>();
   auto second_vals = result | std::views::elements<1> | std::ranges::to<std::vector>();

   std::ranges::sort(first_vals);
   std::ranges::sort(second_vals);

   // total distance
   auto result_1 = std::ranges::fold_left(std::ranges::views::zip(first_vals, second_vals), size_t{ 0 },
      [](size_t acc, auto const& pair) {
         auto const& [a, b] = pair;
         return acc + Difference(a, b);
      });
   std::println(std::cout, "the result for the 1st part is {}", result_1);  // 1889772

   // similarity score
   auto result_2 = std::ranges::fold_left(first_vals, size_t{ 0 },
      [&second_vals](size_t acc, auto const& val) {
         return acc + val * std::ranges::count(second_vals, val);
      });
   std::println(std::cout, "the result for the 2nd part is {}", result_2);   // 23228917
   std::println(std::clog, "1st day finished");
   return { to_String(result_1), to_String(result_2) };
   }

} // end of namespace nsDay01