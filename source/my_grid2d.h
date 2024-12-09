#pragma once

#include <string>
#include <vector>
#include <array>
#include <iterator>
#include <stdexcept>

#include <type_traits>
#include <concepts>

#include <mdspan>

using namespace std::string_literals;

// todo copying of grid and pathway is still a problem in assignment 

namespace own {

   namespace grid {

      //using coord_2D_type = std::tuple<size_t, size_t>;

      using coord_2D_distance_ty = std::tuple<int64_t, int64_t>;

      class coord_2D_type {
      private:
         std::tuple<size_t, size_t> coords;
      public:
         coord_2D_type() : coords(0, 0) { }
         coord_2D_type(std::tuple<size_t, size_t>&& other) noexcept : coords(std::move(other)) {}
         coord_2D_type(std::tuple<size_t, size_t> const& other) : coords(std::get<0>(other), std::get<1>(other)) {}
         coord_2D_type(size_t r, size_t c) : coords(r, c) {}
         coord_2D_type(coord_2D_type const& other) : coords(other.coords) {}
         coord_2D_type(coord_2D_type&& other) noexcept : coords(std::move(other.coords)) {}
         ~coord_2D_type() = default;

         coord_2D_type& operator = (coord_2D_type const& other) {
            if (this != &other) coords = other.coords;
            return *this;
            }

         coord_2D_type& operator = (coord_2D_type&& other) noexcept {
            if (this != &other) coords = std::move(other.coords);
            return *this;
            }

         coord_2D_type& operator = (std::tuple<size_t, size_t> const& other) {
            coords = std::move(other);
            return *this;
            }

         std::optional<coord_2D_type> operator + (coord_2D_distance_ty const& other) const {
            auto val1 = static_cast<int64_t>(row()) + std::get<0>(other);
            auto val2 = static_cast<int64_t>(col()) - std::get<1>(other);
            if (val1 < 0 || val2 < 0) return { };
            else return { { static_cast<size_t>(val1), static_cast<size_t>(val2) } };
            }


         std::optional<coord_2D_distance_ty> operator - (coord_2D_type const& other) const {
            return { { static_cast<int64_t>(row()) - other.row(), static_cast<int64_t>(col()) - other.col()  } };
            }


         operator std::tuple<size_t, size_t>& () { return coords; }
         operator std::tuple<size_t, size_t> () const { return coords; }

         size_t row() const { return std::get<0>(coords);  }
         size_t col() const { return std::get<1>(coords); }

         std::tuple<size_t, size_t>& get_data() { return coords; }
         std::tuple<size_t, size_t> const& get_data() const { return coords; }
      };

      template <typename ty>
      class grid_2D {
      public:
         using mdspan2d_t = std::mdspan<ty, std::dextents<size_t, 2>>;
      
      private:
         std::vector<ty> raw_data;
         mdspan2d_t      data_span;
         size_t          rows_val, cols_val;
      public:

         grid_2D(size_t rows_para, size_t cols_para) : raw_data(rows_para * cols_para), 
                                                       data_span(raw_data.data(), rows_para, cols_para), 
                                                       rows_val(rows_para), cols_val(cols_para) { }

         grid_2D(grid_2D const& other) : raw_data(other.raw_data), 
                                         data_span(raw_data.data(), other.rows_val, other.cols_val),
                                         rows_val(other.rows_val), cols_val(other.cols_val) {  }

         grid_2D(grid_2D&& other) noexcept : raw_data(std::move(other.raw_data)), 
                                             data_span(raw_data.data(), other.rows_val, other.cols_val),
                                             rows_val(std::exchange(other.rows_val, 0)), 
                                             cols_val(std::exchange(other.cols_val, 0)) { }

         virtual ~grid_2D() = default;

         grid_2D& operator = (grid_2D const& other) {
            if (this != &other) {
               raw_data  = other.raw_data;
               rows_val  = other.rows_val;
               cols_val  = other.cols_val;
               data_span = mdspan_t(raw_data.data(), rows_val, cols_val);
               }
            return *this;
            }

         grid_2D& operator = (grid_2D&& other) noexcept {
            if (this != &other) {
               raw_data  = std::move(other.raw_data);
               rows_val  = std::exchange(other.rows_val, 0);
               cols_val  = std::exchange(other.cols_val, 0);
               data_span = mdspan_t(raw_data.data(), rows_val, cols_val);
               }
            return *this;
            }

         grid_2D& operator = (std::string const& str)  requires std::is_convertible_v<char, ty> {
            auto convert_func = [](char val) { return static_cast<ty>(val); };
            const size_t expected_size = rows_val * cols_val;
            if (str.size() < expected_size) {
               throw std::invalid_argument("length of input string does not match grid_2D dimensions");
               }
            else [[likely]] {
               if(str.size() == expected_size) [[likely]] {
                  if constexpr (std::is_same_v<char, ty>) {
                     std::ranges::copy(str, raw_data.begin());
                     }
                  else {
                     std::ranges::copy(str | std::views::transform(convert_func), raw_data.begin());
                     }
                  }
               else {
                  if constexpr (std::is_same_v<char, ty>) {
                     std::ranges::copy_n(str.begin(), expected_size, raw_data.begin());
                     }
                  else {
                     std::ranges::copy_n(str.begin() | convert_func, expected_size, raw_data.begin());
                     }
                  }
               }
            return *this;
            }

         template <typename other_ty>
         grid_2D& operator = (std::vector<other_ty> const& vec) requires std::is_convertible_v<other_ty, ty> {
            auto convert_func = [](other_ty val) { return static_cast<ty>(val); };
            const size_t expected_size = rows_val * cols_val;
            if (vec.size() < expected_size) {
               throw std::invalid_argument("length of input vector does not match grid_2D dimensions");
               }
            else [[likely]] { 
               if (vec.size() == expected_size) [[likely]] {
                  if constexpr (std::is_same_v<other_ty, ty>) {
                     std::ranges::copy(vec, raw_data.begin());
                     }
                  else {
                     std::ranges::copy(vec | convert_func, raw_data.begin());
                     }
                  }
               else {
                  if constexpr (std::is_same_v<other_ty, ty>) {
                     std::ranges::copy_n(vec, expected_size, raw_data.begin());
                     }
                  else {
                     std::ranges::copy_n(vec | convert_func, expected_size, raw_data.begin());
                     }
                  }
               };
            return *this;
            }
         

         grid_2D& operator = (std::vector<ty>&& vec) requires std::is_same_v<char, ty> {
            const size_t expected_size = rows_val * cols_val;
            if (vec.size() != expected_size) {
               raw_data = std::move(vec);
               }
            return *this;
            }
         
         void CheckBounds(size_t row, size_t col) const {
            if (row >= rows_val || col >= cols_val) {
               throw std::out_of_range("index for grid2d out of bounds");
               }
            }
         
         ty& operator[](size_t row, size_t col) {
            CheckBounds(row, col);
            return data_span[row, col];
            }

         ty const& operator[](size_t row, size_t col) const {
            CheckBounds(row, col);
            return data_span[row, col];
            }

         ty& operator[](coord_2D_type const& indices) {
            auto const& [row, col] = indices;
            CheckBounds(row, col);
            return data_span[row, col];
            }

         ty const& operator[](coord_2D_type const& indices) const {
            auto const& [row, col] = indices;
            CheckBounds(row, col);
            return data_span[row, col];
            }

         ty& operator()(size_t row, size_t col)             { return data_span(row, col);  }
         ty const& operator()(size_t row, size_t col) const { return data_span(row, col);  }
         ty& operator()(coord_2D_type pos)                  { return data_span(std::get<0>(pos), std::get<1>(pos)); }
         ty const& operator()(coord_2D_type pos) const      { return data_span(std::get<0>(pos), std::get<1>(pos)); }
         
         coord_2D_type operator()(size_t pos) const {
            if (pos >= raw_data.size()) {
               throw std::out_of_range("position in grid_2D is out of bounds");
               }
            return { pos / cols_val, pos % cols_val };
            }
         
         size_t rows() const { return rows_val; }
         size_t cols() const { return cols_val; }

         using iterator       = typename std::vector<ty>::iterator;
         using const_iterator = typename std::vector<ty>::const_iterator;

         iterator       begin()        { return raw_data.begin(); }
         iterator       end()          { return raw_data.end(); }

         const_iterator begin() const  { return raw_data.begin(); }
         const_iterator end() const    { return raw_data.end(); }

         const_iterator cbegin() const { return raw_data.cbegin(); }
         const_iterator cend() const   { return raw_data.cend(); }

      };

      // -----------------------------------------------------------------------------------------------------


      template <typename ty>
      class grid2d_view {
      public:

         template <bool IsConst>
         class iterator_base {
         private:
            using grid2d_ref = std::conditional_t<IsConst, grid2d_view const&, grid2d_view&>;
            grid2d_ref        grid;
            size_t            current_index;
         public:
            using iterator_category = std::random_access_iterator_tag;
            using value_type        = ty;
            using difference_type   = std::ptrdiff_t;
            using pointer           = ty*;
            using reference         = ty&;
            using const_reference   = ty const&;

            iterator_base(grid2d_ref owner, size_t index = 0) : grid(owner), current_index(index) {
               if (index > grid.path_way.size()) {
                  throw std::out_of_range("iterator index for grid_view is out of range");
                  }
               }
     
            iterator_base(iterator_base const& other) : grid(other.grid), current_index(other.current_index) { }

            iterator_base(iterator_base && other) noexcept : grid(other.grid), current_index(std::exchange(other.current_index, 0)) {
               }

            iterator_base& operator = (iterator_base const& other) {
               if (this != &other) {
                  current_index = other.current_index;
                  }
               return *this;
               }

            iterator_base& operator = (iterator_base&& other) noexcept {
               if (this != &other) {
                  current_index = std::exchange(other.current_index, 0);
                  }
               return *this;
               }

            reference operator * () {
               auto const& [x, y] = grid.path_way[current_index];
               //std::println(std::cout, "----> {},{}  ({})", x, y, current_index);
               return grid[x, y];
               }

            const_reference operator * () const {
               auto const& [x, y] = grid.path_way[current_index];
               //std::println(std::cout, "----> {},{}  ({})", x, y, current_index);
               return grid[x, y];
               }

            pointer operator -> () const {
               auto const& [x, y] = grid.path_way[current_index];
               return &grid[x, y];
               }

            coord_2D_type get_coords() const {
               auto const& [x, y] = grid.path_way[current_index];
               //std::println(std::cout, "~~~~> {},{}  ({})", x, y, current_index);
               return grid.path_way[current_index];
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

         using iterator        = iterator_base<false>;
         using const_iterator  = iterator_base<true>;
         using value_type      = ty;
         using reference       = ty&;
         using const_reference = const ty&;
         using size_type       = std::size_t;

         grid2d_view(grid_2D<ty>& data, std::vector<coord_2D_type> const& coords) : data_grid(data), path_way(coords) {  }
         grid2d_view(grid2d_view const& other) : data_grid(other.data_grid), path_way(other.path_way) { }
         grid2d_view(grid2d_view&& other) noexcept : data_grid(std::move(other.data_grid)), path_way(std::move(other.path_way)) {  }
         ~grid2d_view() = default;

     
         iterator begin()                           { return iterator(*this); }
         iterator end()                             { return iterator(*this, path_way.size()); }
         iterator pos(size_t para_pos)              { return iterator(*this, para_pos); }


         const_iterator begin() const               { return const_iterator(*this); }
         const_iterator end() const                 { return const_iterator(*this, path_way.size()); }
         const_iterator pos(size_t para_pos) const  { return const_iterator(*this, para_pos); }

         const_iterator cbegin() const              { return begin(); }
         const_iterator cend() const                { return end(); }
         const_iterator cpos(size_t para_pos) const { return iterator(*this, para_pos); }

         auto rbegin()                    { return std::reverse_iterator(end()); }
         auto rend()                      { return std::reverse_iterator(begin()); }
         auto rpos(size_t para_pos)       { return std::reverse_iterator(pos(para_pos)); }

         auto rbegin() const               { return std::reverse_iterator(end()); }
         auto rend() const                 { return std::reverse_iterator(begin()); }
         auto rpos(size_t para_pos) const  { return std::reverse_iterator(pos(para_pos)); }

         auto crbegin() const              { return std::reverse_iterator(end()); }
         auto crend() const                { return std::reverse_iterator(begin()); }
         auto crpos(size_t para_pos) const { return std::reverse_iterator(pos(para_pos)); }

         size_type size() const { return path_way.size(); }
         bool empty() const { return path_way.empty(); }

         reference operator[](size_type index) {
            auto const& [x, y] = path_way.at(index);
            return data_grid(x, y);
            }

         const_reference operator[](size_type index) const {
            auto const& [x, y] = path_way.at(index);
            return data_grid(x, y);
            }

         ty& operator[](size_t row, size_t col) {
            return data_grid[row, col];
            }

         ty const& operator[](size_t row, size_t col) const {
            return data_grid[row, col];
            }

         ty& operator[](coord_2D_type const& indices) {
            return data_grid[indices];
            }

         ty const& operator[](coord_2D_type const& indices) const {
            return data_grid[indices];
            }

      private:
         grid_2D<ty>&                      data_grid;
         std::vector<coord_2D_type> const& path_way;
      };

      
      template <typename ty>
      inline std::vector<std::vector<coord_2D_type>> find_horz_inner_paths(grid_2D<ty> const& grid) {
         std::vector<std::vector<coord_2D_type>> results;

         for(auto row_idx : std::views::iota(size_t { 1 }, grid.rows() - 1)) {
            results.emplace_back( std::views::zip(std::views::repeat(row_idx) | std::views::take(grid.cols()), 
                                                  std::views::iota(size_t { 1 }, grid.cols() - 1))
                                                 | std::views::transform([](auto val) { return coord_2D_type(std::move(val)); })
                                                 | std::ranges::to<std::vector>());
            }
         return results;
         }

      template <typename ty>
      inline std::vector<std::vector<coord_2D_type>> find_horz_paths(grid_2D<ty> const& grid) {
         std::vector<std::vector<coord_2D_type>> results;

         for (auto row_idx : std::views::iota(size_t{ 0 }, grid.rows())) {
            results.emplace_back( std::views::zip(std::views::repeat(row_idx) | std::views::take(grid.cols()),
                                                  std::views::iota(size_t{ 0 }, grid.cols()))
                                                | std::views::transform([](auto val) { return coord_2D_type(std::move(val)); })
                                                | std::ranges::to<std::vector>());
            }
         return results;
         }

      template <typename ty>
      inline std::vector<std::vector<coord_2D_type>> find_vert_paths(grid_2D<ty> const& grid) {
         std::vector<std::vector<coord_2D_type>> results;

         for (auto col_idx : std::views::iota(size_t{ 0 }, grid.cols())) {
            results.emplace_back(std::views::zip(std::views::iota(size_t{ 0 }, grid.rows()),
                                                 std::views::repeat(col_idx) | std::views::take(grid.rows()))
                                                | std::views::transform([](auto val) { return coord_2D_type(std::move(val)); })
                                                | std::ranges::to<std::vector>());
            }
         return results;
         }



      template <typename ty>
      inline std::vector<std::vector<coord_2D_type>> find_all_paths(grid_2D<ty> const& grid) {
         std::vector<std::vector<coord_2D_type>> results;

         for (auto& vec : find_horz_paths(grid)) { results.emplace_back(std::move(vec)); }
         for (auto& vec : find_vert_paths(grid)) { results.emplace_back(std::move(vec)); }

   
         for (auto row_idx : std::views::iota(size_t{ 0 }, grid.rows())) {
            results.emplace_back(std::views::zip(std::views::iota(row_idx, grid.rows()),
                                                 std::views::iota(size_t{ 0 }, grid.cols() - row_idx))
                                                | std::views::transform([](auto val) { return coord_2D_type(std::move(val)); })
                                                | std::ranges::to<std::vector>());
            } 
   

         for (auto col_idx : std::views::iota(size_t{ 1 }, grid.cols())) {
            results.emplace_back(std::views::zip(std::views::iota(size_t { 0 }, grid.rows()),
                                                 std::views::iota(col_idx, grid.cols()))
                                                | std::views::transform([](auto val) { return coord_2D_type(std::move(val)); })
                                                | std::ranges::to<std::vector>());
            }

         for(auto col_idx : std::views::iota(size_t{ 1 }, grid.cols())) {
            auto coords = std::views::zip(std::views::iota(size_t { 0 }, grid.rows()),
                                          std::views::iota(size_t { 0 }, col_idx) | std::views::reverse)
                                          | std::views::transform([](auto val) { return coord_2D_type(std::move(val)); })
                                          | std::ranges::to<std::vector>();
            if (coords.size() > 1) results.emplace_back(coords);
            }

         for (auto row_idx : std::views::iota(size_t{ 0 }, grid.rows() - 1)) {
            auto coords = std::views::zip(std::views::iota(row_idx, grid.rows()),
                                          std::views::iota(size_t{ 0 }, grid.cols()) | std::views::reverse)
                                          | std::views::transform([](auto val) { return coord_2D_type(std::move(val)); })
                                          | std::ranges::to<std::vector>();
            results.emplace_back(coords);
            }

         return results;
         }

   } // end of namespace grid

}  // end of namespace

template <>
struct std::formatter<own::grid::coord_2D_type> : std::formatter<std::string_view> {
   std::string format_string;

   constexpr auto parse(std::format_parse_context& ctx) {
      auto pos = ctx.begin();
      format_string = "{:";
      while (pos != ctx.end() && *pos != '}') {
         format_string += *pos;
         ++pos;
         }
      format_string += "}";
      return pos; // returns the iterator to the last parsed character in the format string, in this case we just swallow everything
      }

   auto format(own::grid::coord_2D_type const& val, std::format_context& ctx) const {
      std::string temp;

      size_t row_val = val.row(); // otherwise problems with make_format_args
      size_t col_val = val.col();
      std::string fmt = std::format("( {0:}, {0:} )", format_string);
      std::vformat_to(std::back_inserter(temp), fmt, std::make_format_args(row_val, col_val));
      return std::formatter<std::string_view>::format(temp, ctx);
      }
};



namespace std::ranges {
   template <typename ty>
   inline constexpr bool enable_view<own::grid::grid2d_view<ty>> = true;

   template <typename ty>
   auto begin(own::grid::grid2d_view<ty>& grid) {
      return grid.begin();
      }

   template <typename ty>
   auto end(own::grid::grid2d_view<ty>& grid) {
      return grid.end();
      }

   template <typename ty>
   auto begin(own::grid::grid2d_view<ty> const& grid) {
      return grid.begin();
      }

   template <typename ty>
   auto end(own::grid::grid2d_view<ty> const& grid) {
      return grid.end();
      }
} // end of std::ranges

namespace std {
   template<>
   struct tuple_size<own::grid::coord_2D_type> : std::integral_constant<size_t, 2> {};

   template<size_t N>
   struct tuple_element<N, own::grid::coord_2D_type> {
      using type = size_t;
      };

   template<size_t N>
   decltype(auto) get(own::grid::coord_2D_type const& obj) {
      if constexpr (N == 0) return std::get<0>(obj.get_data());
      else if constexpr (N == 1) return std::get<1>(obj.get_data());
      }

   template<size_t N>
   decltype(auto) get(own::grid::coord_2D_type& obj) {
      if constexpr (N == 0) return std::get<0>(obj.get_data());
      else if constexpr (N == 1) return std::get<1>(obj.get_data());
      }

}

