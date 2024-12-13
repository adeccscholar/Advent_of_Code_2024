#pragma once

// Project: Solutions for Advent of Code 2024
// file with Advent of Code, 10th day : Hoof It
// author: Volker Hillmann
// date:   10.12.204
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.

#include "my_common_tools.h"
#include "my_grid2d.h"

#include <string>
#include <utility>
#include <ranges>

std::pair<std::string, std::string> RiddleDay10(std::string&& text) { 
   const auto rows = std::ranges::count(text, '\n');
   const auto cols = text.size() / std::ranges::count(text, '\n') - 1;
   text.erase(std::ranges::remove(text, '\n').begin(), text.end());
   own::grid::grid_2D<char> grid(rows, cols);
   grid = text;



   size_t result_1 = 0, result_2 = 0;
   return { to_String(result_1), to_String(result_2) };
   }
