#pragma once

// Project: Solutions for Advent of Code 2024
// file with Advent of Code 2024, 14th day : Restroom Redoubt
// author: Volker Hillmann
// date:   14.12.2024
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.

#include "my_common_tools.h"
#include "my_grid2d.h"

#include <string>
#include <utility>
#include <functional>
#include <span>
#include <ranges>
#include <print>

using namespace std::string_literals;

namespace nsDay14 {

bool checkLine(std::string_view line) {
   //static std::regex regex(R"(^p=([0]|([1-9]\d{0,3})),([0]|([1-9]\d{0,3})) v=(-?[0]|([1-9]\d{0,3})),(-?[0]|([1-9]\d{0,3}|0))$)");

   static std::regex regex(R"(^p=(0|([1-9][0-9]{0,3})),(0|([1-9][0-9]{0,3})) v=(-?(0|([1-9][0-9]{0,3}))),(-?(0|([1-9][0-9]{0,3}))))");
   std::string input = { line.begin(), line.end() };
   std::smatch match;
   bool ret = std::regex_match(input, match, regex);
   return ret;
   }

template <my_integral_ty pos_ty, my_integral_ty move_ty>
std::tuple<pos_ty, pos_ty, move_ty, move_ty> parseLine(std::string const& input) {
   static std::regex regex(R"(^p=(0|([1-9][0-9]{0,3})),(0|([1-9][0-9]{0,3})) v=(-?(0|([1-9][0-9]{0,3}))),(-?(0|([1-9][0-9]{0,3}))))");
   std::smatch match;
   if (std::regex_match(input, match, regex)) [[likely]] {
      auto p1 = toInt<pos_ty> (match[1].str());
      auto p2 = toInt<pos_ty> (match[3].str());
      auto v1 = toInt<move_ty>(match[5].str());
      auto v2 = toInt<move_ty>(match[8].str());
      return { p1, p2, v1, v2 };
      }
   else {
      throw std::invalid_argument("unexpected input: "s + input);
      }
   }

template <std::ranges::input_range range_ty>
std::pair<std::string, std::string> RiddleDay14(range_ty const& values) {
   size_t result_1 = 0, result_2 = 0;

   auto input = values | std::views::filter(checkLine) 
                       | std::views::transform(to_String<std::string_view>)
                       | std::views::transform(parseLine<uint32_t, int32_t>) 
                       | std::ranges::to<std::vector>();
 
   for (auto const& [px, py, vx, vy] : input)
      std::println(std::cout, "p={:>3},{:>3} v={:>3},{:>3}", px, py, vx, vy);

   std::println(std::cout, "the result for the 1st part is {}", result_1);
   std::println(std::cout, "the result for the 2nd part is {}", result_2);

   return { to_String(result_1), to_String(result_2) };
   }

} // end of namespace nsDay14