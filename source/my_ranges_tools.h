#pragma once

// Project: Solutions for Advent of Code 2024
// own help function for ranges
// example to find and isolate basic functionality for different problems
// this classes are part of the adecc Scholars repositories and separated for this project
// 
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.



#include <ranges>

namespace own {

   struct first_view {
      template <std::ranges::input_range range_ty>
      auto operator () (range_ty&& r) const {
         return std::forward<range_ty>(r) | std::views::transform([](auto const& e) { return e.first; });
         }

      template <std::ranges::input_range range_ty>
      friend auto operator | (range_ty&& r, first_view const& view) {
         return view(std::forward<range_ty>(r));
         }
      };

   struct second_view {
      template <std::ranges::input_range range_ty>
      auto operator () (range_ty&& r) const {
         return std::forward<range_ty>(r) | std::views::transform([](auto const& e) { return e.second; });
         }

      template <std::ranges::input_range range_ty>
      friend auto operator | (range_ty&& r, second_view const& view) {
         return view(std::forward<range_ty>(r));
         }
      };

   namespace views {
      inline constexpr auto first = first_view{};
      inline constexpr auto second = second_view{};
      }
};
