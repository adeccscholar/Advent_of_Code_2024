#pragma once

// AOC 2024, 1st day : Historian Hysteria

#include "my_common_tools.h"

#include <string>
#include <vector>
#include <ranges>
#include <print>

template <std::ranges::input_range range_ty>
std::pair<std::string, std::string> RiddleDay1(range_ty const& values) {
   auto result = values | std::ranges::views::filter([](auto const& p) { return p.size() > 0; })
      | std::ranges::views::transform(parsePair<size_t>)
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
