// Project: Solutions for Advent of Code 2024
// file with test data and solutions for the days, test function
// author: Volker Hillmann
// date:   05.12.2024, last change: 15.12.2024
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.

#include "processes_aoc2024.h"

#include "qt_form.h"
#include "my_common_tools.h"
#include "my_grid2d.h"

#include <algorithm>
#include <cmath>
#include <utility>
#include <iterator>
#include <stdexcept>
#include <string_view>
#include <ranges>
#include <mdspan>
#include <print>

#include "my_grid2d.h"


using namespace std::string_literals;
using namespace std::string_view_literals;

TAOC2024Processes::testDatas TAOC2024Processes::test = {
   {  1, { "3   4\n4   3\n2   5\n1   3\n3   9\n3   3"s, { { 11, 31 } } } },
   {  2, { "7 6 4 2 1\n1 2 7 8 9\n9 7 6 2 1\n1 3 2 4 5\n8 6 4 4 1\n1 3 6 7 9"s, { { 2, 4 } } } },
   {  3, { "xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))"s, { { 161, 48 } } } },
   {  4, { "MMMSXXMASM\nMSAMXMSMSA\nAMXSXMAAMM\nMSAMASMSMX\nXMASAMXAMM\nXXAMMXXAMA\nSMSMSASXSS\n"s +
           "SAXAMASAAA\nMAMMMXMMMM\nMXMXAXMASX"s, { { 18, 9 } } } },
   {  5, { "47|53\n97|13\n97|61\n97|47\n75|29\n61|13\n75|53\n29|13\n97|29\n53|29\n61|53\n97|53\n"s +
           "61|29\n47|13\n75|47\n97|75\n47|61\n75|61\n47|29\n75|13\n53|13\n"s +
           "\n"s +
           "75,47,61,53,29\n97,61,53,29,13\n75,29,13\n75,97,47,61,53\n61,13,29\n97,13,75,29,47"s, { { 143, 123 } } } },
   {  6, { "....#.....\n.........#\n..........\n..#.......\n.......#..\n..........\n.#..^.....\n"s + 
           "........#.\n#.........\n......#..."s, { { 41, 6 } } } },
   {  7, { "190: 10 19\n3267: 81 40 27\n83 : 17 5\n156 : 15 6\n7290 : 6 8 6 15\n161011 : 16 10 13\n"s +
           "192 : 17 8 14\n21037 : 9 7 18 13\n292 : 11 6 16 20"s, { { 3749, 11387 } } }  },
   {  8, { "............\n........0...\n.....0......\n.......0....\n....0.......\n......A.....\n"s +
           "............\n............\n........A...\n.........A..\n............\n............"s, { { 14, 34 } } } },
   {  9, { "2333133121414131402"s, { { 1928, 0 } } } },
   { 10, { "89010123\n78121874\n87430965\n96549874\n45678903\n32019012\n01329801\n10456732"s, { { 36, 0 } } } },
   { 11, { "125 17"s, { { 55312, 0 } } } },
   { 12, { "RRRRIICCFF\nRRRRIICCCF\nVVRRRCCFFF\nVVRCCCJFFF\nVVVVCJJCFE\nVVIVCCJJEE\n"s +
           "VVIIICJJEE\nMIIIIIJJEE\nMIIISIJEEE\nMMMISSJEEE"s, { { 1930, 1206 } } } },
   { 13, { "Button A : X + 94, Y + 34\nButton B : X + 22, Y + 67\nPrize : X = 8400, Y = 5400\n\n"s +
           "Button A : X + 26, Y + 66\nButton B : X + 67, Y + 21\nPrize : X = 12748, Y = 12176\n\n"s +
           "Button A : X + 17, Y + 86\nButton B : X + 84, Y + 37\nPrize : X = 7870, Y = 6450\n\n"s +
           "Button A : X + 69, Y + 23\nButton B : X + 27, Y + 71\nPrize : X = 18641, Y = 10279" , { { 36, 0 } } } },
   { 14, { "p=0,4 v=3,-3\np=6,3 v=-1,3\np=10,3 v=-1,2\np=2,0 v=2,-1\np=0,0 v=1,3\n" +
           "p=3,0 v=-2,-2\np=7,6 v=-1,-3\np=3,0 v=-1,-2\np=9,3 v=2,3\np=7,3 v=-1,2\n"s +
           "p=2,4 v=2,-3\np=9,5 v=-3,-3"s, { { 12, 0 } } } },
   { 15, { "##########\n#..O..O.O#\n#......O.#\n#.OO..O.O#\n#..O@..O.#\n"s +
           "#O#..O...#\n#O..O..O.#\n#.OO.O.OO#\n#....O...#\n##########\n\n"s +
           "<vv>^<v^>v>^vv^v>v<>v^v<v<^vv<<<^><<><>>v<vvv<>^v^>^<<<><<v<<<v^vv^v>^\n"s +
           "vvv<<^>^v^^><<>>><>^<<><^vv^^<>vvv<>><^^v>^>vv<>v<<<<v<^v>^<^^>>>^<v<v\n"s +
           "><>vv>v^v^<>><>>>><^^>vv>v<^^^>>v^v^<^^>v^^>v^<^v>v<>>v^v^<v>v^^<^^vv<\n"s +
           "<<v<^>>^^^^>>>v^<>vvv^><v<<<>^^^vv^<vvv>^>v<^^^^v<>^>vvvv><>>v^<<^^^^^\n"s +
           "^><^><>>><>^^<<^^v>>><^<v>^<vv>>v>>>^v><>^v><<<<v>>v<v<v>vvv>^<><<>^><\n"s +
           "^>><>^v<><^vvv<^^<><v<<<<<><^v<<<><<<^^<v<^^^><^>>^<v^><<<^>>^v<v^v<v^\n"s +
           ">^>>^v>vv>^<<^v<>><<><<v<<v><>v<^vv<<<>^^v^>^^>>><<^v>>v^v><^^>>^<>vv^\n"s +
           "<><^^>^^^<><vvvvv^v<v<<>^v<v>v<<^><<><<><<<^^<<<^<<>><<><^^^>^^<>^>v<>\n"s +
           "^^>vv<^v^v<vv>^<><v<^v>^^^>>>^^vvv^>vvv<>>>^<^>>>>>^<<^v>^vvv<>^<><<v>\n"s +
           "v^^>>><<^^<>>^v^<v^vv<>v^<<>^<^v^v><^<<<><<^<v><v<>vv>>v><v^<vv<>v^<<^"s , { { 10092, 0 } } } },
   { 16, { "#################\n#...#...#...#..E#\n#.#.#.#.#.#.#.#.#\n#.#.#.#...#...#.#\n"s +
           "#.#.#.#.###.#.#.#\n#...#.#.#.....#.#\n#.#.#.#.#.#####.#\n#.#...#.#.#.....#\n"s +
           "#.#.#####.#.###.#\n#.#.#.......#...#\n#.#.###.#####.###\n#.#.#...#.....#.#\n"s +
           "#.#.#.#####.###.#\n#.#.#.........#.#\n#.#.#.#########.#\n#S#.............#\n"s +
           "#################"s , { { 11048, 0 } } } }
   };



template <own::grid::my_grid_ty grid_ty, typename ty>
std::vector<std::pair<ty, ty>> combine(grid_ty const& grid, std::vector<ty> const& elements) {
   //using grid_ty = own::grid::grid_2D<char, own::grid::EKind::grid>;
   using coord_ty = typename grid_ty::coord_ty;
   const size_t n = elements.size();
   const size_t pair_count = (n * (n - 1)) / 2;
   std::vector<std::pair<coord_ty, coord_ty>> result;
   result.reserve(pair_count);

   for (auto i = 0u; i < elements.size(); ++i) {
      for (auto j = i + 1; j < elements.size(); ++j) {
         result.emplace_back(elements[i], elements[j]);
      }
   }
   return result;
}


void TAOC2024Processes::TestCase(TMyForm&& frm) {

   using grid_ty = own::grid::grid_2D<char, own::grid::EKind::grid>;
   using coord_ty = typename grid_ty::coord_ty;

   frm.SetLabel("lblEvaluation"s, ""s);

   std::string text = std::get<0>(test[8]) + "\n";
   using grid_ty = own::grid::grid_2D<char, own::grid::EKind::grid>;
   using coord_ty = typename grid_ty::coord_ty;
   const auto rows = std::ranges::count(text, '\n');
   const auto cols = text.size() / std::ranges::count(text, '\n') - 1;
   text.erase(std::ranges::remove(text, '\n').begin(), text.end());
   grid_ty grid(rows, cols);
   grid = text;

   grid.print(std::cout, 1);

   auto not_empty = [](char c) -> bool { return c != '.'; };

   std::map<char, std::vector<coord_ty>> values;
   for (auto it = std::find_if(grid.begin(), grid.end(), not_empty); it != grid.end();
         it = std::find_if(++it, grid.end(), not_empty)) {
      values[*it].emplace_back(grid.GetCoordinates(it));
      }

   for (auto const& [key, vals] : values) {
      auto pairs = combine<grid_ty, coord_ty>(grid, vals);
      for (auto const& [first, second] : pairs) {
         auto distance = second - first;
         if (auto new_coord_1 = first  - distance; new_coord_1.has_value() && grid[*new_coord_1] == '.' ) grid[*new_coord_1] = '#';
         if (auto new_coord_2 = second + distance; new_coord_2.has_value() && grid[*new_coord_2] == '.' ) grid[*new_coord_2] = '#';
        }
      }

   size_t result_1 = std::ranges::count(grid, '#');
   grid.print(std::cout, 1);

   auto a = grid.GetCoordinates(2, 3);
   auto b = grid.GetCoordinates(5, 7);
   auto c = grid.GetCoordinates(1, 1);
   auto d = b - a;

   auto n1 = c + d;
   auto n2 = c - d;
   std::println(std::cout, "test");

   }