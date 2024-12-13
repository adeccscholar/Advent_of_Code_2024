#pragma once

// Project: Solutions for Advent of Code 2024
// file with Advent of Code 2024, 4th day : Ceres Search 
// solution with the own grid, changed to the new implementation
// author: Volker Hillmann
// date:   04.12.2024, last change: 09.12.2024
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.

#include "my_common_tools.h"
#include "my_grid2d.h"

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <regex>
#include <functional>
#include <ranges>
#include <print>
#include <execution>
#include <atomic>

using namespace std::string_literals;

inline std::pair<std::string, std::string> RiddleDay4(std::string&& text) { 
   std::atomic<size_t> result_1 = 0, result_2 = 0;

   auto rows = std::ranges::count(text, '\n');
   auto cols = text.size() / rows - 1;
   text.erase(std::ranges::remove(text, '\n').begin(), text.end());

   own::grid::grid_2D<char> vault(rows, cols);
   vault = text;

    std::vector<std::string> patters = { "XMAS"s, "SAMX"s };
   auto pathways = own::grid::find_all_paths(vault);

   for (auto const& patter : patters) {
      auto searcher = std::boyer_moore_searcher(patter.begin(), patter.end());
      std::for_each(std::execution::par, pathways.begin(), pathways.end(),
                           [&vault, &searcher, &result_1, &patter](auto const& pathway) {
            own::grid::grid2d_view<char> grid(vault, pathway);

            size_t pos = 0u;
            auto result = std::search(grid.begin() + pos, grid.end(), searcher);
            for (; result != grid.end(); result = std::search(grid.begin() + pos, grid.end(), searcher)) {
               ++result_1;
               pos = (result - grid.begin()) + patter.size();
               }
         });
      }

   std::println(std::cout, "the result for the 1st part is {}", result_1.load());

   auto innerPathways = own::grid::find_horz_inner_paths(vault);

   auto xmas_match = [&vault](std::tuple<size_t, size_t> const& val) -> bool {
      auto const& [x, y] = val;
      if (vault[x, y] == 'A') {
         if ((vault[x-1, y-1] == 'M' && vault[x-1, y+1] == 'S' && vault[x+1, y-1] == 'M' && vault[x+1, y+1] == 'S') ||
             (vault[x-1, y-1] == 'M' && vault[x-1, y+1] == 'M' && vault[x+1, y-1] == 'S' && vault[x+1, y+1] == 'S') ||
             (vault[x-1, y-1] == 'S' && vault[x-1, y+1] == 'S' && vault[x+1, y-1] == 'M' && vault[x+1, y+1] == 'M') ||
             (vault[x-1, y-1] == 'S' && vault[x-1, y+1] == 'M' && vault[x+1, y-1] == 'S' && vault[x+1, y+1] == 'M')) return true;
         else [[likely]] return false;
         }
      else [[likely]] return false;
      };

   for(auto const& pathway : innerPathways) {
      result_2 += std::ranges::count_if(pathway, xmas_match);
      }

   std::println(std::cout, "the result for the 2nd part is {}", result_2.load());
   return { to_String(result_1.load()), to_String(result_2.load()) };
   }

