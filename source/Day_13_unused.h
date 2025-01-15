#pragma once

#include "my_common_tools.h"

#include <tuple>

namespace nsDay13 {

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
   std::vector<std::pair<ty, ty>> solve_linear_diophantine(ty a1, ty b1, ty c1, ty a2, ty b2, ty c2) {
      std::vector<std::pair<ty, ty>> solutions;

      auto [gcd, x, y] = extended_gcd(a1, b2);

      if (c1 % gcd != 0 || c2 % gcd != 0)  return solutions; // no solution if c1 and c2 are not divisible by the gcd.

      // scale the specific solution and adjust for the system of equations
      uint64_t k = c1 / gcd;
      x *= k;
      y *= k;

      auto abs = [](int64_t value) -> int64_t {
         return value >= 0 ? value : -value;
         };

      // Determine the general solution, use here int64_t for negative values
      int64_t factor = abs((b1 * a2 - a1 * b2) / gcd);
      for (int64_t i = -factor; i <= factor; ++i) {
         solutions.emplace_back(x + i * b2 / gcd, y - i * a2 / gcd);
      }

      return solutions;
   }
};