#pragma once

// Project: Solutions for Advent of Code 2024
// file with Advent of Code 2024, 3rd day : Mull It Over
// author: Volker Hillmann
// date:   03.12.2024
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.


#include "my_common_tools.h"

#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <regex>
#include <ranges>
#include <print>

using namespace std::string_literals;

template <std::ranges::input_range range_ty>
inline std::pair<std::string, std::string> RiddleDay3(range_ty const& values) {
   static std::regex pattern1(R"(mul\(([1-9][0-9]{0,2}),([1-9][0-9]{0,2})\))");
   std::vector<std::pair<size_t, size_t>> results;
   for (auto const& data : values) {
      std::string input = { data.begin(), data.end() };
      auto begin = std::sregex_iterator(input.begin(), input.end(), pattern1);
      auto end = std::sregex_iterator();

      for (std::sregex_iterator it = begin; it != end; ++it) {
         std::smatch match = *it;
         results.emplace_back(toInt<size_t>(match[1].str()), toInt<size_t>(match[2].str()));
         }
      }

   auto result_1 = std::accumulate(results.begin(), results.end(), size_t { 0 }, 
                                   [](size_t acc, std::pair<size_t, size_t>& p) {
                                       return acc + (p.first * p.second);
                                       }
                                   );
   //*
   results.clear();
   static std::regex pattern2(R"((mul\(([1-9][0-9]{0,2}),([1-9][0-9]{0,2})\))|(do\(\))|(don't\(\)))");
   for (bool boActive = true; auto const& data : values) {
      std::string input = { data.begin(), data.end() };
      auto begin = std::sregex_iterator(input.begin(), input.end(), pattern2);
      auto end = std::sregex_iterator();

      for (std::sregex_iterator it = begin; it != end; ++it) {
         std::smatch const& match = *it;


         if (match[4].matched) boActive = true;
         else if (match[5].matched) boActive = false;
         else if (match[1].matched && boActive) results.emplace_back(toInt<size_t>(match[2].str()), toInt<size_t>(match[3].str()));
         }
      }
   //*/
   /*
   bool boActive = true;
   static std::string strStart = "do()"s;
   static std::string strStop  = "don't()"s;

   for (results.clear(); auto const& data : values) {
      std::string input = { data.begin(), data.end() };

      for (size_t pos = (boActive ? 0u : input.find(strStart) + strStart.size()); pos != std::string::npos ; ) {
         size_t endpos = input.find(strStop, pos);
         auto begin = std::sregex_iterator(input.begin() + pos, (endpos == std::string::npos ? input.end() : input.begin() + endpos), pattern1);
         auto end   = std::sregex_iterator();
         boActive = true;
         for (std::sregex_iterator it = begin; it != end; ++it) {
            std::smatch match = *it;
            results.emplace_back(toInt<size_t>(match[1].str()), toInt<size_t>(match[2].str()));
            }

         if (endpos == std::string::npos) {
            pos = endpos;
            continue;
            }
         else { 
            pos = input.find(strStart, endpos + strStop.size());
            if(pos != std::string::npos) pos += strStart.size();
            else boActive = false;
            }
         }
      }
   */

   auto result_2 = std::accumulate(results.begin(), results.end(), size_t{ 0 },
                                       [](size_t acc, std::pair<size_t, size_t>& p) {
                                            return acc + (p.first * p.second);
                                            }
                                  );


   std::println(std::cout, "the result for the 1st part is {}", result_1);
   std::println(std::cout, "the result for the 2nd part is {}", result_2);
   return { to_String(result_1), to_String(result_2) };
   }
