#pragma once

// Project: Solutions for Advent of Code 2024
// file with Advent of Code 2024, 15t day : Warehouse Woes
// author: Volker Hillmann
// date:   29.12.2024
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.


#include "my_common_tools.h"

#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <regex>
#include <ranges>
#include <print>

using namespace std::string_literals;

namespace nsDay15 {

   enum class EDirections : char { upward = '^', rightward = '>', downward = 'v', leftward = '<' };
   enum class EWarehouse  : char { free = '.', wall = '#', box = 'O', used = '@' };


std::pair<std::string, std::string> SplitInput(std::string const& input) {
   auto pos = input.find("\n\n");
   if (pos == std::string::npos) throw std::invalid_argument("unexpected input for the day 15.");
   std::string first_text = input.substr(0, pos + 1);
   std::string last_text  = input.substr(pos + 2);
   return { first_text, last_text };
   }

inline std::pair<std::string, std::string> RiddleDay15(std::string&& input) {
   
   const own::grid::EKind grid_kind = own::grid::EKind::matrix;
   using grid_ty = own::grid::grid_2D<EWarehouse, grid_kind>;
   using coord_ty = typename grid_ty::coord_ty;
   using distance_ty = coord_ty::distance_ty;

   std::map<EDirections, distance_ty> directions = {
             { EDirections::upward,    { -1,  0 } },
             { EDirections::rightward, {  0,  1 } },
             { EDirections::downward,  {  1,  0 } },
             { EDirections::leftward,  {  0, -1 } }
            };

   auto [warehouse_text, direction_text] = SplitInput(input);

   const auto rows = std::ranges::count(warehouse_text, '\n');
   const auto cols = warehouse_text.size() / std::ranges::count(warehouse_text, '\n') - 1;

   warehouse_text.erase(std::ranges::remove(warehouse_text, '\n').begin(), warehouse_text.end());
   grid_ty grid(rows, cols);
   grid = warehouse_text;

   coord_ty roboter_pos{ grid, 0, 0 }, end_at = { grid, 0, 0 };
   if (auto pos = std::find(grid.begin(), grid.end(), EWarehouse::used); pos != grid.end()) {
      roboter_pos = grid.get_coords(pos);
      }

 
   for (EDirections direction : direction_text | std::views::filter([](char c) { return c != '\n'; }) 
                                               | std::views::transform([](char c) { return static_cast<EDirections>(c); })) {
      if(auto next = roboter_pos.Add(directions[direction]); next.has_value() && grid[*next] != EWarehouse::wall) {
         auto move_boxes = [&grid, &directions](auto pos, EDirections dir) -> std::optional<coord_ty> {
            do {
               pos = pos.value().Add(directions[dir]);
               } 
            while (pos.has_value() && grid[*pos] == EWarehouse::box);
            if (pos.has_value()) {
               if (grid[*pos] == EWarehouse::wall) return std::nullopt;
               else return *pos;
               }
            else return std::nullopt;
            };

         switch(grid[*next]) {
            case EWarehouse::free: 
               grid[roboter_pos] = EWarehouse::free;
               grid[*next] = EWarehouse::used;
               roboter_pos = *next;
               break;
            case EWarehouse::box:
               if(auto new_pos = move_boxes(next, direction); new_pos.has_value()) {
                  grid[*new_pos] = EWarehouse::box;
                  grid[roboter_pos] = EWarehouse::free;
                  grid[*next] = EWarehouse::used;
                  roboter_pos = *next;
                  }
               break;
            default:
               throw std::runtime_error("unexpected situation in warehouse.");
            ;
            }
         }
      }

   size_t result_1 = 0, result_2 = 0;
   for(auto pos = std::find(grid.begin(), grid.end(), EWarehouse::box); pos != grid.end(); pos = std::find(++pos, grid.end(), EWarehouse::box )) {
      auto coord = grid.get_coords(pos);
      result_1 += (coord.y() * 100 + coord.x());
      }

   grid.print(std::cout, 1);
   


   std::println(std::cout, "the result for the 1st part is {}", result_1);
   std::println(std::cout, "the result for the 2nd part is {}", result_2);
   return { to_String(result_1), to_String(result_2) };
   }

} // end of namespace nsDay15