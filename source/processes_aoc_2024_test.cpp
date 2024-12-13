// Project: Solutions for Advent of Code 2024
// file with test data and solutions for the days, test function
// author: Volker Hillmann
// date:   05.12.2024, last change: 13.12.2024
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

#include "Day_08.h"
#include "my_dynamic_array.h"


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
           "............\n............\n........A...\n.........A..\n............\n............"s, { { 14, 0 } } } },
   {  9, { "2333133121414131402"s, { { 1928, 0 } } } },
   { 10, { "89010123\n78121874\n87430965\n96549874\n45678903\n32019012\n01329801\n10456732"s, { { 36, 0 } } } },
   { 11, { "125 17"s, { { 55312, 0 } } } },
   { 12, { "RRRRIICCFF\nRRRRIICCCF\nVVRRRCCFFF\nVVRCCCJFFF\nVVVVCJJCFE\nVVIVCCJJEE\n"s +
           "VVIIICJJEE\nMIIIIIJJEE\nMIIISIJEEE\nMMMISSJEEE"s, { { 0, 0 } } } },
   { 13, { "Button A : X + 94, Y + 34\nButton B : X + 22, Y + 67\nPrize : X = 8400, Y = 5400\n\n"s +
           "Button A : X + 26, Y + 66\nButton B : X + 67, Y + 21\nPrize : X = 12748, Y = 12176\n\n"s +
           "Button A : X + 17, Y + 86\nButton B : X + 84, Y + 37\nPrize : X = 7870, Y = 6450\n\n"s +
           "Button A : X + 69, Y + 23\nButton B : X + 27, Y + 71\nPrize : X = 18641, Y = 10279" , { { 36, 0 } } } }
   };





void TAOC2024Processes::TestCase(TMyForm&& frm) {
   frm.SetLabel("lblEvaluation"s, ""s);




   }