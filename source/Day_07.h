#pragma once

// Project: Solutions for Advent of Code 2024
// file with Advent of Code 2024, 7th day : Bridge Repair
// solution with a recursion
// author: Volker Hillmann
// date:   07.12.2024, last change: 10.12.2024
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.

#include "my_common_tools.h"

#include <string>
#include <utility>
#include <functional>
#include <span>
#include <ranges>
#include <print>

namespace nsDay07 {

template <my_integral_ty ty, typename func_ty>
bool recursion_func_1(std::span<ty> values, ty res, func_ty&& func, ty acc) {
   return values.empty() ? acc == res :
             recursion_func_1<ty>(values.subspan(1), res, std::plus<ty>{}, func(acc, values.front())) ? true :
             recursion_func_1<ty>(values.subspan(1), res, std::multiplies<ty>{}, func(acc, values.front()));
   }

template <my_integral_ty ty, typename func_ty>
bool recursion_func_2(std::span<ty> values, ty res, func_ty&& func, ty acc) {
   return values.empty() ? acc == res :
      recursion_func_2<ty>(values.subspan(1), res, std::plus<ty>{}, func(acc, values.front())) ? true :
      recursion_func_2<ty>(values.subspan(1), res, std::multiplies<ty>{}, func(acc, values.front()))? true :
      recursion_func_2<ty>(values.subspan(1), res, concatenateNumbers_ty<ty>{}, func(acc, values.front()));
   }

template <std::ranges::input_range range_ty>
std::pair<std::string, std::string> RiddleDay7(range_ty const& values) {
   size_t result_1 = 0, result_2 = 0;

   auto tasks = values | std::views::transform([](std::string_view const& p) { return splitString(p, ':'); })
                       | std::views::filter([](auto const& opt) { return opt.has_value(); })
                       | std::views::transform([](auto const& p) {
                             auto result = toInt<size_t>(std::get<0>(*p));
                             auto input = extractNumbers<size_t>(std::get<1>(*p));
                             return std::make_tuple(result, input);
                             })
                       | std::ranges::to<std::vector>();


   for (auto& [result, input] : tasks) {
      result_1 += recursion_func_1<size_t>(input, result, std::plus<size_t>{}, 0u) ? result : 0;
      result_2 += recursion_func_2<size_t>(input, result, std::plus<size_t>{}, 0u) ? result : 0;
      }

   std::println(std::cout, "the result for the 1st part is {}, expected {}", result_1, 20281182715321);
   std::println(std::cout, "the result for the 2nd part is {}, solved but wrong {}", result_2, 32565384255325);

   return { to_String(result_1), to_String(result_2) };
   }
     
} // end of namespace nsDay07