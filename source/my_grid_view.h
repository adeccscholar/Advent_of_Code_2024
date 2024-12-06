#pragma once

#include <string>
#include <vector>
#include <iterator>
#include <stdexcept>

#include <type_traits>

#include <mdspan>

using namespace std::string_literals;

// todo copying of grid and pathway is still a problem in assignment 

namespace own {
  
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
      grid_view(std::mdspan<ty, Extents>& data_view, std::vector<std::tuple<size_t, size_t>> const& coords)
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
      std::vector<std::tuple<size_t, size_t>> const& path_way;
   };


   inline std::vector<std::vector<std::tuple<size_t, size_t>>> find_horz_inner_paths(std::mdspan<char, std::extents<size_t, std::dynamic_extent, std::dynamic_extent>> const& vault) {
      size_t rows = vault.extent(0);
      size_t cols = vault.extent(1);

      std::vector<std::vector<std::tuple<size_t, size_t>>> results;

      for(auto row_idx : std::views::iota(size_t { 1 }, rows - 1)) {
         results.emplace_back( std::views::zip(std::views::repeat(row_idx) | std::views::take(cols), 
                                               std::views::iota(size_t { 1 }, cols - 1))
                                       | std::ranges::to<std::vector>());
         }
      return results;
      }


   inline std::vector<std::vector<std::tuple<size_t, size_t>>> find_all_paths(std::mdspan<char, std::extents<size_t, std::dynamic_extent, std::dynamic_extent>> const& vault) {
      size_t rows = vault.extent(0);
      size_t cols = vault.extent(1);

      std::vector<std::vector<std::tuple<size_t, size_t>>> results;

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


}  // end of namespace

namespace std::ranges {
   template <typename ty, typename Extents>
   inline constexpr bool enable_view<own::grid_view<ty, Extents>> = true;

   template <typename ty, typename Extents>
   auto begin(own::grid_view<ty, Extents>& grid) {
      return grid.begin();
      }

   template <typename ty, typename Extents>
   auto end(own::grid_view<ty, Extents>& grid) {
      return grid.end();
      }

   template <typename ty, typename Extents>
   auto begin(own::grid_view<ty, Extents> const& grid) {
      return grid.begin();
      }

   template <typename ty, typename Extents>
   auto end(own::grid_view<ty, Extents> const& grid) {
      return grid.end();
      }
}
