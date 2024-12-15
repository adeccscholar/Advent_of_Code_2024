#pragma once

// Project: Solutions for Advent of Code 2024
// 2 dimensional grid with std::mdspan with iterator and pathways, class for coordinates
// example to find and isolate basic functionality for different problems
// 
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.

// problem with differences

#include <string>
#include <vector>
#include <array>
#include <iterator>
#include <stdexcept>

#include <type_traits>
#include <concepts>
#include <ranges>

#include <mdspan>

using namespace std::string_literals;

namespace own {

   namespace grid {

      using coord_type = std::tuple<size_t, size_t>;

      /// concept for the base type definition of the grid types, reduced to a possible mapping to char
      template <typename ty>
      concept grid_base_type = std::is_same_v<ty, char> ||                       
                               (std::is_enum_v<ty> && std::is_same_v<std::underlying_type_t<ty>, char>); 

      /// simple form to use a mdspan for this base type grid, not used
      template <grid_base_type ty = char, typename Extents = std::extents<size_t, std::dynamic_extent, std::dynamic_extent>>
      using grid_raw_type = std::mdspan<ty, Extents>;

      // variadic type for the inner grid representation
      template <grid_base_type ty, std::size_t Dimensions>
      using extended_span_type = std::mdspan<ty, std::dextents<std::size_t, Dimensions>>;

      template <std::size_t... Indices>
      using create_tuple_type = std::tuple<std::integral_constant<std::size_t, Indices>...>;

      template <typename ty>
      struct base_coord_type_helper;
     
      template <std::size_t... Indices>
      struct base_coord_type_helper<std::index_sequence<Indices...>> {
         using type = create_tuple_type<Indices...>;
         };

      template <std::size_t Dimensions>
      using base_coord_type = typename base_coord_type_helper<std::make_index_sequence<Dimensions>>::type;



      // helper type to check the valid count of arguments
      template <std::size_t Dimensions, typename... Args>
      using is_valid_dimensions_count = std::enable_if_t<sizeof...(Args) == Dimensions, bool>;

      template <std::size_t Dimensions, typename... args_ty>
      concept valid_grid_args = sizeof...(args_ty) == Dimensions; // && (std::is_convertible_v<args_ty, std::size_t> && ...);

      template <typename func_ty, typename tuple_ty, std::size_t... Indices>
      auto call_with_tuple(func_ty&& fn, tuple_ty&& tuple, std::index_sequence<Indices...>) {
         return fn(std::get<Indices>(std::forward<tuple_ty>(tuple))...);
         }

      /// type with a variadic grid
      template <grid_base_type ty = char, size_t Dimensions = 2>
      class grid {
         //friend void swap(grid& lhs, grid& rhs) noexcept { lhs.swap(rhs); }
      public:
         using coord_type = typename base_coord_type<Dimensions>;
         using vault_type = extended_span_type<ty, Dimensions>;

         /*template <typename... args_ty, typename = std::enable_if_t<sizeof...(args_ty) == Dimensions &&
                                                              (std::is_convertible_v<args_ty, std::size_t> && ...)>>
         */
         template <typename... args_ty> requires valid_grid_args<Dimensions, args_ty...>
         grid(ty* data, args_ty... args)
            : vault(data, std::mdspan<ty, std::extents<std::size_t, Dimensions>>{
            data,
               std::extents<std::size_t, Dimensions>{static_cast<std::size_t>(args)...} // Verwende diese Form der Initialisierung
         })
         {
            }


         grid() = delete;
         grid(vault_type& data) : vault(data) { }
         // std::vector with product of dimensions
         // extended_span_type

         grid(grid const& other) : vault(other.vault) {  }

         grid(grid&& other) noexcept : vault(std::move(other.vault)) { }

         ~grid() = default;


         ty& operator[](coord_type const& indices) {
            return call_with_tuple([this](auto... args) { return vault(args...); }, indices, std::make_index_sequence<Dimensions>{});
            }

         ty& operator[](std::initializer_list<std::size_t> indices) {
            if (indices.size() != Dimensions) {
               throw std::out_of_range("number of indices must match Dimension = "s + to_String(Dimensions) + " in own::grid::grid");
               }
            return std::apply([this](auto... args) { return mdspan_(args...); }, indices);
            }

      private:
         vault_type vault;
      };



      // -----------------------------------------------------------------------------------------------------


      template <typename ty, typename Extents = std::extents<size_t, std::dynamic_extent, std::dynamic_extent>>
      class grid_view {
         friend void swap(grid_view& lhs, grid_view& rhs) noexcept { lhs.swap(rhs); }
      public:

         template <typename GridType, bool IsConst>
         class iterator_base {
         private:
            using GridRef = std::conditional_t<IsConst, GridType const&, GridType&>;
            GridRef grid;
            size_t current_index;
         public:
            using iterator_category = std::random_access_iterator_tag;
            using value_type        = ty;
            using difference_type   = std::ptrdiff_t;
            using pointer           = ty*;
            using reference         = ty&;

            iterator_base(GridRef owner, size_t index = 0) : grid(owner), current_index(index) {
               if (index > grid.path_way.size()) {
                  throw std::out_of_range("iterator index for grid_view is out of range");
                  }
               }
     
            iterator_base(iterator_base const& other) : grid(other.grid), current_index(other.current_index) {
               }

            iterator_base(iterator_base && other) noexcept : grid(other.grid), current_index(std::exchange(other.current_index, 0)) {
               }

            iterator_base& operator = ( iterator_base const& other) {
               if (this != &other) {
                  grid = other.grid; 
                  current_index = other.current_index;
                  }
               return *this;
               }

            iterator_base& operator = (iterator_base&& other) noexcept {
               if (this != &other) {
                  //grid = other.grid; // GridRef ist hier eine Referenz, also kein `std::move` erforderlich.
                  current_index = std::exchange(other.current_index, 0);
                  }
               return *this;
               }

            value_type operator * () const {
               auto const& [x, y] = grid.path_way[current_index];
               return grid.vault[x, y];
               }

            pointer operator -> () const {
               auto const& [x, y] = grid.path_way[current_index];
               return &grid.vault[x, y];
               }

            iterator_base& operator ++ () {
               ++current_index;
               return *this;
               }

            iterator_base operator ++ (int) {
               iterator tmp = *this;
               ++(*this);
               return tmp;
               }

            iterator_base& operator -- () {
               --current_index;
               return *this;
               }

            iterator_base operator -- (int) {
               iterator tmp = *this;
               --(*this);
               return tmp;
               }

            iterator_base operator + (difference_type n) const {
               return iterator(grid, current_index + n);
               }

            iterator_base operator - (difference_type n) const {
               return iterator(grid, current_index - n);
               }

            difference_type operator - (iterator_base const& other) const {
               return current_index - other.current_index;
               }

            iterator_base& operator += (difference_type n) {
               current_index += n;
               return *this;
               }

            iterator_base& operator -= (difference_type n) {
               current_index -= n;
               return *this;
               }

            bool operator==(const iterator_base& other) const {
               return current_index == other.current_index;
               }

            bool operator != (iterator_base const& other) const {
               return !(*this == other);
               }

            bool operator < (iterator_base const& other) const {
               return current_index < other.current_index;
               }

            bool operator > (iterator_base const& other) const {
               return current_index > other.current_index;
               }

            bool operator <= (iterator_base const& other) const {
               return current_index <= other.current_index;
               }

            bool operator >= (iterator_base const& other) const {
               return current_index >= other.current_index;
               }

         };

         using iterator       = iterator_base<grid_view, false>;
         using const_iterator = iterator_base<grid_view, true>;
         using value_type = ty;
         using reference = ty&;
         using const_reference = const ty&;
         using size_type = std::size_t;

         // Konstruktor
         grid_view(std::mdspan<ty, Extents>& data_view, std::vector<coord_type> const& coords)
            : vault(data_view), path_way(coords) {
            }

         grid_view(grid_view const& other) : vault(other.vault), /*extents_info(other.extents_info),*/ path_way(other.path_way) {
            }

         grid_view(grid_view && other) noexcept : vault(std::move(other.vault)), /*extents_info(std::move(other.extents_info)), */path_way(std::move(other.path_way)) {
            }

         ~grid_view() = default;

     
         grid_view& operator = (grid_view const& other) {
            if (this != &other) {
               vault = other.vault;
               //path_way = other.path_way;
               //   extents_info = other.extents_info;
               }
            return *this;
            }

         grid_view& operator = (grid_view && other) noexcept {
            if (this != &other) {
               vault = std::move(other.vault);
               //path_way = std::move(other.path_way);
               //  extents_info = std::move(other.extents_info);
               }
            return *this;
            }
      
         void swap(grid_view& other) noexcept {
            std::swap(vault, other.vault);
            //  std::swap(extents_info, other.extents_info);
            std::swap(path_way, other.path_way);
            }

         iterator begin() { return iterator(*this); }
         iterator end() { return iterator(*this, path_way.size()); }

         const_iterator begin() const { return const_iterator(*this); }
         const_iterator end() const { return const_iterator(*this, path_way.size()); }

         const_iterator cbegin() const { return begin(); }
         const_iterator cend() const { return end(); }

         size_type size() const { return path_way.size(); }
         bool empty() const { return path_way.empty(); }

         reference operator[](size_type index) {
            auto const& [x, y] = path_way.at(index);
            return vault(x, y);
            }

         const_reference operator[](size_type index) const {
            auto const& [x, y] = path_way.at(index);
            return vault(x, y);
            }

      private:
         std::mdspan<ty, Extents> vault;
         // Extents                  extents_info;
         std::vector<coord_type> const& path_way;
      };


      inline std::vector<std::vector<coord_type>> find_horz_inner_paths(std::mdspan<char, std::extents<size_t, std::dynamic_extent, std::dynamic_extent>> const& vault) {
         size_t rows = vault.extent(0);
         size_t cols = vault.extent(1);

         std::vector<std::vector<coord_type>> results;

         for(auto row_idx : std::views::iota(size_t { 1 }, rows - 1)) {
            results.emplace_back( std::views::zip(std::views::repeat(row_idx) | std::views::take(cols), 
                                                  std::views::iota(size_t { 1 }, cols - 1))
                                          | std::ranges::to<std::vector>());
            }
         return results;
         }


      inline std::vector<std::vector<coord_type>> find_all_paths(std::mdspan<char, std::extents<size_t, std::dynamic_extent, std::dynamic_extent>> const& vault) {
         size_t rows = vault.extent(0);
         size_t cols = vault.extent(1);

         std::vector<std::vector<coord_type>> results;

         for(auto row_idx : std::views::iota(size_t { 0 }, rows)) {
            results.emplace_back( std::views::zip(std::views::repeat(row_idx) | std::views::take(cols), 
                                                  std::views::iota(size_t { 0 }, cols))
                                          | std::ranges::to<std::vector>());
            }
   
         for(auto col_idx : std::views::iota(size_t { 0 }, cols)) {
            results.emplace_back(std::views::zip(std::views::iota(size_t { 0 }, rows), 
                                                 std::views::repeat(col_idx) | std::views::take(rows))
                                          | std::ranges::to<std::vector>());
            }   
   
         for (auto row_idx : std::views::iota(size_t{ 0 }, rows)) {
            results.emplace_back(std::views::zip(std::views::iota(row_idx, rows), 
                                                 std::views::iota(size_t{ 0 }, cols - row_idx))
                                          | std::ranges::to<std::vector>());
            } 
   
         for (auto col_idx : std::views::iota(size_t{ 1 }, cols)) {
            results.emplace_back(std::views::zip(std::views::iota(size_t { 0 }, rows),
                                                 std::views::iota(col_idx, cols ))
                                          | std::ranges::to<std::vector>());
            }

         for(auto col_idx : std::views::iota(size_t{ 1 }, cols)) {
            auto coords = std::views::zip(std::views::iota(size_t { 0 }, rows), 
                                          std::views::iota(size_t { 0 }, col_idx) | std::views::reverse)
                                          | std::ranges::to<std::vector>();
            if (coords.size() > 1) results.emplace_back(coords);
            }

         for (auto row_idx : std::views::iota(size_t{ 0 }, rows - 1)) {
            auto coords = std::views::zip(std::views::iota(row_idx, rows),
                                          std::views::iota(size_t{ 0 }, cols) | std::views::reverse)
                                          | std::ranges::to<std::vector>();
            results.emplace_back(coords);
            }

         return results;
         }

   } // end of namespace grid

}  // end of namespace

namespace std::ranges {
   template <typename ty, typename Extents>
   inline constexpr bool enable_view<own::grid::grid_view<ty, Extents>> = true;

   template <typename ty, typename Extents>
   auto begin(own::grid::grid_view<ty, Extents>& grid) {
      return grid.begin();
      }

   template <typename ty, typename Extents>
   auto end(own::grid::grid_view<ty, Extents>& grid) {
      return grid.end();
      }

   template <typename ty, typename Extents>
   auto begin(own::grid::grid_view<ty, Extents> const& grid) {
      return grid.begin();
      }

   template <typename ty, typename Extents>
   auto end(own::grid::grid_view<ty, Extents> const& grid) {
      return grid.end();
      }

} // end of std::ranges
