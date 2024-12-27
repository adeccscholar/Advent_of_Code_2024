#pragma once

// Project: Solutions for Advent of Code 2024
// file with Advent of Code 2024, 6th day : Guard Gallivant
// author: Volker Hillmann
// date:   06.12.2024, last change: 15.12.2024
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.

// adoption of the template solution, array take unique_ptr now
// preparation of the 2nd part took back

#include "my_grid2d.h"

#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <functional>
#include <memory>
#include <tuple>
#include <array>
#include <unordered_set>
#include <ranges>
#include <print>

namespace nsDay06 {

enum class EDirections : char {
   upward = '^', right = '>', downward = 'v', left = '<'
  };


template <own::grid::my_grid_ty grid_ty>
void step_func(grid_ty const& grid, typename grid_ty::coord_ty const& coords, EDirections direction,
               std::vector<std::tuple<typename grid_ty::coord_ty, EDirections, bool>>& path) {
   using coord_ty = typename grid_ty::coord_ty;
   using path_ty = std::tuple<typename grid_ty::coord_ty, EDirections, bool>;
   coord_ty element(coords);
   //if (std::ranges::find_if(path, [&coords](auto const& elem) { return std::get<0>(elem) == coords; }) == path.end()) {
      path.emplace_back(path_ty { element, direction, false });
   //   }
   //else path.emplace_back(path_ty { element, direction, true });;
   }

const size_t UseLogLevel = 3;

template <size_t LogLevel = UseLogLevel>
inline std::pair<std::string, std::string> RiddleDay6(std::string&& text) { 
   const auto rows = std::ranges::count(text, '\n');
   const auto cols = text.size() / std::ranges::count(text, '\n') - 1;
   const own::grid::EKind grid_kind = own::grid::EKind::grid;
   using grid_ty  = own::grid::grid_2D<char, grid_kind>;
   using coord_ty = typename grid_ty::coord_ty;
   using hash_ty = typename grid_ty::coord_ty_hash;
   
   text.erase(std::ranges::remove(text, '\n').begin(), text.end());
   grid_ty grid(rows, cols);
   grid = text;

   auto horizontal = own::grid::find_horz_paths<char, own::grid::EKind::grid>(grid);
   auto vertical = own::grid::find_vert_paths<char, grid_kind>(grid);
   
   using path_ty = std::tuple<coord_ty, EDirections, bool>;
   std::vector<path_ty> path;

   auto turn_right = [](EDirections dir) -> EDirections {
      static std::map<EDirections, EDirections> rules = {
               { EDirections::upward ,  EDirections::right },
               { EDirections::right,    EDirections::downward },
               { EDirections::downward, EDirections::left },
               { EDirections::left,     EDirections::upward }
               };
      return rules[dir];
      };

 
   using move_ret_type = std::tuple<size_t, size_t, bool>;
   std::map<EDirections, std::function<move_ret_type(size_t row, size_t col, bool edge)>> moves_func = {
      { EDirections::upward, [&grid, &vertical, &path](size_t row, size_t col, bool edge) -> move_ret_type {
              grid_ty::path_view view(grid, vertical[col]);
              auto start = view.rpos(row);
              if (auto it = std::find(start, view.rend(), '#'); it != view.rend()) {
                 for (auto iter = start; iter != it; ++iter) {
                    step_func(grid, (iter.base() - 1).get_coords(), EDirections::upward, path); // from reverse
                    }
                 auto const& new_row = (it.base() - 1).get_coords().row();
                 auto const& new_col = (it.base() - 1).get_coords().col();
                 return { new_row + 1, new_col, true };
                 }
              else {
                 for (auto iter = start; iter != it; ++iter) {
                    step_func<grid_ty>(grid, (iter.base() - 1).get_coords(), EDirections::upward, path); // from reverse
                    }
                 return { 0, col, false };
                 }
              }},
      { EDirections::right, [&grid, &horizontal, &path](size_t row, size_t col, bool edge) -> move_ret_type {
              grid_ty::path_view view(grid, horizontal[row]);
              auto start = view.pos(col);
              if (auto it = std::find(start, view.end(), '#'); it != view.end()) {
                 for (auto iter = start + 1; iter != it; ++iter)  { step_func<grid_ty>(grid, iter.get_coords(), EDirections::right, path); };
                 auto const& new_row = it.get_coords().row();
                 auto const& new_col = it.get_coords().col();
                 return { new_row, new_col - 1, true };
                 }
              else {
                 for (auto iter = start + 1; iter != it; ++iter) { step_func<grid_ty>(grid, iter.get_coords(), EDirections::right, path); };
                 return { row, grid.cols(), false };
                 }
              }},
      { EDirections::downward, [&grid, &vertical, &path](size_t row, size_t col, bool edge) -> move_ret_type {
              grid_ty::path_view view(grid, vertical[col]);
              auto start = view.pos(row);
              if (auto it = std::find(start, view.end(), '#'); it != view.end()) {
                 for (auto iter = start + 1; iter != it; ++iter)  { step_func<grid_ty>(grid, iter.get_coords(), EDirections::downward, path); }
                 auto const& new_row = it.get_coords().row();
                 auto const& new_col = it.get_coords().col();
                 return { new_row - 1, new_col, true };
                 } 
              else {
                 for (auto iter = start + 1; iter != it; ++iter) { step_func<grid_ty>(grid, iter.get_coords(), EDirections::downward, path); }
                 return { grid.rows(), col, false };
                 }
              }},
      { EDirections::left, [&grid, &horizontal, &path](size_t row, size_t col, bool edge) -> move_ret_type {
              grid_ty::path_view view(grid, horizontal[row]);
              auto start = view.rpos(col);
              if (auto it = std::find(start, view.rend(), '#'); it != view.rend()) {
                 for (auto iter = start; iter != it; ++iter) { step_func<grid_ty>(grid, (iter.base() - 1).get_coords(), EDirections::left, path); }; // from reverse
                 auto const& new_row = (it.base() - 1).get_coords().row();
                 auto const& new_col = (it.base() - 1).get_coords().col();
                 return { new_row, new_col + 1, true };
                 }
              else {
                 for (auto iter = start; iter != it; ++iter) { step_func<grid_ty>(grid, (iter.base() - 1).get_coords(), EDirections::left, path); } // from reverse
                 return { 0, col, false };
                 }
              } }
      };

   size_t result_1 = 0, result_2 = 0;

   if(auto pos = std::find(grid.begin(), grid.end(), '^'); pos != grid.end()) {
      auto const& c_row = grid(std::distance(grid.begin(), pos)).row();
      auto const& c_col = grid(std::distance(grid.begin(), pos)).col();
      path.emplace_back( path_ty { coord_ty { grid, c_row, c_col }, EDirections::upward, false });
      EDirections move = EDirections::upward;
      auto [row_, col_, ok_] = moves_func[move](c_row, c_col, true);
      for(; ok_; std::tie(row_, col_, ok_) = moves_func[move](row_, col_, true)) {
         //edges.insert({{ grid, row_, col_ }, move });
         move = turn_right(move);
         }
     }
  // 236 to low



   //if constexpr (LogLevel > 3) for (auto const& p : path) std::println(std::cout, "{}", p);
   std::unordered_set<coord_ty, hash_ty> counter;
   for (auto const& [coord, dir, edge] : path) {
      counter.insert(coord);
      grid[coord] = static_cast<char>(dir);
      }
   //if constexpr (LogLevel > 1) {
      grid.print(std::cout, 1);
      std::println(std::cout, "");
   //   }
   
   result_1 = counter.size();
   result_2 = path.size(); // std::ranges::count_if(path, [](auto const& e) { return std::get<2>(e) == true; });
   std::println(std::cout, "the result for the 1st part is {}", result_1);
   std::println(std::cout, "the result for the 2nd part is {}", result_2);
   return { to_String(result_1), to_String(result_2) };
   }

} // end of namespace nsDay06
