#pragma once

// Project: Solutions for Advent of Code 2024
// file with Advent of Code 2024, 5th day : Print Queue
// author: Volker Hillmann
// date:   05.12.2024, last change: 06.12.2024
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.


// there seems a problem with the iterator, when passed as parameter

#include "qt_form.h"
#include "qt_iteratoren.h"
#include "my_common_tools.h"

#include <string>
#include <utility>
#include <vector>
#include <tuple>
#include <ranges>
#include <print>


inline std::pair<std::string, std::string> RiddleDay5(TMyForm& frm) { 
   auto values = frm.GetLines("memInput");
   auto rules        = values | std::views::transform(toString<std::string_view>)
                              | std::views::filter(checkSeparatedIntegersPairs)
                              | std::views::transform([](auto const& p) { return parseSeparatedPairs<size_t>({ p.begin(), p.end() });})
                              | std::ranges::to<std::set>();
   auto update_pages = values | std::views::transform(toString<std::string_view>)
                              | std::views::filter(checkSeparatedIntegers<','>)
                              | std::views::transform([](auto const& p) { return parseSeparatedIntegers<size_t>(std::string { p.begin(), p.end() });})
                              | std::ranges::to<std::vector<std::vector<size_t>>>();
  
   auto check = [&rules](std::vector<size_t> const& line) -> bool {
      for (auto const& pair_vals : line | std::views::slide(2)) {
         std::vector<size_t> slidevals;
         for (auto elem : pair_vals) slidevals.emplace_back(elem);
         if (rules.find(std::make_tuple(slidevals[0], slidevals[1])) == rules.end() &&
            rules.find(std::make_tuple(slidevals[1], slidevals[0])) != rules.end()) return false;
         }
      return true;
      };

   size_t result_1 = 0;
   for(auto const& current_update : update_pages) {
      if (check(current_update) && current_update.size() % 2 != 0) result_1 += current_update[current_update.size() / 2];
      }

   std::println(std::cout, "the result for the 1st part is {}", result_1);

   size_t result_2 = 0;
   static auto swap_elements = [](size_t& val1, size_t& val2) {
      auto tmp = val1;
      val1 = val2;
      val2 = tmp;
      };

   for (std::vector<size_t>& current_update : (update_pages | std::views::filter([&rules, &check](auto const& vec) { return !check(vec); })) ) {
      do {
         for (auto const& vals : std::ranges::views::zip(std::views::iota(1) | std::views::take(current_update.size() - 1), std::views::iota(0))) {
            auto& [idx, prev_idx] = vals;
            size_t& element = current_update[idx];
            size_t& prev = current_update[prev_idx];
            if (rules.find(std::make_tuple(prev, element)) == rules.end() &&
               rules.find(std::make_tuple(element, prev)) != rules.end()) swap_elements(element, prev);
            }
         } 
      while (!check(current_update));
      result_2 += current_update[current_update.size() / 2];
      //std::println(std::cout, "{}", current_update);
      }
   
   std::println(std::cout, "the result for the 2nd part is {}", result_2);

   return { to_String(result_1), to_String(result_2) };
   }
