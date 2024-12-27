#pragma once

// Project: Solutions for Advent of Code 2024
// file with Advent of Code 2024, 23rd day : LAN Party
// author: Volker Hillmann
// date:   23.12.2024
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.

#include "my_common_tools.h"

#include <iostream>
#include <string>
#include <queue>
#include <utility>
#include <vector>
#include <algorithm> 
#include <stdexcept>
#include <unordered_set>
#include <unordered_map>
#include <string_view>
#include <ranges>
#include <print>



namespace nsDay23 {

   using name_ty = std::string;
   using NetworkTopology = std::unordered_map<name_ty, std::unordered_set<name_ty>>;

   struct hash_name_ty {
      std::size_t operator()(name_ty const& name) const {
         return std::hash<name_ty>{}(name);  
         }
      };

   inline bool checkTopology(std::string const& text) {
      static const std::regex TopologyPattern(R"(([a-z]{2})-([a-z]{2}))");
      return std::regex_match(text, TopologyPattern) ? true : false;
      }

   inline std::pair<name_ty, name_ty> parseTopology(std::string const& text) {
      static const std::regex TopologyPattern(R"(([a-z]{2})-([a-z]{2}))");
      std::pair<name_ty, name_ty> results;
      std::smatch match;
      if (std::regex_match(text, match, TopologyPattern)) {
         return { match[1], match[2] };
         }
      else {
         throw std::invalid_argument("unexpected input.");
         }
      }

   inline NetworkTopology buildTopology(std::vector<std::pair<name_ty, name_ty>> const& connections) {
      NetworkTopology topology;

      for (auto const& [node1, node2] : connections) {
         topology[node1].insert(node2);
         topology[node2].insert(node1);
         }
      return topology;
      }

   void PrintTopology(NetworkTopology const& topology, std::ostream& out) {
      for (auto const& [key, values] : topology) {
         std::print(out, "Connections for {}: ", key);
         for (auto const& node : values) {
            std::print(out, "{} ", node);
            }
         std::println(out, "");
         }
      }

 
   template <std::ranges::input_range range_ty>
   std::pair<std::string, std::string> RiddleDay23(range_ty const& values) {

      auto rules = values | std::views::transform(toString<std::string_view>)
         | std::views::filter(checkTopology)
         | std::views::transform([](auto const& p) { return parseTopology({ p.begin(), p.end() }); })
         | std::ranges::to<std::vector>();

      size_t result_1 = 0, result_2 = 0;

      auto topology = buildTopology(rules);
      PrintTopology(topology, std::cout);

      std::print("");
      /*
      std::cout << "Gefundene Cluster:\n";
      for (const auto& cluster : clusters) {
         std::cout << "{ ";
         for (const auto& node : cluster) {
            std::cout << node << " ";
         }
         std::cout << "}\n";
      }
      */
      

      std::println(std::cout, "the result for the 1st part is {}", result_1);
      std::println(std::cout, "the result for the 2nd part is {}", result_2);
      std::println(std::clog, "1st day finished");
      return { to_String(result_1), to_String(result_2) };
   }


} // end of namespace nsDay23
