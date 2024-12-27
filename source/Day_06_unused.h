#pragma once

// Project: Solutions for Advent of Code 2024
// file with Advent of Code 2024, 6th day : Guard Gallivant
// unused code form tries to solve day 6 with a general algorithm
// author: Volker Hillmann
// date:   21.12.2024
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.



#include "my_grid2d.h"

#include <stdexcept>
#include <vector>
#include <array>
#include <algorithm>
#include <ranges>


bool is_rectangle(std::array<size_t, 4>& rows, std::array<size_t, 4>& cols) {
   std::ranges::sort(rows);
   std::ranges::sort(cols);
   auto last_row = std::unique(rows.begin(), rows.end());
   auto last_col = std::unique(cols.begin(), cols.end());
   size_t unique_rows = std::distance(rows.begin(), last_row);
   size_t unique_cols = std::distance(cols.begin(), last_col);
   return unique_rows == 2 && unique_cols == 2;
}


template <own::grid::my_grid_ty grid_ty>
bool coords_to_rectangle(std::vector<typename grid_ty::coord_ty> const& points) {
   std::array<size_t, 4> rows, cols;
   if (points.size() != 4) throw std::invalid_argument("size of input for is_rectancle isn't matching");
   for (size_t i = 0; i < 4; ++i) {
      rows[i] = points[i].row();
      cols[i] = points[i].col();
   }
   return is_rectangle(rows, cols);
}

template <own::grid::my_grid_ty grid_ty>
bool point_to_rectangle(typename grid_ty::coord_ty const& edge, std::vector<typename grid_ty::coord_ty> const& points) {
   std::array<size_t, 4> rows, cols;
   if (points.size() != 3) throw std::invalid_argument("size of input for is_rectancle isn't matching");
   rows[0] = edge.row(); cols[0] = edge.col();
   for (size_t i = 1; i < 4; ++i) {
      rows[i] = points[i - 1].row(); cols[i] = points[i - 1].col();
   }
   return is_rectangle(rows, cols);
}


template <typename ty, size_t SIZE>
bool next_combination(std::vector<ty>& indices, size_t n) {
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
std::vector<std::vector<typename grid_ty::coord_ty>> combination(grid_ty const& grid, std::span<typename grid_ty::coord_ty> values) {
   std::vector<std::vector<typename grid_ty::coord_ty>> result;
   if (values.size() < SIZE) return result;

   std::vector<size_t> indices;
   indices.resize(SIZE);
   std::iota(indices.begin(), indices.end(), 0);

   do {
      std::vector<typename grid_ty::coord_ty> combination;
      for (size_t i = 0; i < SIZE; ++i)
         combination.emplace_back(grid, values[indices[i]]);
      result.emplace_back(combination);
   } while (next_combination<size_t, SIZE>(indices, values.size()));
   return result;
}

/*
  auto neighborhood = [&grid, &path, &edges](path_ty const& position) -> bool {
      static std::map<EDirections,std::tuple<grid_ty::coord_ty::distance_ty, EDirections>> neighbors = {
               { EDirections::upward,   { {  0,  1 }, EDirections::right }},
               { EDirections::right,    { { -1,  0 }, EDirections::downward }},
               { EDirections::downward, { {  0, -1 }, EDirections::left }},
               { EDirections::left,     { {  0, -1 }, EDirections::upward }} };
      coord_ty coord        = std::get<0>(position);
      EDirections direction = std::get<1>(position);
         auto neighbor = coord.Add(std::get<0>(neighbors[direction]));
         auto seek_dir = std::get<1>(neighbors[direction]);
         return std::ranges::find_if(path, [&path, &neighbor, &seek_dir](auto const& elem) {

                          return std::get<0>(elem) == neighbor && std::get<1>(elem) == seek_dir;
                          }) != path.end();
      };



      auto targets = path | std::views::filter(neighborhood) | std::ranges::to<std::vector>();

*/