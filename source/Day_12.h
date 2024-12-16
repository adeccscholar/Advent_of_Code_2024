#pragma once

// Project: Solutions for Advent of Code 2024
// file with Advent of Code 2024, 12th day : Garden Groups
// author: Volker Hillmann
// date:   12.12.2024
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.

#include "my_common_tools.h"

#include <iostream>
#include <cmath>
#include <string>
#include <utility>
#include <ranges>
#include <print>

namespace nsDay12 {

const own::grid::EKind grid_kind = own::grid::EKind::grid;
using grid_ty  = own::grid::grid_2D<char, own::grid::EKind::grid>;
using coord_ty = typename grid_ty::coord_ty;
using area_ty  = std::vector<typename grid_ty::coord_ty>;
using areas_ty = std::vector<area_ty>;


void dfs(grid_ty const& grid, int32_t row, int32_t col, char value, area_ty& component, std::vector<bool>& visited) {
   if (row < 0 || row >= grid.rows() || col < 0 || col >= grid.cols() ||
       visited[row * grid.cols() + col] || grid[row, col] != value) {
       return;
      }

   visited[row * grid.cols() + col] = true;
   component.emplace_back(coord_ty{ grid,  static_cast<uint64_t>(row), static_cast<uint64_t>(col) });

   dfs(grid, row - 1, col, value, component, visited); 
   dfs(grid, row + 1, col, value, component, visited); 
   dfs(grid, row, col - 1, value, component, visited); 
   dfs(grid, row, col + 1, value, component, visited); 
   }

// 4 - connected region labeling mit dfs
areas_ty find_connected_4_components(grid_ty const& grid) {
   areas_ty components;
   std::vector<bool> visited(grid.rows() * grid.cols(), false);

      for (int row = 0; row < grid.rows(); ++row) {
         for (int col = 0; col < grid.cols(); ++col) {
            if (!visited[row * grid.cols() + col]) {
               area_ty component;
               dfs(grid, row, col, grid[row, col], component, visited);
               components.emplace_back(component);
               }
            }
         }

      return components;
      }

uint64_t calculate_edge_lengths(grid_ty const& grid, area_ty const& comp) {
   uint64_t edge_length = 0;

   const std::vector<std::pair<uint64_t, uint64_t>> directions = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };

   char cValue = grid[comp[0]];

   for (const auto& cell : comp) {
      int row = cell.row();
      int col = cell.col();


      for (const auto& dir : directions) {
         uint64_t newRow = row + dir.first;
         uint64_t newCol = col + dir.second;

         if (newRow < 0 || newRow >= grid.rows() || newCol < 0 || newCol >= grid.cols())  ++edge_length;
         else if(grid[newRow, newCol] != cValue) ++edge_length;
         }
      }
   return edge_length;
   }

inline std::pair<std::string, std::string> RiddleDay12(std::string&& text) {
   auto rows = std::ranges::count(text, '\n');
   auto cols = text.size() / rows - 1;
   text.erase(std::ranges::remove(text, '\n').begin(), text.end());

   grid_ty vault(rows, cols);
   vault = text;

   auto results = find_connected_4_components(vault);

   size_t result_1 = 0;
   for(auto const& result : results) {
      auto S = vault[result[0].row(), result[0].col()];
      auto size = calculate_edge_lengths(vault, result);
      auto check = result.size() * calculate_edge_lengths(vault, result);
      //std::println(std::cout, "{}: {} * {} = {}", S, result.size(), size, check);
      result_1 += check;
      }
      
   size_t result_2 = 0;

   return { to_String(result_1), to_String(result_2) };
   }

} // end of namespace