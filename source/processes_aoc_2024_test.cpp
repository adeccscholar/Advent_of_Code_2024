#include "processes_aoc2024.h"

#include "qt_form.h"
#include "my_common_tools.h"
#include "my_grid_view.h"

#include <string_view>
#include <algorithm>
#include <utility>
#include <iterator>
#include <stdexcept>
#include <ranges>
#include <mdspan>
#include <print>

using namespace std::string_literals;
using namespace std::string_view_literals;

TAOC2024Processes::testDatas TAOC2024Processes::test = {
   { 1, "3   4\n4   3\n2   5\n1   3\n3   9\n3   3"s },
   { 2, "7 6 4 2 1\n1 2 7 8 9\n9 7 6 2 1\n1 3 2 4 5\n8 6 4 4 1\n1 3 6 7 9"s },
   { 3, "xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))"s },
   { 4, "MMMSXXMASM\nMSAMXMSMSA\nAMXSXMAAMM\nMSAMASMSMX\nXMASAMXAMM\nXXAMMXXAMA\nSMSMSASXSS\nSAXAMASAAA\nMAMMMXMMMM\nMXMXAXMASX"s },
   { 5, "47|53\n97|13\n97|61\n97|47\n75|29\n61|13\n75|53\n29|13\n97|29\n53|29\n61|53\n97|53\n"s +
        "61|29\n47|13\n75|47\n97|75\n47|61\n75|61\n47|29\n75|13\n53|13\n"s +
        "\n"s +
        "75,47,61,53,29\n97,61,53,29,13\n75,29,13\n75,97,47,61,53\n61,13,29\n97,13,75,29,47"s }
   };





void TAOC2024Processes::TestCase(TMyForm&& frm) {
   auto input = frm.GetLines("memInput");
   auto rules   = input | std::views::transform(toString<std::string_view>)
                        | std::views::filter(checkSeparatedIntegersPairs)
                        | std::views::transform([](auto const& p) { return parseSeparatedPairs<size_t>({ p.begin(), p.end() });})
                        | std::ranges::to<std::set>();
   auto updates = input | std::views::transform(toString<std::string_view>)
                        | std::views::filter(checkCommaSeparatedIntegers)
                        | std::views::transform([](auto const& p) { return parseCommaSeparatedIntegers<size_t>({ p.begin(), p.end() });})
                        | std::ranges::to<std::vector>();
  
   auto check = [&rules](std::vector<size_t> const& line) -> bool {
      for (auto const& pair_vals : line | std::views::slide(2)) {
         std::vector<size_t> slidevals;
         for (auto elem : pair_vals) slidevals.emplace_back(elem);
         if (rules.find(std::make_tuple(slidevals[0], slidevals[1])) == rules.end() &&
            rules.find(std::make_tuple(slidevals[1], slidevals[0])) != rules.end()) return false;
         }
      return true;
      };



   size_t result_1 = 0;
   for(auto const& update : updates ) {
      if (check(update) && update.size() % 2 != 0) result_1 += update[update.size() / 2];
      }

   std::println(std::cout, "solution is {}", result_1);

   size_t result_2 = 0;
   static auto swap_elements = [](size_t& val1, size_t& val2) {
      auto tmp = val1;
      val1 = val2;
      val2 = tmp;
      };

   std::vector<size_t> results;
   for (auto& update : updates | std::views::filter([&rules, &check](auto const& vec) { return !check(vec); }) ) {
      do {
         for (auto const& vals : std::ranges::views::zip(std::views::iota(1) | std::views::take(update.size() - 1), std::views::iota(0))) {
            auto& [idx, prev_idx] = vals;
            size_t& element = update[idx];
            size_t& prev = update[prev_idx];
            if (rules.find(std::make_tuple(prev, element)) == rules.end() &&
               rules.find(std::make_tuple(element, prev)) != rules.end()) swap_elements(element, prev);
            }
         } 
      while (!check(update));
      result_2 += update[update.size() / 2];
      std::println(std::cout, "{}", update);
      }
   
   std::println(std::cout, "solution is {}", result_2);

   return;
   }