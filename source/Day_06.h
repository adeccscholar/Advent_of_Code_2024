﻿#pragma once

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
#include <ranges>
#include <print>

namespace nsDay06 {

enum class EDirections : char {
   upward = '^', right = '>', downward = 'v', left = '<'
  };

template <own::grid::my_grid_ty grid_ty>
bool is_rectangle(std::array<std::unique_ptr<typename grid_ty::coord_ty>, 4> const& points) {
   std::array<size_t, 4> rows, cols;
   for (size_t i = 0; i < 4; ++i) {
      if(points[i]) {
         rows[i] = (*points[i]).row();
         cols[i] = (*points[i]).col();
         }
      }

   std::ranges::sort(rows);
   std::ranges::sort(cols);

   auto last_row = std::unique(rows.begin(), rows.end());
   auto last_col = std::unique(cols.begin(), cols.end());

   size_t unique_rows = std::distance(rows.begin(), last_row);
   size_t unique_cols = std::distance(cols.begin(), last_col);

   return unique_rows == 2 && unique_cols == 2;
   }

template <typename ty, size_t SIZE>
bool next_combination(std::array<ty, SIZE>& indices, size_t n) {
   size_t k = SIZE;
   for (size_t i = k; i-- > 0;) {
      if (indices[i] != i + n - k) {
         ++indices[i];
         for (size_t j = i + 1; j < k; ++j) {
            indices[j] = indices[j - 1] + 1;
            }
         return true;
         }
      }
   return false;
   }

template <own::grid::my_grid_ty grid_ty, size_t SIZE>
std::vector<std::array<std::unique_ptr<typename grid_ty::coord_ty>, SIZE>> combination(grid_ty const& grid, std::span<typename grid_ty::coord_ty> values) {
   std::vector<std::array<std::unique_ptr<typename grid_ty::coord_ty>, SIZE>> result;
   if (values.size() < SIZE) return result;

   std::array<size_t, SIZE> indices;
   std::iota(indices.begin(), indices.end(), 0);

   do {
      typename grid_ty::coord_ty empty = { grid, 0, 0 };
      std::array<std::unique_ptr<typename grid_ty::coord_ty>, SIZE> combination;
      for (size_t i = 0; i < SIZE; ++i) 
         combination[i] = std::make_unique<typename grid_ty::coord_ty>( grid, values[indices[i]]);
      result.emplace_back(combination);
      } 
   while (next_combination<size_t, SIZE>(indices, values.size()));
   return result;
   }

template <own::grid::my_grid_ty grid_ty>
void step_func(typename grid_ty::coord_ty const& coords,
               std::vector<typename grid_ty::coord_ty>& edges,
               std::vector<typename grid_ty::coord_ty>& path) {
   if (std::ranges::find_if(path, [&edges, &coords](auto const& elem) { return elem == coords; }) != path.end()) {
      if (std::ranges::find_if(edges, [&coords](auto const& elem) { return elem == coords; }) == edges.end()) {
          edges.emplace_back(coords);
         }
      }
   else path.emplace_back(coords);
   }

template <own::grid::my_grid_ty grid_ty>
void step_rev_func(typename grid_ty::coord_ty const& coords,
                   std::vector<typename grid_ty::coord_ty>& edges,
                   std::vector<typename grid_ty::coord_ty>& path) {   
   //auto coords = iter.base().get_coords();
   if (std::ranges::find_if(path, [&edges, &coords](auto const& elem) { return elem == coords; }) != path.end()) {
      edges.emplace_back(coords);
      }
   else path.emplace_back(coords);
   }


const size_t UseLogLevel = 0;

template <size_t LogLevel = UseLogLevel>
inline std::pair<std::string, std::string> RiddleDay6(std::string&& text) { 
   const auto rows = std::ranges::count(text, '\n');
   const auto cols = text.size() / std::ranges::count(text, '\n') - 1;
   const own::grid::EKind grid_kind = own::grid::EKind::grid;
   using grid_ty  = own::grid::grid_2D<char, grid_kind>;
   using coord_ty = typename grid_ty::coord_ty;
   
   text.erase(std::ranges::remove(text, '\n').begin(), text.end());
   grid_ty grid(rows, cols);
   grid = text;

   auto horizontal = own::grid::find_horz_paths<char, own::grid::EKind::grid>(grid);
   auto vertical = own::grid::find_vert_paths<char, grid_kind>(grid);
   
   std::vector<coord_ty> edges;
   std::vector<coord_ty> path;

   auto turn_right = [](EDirections dir) -> EDirections {
      static std::map<EDirections, EDirections> rules = {
               { EDirections::upward , EDirections::right },
               { EDirections::right,   EDirections::downward },
               { EDirections::downward, EDirections::left },
               { EDirections::left, EDirections::upward }
               };
      return rules[dir];
      };

 

   using move_ret_type = std::tuple<size_t, size_t, bool>;
   std::map<EDirections, std::function<move_ret_type(size_t row, size_t col)>> moves_func = {
      { EDirections::upward, [&grid, &vertical, &edges, &path](size_t row, size_t col) -> move_ret_type {
              grid_ty::path_view view(grid, vertical[col]);
              auto start = view.rpos(row);
              if (auto it = std::find(start, view.rend(), '#'); it != view.rend()) {
                 for (auto iter = start; iter != it; ++iter) {
                    step_rev_func<grid_ty>((iter.base() - 1).get_coords(), edges, path);
                    }
                 auto const& new_row = (it.base() - 1).get_coords().row();
                 auto const& new_col = (it.base() - 1).get_coords().col();
                 return { new_row + 1, new_col, true };
                 }
              else {
                 for (auto iter = start; iter != it; ++iter) {
                    step_rev_func<grid_ty>((iter.base() - 1).get_coords(), edges, path);
                    }
                 return { 0, col, false };
                 }
              }},
      { EDirections::right, [&grid, &horizontal, &edges, &path](size_t row, size_t col) -> move_ret_type {
              grid_ty::path_view view(grid, horizontal[row]);
              auto start = view.pos(col);
              if (auto it = std::find(start, view.end(), '#'); it != view.end()) {
                 for (auto iter = start + 1; iter != it; ++iter)  { step_func<grid_ty>(iter.get_coords(), edges, path); };
                 auto const& new_row = it.get_coords().row();
                 auto const& new_col = it.get_coords().col();
                 return { new_row, new_col - 1, true };
                 }
              else {
                 for (auto iter = start + 1; iter != it; ++iter) { step_func<grid_ty>(iter.get_coords(), edges, path); };
                 return { row, grid.cols(), false };
                 }
              }},
      { EDirections::downward, [&grid, &vertical, &edges, &path](size_t row, size_t col) -> move_ret_type {
              grid_ty::path_view view(grid, vertical[col]);
              auto start = view.pos(row);
              if (auto it = std::find(start, view.end(), '#'); it != view.end()) {
                 for (auto iter = start + 1; iter != it; ++iter)  { step_func<grid_ty>(iter.get_coords(), edges, path); }
                 auto const& new_row = it.get_coords().row();
                 auto const& new_col = it.get_coords().col();
                 return { new_row - 1, new_col, true };
                 } 
              else {
                 for (auto iter = start + 1; iter != it; ++iter) { step_func<grid_ty>(iter.get_coords(), edges, path); }
                 return { grid.rows(), col, false };
                 }
              }},
      { EDirections::left, [&grid, &horizontal, &edges, &path](size_t row, size_t col) -> move_ret_type {
              grid_ty::path_view view(grid, horizontal[row]);
              auto start = view.rpos(col);
              if (auto it = std::find(start, view.rend(), '#'); it != view.rend()) {
                 for (auto iter = start; iter != it; ++iter) { step_rev_func<grid_ty>((iter.base() - 1).get_coords(), edges, path); };
                 auto const& new_row = (it.base() - 1).get_coords().row();
                 auto const& new_col = (it.base() - 1).get_coords().col();
                 return { new_row, new_col + 1, true };
                 }
              else {
                 for (auto iter = start; iter != it; ++iter) { step_rev_func<grid_ty>((iter.base() - 1).get_coords(), edges, path); }
                 return { 0, col, false };
                 }
              } }
      };

   if(auto pos = std::find(grid.begin(), grid.end(), '^'); pos != grid.end()) {
      auto const& c_row = grid(std::distance(grid.begin(), pos)).row();
      auto const& c_col = grid(std::distance(grid.begin(), pos)).col();
      path.emplace_back(coord_ty{ grid, c_row, c_col });
      EDirections move = EDirections::upward;
      //path.emplace_back(coord_ty{ grid, c_row, c_col });
      auto [row_, col_, ok_] = moves_func[move](c_row, c_col);
      for(; ok_; std::tie(row_, col_, ok_) = moves_func[move](row_, col_)) {
         edges.emplace_back(coord_ty{ grid, row_, col_ });
         move = turn_right(move);
         }
      //path.emplace_back(coord_ty { grid, row_, col_ });

      /*
      auto res2 = combination<grid_ty, 4>(grid, edges)
                      | std::views::filter([&grid](auto const& v) { return is_rectangle<grid_ty>(*); })
                      | std::ranges::to<std::vector>();

      */
      /*
      if constexpr (LogLevel > 2) {
      for(auto const& val : res2) 
         std::apply([](auto&&... args) { return std::println(std::cout, "{} {} {} {}", args...); }, val);
         }

      if constexpr (LogLevel > 4) grid.print(std::cout);
   */   
     }
  

   //if constexpr (LogLevel > 3) for (auto const& p : path) std::println(std::cout, "{}", p);
   size_t result_1 = 0, result_2 = 0;
   result_1 = path.size();
   std::println(std::cout, "the result for the 1st part is {}", result_1);
   return { to_String(result_1), to_String(result_2) };
   }

} // end of namespace nsDay06
