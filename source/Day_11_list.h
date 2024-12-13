#pragma once

#include "my_common_tools.h"

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
                   });
   }


template <my_integral_ty ty>
void print_list(std::list<ty> const& list, std::ostream& out) {
   for (auto const& node : list) {
      std::print(out, ";{}", node);
      }
   std::println(out, "");
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
void make_step(linked_list_type<ty>& list) {
   using node_ty  = linked_list_type<ty>::node_ty;
   using func_ty  = std::function<bool(node_ty* node)>;
   using rules_ty = std::vector<func_ty>;

   rules_ty rules = { [](node_ty* node)      { if (node->node_value == 0) {
                                                  node->node_value = 1;
                                                  return false;
                                                  }
                                               else return true;
                                             },
                      [&list](node_ty* node) { if(auto digits = countDigits(node->node_value); digits % 2 == 0) {
                                                  auto [first, second]= splitEvenDigits(node->node_value, digits);
                                                  list.insert_into_list_before(first, node);
                                                  node->node_value = second;
                                                  
                                               /* if(auto [digits, number] = countDigitsString(node->node_value); digits % 2 == 0) {
                                                  auto [first, second] = splitEvenDigitsString<ty>(number);
                                                  list.insert_into_list_before(first, node);
                                                  node->node_value = second;
                                               */
                                                  return false;
                                                  }
                                               else return true;
                                             },
                      [](node_ty* node)      { node->node_value *= 2024ull;
                                               return false;
                                             }
                    };

   for(node_ty* node = list.head_of_list; node != nullptr; node = node->next_node) {
      for(auto rule : rules) {
         if (!rule(node)) break;
         }
      }
   }


inline std::pair<std::string, std::string> RiddleDay11(std::string&& text) {
   std::forward_list<uint32_t> values;
   size_t countNodes = 0ull;
   
   for(auto const& val : parse_numbers<uint32_t>(text) | std::views::reverse ) {
      values.emplace_front(val);
      ++countNodes;
      }

   using node_ty  = std::forward_list<uint32_t>::iterator;
   using func_ty  = std::function<bool(node_ty& node)>;
   using rules_ty = std::vector<func_ty>;

   rules_ty rules = { [](node_ty& node) {        
                                  if (*node == 0) {
                                     *node = 1;
                                     return false;
                                     }
                                  else return true;
                                  },
                      [&values, &countNodes](node_ty& node) { 
                                  if (auto digits = countDigits(*node); digits % 2 == 0) {
                                     auto [first, second] = splitEvenDigits(*node, digits);
                                     *node = first;
                                     auto pos = values.emplace_after(node, std::forward<uint64_t>(second));
                                     ++countNodes;
                                     node = pos;
                                     return false;
                                     }
                                  else return true;
                                  },
                      [](node_ty& node) { 
                                  *node *= 2024ull;
                                  return false;
                                  }
                     };

   auto make_step = [&rules](std::forward_list<uint32_t>& values) {
      for (auto node = values.begin(); node != values.end(); ++node) {
         for (auto& rule : rules) {
            if (!rule(node)) break;
            }
         }
      };




   // 230753 to high
   //*
   for(auto steps : std::views::iota(1, 26)) { // 26
      make_step(values);
      }
   //*/
   //size_t result_1 = 0;
   //for (auto val = list_values.head_of_list; val != nullptr; val = val->next_node) ++result_1;

   size_t result_1 = countNodes;

   size_t result_2 = 0;
   return { to_String(result_1), to_String(result_2) };
}