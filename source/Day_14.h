#pragma once

// Project: Solutions for Advent of Code 2024
// file with Advent of Code 2024, 14th day : Restroom Redoubt
// author: Volker Hillmann
// date:   30.12.2024
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.

#include "my_common_tools.h"
#include "my_grid2d.h"

#include <string>
#include <utility>
#include <functional>
#include <span>
#include <ranges>
#include <print>

using namespace std::string_literals;

namespace nsDay14 {

   

   using grid_ty = own::grid::grid_2D<char, own::grid::EKind::matrix>;
   using coord_ty = typename grid_ty::coord_ty;
   using distance_ty = typename coord_ty::distance_ty;

   //const grid_ty::size_ty m = 11, n = 7, m2 = 5, n2 = 3;
   const grid_ty::size_ty m = 101, n = 103, m2 = 50, n2 = 51;

   struct robot_ty {
      static grid_ty inline range = grid_ty(m, n);    // test 11 wide and 7 tall, task later 101 wide and 103 tall
      using size_ty = typename grid_ty::size_ty;
      coord_ty    position;
      distance_ty velocity;

      robot_ty(std::tuple<size_ty, size_ty> c, distance_ty v) : position { range, c }, velocity { v } { }
   };


bool checkLine(std::string const& line) {
   static std::regex regex(R"(^p=(0|([1-9][0-9]{0,3})),(0|([1-9][0-9]{0,3})) v=(-?(0|([1-9][0-9]{0,3}))),(-?(0|([1-9][0-9]{0,3}))))");
   std::smatch match;
   bool ret = std::regex_match(line, match, regex);
   return ret;
   }

template <my_integral_ty pos_ty, my_integral_ty move_ty>
//std::tuple<pos_ty, pos_ty, move_ty, move_ty> parseLine(std::string const& input) {
robot_ty parseLine(std::string const& input) {
   static std::regex regex(R"(^p=(0|([1-9][0-9]{0,3})),(0|([1-9][0-9]{0,3})) v=(-?(0|([1-9][0-9]{0,3}))),(-?(0|([1-9][0-9]{0,3}))))");
   std::smatch match;
   if (std::regex_match(input, match, regex)) [[likely]] {
      auto p1 = toInt<pos_ty> (match[1].str());
      auto p2 = toInt<pos_ty> (match[3].str());
      auto v1 = toInt<move_ty>(match[5].str());
      auto v2 = toInt<move_ty>(match[8].str());
      return { { p2, p1 }, { v2, v1 } }; // direct use of data_ty for coord_ty need twist 
      }
   else {
      throw std::invalid_argument("unexpected input: "s + input);
      }
   }


template <std::ranges::input_range range_ty>
std::pair<std::string, std::string> RiddleDay14(range_ty const& values) {
   size_t result_1 = 0, result_2 = 0;



   auto robots = values | std::views::transform(to_String<std::string_view>)
                        | std::views::filter(checkLine) 
                        | std::views::transform(parseLine<grid_ty::size_ty, grid_ty::int_ty>) 
                        | std::ranges::to<std::vector>();
 
   //for (auto const& [px, py, vx, vy] : input)
   //   std::println(std::cout, "p={:>3},{:>3} v={:>3},{:>3}", px, py, vx, vy);

   for(auto i : std::views::iota(1) | std::views::take(100)) {
      for(auto& robot : robots) {
         //std::print(std::cout, "from position ({}, {}) with ({}, {}) ", robot.position.x(), robot.position.y(),
         //                                   robot.velocity.delta_x(), robot.velocity.delta_y());
         if (auto newPos = robot.position.Add(robot.velocity); newPos.has_value()) {
            robot.position = *newPos;
            }
         else {
            auto position = newPos.error().first;
            auto distance = newPos.error().second;

            auto new_x = distance.delta_x() == 0 ? position.x() :
                             robot.velocity.delta_x() < 0 ? robot.position.x_dim() + distance.delta_x() : distance.delta_x() - 1;
            auto new_y = distance.delta_y() == 0 ? position.y() :
                             robot.velocity.delta_y() < 0 ? robot.position.y_dim() + distance.delta_y() : distance.delta_y() - 1;
            robot.position = { new_y, new_x }; // twist while use data_ty directly
            }
         //std::println(std::cout, "to position ({}, {}) after {} sec", robot.position.x(), robot.position.y(), i);
         }
      }

   std::array<uint64_t, 4> quarters = { 0, 0, 0, 0 };
   for (auto& robot : robots) {
      if(robot.position.x() < m2) {
         if (robot.position.y() < n2) ++quarters[0];
         else if (robot.position.y() > n2) ++quarters[2];
         }
      else if (robot.position.x() > m2) {
         if (robot.position.y() < n2) ++quarters[1];
         else if (robot.position.y() > n2) ++quarters[3];
         }
      //std::println(std::cout, "position ({}, {})", robot.position.x(), robot.position.y());
      }

   result_1 = std::ranges::fold_left(quarters, 1ull, std::multiplies<>{});

   std::println(std::cout, "the result for the 1st part is {}", result_1);
   std::println(std::cout, "the result for the 2nd part is {}", result_2);

   return { to_String(result_1), to_String(result_2) };
   }

} // end of namespace nsDay14