#pragma once

// Project: Solutions for Advent of Code 2024
// file with Advent of Code 2024, 15t day : Warehouse Woes
// author: Volker Hillmann
// date:   15.12.2024
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.


#include "my_common_tools.h"
#include "my_grid2d.h"

#include <string>
#include <vector>
#include <queue>
#include <utility>
#include <algorithm>
#include <unordered_set>
#include <regex>
#include <execution>
#include <ranges>
#include <print>

namespace nsDay16 {

using namespace std::string_literals;


enum class EDirections : char { upward = '^', rightward = '>', downward = 'v', leftward = '<' };
enum class EMaze : char { free = '.', blocked = '#', used = 'x', start_at = 'S', end_at = 'E' };

bool operator==(EMaze lhs, EMaze rhs) {
   return static_cast<char>(lhs) == static_cast<char>(rhs);
}

bool operator!=(EMaze lhs, EMaze rhs) {
   return !(lhs == rhs);
}

bool operator<(EMaze lhs, EMaze rhs) {
   return static_cast<char>(lhs) < static_cast<char>(rhs);
}

template <own::grid::my_grid_ty grid_ty>
std::vector<std::vector<typename grid_ty::coord_ty>> dfs(grid_ty const& grid, typename grid_ty::coord_ty const& start_at, typename grid_ty::coord_ty const& end_at) {
   using coord_ty    = typename grid_ty::coord_ty;
   using distance_ty = coord_ty::distance_ty;

   std::map<EDirections, std::vector<EDirections>> directions = {
                 { EDirections::leftward,  { EDirections::leftward,  EDirections::upward,    EDirections::downward } },
                 { EDirections::rightward, { EDirections::rightward, EDirections::upward,    EDirections::downward } },
                 { EDirections::upward,    { EDirections::upward,    EDirections::rightward, EDirections::leftward } },
                 { EDirections::downward,  { EDirections::downward,  EDirections::rightward, EDirections::leftward } }
                 };

   std::map<EDirections, distance_ty> moves = { 
                 { EDirections::rightward, {  0,  1 } },
                 { EDirections::upward,    { -1,  0 } },
                 { EDirections::leftward,  {  0, -1 } },
                 { EDirections::downward,  {  1,  0 } }
                 };
   
   std::vector<std::vector<coord_ty>> components;
   
   std::stack<std::tuple<coord_ty, EDirections, std::vector<coord_ty>>> stack;
   stack.emplace(start_at, EDirections::rightward, std::vector<coord_ty> { start_at } );

   while (!stack.empty()) {
      auto [current, curr_dir, path] = std::move(stack.top());
      stack.pop();
      
      if(current == end_at) {
         components.emplace_back(path);
         continue;
         }

      for( auto const& dir : directions[curr_dir]) {
         if (auto next = current.Add(moves[dir]); next.has_value() && std::find(std::execution::par, path.begin(), path.end(), *next) == path.end() && grid[*next] != '#') {
            path.push_back(*next); 
            stack.emplace(*next, dir, path);
            }
         }
      }
   return components;
   }


template <own::grid::my_grid_ty grid_ty> requires own::grid::is_matrix<grid_ty::kind_of_grid()>
std::vector<typename grid_ty::coord_ty> bfs(grid_ty const& grid, typename grid_ty::coord_ty const& start_at, typename grid_ty::coord_ty const& end_at) {
   using coord_ty = typename grid_ty::coord_ty;
   using distance_ty = coord_ty::distance_ty;
   using hash_ty = typename grid_ty::coord_ty_hash;
   std::map<EDirections, distance_ty> directions = {
           { EDirections::upward,    {  0, -1 } }, { EDirections::rightward, {  1,  0 } },
           { EDirections::downward,  {  0,  1 } }, { EDirections::leftward,  { -1,  0 } } };

   std::unordered_set<coord_ty, hash_ty> visited;

   std::queue<std::pair<coord_ty, std::vector<coord_ty>>> q;
   q.emplace(start_at, std::vector<coord_ty>{ start_at });

   while (!q.empty()) {
      auto [pos, path] = q.front();
      q.pop();
      if (pos == end_at)  return path;

      for (auto const& [dir, move] : directions) {
         if (auto next = pos.Add(move); next.has_value() && grid[*next] != EMaze::blocked && visited.find(*next) == visited.end()) {
            visited.insert(*next);
            q.emplace(*next, std::vector<coord_ty>(path.begin(), path.end()));
            q.back().second.emplace_back(*next);
            }
         }
      }
   return {};
   }


template <own::grid::my_grid_ty grid_ty> requires own::grid::is_matrix<grid_ty::kind_of_grid()>
std::pair<std::vector<typename grid_ty::coord_ty>, int64_t> dijkstra(grid_ty const& grid, typename grid_ty::coord_ty const& start_at, EDirections start_dir, typename grid_ty::coord_ty const& end_at) {
   using coord_ty    = typename grid_ty::coord_ty;
   using distance_ty = coord_ty::distance_ty; 
   using hash_ty     = typename grid_ty::coord_ty_hash;
   std::unordered_set<coord_ty, hash_ty> attended;

   std::map<EDirections, distance_ty> possible_moves = {
             { EDirections::upward,    { -1,  0 } },
             { EDirections::rightward, {  0,  1 } },
             { EDirections::downward,  {  1,  0 } },
             { EDirections::leftward,  {  0, -1 } }
          };

   using weight_ty = int64_t;               // weight for cost

   std::map <EDirections, std::vector<std::pair<EDirections, weight_ty>>> weighted_moves = {
             { EDirections::upward,    { { EDirections::upward,    1 }, { EDirections::rightward, 1001 }, { EDirections::leftward,  1001 } } },
             { EDirections::rightward, { { EDirections::rightward, 1 }, { EDirections::downward,  1001 }, { EDirections::upward,    1001 } } },
             { EDirections::downward,  { { EDirections::downward,  1 }, { EDirections::leftward,  1001 }, { EDirections::rightward, 1001 } } },
             { EDirections::leftward,  { { EDirections::leftward,  1 }, { EDirections::upward,    1001 }, { EDirections::downward,  1001 } } }
          };


   struct WeightedNode {
      coord_ty              position;
      weight_ty             cost;
      std::vector<coord_ty> path;
      EDirections           direction;

      bool operator > (WeightedNode const& other) const {
         return cost > other.cost;
         }
      bool operator < (WeightedNode const& other) const {
         return cost < other.cost;
         }
      };

   std::priority_queue<WeightedNode, std::vector<WeightedNode>, std::greater<WeightedNode>> prio_queue;
   WeightedNode new_weighted_node = { start_at, 0, { start_at }, start_dir };
   prio_queue.emplace(std::move(new_weighted_node));

   while (!prio_queue.empty()) {
      WeightedNode at_the_time = prio_queue.top();
      prio_queue.pop();

      if (attended.find(at_the_time.position) != attended.end()) continue;
      else attended.insert(at_the_time.position);

      if (at_the_time.position == end_at) return { at_the_time.path, at_the_time.cost };

      for (auto const& [dir, cost] : weighted_moves[at_the_time.direction]) {
         if (auto next = at_the_time.position.Add(possible_moves[dir]); next.has_value()) {
            if(grid[*next] != EMaze::blocked && attended.find(*next) == attended.end()) {
               WeightedNode new_weighted_node = { *next, at_the_time.cost + cost, at_the_time.path, dir };
               new_weighted_node.path.emplace_back(std::move(*next));
               prio_queue.emplace(std::move(new_weighted_node));
               }
            }
         }
      }
   return {}; 
   }



inline std::pair<std::string, std::string> RiddleDay16(std::string&& text) {
   
   const auto rows = std::ranges::count(text, '\n');
   const auto cols = text.size() / std::ranges::count(text, '\n') - 1;
   const own::grid::EKind grid_kind = own::grid::EKind::matrix;
   using grid_ty = own::grid::grid_2D<EMaze, grid_kind>;
   using coord_ty = typename grid_ty::coord_ty;

   text.erase(std::ranges::remove(text, '\n').begin(), text.end());
   grid_ty grid(rows, cols);
   grid = text;

   coord_ty start_at { grid, 0, 0 }, end_at = { grid, 0, 0 };
   if (auto pos = std::find(grid.begin(), grid.end(), EMaze::start_at); pos != grid.end()) {
      start_at = grid.GetCoordinates(pos);
      }
   if (auto pos = std::find(grid.begin(), grid.end(), EMaze::end_at); pos != grid.end()) {
      end_at = grid.GetCoordinates(pos);
      }

   //*
   //auto test = bfs(grid, start_at, end_at);
   //for (auto const& position : test) grid[position] = 'x';
   auto test = dijkstra(grid, start_at, EDirections::rightward, end_at);
   // 73400 to high (before 76404: 441344)
   for (auto const& position : test.first) grid[position] = EMaze::used;
   grid.print(std::cout, 1);
   //*/
   //auto test = bfs(grid, start_at, end_at);

   size_t result_1 = test.second, result_2 = test.first.size();
   std::println(std::cout, "the result for the 1st part is {}", result_1);
   std::println(std::cout, "the result for the 2nd part is {}", result_2);
   return { to_String(result_1), to_String(result_2) };
   }

} // end of namespace nsDay16