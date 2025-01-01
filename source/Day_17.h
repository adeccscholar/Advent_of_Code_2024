#pragma once

// Project: Solutions for Advent of Code 2024
// file with Advent of Code 2024, 17t day : Chronospatial Computer
// author: Volker Hillmann
// date:   30.12.2024
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.


#include "my_common_tools.h"

#include <string>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <regex>
#include <ranges>
#include <print>

using namespace std::string_literals;

namespace nsDay17 {

   using int_ty = uint64_t;

   enum class EOps : char { adv = 0, bxl = 1, bst = 2, jnz = 3, bxc = 4, out = 5, bdv = 6, cdv = 7 };

   int_ty register_a = 0;
   int_ty register_b = 0;
   int_ty register_c = 0;

   size_t instruction_ptr;
   size_t cnt;
   std::ostringstream out;

   inline void Set(std::pair<char, int_ty> const& value) {
      switch(auto const& [reg, val] = value; reg) {
         case 'A': register_a = val; break;
         case 'B': register_b = val; break;
         case 'C': register_c = val; break;
         default: throw std::invalid_argument("unexpected register to set for this program.");
         }
      }

   inline int_ty get_combo_op(int_ty op) {
      switch(op) {
         case 0:
         case 1:
         case 2:
         case 3:
            return op;
         case 4:
            return register_a;
         case 5:
            return register_b;
         case 6:
            return register_c;
         default:
            throw std::invalid_argument("invalid op for combo operant.");
         }
      }

   inline constexpr int_ty get_literal_op(int_ty op) {
      return op;
      }

   std::map<EOps, std::function<bool(int_ty)>> rules = {
      { EOps::adv, [](int_ty   op  ) { register_a /= std::pow(2, get_combo_op(op)); return true; } },
      { EOps::bxl, [](int_ty   op  ) { register_b ^= get_literal_op(op); return true; }  },
      { EOps::bst, [](int_ty   op  ) { register_b = get_combo_op(op) % 8 & 0b111; return true; } },
      { EOps::jnz, [](int_ty   op  ) { if (register_a == 0) return true; else { instruction_ptr = get_literal_op(op); return false; } } },
      { EOps::bxc, [](int_ty /*op*/) { register_b ^= register_c; return true; } },
      { EOps::out, [](int_ty   op  ) { out << (cnt++ > 0 ? "," : "") << get_combo_op(op) % 8; return true; } },
      { EOps::bdv, [](int_ty   op  ) { register_b = register_a / std::pow(2, get_combo_op(op)); return true; } },
      { EOps::cdv, [](int_ty   op  ) { register_c = register_a / std::pow(2, get_combo_op(op)); return true; } }
      };


   bool filter_register_lines(std::string const& line) {
      static std::regex parser(R"(Register\s+[A-C]:\s+\d+)");
      return std::regex_search(line, parser);
      }
   

   std::pair<char, int_ty> parse_register_values(std::string const& line) {
      static std::regex pattern(R"(Register\s+([A-C]):\s+(\d+))");
      std::smatch match;
      if (std::regex_search(line, match, pattern)) {
         return { static_cast<char>(match[1].str()[0]), toInt<int_ty>(match[2].str()) };
         }
      else throw std::runtime_error("unexpected input for register values.");
      }

   bool filter_program_lines(std::string const& line) {
      return line.find("Program:") != std::string::npos;
      }

   std::vector<int_ty> parse_program_values(const std::string& line) {
      if (auto pos = line.find(":"); pos != std::string::npos) {
         auto numbers = line.substr(pos + 1);
         numbers.erase(std::remove(numbers.begin(), numbers.end(), ' '), numbers.end());
         return numbers | std::views::split(',') 
                        | std::views::filter([](auto subrange) { return !subrange.empty(); })
                        | std::views::transform([](auto val) { return toInt<int_ty>(std::string { val.begin(), val.end() }); })
                        | std::ranges::to<std::vector>();
         }
      else throw std::invalid_argument("unepected values in program code.");
      }

template <std::ranges::input_range range_ty>
inline std::pair<std::string, std::string> RiddleDay17(range_ty const& values) {
   auto registers = values | std::views::transform(toString<std::string_view>)
                           | std::views::filter(filter_register_lines)
                           | std::views::transform(parse_register_values)
                           | std::ranges::to<std::vector>();

   auto instructions = values | std::views::transform(toString<std::string_view>)
                              | std::views::filter(filter_program_lines)
                              | std::views::transform(parse_program_values)
                              | std::ranges::to<std::vector>();


   auto code = instructions   | std::ranges::views::join 
                              | std::views::chunk(2)
                              | std::views::transform([](auto const& ch) -> std::pair<EOps, int_ty> {
                                    auto it  = ch.begin();
                                    EOps op  = static_cast<EOps>(*it);
                                    int_ty val = *(++it);
                                    return { op, val }; 
                                    })
                              | std::ranges::to<std::vector>();
      

   std::ostringstream backup_stream;
   backup_stream << static_cast<int_ty>(code[0].first) << "," << code[0].second;
   for (auto vals : code | std::views::drop(1)) backup_stream << ',' << static_cast<int_ty>(vals.first) << ',' << vals.second;
   std::string backup = backup_stream.str();
   instruction_ptr = 0;
   cnt             = 0;
   out.str("");
   for (auto const& reg : registers) Set(reg);
   for(instruction_ptr = 0, cnt = 0; instruction_ptr < code.size();) {
      if (rules[code[instruction_ptr].first](code[instruction_ptr].second)) ++instruction_ptr;
      }

   std::string result_1 = out.str();

   /*
   for(int_ty step = 0; ; ++step) {
      instruction_ptr = 0;
      cnt = 0;
      out.str("");
      for (auto const& reg : registers) Set(reg);

      Set( { 'A', step } );

      for (instruction_ptr = 0, cnt = 0; instruction_ptr < code.size();) {
         if (rules[code[instruction_ptr].first](code[instruction_ptr].second)) ++instruction_ptr;
         }

      if(out.str() == backup) {
         std::cout << step << " !!!\n";
         break;
         }

      }
   */

   std::string result_2 = ""s;
   std::println(std::cout, "the result for the 1st part is {}", result_1);
   std::println(std::cout, "the result for the 2nd part is {}", result_2);
   return { result_1, result_2 };
   }

} // end of namespace nsDay17