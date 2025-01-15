#pragma once

// Project: Solutions for Advent of Code 2024
// file with Advent of Code, 10th day : Hoof It
// author: Volker Hillmann
// date:   10.12.204
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.

#include "my_common_tools.h"
#include "my_grid2d.h"

#include <string>
#include <utility>
#include <stack>
#include <vector>
#include <ranges>

namespace nsDay10 {

   const own::grid::EKind grid_kind = own::grid::EKind::grid;

   using grid_ty     = own::grid::grid_2D<char, own::grid::EKind::grid>;
   using coord_ty    = typename grid_ty::coord_ty;
   using distance_ty = coord_ty::distance_ty;

   std::vector<distance_ty> directions = { {  0, -1 }, {  0,  1 }, {  1,  0 }, { -1,  0 } };

   template <bool AS_VECTOR>
   std::conditional_t<AS_VECTOR, std::vector<coord_ty>, std::set<coord_ty>> FindPath(grid_ty const& grid, coord_ty const& start_at) {
      using return_ty = typename std::conditional<AS_VECTOR, std::vector<coord_ty>, std::set<coord_ty>>::type;
      return_ty results;
      std::stack<coord_ty> stack;
      stack.emplace(start_at);
      while (!stack.empty()) {
         auto current = stack.top();
         stack.pop();
         if(grid[current] == '9') {
            if constexpr (AS_VECTOR) results.emplace_back(std::move(current));
            else                     results.emplace(std::move(current));
            continue;
            }
         for (auto const& dir : directions) {
            if (auto next = current.Add(dir); next.has_value() && grid[*next] == grid[current] + 1) {
               stack.emplace(*next);
               }
            }
         }
      return results;
      }


   std::pair<std::string, std::string> RiddleDay10(std::string&& text) {
      const auto rows = std::ranges::count(text, '\n');
      const auto cols = text.size() / std::ranges::count(text, '\n') - 1;
      text.erase(std::ranges::remove(text, '\n').begin(), text.end());
      grid_ty grid(rows, cols);
      grid = text;

      //grid.print(std::cout, 1);

      std::vector<coord_ty> start_positions;
      for (auto pos = std::find(grid.begin(), grid.end(), '0'); pos != grid.end(); pos = std::find(pos + 1, grid.end(), '0')) {
         start_positions.emplace_back(grid.get_coords(pos));
         }

      size_t result_1 = 0, result_2 = 0;
      for(auto const& start_at : start_positions) {
         result_1 += FindPath<false>(grid, start_at).size();
         result_2 += FindPath<true>(grid, start_at).size();
         }

      std::println(std::cout, "the result for the 1st part is {}", result_1);
      std::println(std::cout, "the result for the 2nd part is {}", result_2);

      return { to_String(result_1), to_String(result_2) };
   }

}; // end of namespace nsDay
