#pragma once

// Project: Solutions for Advent of Code 2024
// file with Advent of Code 2024, 5th day : Print Queue
// author: Volker Hillmann
// date:   05.12.2024, last change: 06.12.2024
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.


/*

Use of std::regex to check various types of input lines, thereby 
separating them and processing them in two separate std::ranges pipes. 
The paired rules are read into a std::set<std::pair<size_t, size_t>> 
so that direct access is possible using std::set::find. 
The pages of a line are read into a std::vector<int> and passed on 
through the pipe, so that all lines with pages in the input are returned 
as a std::vector<std::vector<size_t>>.

Indirect indexing of pages in the inner std::vector via the rules.
The generic template methods checkSeparatedIntegers, parseSeparatedIntegers, 
checkSeparatedIntegersPair, and parseSeparatedPairs provide the functionality 
for reading the data in a reusable manner. For the check methods, different 
delimiters can also be specified as template parameters.

*/

#include "qt_form.h"
#include "qt_iteratoren.h"
#include "my_common_tools.h"

#include <string>
#include <utility>
#include <vector>
#include <tuple>
#include <ranges>
#include <print>

namespace nsDay05 {

template <char Sep = ','>
inline bool checkSeparatedIntegers(std::string const& text) {
   static const std::regex fullPattern("^(0|[1-9]\\d+)("s + Sep + "(0|[1-9]\\d+))*$"s);
   return std::regex_match(text, fullPattern) ? true : false;
   }

template <char Sep = ','>
inline bool checkSeparatedIntegers_view(std::string_view const& text) {
   return checkCommaSeparatedIntegers<Sep>({ text.begin(), text.end() });
   }

template <my_integral_ty ty>
std::vector<ty> parseSeparatedIntegers(std::string const& text) {
   static const std::regex numberPattern(R"(0|[1-9]\d+)");
   std::vector<ty> results;
   for(std::sregex_iterator iter(text.begin(), text.end(), numberPattern); iter != std::sregex_iterator(); ++iter) {
      results.emplace_back(toInt<ty>(iter->str())); 
      }
   return results;
   }

template <my_integral_ty ty>
std::vector<ty> parseSeparatedIntegers_view(std::string_view const& text) {
   return parseCommaSeparatedIntegers<ty>({ text.begin(), text.end() });
   }

inline bool checkSeparatedIntegersPairs(std::string const& text) {
   static std::regex fullPattern(R"(^(([1-9][0-9]{0,2})\|([1-9][0-9]{0,2}))$)");
   return std::regex_match(text, fullPattern) ? true : false;
   }

template <my_integral_ty ty>
std::tuple<ty, ty> parseSeparatedPairs(std::string const& text) {
   static std::regex fullPattern(R"(^(([1-9][0-9]{0,2})\|([1-9][0-9]{0,2}))$)");
   std::smatch match;
   if (std::regex_match(text, match, fullPattern)) [[likely]]
      return { toInt<ty>(match[2].str()), toInt<ty>(match[3].str()) };
   else throw std::invalid_argument(std::format("unexpected input: ", text));;
   }

// problem with the std::ranges::input_range. formular wrapper as paramter and read in function
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
      }
   
   std::println(std::cout, "the result for the 2nd part is {}", result_2);

   return { to_String(result_1), to_String(result_2) };
   }

} // end of namespace nsDay05