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
#include <ranges>
#include <print>

template <my_integral_ty ty>
std::tuple<ty, ty, ty> extended_gcd(ty a, ty b) {
   ty s = 0, old_s = 1;
   ty t = 1, old_t = 0;
   ty r = b, old_r = a;

   while (r != 0) {
      ty quotient = old_r / r;
      ty tmp = old_r;
      old_r = r;
      r = tmp - quotient * r;
      tmp = old_s;
      old_s = s;
      s = tmp - quotient * s;
      tmp = old_t;
      old_t = t;
      t = tmp - quotient * t;
      }
   return { old_r, old_s, old_t }; // gcd, x, y
   }


// extended euclidean algorithm: the function extendedEuclid recursively calculates the gcd of a and b 
// as well as the coefficients x and y, such that a * x + b * y = gcd(a, b)
template <my_integral_ty ty>
std::tuple<ty, ty, ty> extendedEuclid(ty a, ty b) {
   if (b == 0) {
      return { a, 1, 0 };
      }
   else {
      auto [gcd, x, y] = extendedEuclid(b, a % b);
      return { gcd, y, x - (a / b) * y };
      }
   }

template <my_integral_ty ty>
std::vector<std::pair<ty, ty>> solve_linear_diophantine(ty a1,  ty b1, ty c1, ty a2, ty b2, ty c2) {
   std::vector<std::pair<ty, ty>> solutions;

   auto [gcd, x, y] = extendedEuclid(a1, b2);

   if (c1 % gcd != 0 || c2 % gcd != 0)  return solutions; // no solution if c1 and c2 are not divisible by the gcd.

   // Skalieren der speziellen Lösung und Anpassung für das Gleichungssystem
   uint64_t k = c1 / gcd;
   x *= k;
   y *= k;

   // Bestimmen der allgemeinen Lösung
   uint64_t factor = std::abs((b1 * a2 - a1 * b2) / gcd);
   for (int64_t i = -factor; i <= factor; ++i) { // int64_t für negative Werte
      solutions.emplace_back(x + i * b2 / gcd, y - i * a2 / gcd);
      }

   return solutions;
   }

template <std::ranges::input_range range_ty>
std::pair<std::string, std::string> RiddleDay13(range_ty const& values) {
   size_t result_1 = 0, result_2 = 0;
   return { to_String(result_1), to_String(result_2) };
   }
