#pragma once

// Project: Solutions for Advent of Code 2024
// file with Advent of Code 2024, 6th day : Guard Gallivant
// author: Volker Hillmann
// date:   06.12.2024, last change: 15.12.2024
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.

// adoption of the template solution, array take unique_ptr now
// preparation of the 2nd part took back

#include "my_grid2d.h"

#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <functional>
#include <memory>
#include <tuple>
#include <array>
#include <optional>
#include <unordered_set>
#include <ranges>
//#include <expected>
#include <print>

namespace nsDay06 {


   template <typename ty, typename = void>
   struct has_get_coords : std::false_type {};

   template <typename ty>
   struct has_get_coords<ty, std::void_t<decltype(std::declval<ty>().get_coords())>> : std::true_type {};

   template <typename ty>
   constexpr bool has_get_coords_v = has_get_coords<ty>::value;

   // -----------------------------------------------------------------------------

   template <typename ty, typename = void>
   struct has_base : std::false_type {};

   template <typename ty>
   struct has_base<ty, std::void_t<decltype(std::declval<ty>().base())>> : std::true_type {};

   template <typename ty>
   constexpr bool has_base_v = has_base<ty>::value;

   // -----------------------------------------------------------------------------

   template <typename ty, typename = void>
   struct has_reverse_iterator_traits : std::false_type {};

   template <typename ty>
   struct has_reverse_iterator_traits<ty, std::void_t<typename ty::iterator_type, decltype(std::declval<ty>().base())>> : std::true_type {};

   template <typename ty>
   constexpr bool has_reverse_iterator_traits_v = has_reverse_iterator_traits<ty>::value;

   // -----------------------------------------------------------------------------

   template <typename ty>
   concept ReverseIteratorWithGetCoords = has_reverse_iterator_traits_v<ty> &&      
                                          has_get_coords_v<typename ty::iterator_type>; 

   template <typename Iter_ty>
   concept PathViewIterator = // std::input_or_output_iterator<Iter_ty> &&
                              (has_get_coords_v<Iter_ty> || ReverseIteratorWithGetCoords<Iter_ty>); 

   // -----------------------------------------------------------------------------




enum class EDirections : char {
   upward = '^', right = '>', downward = 'v', left = '<'
   };

static std::map<EDirections, EDirections> directions_rules = {
         { EDirections::upward ,  EDirections::right },
         { EDirections::right,    EDirections::downward },
         { EDirections::downward, EDirections::left },
         { EDirections::left,     EDirections::upward }
   };


const size_t UseLogLevel = 0;

template <EDirections direction, own::grid::my_grid_ty grid_ty>
std::optional<typename grid_ty::coord_ty> move_func(grid_ty const& grid,
                                                    typename grid_ty::path_view const& view, 
                                                    typename grid_ty::coord_ty const& start,
                                                    std::vector<std::tuple<typename grid_ty::coord_ty, EDirections, bool>>& retvals) {
   using coord_ty = typename grid_ty::coord_ty;
   using path_ty = std::tuple<typename grid_ty::coord_ty, EDirections, bool>;
   
   auto start_it = [&view, &start]() { if constexpr (direction == EDirections::upward) return view.rpos(start.row());
                                           else if constexpr (direction == EDirections::right) return view.pos(start.col());
                                           else if constexpr (direction == EDirections::downward) return view.pos(start.row());
                                           else return view.rpos(start.col());  }();

   auto end_it   = [&view]() { if constexpr (direction == EDirections::upward || direction == EDirections::left) return view.rend();
                               else return view.end(); }();
   
   // reverse iterator moved in one position, increment isn't 
   if(auto it = std::find(start_it, end_it, '#'); it != end_it) {
      if constexpr (direction == EDirections::upward || direction == EDirections::left) {
         if constexpr (UseLogLevel > 3) {
           std::println(std::cout, "Found: ({}, {})", (it.base() - 1).get_coords().row(), (it.base() - 1).get_coords().col());
            }
         for (auto iter = start_it; iter != it - 1; ++iter) {
            coord_ty element((iter.base() - 1).get_coords());
            retvals.emplace_back(path_ty { element, direction, false } );
            }
         }
      else {
         if constexpr (UseLogLevel > 3) {
            std::println(std::cout, "Found: ({}, {})", it.get_coords().row(), it.get_coords().col());
            }
         for (auto iter = ++start_it; iter != it - 1; ++iter) {
            coord_ty element(iter.get_coords());
            retvals.emplace_back(path_ty{ element, direction, false });
            }   
         }
      if constexpr (direction == EDirections::upward || direction == EDirections::left) {
         coord_ty element(((it - 1).base() - 1).get_coords());
         retvals.emplace_back(path_ty{ element, direction, true });
         return element;
         }
      else {
         coord_ty element((it - 1).get_coords());
         retvals.emplace_back(path_ty{ element, direction, true });
         return element;
         }
      }
   else {
      if constexpr (direction == EDirections::upward || direction == EDirections::left) {
         for (auto iter = start_it; iter != end_it; ++iter) {
            coord_ty element((iter.base() - 1).get_coords());
            retvals.emplace_back(path_ty{ element, direction, false });
            }
         }
      else {
         for (auto iter = ++start_it; iter != end_it; ++iter) {
            coord_ty element(iter.get_coords());
            retvals.emplace_back(path_ty{ element, direction, false });
            }
         }
      return std::nullopt;
      }
   }


template <size_t LogLevel = UseLogLevel>
inline std::pair<std::string, std::string> RiddleDay6(std::string&& text) { 
   const auto rows = std::ranges::count(text, '\n');
   const auto cols = text.size() / std::ranges::count(text, '\n') - 1;
   const own::grid::EKind grid_kind = own::grid::EKind::grid;
   using grid_ty  = own::grid::grid_2D<char, grid_kind>;
   using coord_ty = typename grid_ty::coord_ty;
   using hash_ty = typename grid_ty::coord_ty_hash;
   
   text.erase(std::ranges::remove(text, '\n').begin(), text.end());
   grid_ty grid(rows, cols);
   grid = text;

   auto horizontal = own::grid::find_horz_paths<char, own::grid::EKind::grid>(grid);
   auto vertical = own::grid::find_vert_paths<char, grid_kind>(grid);
   
   using path_ty = std::tuple<coord_ty, EDirections, bool>;
   std::vector<path_ty> path;

   size_t result_1 = 0, result_2 = 0;

   if(auto pos = std::find(grid.begin(), grid.end(), '^'); pos != grid.end()) {
      auto current = grid.get_coords(pos);
      EDirections move = EDirections::upward;
      path.emplace_back( path_ty { current, EDirections::upward, false });

      auto call_func = [&grid, &path, &horizontal, &vertical](coord_ty const& position, EDirections direction) {
         switch(direction) {
            case EDirections::upward: { 
               grid_ty::path_view view(grid, vertical[position.col()]); 
               return move_func<EDirections::upward>(grid, view, position, path);
               }
            case EDirections::right: {
               grid_ty::path_view view(grid, horizontal[position.row()]);
               return move_func<EDirections::right>(grid, view, position, path);
               }
            case EDirections::downward: {
               grid_ty::path_view view(grid, vertical[position.col()]);
               return move_func<EDirections::downward>(grid, view, position, path);
               }
            case EDirections::left: {
               grid_ty::path_view view(grid, horizontal[position.row()]);
               return move_func<EDirections::left>(grid, view, position, path);
               }
            default: throw std::runtime_error("unexpected direction, shouldn't be possible");
            }
         };

      for (auto position = call_func(current, move); position; position = call_func(*position, move)) {
         move = directions_rules[move];
         }
      }

   for(auto it = path.begin(); it != path.end(); ++it) {
      if (std::get<2>(*it)) continue;
      auto const& direction = directions_rules[std::get<1>(*it)];

      static auto compare = [&it, direction](path_ty const& it2) {
         if(std::get<2>(it2) == true) {
            if (direction == std::get<1>(it2)) {
               auto coord1 = std::get<0>(*it);
               auto coord2 = std::get<0>(it2);
               switch(direction) {
                  case EDirections::downward:
                     if (coord1.col() == coord2.col() && coord1.row() < coord2.row()) return true;
                     else return false;
                  case EDirections::upward:
                     if (coord1.col() == coord2.col() && coord1.row() > coord2.row()) return true;
                     else return false;
                  case EDirections::left:
                  case EDirections::right:
                     return true;
                  default:
                     throw std::runtime_error("unexpected directions in the path.")
                  }
               }
            else [[likely]] {
               return false;
               }
            }
         else [[likely]] return false;
         };

      auto reverse_it = std::make_reverse_iterator(it);
      auto found_edge = std::find_if(reverse_it, path.rend(), [](auto rev_it) { return compare(rev_it); });  // row or col, turn, ...
      if (found_edge != path.rend()) ; // hit
      }

   
   if constexpr (LogLevel > 3) {
      for(auto const& pos : path) { 
         std::println(std::cout, "({:>2}, {:>2}) {} - {}", std::get<0>(pos).row(), std::get<0>(pos).col(), 
                                                           static_cast<char>(std::get<1>(pos)),
                                                           std::get<2>(pos) ? "edge" : "way");
         }
      }

   // 236 to low

   std::unordered_set<coord_ty, hash_ty> counter;
   for (auto const& [coord, dir, edge] : path) {
      counter.insert(coord);
      grid[coord] = static_cast<char>(dir);
      }
   
   if constexpr (LogLevel > 1) {
      grid.print(std::cout, 1);
      std::println(std::cout, "");
      }
   
   result_1 = counter.size();
   result_2 = path.size(); // std::ranges::count_if(path, [](auto const& e) { return std::get<2>(e) == true; });
   std::println(std::cout, "the result for the 1st part is {}", result_1);
   std::println(std::cout, "the result for the 2nd part is {}", result_2);
   return { to_String(result_1), to_String(result_2) };
   }

} // end of namespace nsDay06
