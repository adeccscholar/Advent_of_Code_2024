#pragma once

// AOC 2024, 2nd day : Red-Nosed Reports
// Project: Solutions for Advent of Code 2024
// file with Advent of Code 2024, 2nd day : Red-Nosed Reports
// author: Volker Hillmann
// date:   02.12.2024
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.

#include "my_common_tools.h"

#include <string>
#include <utility>
#include <vector>
#include <stdexcept>
#include <ranges>
#include <print>

using namespace std::string_literals;

template <std::ranges::input_range range_ty>
std::pair<std::string, std::string> RiddleDay2(range_ty const& values) {

   size_t result_1 = 0, result_2 = 0;
   for (auto const& data : values) {
      std::string input = { data.begin(), data.end() };
      if (!checkSpaceSeparatedIntegers<3>(input)) {
         throw std::invalid_argument("unexpected input \""s + input + "\" in input for Riddle2"s);
      }

      auto row = extractNumbers<size_t>(data);
      // std::print(std::cout, "{:>3} ->", row);
      std::vector<int> differences;
      for (auto const& diffs : row | std::views::slide(2)) {
         std::vector<size_t> slidevals;
         for (auto elem : diffs) {
            slidevals.emplace_back(elem);
         }
         differences.emplace_back(static_cast<int>(static_cast<long long>(slidevals[1]) - static_cast<long long>(slidevals[0])));
      }

      // std::print(std::cout, "   {:>3}\t", differences);

      bool allIncreasing = std::ranges::all_of(differences, [](int diff) { return diff > 0 && diff <= 3; });
      bool allDecreasing = std::ranges::all_of(differences, [](int diff) { return diff < 0 && diff >= -3; });
      bool allChecks = allIncreasing || allDecreasing;

      if (allChecks) {
         result_1 += 1;
         result_2 += 1;
         // std::println(std::cout, "korrekt");
      }
      else [[likely]] {
         for (size_t i = 0; i < row.size(); ++i) {
            auto subrow = row | std::views::enumerate | std::views::filter([i](auto const& p) {
               auto const& [idx, val] = p;
               return idx != i;
               })
               | std::views::transform([](auto const& p) {
               auto const& [idx, val] = p;
               return val;
                  })
               | std::ranges::to<std::vector>();

            std::vector<int> subdiffs;
            for (auto const& diffs : subrow | std::views::slide(2)) {
               std::vector<size_t> slidevals;
               for (auto elem : diffs) {
                  slidevals.emplace_back(elem);
               }
               subdiffs.emplace_back(static_cast<int>(static_cast<long long>(slidevals[1]) - static_cast<long long>(slidevals[0])));
            }

            bool allIncreasing = std::ranges::all_of(subdiffs, [](int diff) { return diff > 0 && diff <= 3; });
            bool allDecreasing = std::ranges::all_of(subdiffs, [](int diff) { return diff < 0 && diff >= -3; });
            bool allChecks = allIncreasing || allDecreasing;
            if (allChecks) {
               result_2 += 1;
               break;
            }
         }
         }
         /*
            auto cntIncreasing = std::ranges::count_if(differences, [](int diff) { return diff >= 0; });
            auto cntDecreasing = std::ranges::count_if(differences, [](int diff) { return diff <= 0; });

            if(cntIncreasing > cntDecreasing) {
               if(auto it = std::ranges::find_if(differences, [](int diff) {  return diff <= 0 || diff > 3; }); it != differences.end()) {
                  if (auto next = it + 1; it != differences.begin() && next != differences.end()) *next += *it;
                  differences.erase(it);
                  }
               allIncreasing = std::ranges::all_of(differences, [](int diff) { return diff > 0 && diff <= 3; });
               allDecreasing = false;
               }
            else {
               if (auto it = std::ranges::find_if(differences, [](int diff) {  return diff >= 0 || diff < -3; }); it != differences.end()) {
                  if (auto next = it + 1; it != differences.begin() && next != differences.end()) *next += *it;
                  differences.erase(it);
                  }
               allIncreasing = false;
               allDecreasing = std::ranges::all_of(differences, [](int diff) { return diff < 0 && diff >= -3; });
               }
            result_2 += allIncreasing || allDecreasing ? 1 : 0;
            std::println(std::cout, "{:3}\t{}", differences, (allIncreasing || allDecreasing ? "korrigiert" : "ungültig"));
            }
            */
   }

   std::println(std::cout, "the result for the 1st part is {}", result_1);
   std::println(std::cout, "the result for the 2nd part is {}", result_2);
   return { to_String(result_1), to_String(result_2) };
}
