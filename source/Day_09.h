#pragma once

// Project: Solutions for Advent of Code 2024
// file with Advent of Code, 9th day : Disk Fragmenter
// solution with a std::vector and reserve
// author: Volker Hillmann
// date:   09.12.2024, last change: 12.12.2024
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.


#include "my_common_tools.h"

#include <string>
#include <utility>
#include <ranges>

namespace nsDay09 {

inline std::pair<std::string, std::string> RiddleDay9(std::string&& text) {
   auto values = text | std::views::filter([](char c) { return c >= '0' && c <= '9'; })
                      | std::views::transform([](char c) { return static_cast<size_t>(c - '0'); }) 
                      | std::ranges::to<std::vector>();

   auto count = std::ranges::fold_left(values, size_t { 0 }, [](size_t acc, size_t val) {
                                                                  return acc + val;
                                                                  } );

   std::vector<std::optional<size_t>> disk;
   disk.reserve(count);

   size_t id = size_t { 0 };

   auto zipped = std::views::zip(values, 
                                 std::views::iota(0) | std::views::transform([](auto b) -> bool { return b % 2 == 0; }),
                                 std::views::iota(0) | std::views::transform([](auto b) -> size_t { return b / 2;}))
                                | std::ranges::to<std::vector>();


   for(auto const& value : zipped) {
      if (std::get<1>(value)) disk.insert(disk.end(), std::get<0>(value), std::optional { std::get<2>(value) });
      else disk.insert(disk.end(), std::get<0>(value), std::nullopt );
      }

   for (auto [fwd, bwd] = std::make_pair(disk.begin(), disk.rbegin()) ;; ) {
      auto seek_fwd = [&fwd, &disk]() { return std::find_if(fwd, disk.end(), [](auto const& val) { return !val; }); };
      auto seek_bwd = [&bwd, &disk]() { return std::find_if(disk.rbegin(), disk.rend(), [](auto const& val) { return val; }); };

      if ((fwd = seek_fwd()) != disk.end()) {
         if ((bwd = seek_bwd()) != disk.rend()) {
            if (fwd == bwd.base()) break;
            std::swap(*fwd, *bwd);
            ++bwd;
         }
         ++fwd;
      }
      else break;
      }

   auto result_1 = std::ranges::fold_left(std::views::zip(disk, std::views::iota( size_t { 0 })), size_t { 0 },
                                          [](size_t acc, auto const& val) { 
                                                auto const& [value, id] = val;
                                                return !value ? acc : acc + (*value) * id;});

   size_t result_2 = 0;
   return { to_String(result_1), to_String(result_2) };
   }

} // end of namespace nsDay09
