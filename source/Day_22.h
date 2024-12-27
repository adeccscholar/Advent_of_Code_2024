#pragma once


// Project: Solutions for Advent of Code 2024
// file with Advent of Code 2024, 22nd day : Monkey Market
// author: Volker Hillmann
// date:   22.12.2024
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.

#include "my_common_tools.h"

#include <string>
#include <string_view>
#include <map>
#include <unordered_map>
#include <ranges>
#include <print>

namespace nsDay22 {

   template <std::ranges::input_range range_ty>
   std::pair<std::string, std::string> RiddleDay22(range_ty const& values) {
      std::vector<std::function<uint64_t(uint64_t&& value)>> workflow = {
         [](uint64_t && value) -> uint64_t {
            value ^= value * 64;
            value %= 16777216;
            return std::move(value);
            },
         [](uint64_t&& value) -> uint64_t {
            value ^= value / 32;
            value %= 16777216;
            return std::move(value);
            },
         [](uint64_t&& value) -> uint64_t {
            value ^= value * 2048;
            value %= 16777216;
            return std::move(value);
            }
         };
      
      size_t result_1 = 0, result_2 = 0;

      auto datas = values | std::views::transform([](auto const& val) { return toInt<uint64_t>(val); } ) 
                          | std::ranges::to<std::vector>();


      struct array_ty_hash {
         std::size_t operator()(std::array<int16_t, 4> idx) const {
            auto [a, b, c, d] = idx;
            std::size_t hash_a = std::hash<size_t>{}((a + 10) * 10000);
            std::size_t hash_b = std::hash<size_t>{}((b + 10) *  1000);
            std::size_t hash_c = std::hash<size_t>{}((c + 10) *   100);
            std::size_t hash_d = std::hash<size_t>{}(d + 1);
            return hash_a + hash_b + hash_c + hash_d;
         }
      };

      const size_t count = 2000;
      using seq_data = std::unordered_map<std::array<int16_t, 4>, int16_t, array_ty_hash>;
      //using seq_data = std::map<std::array<int16_t, 4>, int16_t>;
      using sequences = std::vector<seq_data>;

      seq_data accumulated;

      for (auto data : datas) {
         std::vector<int16_t> last_ones;
         std::vector<int16_t> diffs;
         seq_data pattern;
         int16_t last_one = data % 10;
         last_ones.reserve(count + 1);
         diffs.reserve(count + 1);

         last_ones.emplace_back(static_cast<uint16_t>(last_one));
         diffs.emplace_back(0);
         for (size_t i : std::views::iota(1) | std::views::take(count)) {
            for (auto const& func : workflow) data = func(std::forward<uint64_t>(data));
            int16_t new_last_one = data % 10;
            diffs.emplace_back(new_last_one - last_one);
            last_one = new_last_one;;
            last_ones.emplace_back(static_cast<uint16_t>(std::move(new_last_one)));
            }
         for(size_t idx = 4; idx < last_ones.size(); ++idx) {
            pattern.insert({ { diffs[idx - 3], diffs[idx - 2], diffs[idx - 1], diffs[idx] }, last_ones[idx] });
            }

         for (auto const& [key, value] : pattern) accumulated[key] += value;
         result_1 += data;
         }
 
      auto maxElement = std::ranges::max_element(accumulated, {}, [](auto const& vals) { return vals.second; });
      if (maxElement != accumulated.end()) {
         const auto& [bestkey, maxsum] = *maxElement;
         result_2 = maxsum;
         }

      std::println(std::cout, "the result for the 1st part is {}", result_1);
      std::println(std::cout, "the result for the 2nd part is {}", result_2);
      std::println(std::clog, "1st day finished");
      return { to_String(result_1), to_String(result_2) };
   }


} // end of namespace nsDay22
