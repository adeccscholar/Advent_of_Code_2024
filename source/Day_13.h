#pragma once

// Project: Solutions for Advent of Code 2024
// file with Advent of Code 2024, 13th day : Claw Contraption
// author: Volker Hillmann
// date:   13.12.2024 
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.

#include "my_common_tools.h"

#include <string>
#include <utility>
#include <vector>
#include <tuple>
#include <cmath>
#include <ranges>
#include <print>

#include <execution>
#include <mutex>

namespace nsDay13 {


   struct equation_ty {
      uint64_t a_x, a_y;
      uint64_t b_x, b_y;
      uint64_t x_t, y_t;
      };


   struct Node {
      uint64_t aCount, bCount;
      equation_ty values;

      struct Hash {
         size_t operator()(const Node& n) const {
            return std::hash<size_t>()(n.aCount) << 32 ^ std::hash<size_t>()(n.bCount);
         }
      };

      struct Compare {
         bool operator()(Node const& a, Node const& b) const {
            return a.Costs() > b.Costs();
         }
      };

      Node(equation_ty const& vals, uint64_t a_val, uint64_t b_val) : values(vals), aCount(a_val), bCount(b_val) {}
      Node(Node const& other) : values(other.values), aCount(other.aCount), bCount(other.bCount) {}

      Node IncrementA() { return Node(values, aCount + 1, bCount); }
      Node IncrementB() { return Node(values, aCount, bCount + 1); }

      std::optional<uint64_t> find_common_factor() const {
         auto current_x = aCount * values.a_x + bCount * values.b_x;
         auto current_y = aCount * values.a_y + bCount * values.b_y;
         if (current_x == 0 || current_y == 0) return std::nullopt;
         if(values.x_t % current_x != 0 || values.y_t % current_y != 0) return std::nullopt;
         if(auto k = values.x_t / current_x; k == values.y_t / current_y) return k;
         return std::nullopt;
         }
      
      uint64_t Costs() const {
         return aCount * 3 + bCount;
         }

      auto operator <=> (Node const& other) const {
         return std::tie(aCount, bCount) <=> std::tie(other.aCount, other.bCount);
         }


      bool operator == (Node const& other) const {
         return aCount == other.aCount && bCount == other.bCount;
         }

      bool isGreater() const {
         return aCount * values.a_x + bCount * values.b_x > values.x_t || aCount * values.a_y + bCount * values.b_y > values.y_t;
         }

      bool isSolution() const {
         return aCount * values.a_x + bCount * values.b_x == values.x_t && aCount * values.a_y + bCount * values.b_y == values.y_t;
         }
   };


   std::optional<Node> solve(equation_ty const& equation) {

      std::priority_queue<Node, std::vector<Node>, Node::Compare> pq;
      //std::unordered_set<Node, Node::Hash> visited;
      std::set<Node> visited;

      Node root = { equation, 0, 0 };
      pq.push(root);
      visited.insert(root);

      while (!pq.empty()) {
         Node current = pq.top();
         pq.pop();

         if (current.isGreater()) continue;
         auto factor = current.find_common_factor();
         if (factor) return Node(current.values, current.aCount * (*factor), current.bCount * (*factor));
         //if (current.isSolution()) return current;

         for (auto&& child : { current.IncrementA(), current.IncrementB() }) {
            if (!child.isGreater() && visited.find(child) == visited.end()) {
               visited.insert(child);
               pq.emplace(std::move(child));
               }
            }
         }
      return std::nullopt;
      }



   std::vector<equation_ty> parse_equations(std::string const& text) {

      std::vector<equation_ty> equations;
      std::regex pattern(R"(Button A: X\+(\d+), Y\+(\d+)\nButton B: X\+(\d+), Y\+(\d+)\nPrize: X=(\d+), Y=(\d+))");

      std::smatch matches;
      std::string::const_iterator it = text.begin();
      while (std::regex_search(it, text.end(), matches, pattern)) {
            equation_ty equation;
            equation.a_x = toInt<uint64_t>(matches[1].str());
            equation.a_y = toInt<uint64_t>(matches[2].str());
            equation.b_x = toInt<uint64_t>(matches[3].str());
            equation.b_y = toInt<uint64_t>(matches[4].str());
            equation.x_t = toInt<uint64_t>(matches[5].str());
            equation.y_t = toInt<uint64_t>(matches[6].str());
            equations.emplace_back(std::move(equation));
            it = matches.suffix().first;
            } 


      return equations;
   }


inline std::pair<std::string, std::string> RiddleDay13(std::string&& text) {
   size_t result_1 = 0, result_2 = 0;
   std::mutex mtx;
   auto equations = parse_equations(std::move(text));

   /*
   for(auto const& equation : equations) {
      auto result = solve(equation);
      if (result) result_1 += result->Costs();
      }
   */
   std::for_each(std::execution::par, equations.begin(), equations.end(), [&result_1, &mtx](equation_ty const& equation) {
                      auto result = solve(equation);
                      if (result) {
                         std::lock_guard<std::mutex> lock(mtx);
                         result_1 += result->Costs();
                         }
                      });

   std::for_each(std::execution::par, equations.begin(), equations.end(), [](equation_ty& equation) {
                      equation.x_t += 10'000'000'000'000;
                      equation.y_t += 10'000'000'000'000;
                      });
   /*
   std::for_each(std::execution::par, equations.begin(), equations.end(), [&result_2, &mtx](equation_ty const& equation) {
                      auto result = solve(equation);
                      if (result) {
                         std::lock_guard<std::mutex> lock(mtx);
                         result_2 += result->Costs();
                         }
                      });
   */
   return { to_String(result_1), to_String(result_2) };
   }

} // end of namespace nsDay13
