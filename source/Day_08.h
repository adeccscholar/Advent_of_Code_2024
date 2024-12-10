#pragma once

// Project: Solutions for Advent of Code 2024
// file with Advent of Code, 8th day : Resonant Collinearity
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.

#include "my_common_tools.h"
#include "my_grid2d.h"

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <algorithm>
#include <ranges>

template <typename ty>
std::vector<std::pair<ty, ty>> combine(std::vector<ty> const& elements) {
   const size_t n = elements.size();
   const size_t pair_count = (n * (n - 1)) / 2;
   std::vector<std::pair<own::grid::coord_2D_type, own::grid::coord_2D_type>> result;
   result.reserve(pair_count);

   for (auto i = 0u; i < elements.size(); ++i) {
      for (auto j = i + 1; j < elements.size(); ++j) {
         result.emplace_back(elements[i], elements[j]);
      }
   }
   return result;
}

//template <std::ranges::input_range range_ty>
std::pair<std::string, std::string> RiddleDay8(std::string&& text) { //range_ty const& values) {
   const auto rows = std::ranges::count(text, '\n');
   const auto cols = text.size() / std::ranges::count(text, '\n') - 1;
   text.erase(std::ranges::remove(text, '\n').begin(), text.end());
   own::grid::grid_2D<char> grid(rows, cols);
   grid = text;

   std::map<char, std::vector<own::grid::coord_2D_type>> values;
   auto current = grid.begin();
   auto it = std::find_if(current, grid.end(), [](char c) { return c != '.'; });
   while (it != grid.end()) {
      values[*it].emplace_back(grid.GetCoordinates(it));
      it = std::find_if(++it, grid.end(), [](char c) { return c != '.'; });
   }

   for (auto const& [key, vals] : values) {
      auto pairs = combine<own::grid::coord_2D_type>(vals);
      for (auto const& [first, second] : pairs) {
         auto distance = first - second;
         if (distance) {
            auto new_coord_1 = first - *distance;
            auto new_coord_2 = second + *distance;
            if (new_coord_1) if (grid.Valid(*new_coord_1)) grid[*new_coord_1] = '#';
            if (new_coord_2) if (grid.Valid(*new_coord_2)) grid[*new_coord_2] = '#';
         }
      }
   }

   grid.print(std::cout);

   size_t result_1 = 0, result_2 = 0;
   return { to_String(result_1), to_String(result_2) };
}
