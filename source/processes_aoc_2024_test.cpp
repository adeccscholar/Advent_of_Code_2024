// Project: Solutions for Advent of Code 2024
// file with test data and solutions for the days, test function
// author: Volker Hillmann
// date:   05.12.2024, last change: 19.12.2024
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

bool TAOC2024Processes::boHasTestButton = true;

void TAOC2024Processes::TestCase(TMyForm&& frm) {
   std::println(std::cout, "test {}", 100000000 % 16777216);

   std::println(std::cout, "test");

   }


TAOC2024Processes::testDatas TAOC2024Processes::test = {
   {  1, { "3   4\n4   3\n2   5\n1   3\n3   9\n3   3"s, 
           testData {       11u,    31u  } } },
   {  2, { "7 6 4 2 1\n1 2 7 8 9\n9 7 6 2 1\n1 3 2 4 5\n8 6 4 4 1\n1 3 6 7 9"s, 
           testData {        2u,     4u } } },
   {  3, { "xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))"s, 
           testData {      161u,    48u } } },
   {  4, { "MMMSXXMASM\nMSAMXMSMSA\nAMXSXMAAMM\nMSAMASMSMX\nXMASAMXAMM\nXXAMMXXAMA\nSMSMSASXSS\n"s +
           "SAXAMASAAA\nMAMMMXMMMM\nMXMXAXMASX"s, 
           testData {       18u,     9u } } },
   {  5, { "47|53\n97|13\n97|61\n97|47\n75|29\n61|13\n75|53\n29|13\n97|29\n53|29\n61|53\n97|53\n"s +
           "61|29\n47|13\n75|47\n97|75\n47|61\n75|61\n47|29\n75|13\n53|13\n"s +
           "\n"s +
           "75,47,61,53,29\n97,61,53,29,13\n75,29,13\n75,97,47,61,53\n61,13,29\n97,13,75,29,47"s, 
           testData {      143u,   123u } } },
   {  6, { "....#.....\n.........#\n..........\n..#.......\n.......#..\n..........\n.#..^.....\n"s +
           "........#.\n#.........\n......#..."s, 
           testData {       41u,     6u } } },
   {  7, { "190: 10 19\n3267: 81 40 27\n83 : 17 5\n156 : 15 6\n7290 : 6 8 6 15\n161011 : 16 10 13\n"s +
           "192 : 17 8 14\n21037 : 9 7 18 13\n292 : 11 6 16 20"s, 
           testData {     3749u, 11387u  } } },
   {  8, { "............\n........0...\n.....0......\n.......0....\n....0.......\n......A.....\n"s +
           "............\n............\n........A...\n.........A..\n............\n............"s, 
           testData {       14u,    34u } } },
   {  9, { "2333133121414131402"s, 
           testData {     1928u,     0u } } },
   { 10, { "89010123\n78121874\n87430965\n96549874\n45678903\n32019012\n01329801\n10456732"s, 
           testData {       36u,     0u } } },
   { 11, { "125 17"s, 
           testData {    55312u,     0u } } },
   { 12, { "RRRRIICCFF\nRRRRIICCCF\nVVRRRCCFFF\nVVRCCCJFFF\nVVVVCJJCFE\nVVIVCCJJEE\n"s +
           "VVIIICJJEE\nMIIIIIJJEE\nMIIISIJEEE\nMMMISSJEEE"s, 
           testData {     1930u,  1206u } } },
   { 13, { "Button A : X + 94, Y + 34\nButton B : X + 22, Y + 67\nPrize : X = 8400, Y = 5400\n\n"s +
           "Button A : X + 26, Y + 66\nButton B : X + 67, Y + 21\nPrize : X = 12748, Y = 12176\n\n"s +
           "Button A : X + 17, Y + 86\nButton B : X + 84, Y + 37\nPrize : X = 7870, Y = 6450\n\n"s +
           "Button A : X + 69, Y + 23\nButton B : X + 27, Y + 71\nPrize : X = 18641, Y = 10279" , 
           testData {       36u,     0u } } },
   { 14, { "p=0,4 v=3,-3\np=6,3 v=-1,3\np=10,3 v=-1,2\np=2,0 v=2,-1\np=0,0 v=1,3\n" +
           "p=3,0 v=-2,-2\np=7,6 v=-1,-3\np=3,0 v=-1,-2\np=9,3 v=2,3\np=7,3 v=-1,2\n"s +
           "p=2,4 v=2,-3\np=9,5 v=-3,-3"s, 
           testData {       12u,  std::nullopt } } },
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
           "v^^>>><<^^<>>^v^<v^vv<>v^<<>^<^v^v><^<<<><<^<v><v<>vv>>v><v^<vv<>v^<<^"s , 
           testData {    10092u,     0u } } },
   { 16, { "#################\n#...#...#...#..E#\n#.#.#.#.#.#.#.#.#\n#.#.#.#...#...#.#\n"s +
           "#.#.#.#.###.#.#.#\n#...#.#.#.....#.#\n#.#.#.#.#.#####.#\n#.#...#.#.#.....#\n"s +
           "#.#.#####.#.###.#\n#.#.#.......#...#\n#.#.###.#####.###\n#.#.#...#.....#.#\n"s +
           "#.#.#.#####.###.#\n#.#.#.........#.#\n#.#.#.#########.#\n#S#.............#\n"s +
           "#################"s , 
           testData {    11048u,     0u } } },
   { 17, { "Register A: 729\nRegister B : 0\nRegister C : 0\n\nProgram : 0,1,5,4,3,0"s , 
           testData { "4,6,3,5,6,3,5,2,1,0"s, std::nullopt } } },
   { 18, { "5,4\n4,2\n4,5\n3,0\n2,1\n6,3\n2,4\n1,5\n0,6\n3,3\n2,6\n5,1\n1,2\n5,5\n"s +
           "2,5\n6,5\n1,4\n0,4\n6,4\n1,1\n6,1\n1,0\n0,5\n1,6\n2,0"s, 
           testData {       22u, { "6,1"s } } } },
   { 19, { "r, wr, b, g, bwu, rb, gb, br\n\nbrwrr\nbggr\ngbbr\nrrbgbr\nubwu\nbwurrg\nbrgr\nbbrgwb"s , 
           testData {        6u,  std::nullopt } } },
   { 20, { "###############\n#...#...#.....#\n#.#.#.#.#.###.#\n#S#...#.#.#...#\n#######.#.#.###\n"s +
           "#######.#.#...#\n#######.#.###.#\n###..E#...#...#\n###.#######.###\n#...###...#...#\n"s +
           "#.#####.#.###.#\n#.#...#.#.#...#\n#.#.#.#.#.#.###\n#...#...#...###\n###############"s,
           testData {       64u,  std::nullopt } } },
   { 21, { "029A\n980A\n179A\n456A\n379A"s,
           testData {   126384u,  std::nullopt } } },
   { 22, { "1\n10\n100\n2024"s,
           testData { 37327623u,  24u } } },
   { 23, { "kh-tc\nqp-kh\nde-cg\nyn-aq\nqp-ub\ncg-tb\nvc-aq\ntb-ka\nwh-tc\nyn-cg\nkh-ub\nta-co\n"s +
           "de-co\ntc-td\ntb-wq\nwh-td\nta-ka\ntd-qp\naq-cg\nwq-ub\nub-vc\nde-ta\nwq-aq\nwq-vc\n"s +
           "wh-yn\nka-de\nkh-ta\nco-tc\nwh-qp\ntb-vc\ntd-yn"s,
           testData {        7u,  std::nullopt } } },
   { 24, { "x00: 1\nx01: 0\nx02: 1\nx03: 1\nx04: 0\ny00: 1\ny01: 1\ny02: 1\ny03: 1\ny04: 1\n\n"s +
           "ntg XOR fgs -> mjb\ny02 OR x01 -> tnw\nkwq OR kpj -> z05\nx00 OR x03 -> fst\ntgd XOR rvg -> z01\n"s +
           "vdt OR tnw -> bfw\nbfw AND frj -> z10\nffh OR nrd -> bqk\ny00 AND y03 -> djm\ny03 OR y00 -> psh\n"s +
           "bqk OR frj -> z08\ntnw OR fst -> frj\ngnj AND tgd -> z11\nbfw XOR mjb -> z00\nx03 OR x00 -> vdt\n"s +
           "gnj AND wpb -> z02\nx04 AND y00 -> kjc\ndjm OR pbm -> qhw\nnrd AND vdt -> hwm\nkjc AND fst -> rvg\n"s +
           "y04 OR y02 -> fgs\ny01 AND x02 -> pbm\nntg OR kjc -> kwq\npsh XOR fgs -> tgd\nqhw XOR tgd -> z09\n"s +
           "pbm OR djm -> kpj\nx03 XOR y03 -> ffh\nx00 XOR y04 -> ntg\nbfw OR bqk -> z06\nnrd XOR fgs -> wpb\n"s +
           "frj XOR qhw -> z04\nbqk OR frj -> z07\ny03 OR x01 -> nrd\nhwm AND bqk -> z03\ntgd XOR rvg -> z12\n"s +
           "tnw OR pbm -> gnj"s,
           testData {     2024u,  std::nullopt } } },
   { 25, { "#####\n.####\n.####\n.####\n.#.#.\n.#...\n.....\n\n"s +
           "#####\n##.##\n.#.##\n...##\n...#.\n...#.\n.....\n\n"s +
           ".....\n#....\n#....\n#...#\n#.#.#\n#.###\n#####\n\n"s +
           ".....\n.....\n#.#..\n###..\n###.#\n###.#\n#####\n\n"s +
           ".....\n.....\n.....\n#....\n#.#..\n#.#.#\n#####"s,
           testData {        3u,  std::nullopt } } }
   };


void TAOC2024Processes::CleanResults(TMyForm& frm) {
   frm.SetEdit({ "edtResult_1"s, "edtResult_2"s }, ""s);
   frm.SetLabel("lblEvaluation"s, ""s);
   }

void TAOC2024Processes::CheckResults(TMyForm& frm, size_t day) {
   if (auto it = test.find(day); it != test.end() && frm.GetCheckBox("chkTestData"s)) {
      bool check1 = false, check2 = false;
      
      auto visitor = [](auto const& value) {
         using used_ty = std::decay_t<decltype(value)>;
         if constexpr (std::is_same_v<used_ty, size_t>) {
            return value;
            }
         else if constexpr (std::is_same_v<used_ty, std::string>) {
            return value;
            }
         };

      auto checking = [&frm, &day, &visitor](std::optional<testValue> const& value, std::string const& field, size_t part, bool& checked) {
         if (value.has_value()) {
            auto visitor = [&frm, &field, &part, &checked](auto const& value) {
               using used_ty = std::decay_t<decltype(value)>;
               if constexpr (std::is_same_v<used_ty, size_t>) {
                  size_t result = toInt<size_t>(frm.GetEditAs(field));
                  checked = result == value;
                  std::println(std::cout, " {} part: {} {} {}", part == 1 ? "1st" : "2nd", result, (checked ? "==" : "!="), value);
                  }
               else if constexpr (std::is_same_v<used_ty, std::string>) {
                  std::string result;
                  frm.GetEdit(field, result);
                  checked = result == value;
                  std::println(std::cout, " {} part: {} {} {}", part == 1 ? "1st" : "2nd", result, (checked ? "==" : "!="), value);
                  }
               };

            std::visit(visitor, *value);
         }
         else { 
            std::println(std::cout, " {} part: result can't validated because there isn't a test value.", part == 1 ? "1st" : "2nd");
            checked = false;
            }
         };



      auto& [testVal1, testVal2] = std::get<1>(it->second);

      std::println(std::cout, "");
      std::println(std::cout, "Checking the solution with the test values");

      checking(testVal1, "edtResult_1"s, 1, check1); 
      checking(testVal2, "edtResult_2"s, 2, check2);

      
      if (check1) {
         if (check2) {
            frm.SetLabel("lblEvaluation"s, "Both values are valid."s);
            std::println(std::cout, "Congratulations! Both results correspond to the expected values.");
            }
         else {
            frm.SetLabel("lblEvaluation"s, "Only 1st value is valid."s);
            std::println(std::cout, "Only the value for the first part correspond to the expected value.");
            }
         }
      else {
         if(check2) {
            frm.SetLabel("lblEvaluation"s, "Only 2nd value is valid."s);
            std::println(std::cout, "There is some wrong, only the value for the second part correspond to the expected value.");
            }
         else {
            frm.SetLabel("lblEvaluation"s, "None of the values are checkable or valid."s);
            std::println(std::cout, "There is some wrong, none of the values is actually correspond to the expected values.");
            }
         }
      }
   }

