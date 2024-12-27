#pragma once

// Project: Solutions for Advent of Code 2024
// file with Advent of Code 2024, 24th day : Crossed Wires
// author: Volker Hillmann
// date:   24.12.2024
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.

#include "my_common_tools.h"

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <algorithm> 
#include <stdexcept>
#include <tuple>
#include <functional>
#include <optional>
#include <ranges>
#include <print>

using namespace std::string_literals;

namespace nsDay24 {

   enum class EOperation : char { XOR, OR, AND };
   using node_ty        = std::string;
   using register_ty    = std::map<node_ty, std::optional<bool>>;
   using operations_ty  = std::map<EOperation, std::function<void (node_ty const&, node_ty const&, node_ty const&)>>;

   using statement_ty   = std::tuple<node_ty, EOperation, node_ty, node_ty>;
   using statements_ty  = std::vector<statement_ty>;

   register_ty   vault;
   statements_ty statements;

   std::map<std::string, EOperation> ops_from_string = {
      { "XOR"s, EOperation::XOR },
      { "AND"s, EOperation::AND },
      { "OR"s,  EOperation::OR }
      };

   operations_ty operations = {
      { EOperation::XOR, [](node_ty const& in1, node_ty const& in2, node_ty const& out) { } }
   };


   bool Has_Value(std::string const& node) {
      if (auto it = vault.find(node); it != vault.end()) [[likely]] {
         return it->second.has_value();
         }
      else {
         throw std::invalid_argument("node with the identifier "s + node + " couldn't found.");
         }
      }


   std::optional<bool> Read(std::string const& node) {
      if(auto it = vault.find(node); it != vault.end()) [[likely]] {
         return it->second;
         }
      else {
         throw std::invalid_argument("node with the identifier "s + node + " couldn't found.");
         }
      }

   void Add(std::string const& node, std::optional<bool> value) {
      if (auto it = vault.find(node); it == vault.end()) [[likely ]] {
         vault.insert( { node, value } );
         }
      }


   void Set(std::string const& node, bool value) {
      if (auto it = vault.find(node); it != vault.end()) [[likely]] {
         it->second = value;
         }
      else {
         throw std::invalid_argument("node with the identifier "s + node + " couldn't found.");
         }
      }


   bool is_valid_register_line(const std::string& line) {
      static const std::regex line_registers(R"(([xy]\d{2}):\s*(0|1))");
      return std::regex_match(line, line_registers);
      }

   std::pair<std::string, bool> parse_resister_line(std::string const& line) {
      static const std::regex line_registers(R"(([xy]\d{2}):\s*(0|1))");
      std::smatch match;
      if (std::regex_match(line, match, line_registers)) {
            std::string identifier = match[1].str();
            bool value = (match[2].str() == "1");
            return { identifier, value };
         }
      else {
         throw std::invalid_argument("unexpected line: "s + line + " couldn't processed.");
         }
      }

   bool is_valid_instruction_line(std::string const& line) {
      static const std::regex line_instructions(R"(([a-z]{3})\s+(AND|XOR|OR)\s+([a-z]{3})\s*->\s*([a-z]{3}))");
      return std::regex_match(line, line_instructions);
      }

   std::tuple<std::string, std::string, std::string, std::string> parse_instruction_line(std::string const& line) {
      static const std::regex line_instructions(R"(([a-z]{3})\s+(AND|XOR|OR)\s+([a-z]{3})\s*->\s*([a-z]{3}))");
      std::smatch match;
      if (std::regex_match(line, match, line_instructions)) {
         return { match[1].str(), match[2].str(), match[3].str(), match[4].str() };
      }
      else {
         throw std::invalid_argument("unexpected line: "s + line + " couldn't processed.");
         }
      }


template <std::ranges::input_range range_ty>
std::pair<std::string, std::string> RiddleDay24(range_ty const& lines) {

   auto values = lines | std::views::transform(toString<std::string_view>)
                       | std::views::filter(is_valid_register_line)
                       | std::views::transform([](auto const& p) { return parse_resister_line({ p.begin(), p.end() }); })
                       | std::ranges::to<std::vector>();

   auto rules  = lines | std::views::transform(toString<std::string_view>)
                       | std::views::filter(is_valid_instruction_line)
                       | std::views::transform([](auto const& p) { return parse_instruction_line({ p.begin(), p.end() }); })
                       | std::ranges::to<std::vector>();

   for (auto const& [node, value] : values) Add(node, value);

   size_t result_1 = 0, result_2 = 0;


   std::println(std::cout, "the result for the 1st part is {}", result_1);
   std::println(std::cout, "the result for the 2nd part is {}", result_2);
   std::println(std::clog, "1st day finished");
   return { to_String(result_1), to_String(result_2) };
   }


} // end of namespace nsDay23
