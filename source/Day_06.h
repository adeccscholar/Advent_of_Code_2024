#pragma once

// Project: Solutions for Advent of Code 2024
// file with Advent of Code 2024, 6th day : Guard Gallivant
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.

#include "my_grid2d.h"

#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <functional>
#include <tuple>
#include <array>
#include <ranges>
#include <print>

enum class EDirections : char {
   upward = '^', right = '>', downward = 'v', left = '<'
  };


bool is_rectangle(std::array<own::grid::coord_2D_type, 4> const& points) {
   std::array<size_t, 4> rows, cols;
   for (size_t i = 0; i < 4; ++i) {
      //std::tie(rows[i], cols[i]) = points[i];
      rows[i] = std::get<0>(points[i]);
      cols[i] = std::get<1>(points[i]);
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

template <typename ty, size_t SIZE>
std::vector<std::array<ty, SIZE>> combination(std::span<ty> values) {
   std::vector<std::array<ty, SIZE>> result;
   if (values.size() < SIZE) return result;

   std::array<size_t, SIZE> indices;
   std::iota(indices.begin(), indices.end(), 0);

   do {
      std::array<ty, SIZE> combination;
      for (size_t i = 0; i < SIZE; ++i) combination[i] = values[indices[i]];
      result.emplace_back(combination);
      } 
   while (next_combination<size_t, SIZE>(indices, values.size()));
   return result;
   }


//template <std::ranges::input_range range_ty>
std::pair<std::string, std::string> RiddleDay6(std::string&& text) { // range_ty const& values
   const auto rows = std::ranges::count(text, '\n');
   const auto cols = text.size() / std::ranges::count(text, '\n') - 1;
   text.erase(std::ranges::remove(text, '\n').begin(), text.end());
   own::grid::grid_2D<char> grid(rows, cols);
   grid = text;

   auto horizontal = own::grid::find_horz_paths(grid);
   auto vertical = own::grid::find_vert_paths(grid);
   std::vector<own::grid::coord_2D_type> path;

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
      { EDirections::upward, [&grid, &vertical, &path](size_t row, size_t col) -> move_ret_type {
              own::grid::grid2d_view<char> view(grid, vertical[col]);
              auto start = view.rpos(row);
              if (auto it = std::find(start, view.rend(), '#'); it != view.rend()) {
                 for (auto step = start; step < it; ++step) { *step = 'O';  };
                 auto const& [new_row, new_col] = (it.base() - 1).get_coords();  // base the nearest Element, current is protected
                 return { new_row + 1, new_col, true };
                 }
              else {
                 for (auto step = start; step < it; ++step) { *step = 'O'; };
                 return { 0, col, false };
                 }
              }},
      { EDirections::right, [&grid, &horizontal, &path](size_t row, size_t col) -> move_ret_type {
              own::grid::grid2d_view<char> view(grid, horizontal[row]);
              auto start = view.pos(col);
              if (auto it = std::find(start, view.end(), '#'); it != view.end()) {
                 for (auto step = start; step < it; ++step) { *step = 'O'; };
                 auto const& [new_row, new_col] = it.get_coords();  
                 return { new_row, new_col - 1, true };
                 }
              else {
                 for (auto step = start; step < it; ++step) { *step = 'O'; };
                 return { row, grid.cols(), false };
                 }
              }},
      { EDirections::downward, [&grid, &vertical, &path](size_t row, size_t col) -> move_ret_type {
              own::grid::grid2d_view<char> view(grid, vertical[col]);
              auto start = view.pos(row);
              if (auto it = std::find(start, view.end(), '#'); it != view.end()) {
                 for (auto step = start; step < it; ++step) { *step = 'O'; };
                 auto const& [new_row, new_col] = it.get_coords();  
                 return { new_row - 1, new_col, true };
                 } 
              else {
                 for (auto step = start; step < it; ++step) { *step = 'O'; };
                 return { grid.rows(), col, false };
                 }
              }},
      { EDirections::left, [&grid, &horizontal, &path](size_t row, size_t col) -> move_ret_type {
              own::grid::grid2d_view<char> view(grid, horizontal[row]);
              auto start = view.rpos(col);
              if (auto it = std::find(start, view.rend(), '#'); it != view.rend()) {
                 for (auto step = start; step < it; ++step) { *step = 'O'; };
                 auto const& [new_row, new_col] = (it.base() - 1).get_coords();  // base the nearest Element, current is protected
                 return { new_row, new_col + 1, true };
                 }
              else {
                 for (auto step = start; step < it; ++step) { *step = 'O'; };
                 return { 0, col, false };
                 }
              } }
      };

   if(auto pos = std::find(grid.begin(), grid.end(), '^'); pos != grid.end()) {
      auto const& [c_row, c_col] = grid(std::distance(grid.begin(), pos));
      //*pos = 'O';
      EDirections move = EDirections::upward;
      path.emplace_back(own::grid::coord_2D_type{ c_row, c_col });
      auto [row_, col_, ok_] = moves_func[move](c_row, c_col);
      for(; ok_; std::tie(row_, col_, ok_) = moves_func[move](row_, col_)) {
         path.emplace_back(own::grid::coord_2D_type{ row_, col_ });
         move = turn_right(move);
         }
      path.emplace_back(own::grid::coord_2D_type{ row_, col_ });

      auto res2 = combination<own::grid::coord_2D_type, 4>(path)
                      | std::views::filter([](auto const& v) { return is_rectangle(v); })
                      | std::ranges::to<std::vector>();
      for(auto const& val : res2) 
         std::apply([](auto&&... args) { return std::println(std::cout, "{} {} {} {}", args...); }, val);
      //grid.print(std::cout);
      }



   size_t result_1 = 0, result_2 = 0;
   result_1 = std::count(grid.begin(), grid.end(), 'O');
   std::println(std::cout, "the result for the 1st part is {}", result_1);
   return { to_String(result_1), to_String(result_2) };
   }
