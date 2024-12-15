#pragma once

// Project: Solutions for Advent of Code 2024
// file with Advent of Code, 8th day : Resonant Collinearity
// author: Volker Hillmann
// date:   08.12.2024, changed 15.12.2024
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.

// integration of the template solution for grids
// grid_ty::coord_ty need grid ref Parameter now

#include "my_common_tools.h"
#include "my_grid2d.h"

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <algorithm>
#include <ranges>

template <typename grid_ty, typename ty> 
std::vector<std::pair<ty, ty>> combine(grid_ty const& grid, std::vector<ty> const& elements) {
   //using grid_ty = own::grid::grid_2D<char, own::grid::EKind::grid>;
   using coord_ty = typename grid_ty::coord_ty;
   const size_t n = elements.size();
   const size_t pair_count = (n * (n - 1)) / 2;
   std::vector<std::pair<coord_ty, coord_ty>> result;
   result.reserve(pair_count);

   for (auto i = 0u; i < elements.size(); ++i) {
      for (auto j = i + 1; j < elements.size(); ++j) {
         result.emplace_back( elements[i] , elements[j]);
      }
   }
   return result;
}


inline std::pair<std::string, std::string> RiddleDay8(std::string&& text) { 
   using grid_ty = own::grid::grid_2D<char, own::grid::EKind::grid>;
   using coord_ty = typename grid_ty::coord_ty;
   const auto rows = std::ranges::count(text, '\n');
   const auto cols = text.size() / std::ranges::count(text, '\n') - 1;
   text.erase(std::ranges::remove(text, '\n').begin(), text.end());
   grid_ty grid(rows, cols);
   grid = text;

   std::map<char, std::vector<coord_ty>> values;
   auto current = grid.begin();
   auto it = std::find_if(current, grid.end(), [](char c) { return c != '.'; });
   while (it != grid.end()) {
      values[*it].emplace_back(grid.GetCoordinates(it));
      it = std::find_if(++it, grid.end(), [](char c) { return c != '.'; });
   }

   for (auto const& [key, vals] : values) {
      auto pairs = combine<grid_ty, coord_ty>(grid, vals);
      for (auto const& [first, second] : pairs) {
         auto distance = first - second;
         /*
         if (distance) {
            auto new_coord_1 = first - *distance;
            auto new_coord_2 = second + *distance;
            if (new_coord_1) if (grid.Valid(*new_coord_1)) grid[*new_coord_1] = '#';
            if (new_coord_2) if (grid.Valid(*new_coord_2)) grid[*new_coord_2] = '#';
         }
         */
      }
   }

   //grid.print(std::cout);

   size_t result_1 = 0, result_2 = 0;
   return { to_String(result_1), to_String(result_2) };
}
