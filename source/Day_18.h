#pragma once

// Project: Solutions for Advent of Code 2024
// file with Advent of Code 2024, 18th day : RAM Run
// author: Volker Hillmann
// date:   18.12.2024
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.

/*
Using the input method from Day 1 to parse pairs of integers into a std::vector.
Additionally, a grid_2D is used as a matrix (x, y) with an enum convertible to char.

After populating the grid with EMaze::free elements, a subset of input data coordinates 
is used to set specific grid cells to EMaze::blocked. Once again, we leverage our 
specialized grid_2D implementation.

To solve the task, a Breadth-First Search (BFS) algorithm is implemented to find the 
shortest path through the maze. Typically, a std::queue is used for traversal. 

While a std::set could be employed to track visited cells, we implement a hash method 
for grid_2D::coord_ty to take advantage of the more performant std::unordered_set.

Sources: https://en.wikipedia.org/wiki/Breadth-first_search
*/
#include "Day_01.h"

#include "my_common_tools.h"
#include "my_grid2d.h"

#include <string>
#include <utility>
#include <vector>
#include <queue>
#include <unordered_set>
#include <stdexcept>
#include <ranges>
#include <print>

namespace nsDay18 {

   enum class EDirections : char { upward = '^', rightward = '>', downward = 'v', leftward = '<' };
   enum class EMaze : char { free = '.', blocked = '#', used = 'x' };

   template <typename grid_ty>
   std::vector<typename grid_ty::coord_ty> bfs(const grid_ty& grid, const typename grid_ty::coord_ty& start_at, const typename grid_ty::coord_ty& end_at) {
      using coord_ty    = typename grid_ty::coord_ty;
      using distance_ty = coord_ty::distance_ty;
      using hash_ty     = typename grid_ty::coord_ty_hash;
      std::map<EDirections, distance_ty> directions = {
              { EDirections::upward,    {  0, -1 } }, { EDirections::rightward, {  1,  0 } },
              { EDirections::downward,  {  0,  1 } }, { EDirections::leftward,  { -1,  0 } }  };

      std::unordered_set<coord_ty, hash_ty> visited;
      std::queue<std::pair<coord_ty, std::vector<coord_ty>>> q; 
      q.emplace(start_at, std::vector<coord_ty>{ start_at });

      while (!q.empty()) {
         auto [pos, path] = q.front();
         q.pop();
         if (pos == end_at)  return path;

         for (const auto& [dir, move] : directions) {
            if (auto next = pos.Add(move); next.has_value() && grid[*next] != EMaze::blocked && visited.find(*next) == visited.end()) {
               visited.insert(*next);
               q.emplace(*next, std::vector<coord_ty>(path.begin(), path.end())); 
               q.back().second.emplace_back(*next);
               }
            }
         }
      return {};
      }

   template <std::ranges::input_range range_ty>
   std::pair<std::string, std::string> RiddleDay18(range_ty const& values) {
      
      using grid_ty = own::grid::grid_2D<EMaze, own::grid::EKind::matrix>;
      using coord_ty = typename grid_ty::coord_ty;

      auto data = values | std::ranges::views::filter([](auto const& p) { return p.size() > 0; })
                         | std::ranges::views::transform(nsDay01::parsePair<size_t, ',', 4>)
                         | std::ranges::to<std::vector>();

      auto max_x = std::ranges::max(data | std::views::elements<0>); 
      auto max_y = std::ranges::max(data | std::views::elements<1>);

      auto countElements = [&data]() { return data.size() > 50 ? 1024 : 12; }();

      grid_ty grid(max_x + 1, max_y + 1);
      std::ranges::fill(grid, EMaze::free);

      for (auto const& [x, y] : data | std::views::take(countElements)) {
         grid[x, y] = EMaze::blocked;
         }

      grid_ty::coord_ty start_at = { grid, 0, 0 };
      grid_ty::coord_ty end_at   = { grid, max_x, max_y };
      
      auto test = bfs(grid, start_at, end_at);

      size_t result_1 = test.size() - 1;
      std::string result_2;
    
      for (auto const& [x, y] : data | std::views::drop(countElements)) {
         grid[x, y] = EMaze::blocked;
         //std::println(std::cout, "{},{}", next.x(), next.y());
         if(bfs(grid, start_at, end_at).size() == 0) {
            result_2 = std::format("{},{}", x, y);
            break;
            }
         }

      std::println(std::cout, "the result for the 2nd part is {}", result_2);   
      std::println(std::clog, "18th day finished");
      return { to_String(result_1), to_String(result_2) };
      }

} // end of namespace nsDay18
