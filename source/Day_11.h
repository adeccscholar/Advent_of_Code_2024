#pragma once

// Project: Solutions for Advent of Code 2024
// file with Advent of Code 2024, 11th day : Plutonian Pebbles
// solution with a std::forward_list 
// author: Volker Hillmann
// date:   11.12.2024, last change: 12.12.204
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.

#include "my_common_tools.h"

#include <iostream>
#include <cmath>
#include <string>
#include <utility>
#include <functional>
#include <forward_list>
#include <charconv>
#include <ranges>
#include <print>

template <my_integral_ty ty>
auto parse_numbers(std::string_view input) {
   auto is_not_space = [](char c) { return !std::isspace(c); };
   return input | std::views::split(' ')
                | std::views::filter([](auto subrange) { return !subrange.empty(); })
                | std::views::transform([](auto subrange) {
                      ty value = 0;
                      auto str = std::string_view(&*subrange.begin(), subrange.size());
                      std::from_chars(str.data(), str.data() + str.size(), value);
                      return value;
                      })
                | std::ranges::to<std::vector>();
   }

template <my_integral_ty ty>
constexpr size_t countDigits(ty const& number) {
   if constexpr (std::is_signed_v<ty>) if (number < 0)  number = -number;
   if (number == 0) return 1;
   return static_cast<size_t>(log10(number)) + 1;
}

template <my_integral_ty ty>
std::pair<ty, ty> splitEvenDigits(ty const& number, size_t digits) {
   size_t middle = digits / 2;
   size_t divisor = pow(ty { 10 }, middle);
   ty firstPart = static_cast<ty>(number / divisor);
   ty secondPart = static_cast<ty>(number % divisor);
   return { firstPart, secondPart };
   }


template <my_integral_ty ty>
size_t countElements(std::forward_list<ty> const& list) {
   size_t count = 0;
   for (const auto& element : list) ++count; return count;
   }

template <my_integral_ty ty>
using list_ty = std::pair<size_t, std::forward_list<ty>>;

// todo attention: this function don't work
template <my_integral_ty ty>
std::vector<list_ty<ty>> spliceList(std::forward_list<ty>& list, size_t n, size_t k) {
   std::vector<list_ty<ty>> parts(k);
   size_t elements  = n / k;
   size_t remaining = n % k;

   for (auto [i, it] = std::make_pair(0, list.begin()); i < k; ++i) {
      parts[i].first = elements;
      if (remaining > 0) {
         --remaining;
         ++parts[i].first;
         }
      parts[i].second.splice_after(parts[i].second.before_begin(), list, it, std::next(it, parts[i].first));
      it = std::next(it, parts[i].first);
      }

   return parts;
   }

inline std::pair<std::string, std::string> RiddleDay11(std::string&& text) {
   using int_ty = uint64_t;
   
   list_ty<int_ty> values { 0ull, std::forward_list<int_ty> { } };
   
   for(auto const& val : parse_numbers<int_ty>(text) | std::views::reverse ) {
      values.second.emplace_front(val);
      ++values.first;
      }

   using node_ty  = std::forward_list<int_ty>::iterator;
   using func_ty  = std::function<bool(node_ty& node)>;
   using rules_ty = std::vector<func_ty>;

   auto make_step = [](list_ty<size_t>& values) {
      rules_ty rules = { [](node_ty& node) {        
                                  if (*node == 0) {
                                     *node = 1;
                                     return true;
                                     }
                                  else return false;
                                  },
                         [&values](node_ty& node) { 
                                  if (auto digits = countDigits(*node); digits % 2 == 0) {
                                     auto [first, second] = splitEvenDigits<int_ty>(*node, digits);
                                     *node = first;
                                     auto pos = values.second.emplace_after(node, second);
                                     ++values.first;
                                     node = pos;
                                     return true;
                                     }
                                  else return false;
                                  },
                         [](node_ty& node) { 
                                  *node *= 2024ull;
                                  return true;
                                  }
                       };

      for (auto node = values.second.begin(); node != values.second.end(); ++node) {
         for (auto& rule : rules) if (rule(node)) break;
         }
      };



   // 230753 to high   (150515 count after 24 steps is to low)
   for(auto steps : std::views::iota(int_ty { 1 }, int_ty { 26 } )) {
      make_step(values);
      }

   size_t result_1 = values.first;
   std::println(std::cout, "the result for the 1st part is {}", result_1);
   
   /*
   for (auto steps : std::views::iota(int_ty{ 26 }, int_ty{ 76 })) { 
      make_step(values);
      }
   size_t result_2 = countNodes;
   */

   // auto parts = spliceList(values.second, values.first, 16);
   size_t result_2 = 0;
   std::println(std::cout, "the result for the 2nd part is {}", result_2);
   return { to_String(result_1), to_String(result_2) };
   }