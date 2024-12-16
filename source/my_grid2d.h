#pragma once

// Project: Solutions for Advent of Code 2024
// file with 2 dimensional grid, matching coordinates and supporting pathways
// example to demonstrate the implementation of iterators, and use of mapping to acces data
// author: Volker Hillmann
// date: 06.12.2024, rebuild 15.12.2024
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.


#include <string>
#include <vector>
#include <array>
#include <iterator>
#include <stdexcept>

#include <type_traits>
#include <ranges>   // C++20
#include <concepts> // C++20
#include <expected> // C++23
#include <mdspan>   // C++23

using namespace std::string_literals;

// todo copying of grid and pathway is still a problem in assignment 

namespace own {

   namespace grid {

      using int_ty  = int64_t;
      using size_ty = uint64_t;

      enum class EKind : uint32_t { grid, matrix };

      template <EKind kind>
      concept is_grid = kind == EKind::grid;

      template <EKind kind>
      concept is_matrix = kind == EKind::matrix;

      template <typename ty = char, EKind kind = EKind::grid>
      class grid_2D {
         template <typename ty, EKind kind>
         friend class grid2d_view;
      public:
         using value_type = ty;
         using mdspan2d_t = std::mdspan<ty, std::dextents<size_ty, 2>>;
         using iterator = typename std::vector<ty>::iterator;
         using const_iterator = typename std::vector<ty>::const_iterator;

         class coord_ty {
            template <typename ty, EKind kind>
            friend class grid_2D;
            friend void swap(coord_ty& lhs, coord_ty& rhs) noexcept { std::swap(lhs.get_data, rhs.get_data); }

         public:

            class distance_ty {
               friend void swap(distance_ty& lhs, distance_ty& rhs) noexcept { std::swap(lhs.data, rhs.data);  }
            public:
               using data_ty = std::tuple<int_ty, int_ty>;
               data_ty data;
            public:
               distance_ty() : data{ 0, 0 } {};
               distance_ty(distance_ty const& other) : data { other.data } { }
               distance_ty(distance_ty&& other) noexcept : data { std::move(other.data) } { }
               distance_ty(int_ty row, int_ty col) requires is_grid<kind> : data { row, col } { }
               distance_ty(int_ty x_val, int_ty y_val) requires is_matrix<kind> : data{ y_val, x_val } {}
               distance_ty(std::pair<int_ty, int_ty> const& other) : data { other.first, other.second } {}
               distance_ty(data_ty const& values) : data { values } { }
               distance_ty(data_ty && values) : data {  std::move(values) } {}
               ~distance_ty() = default;

               distance_ty& operator = (distance_ty const& other) {
                  if (this != &other) data = other.data;
                  return *this;
                  }

               distance_ty& operator = (distance_ty&& other) noexcept {
                  if (this != &other) std::swap(data, other.data);
                  return *this;
                  }

               auto operator <=> (distance_ty&& rhs) const { return data <=> rhs.data; }

               distance_ty& operator += (distance_ty&& other) noexcept {
                  get<0>(data) += get<0>(other.data);
                  get<1>(data) += get<1>(other.data);
                  return *this;
                  }

               distance_ty& operator -= (distance_ty&& other) noexcept {
                  get<0>(data) -= get<0>(other.data);
                  get<1>(data) -= get<1>(other.data);
                  return *this;
                  }

               distance_ty operator + (distance_ty&& other) noexcept {
                  distance_ty result(data);
                  get<0>(result) += get<0>(other.data);
                  get<1>(result) += get<1>(other.data);
                  return result;
                  }

               distance_ty operator - (distance_ty&& other) noexcept {
                  distance_ty result(data);
                  get<0>(result) -= get<0>(other.data);
                  get<1>(result) -= get<1>(other.data);
                  return result;
                  }

               int_ty const& delta_Rows() const requires is_grid<kind> { return std::get<0>(data); }
               int_ty const& delta_Cols() const requires is_grid<kind> { return std::get<1>(data); }

               int_ty const& delta_x() const requires is_matrix<kind> { return std::get<0>(data); }
               int_ty const& delta_y() const requires is_matrix<kind> { return std::get<1>(data); }

               operator data_ty& () { return data;  }
               operator data_ty const& () const { return data; }


            };
         private:

            grid_2D const& grid;
         public:
            using data_ty = std::tuple<size_ty, size_ty>;
            using error_ty = std::pair<coord_ty, distance_ty>;
            data_ty        data;
         public:
            coord_ty(grid_2D const& parent) : grid { parent }, data { 0, 0 } { }
            coord_ty(coord_ty const& other) : grid{ other.grid }, data { other.data } {}
            coord_ty(coord_ty&& other) noexcept : grid { other.grid }, data { std::move(other.data) } {}
            coord_ty(grid_2D const& parent, data_ty const& other) : grid{ parent }, data { other } {}
            coord_ty(grid_2D const& parent, data_ty&& other) noexcept : grid{ parent }, data { std::move(other) } {}
            coord_ty(grid_2D const& parent, size_ty row, size_ty col) requires is_grid<kind> : grid{ parent }, data { row, col } {}
            coord_ty(grid_2D const& parent, size_ty x_val, size_ty y_val) requires is_matrix<kind> : grid{ parent }, data{ y_val, x_val } {}
            virtual ~coord_ty() = default;

            operator data_ty& () { return data; }
            operator data_ty const& () const { return data; }

            coord_ty& operator = (coord_ty const& other) {
               if (this != &other) data = other.data;
               return *this;
               }

            coord_ty& operator = (coord_ty&& other) noexcept {
               if (this != &other) data = std::move(other.data);
               return *this;
               }

            coord_ty& operator = (data_ty const& other) {
               data = other;
               return *this;
               }

            coord_ty& operator = (data_ty&& other) {
               data = std::move(other);
               return *this;
               }

            auto operator <=> (coord_ty const& rhs) const {
               return data <=> rhs.data;
               }

            bool operator == (coord_ty const& rhs) const {
               return data == rhs.data;
               }

            bool operator != (coord_ty const& rhs) const {
               return data != rhs.data;
               }

            bool operator < (coord_ty const& rhs) const {
               return data < rhs.data;
               }

            // std::pair<coord_ty, distance_ty>
            std::expected<coord_ty, error_ty> operator + (distance_ty const& other) const {
               if constexpr (is_grid<kind>) {
                  auto const& [delta_row, delta_col] = static_cast<distance_ty::data_ty const&>(other);
                  auto new_row = static_cast<int_ty>(row()) + delta_row;
                  auto new_col = static_cast<int_ty>(col()) + delta_col;
                  if (new_row < 0 || new_row >= static_cast<int_ty>(grid.rows()) || 
                      new_col < 0 || new_col >= static_cast<int_ty>(grid.cols())) [[unlikely]] {
                     std::pair<int_ty, int_ty> rest = { 0, 0 };
                     if (new_row < 0) { 
                        rest.first = -new_row; // error
                        new_row = 0; 
                        }                                           
                     else if(new_row >= static_cast<int_ty>(grid.rows())) {
                        rest.first = new_row - static_cast<int_ty>((grid.rows() - 1));
                        new_row = static_cast<int_ty>(grid.rows() - 1);
                        }
                     if (new_col < 0) { 
                        rest.second = -new_col; // error
                        new_col = 0; 
                        }                                         
                     else if(new_col >= static_cast<int_ty>(grid.cols())) {
                        rest.second = new_col - static_cast<int_ty>((grid.cols() - 1));
                        new_col = static_cast<int_ty>(grid.cols() - 1);
                        }
                     return std::unexpected(error_ty { { grid, static_cast<size_ty>(new_row), 
                                                         static_cast<size_ty>(new_col) },
                                                rest });
                     }
                  else return coord_ty { grid, static_cast<size_t>(new_row), static_cast<size_t>(new_col) };
                  }
               else {
                  auto const& [delta_x, delta_y] = static_cast<distance_ty::data_ty const&>(other);
                  auto new_x = static_cast<int64_t>(x()) + delta_x;
                  auto new_y = static_cast<int64_t>(y()) + delta_y;
                  if (new_x < 0 || new_x >= static_cast<int_ty>(grid.x_dim()) || 
                      new_y < 0 || new_y >= static_cast<int_ty>(grid.y_dim())) [[unlikely]] {
                     std::pair<int_ty, int_ty> rest = { 0, 0 };
                     if (new_x < 0) { 
                        rest.first = -new_x; 
                        new_x = 0; 
                        }                                           
                     else if(new_x >= static_cast<int_ty>(grid.x_dim())) {           
                        rest.first = new_x - static_cast<int_ty>((grid.x_dim() - 1));
                        new_x = static_cast<int_ty>(grid.x_dim() - 1);
                        }
                     if (new_y < 0) { 
                        rest.second = -new_y; 
                        new_y = 0; 
                        }                                         
                     else if(new_y >= static_cast<int_ty>(grid.y_dim())) {
                        rest.second = new_y - static_cast<int_ty>((grid.y_dim() - 1));
                        new_y = static_cast<int_ty>(grid.y_dim() - 1);
                        }
                     return std::unexpected( error_ty { { grid, static_cast<size_ty>(new_x), 
                                                          static_cast<size_ty>(new_y) },
                                                          rest });
                     }
                  else return coord_ty { grid, static_cast<size_t>(new_x), static_cast<size_t>(new_y) };
                  }               
               }

            std::expected<coord_ty, error_ty> operator - (distance_ty const& other) const {
               auto const& [delta_first, delta_second] = static_cast<distance_ty::data_ty const&>(other);
               distance_ty operand = { -delta_first, -delta_second };
               return *this + operand;
               }


            distance_ty operator - (coord_ty const& other) const {
               return  { static_cast<int_ty>(std::get<0>(data)) - static_cast<int_ty>(std::get<0>(other.data)),
                         static_cast<int_ty>(std::get<1>(data)) - static_cast<int_ty>(std::get<1>(other.data)) };
               }



            size_ty row() const requires is_grid<kind> { return std::get<0>(data);  }
            size_ty col() const requires is_grid<kind> { return std::get<1>(data); }

            size_ty x() const requires is_matrix<kind> { return std::get<1>(data); }
            size_ty y() const requires is_matrix<kind> { return std::get<0>(data); }

            data_ty&       get_data() { return data; }
            data_ty const& get_data() const { return data; }
         };


      private:
         using data_ty = std::tuple<size_ty, size_ty>; // before size_t  rows_val, cols_val
         std::vector<ty> raw_data;
         mdspan2d_t      data_span;
         data_ty         data;
      public:
         grid_2D(size_ty rows_para, size_ty cols_para) requires is_grid<kind>
                                                     : raw_data(rows_para * cols_para), 
                                                       data_span(raw_data.data(), rows_para, cols_para), 
                                                       data { rows_para, cols_para } { }

         grid_2D(size_ty x_para, size_ty y_para) requires is_matrix<kind>
                                                     : raw_data(x_para * y_para),
                                                       data_span(raw_data.data(), y_para, x_para),
                                                       data { y_para, x_para } { }

         grid_2D(grid_2D const& other)  requires is_grid<kind>
                                                     : raw_data(other.raw_data), 
                                                       data_span(raw_data.data(), std::get<0>(other), std::get<1>(other)),
                                                       data { other.data } {  }

         grid_2D(grid_2D const& other)  requires is_matrix<kind>
                                           : raw_data(other.raw_data), 
                                             data_span(raw_data.data(), std::get<1>(other), std::get<0>(other)),
                                             data { other.data } {  }


         grid_2D(grid_2D&& other) noexcept requires is_grid<kind>
                                           : raw_data { std::move(other.raw_data) }, 
                                             data_span(raw_data.data(), std::get<0>(other), std::get<1>(other)),
                                             data { std::move(other.data) } { }

         grid_2D(grid_2D&& other) noexcept requires is_matrix<kind>
                                           : raw_data { std::move(other.raw_data) }, 
                                             data_span(raw_data.data(), std::get<1>(other), std::get<0>(other)),
                                             data { std::move(other.data) } { }

         virtual ~grid_2D() = default;

         grid_2D& operator = (grid_2D const& other) {
            if (this != &other) {
               raw_data  = other.raw_data;
               data      = other.data;
               if constexpr (kind = EKind::grid)
                  data_span = mdspan_t(raw_data.data(), std::get<0>(data), std::get<1>(data));
               else
                  data_span = mdspan_t(raw_data.data(), std::get<1>(data), std::get<0>(data));
               }
            return *this;
            }

         grid_2D& operator = (grid_2D&& other) noexcept {
            if (this != &other) {
               raw_data  = std::move(other.raw_data);
               data      = std::move(other.data);
               if constexpr (kind = EKind::grid)
                  data_span = mdspan_t(raw_data.data(), std::get<0>(data), std::get<1>(data));
               else
                  data_span = mdspan_t(raw_data.data(), std::get<1>(data), std::get<0>(data));
               }
            return *this;
            }

         grid_2D& operator = (std::string const& str)  requires std::is_convertible_v<char, ty> {
            auto convert_func = [](char val) { return static_cast<ty>(val); };
            const size_ty expected_size = std::get<0>(data) * std::get<1>(data);;
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
            const size_ty expected_size = std::get<0>(data) * std::get<1>(data);
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
         

         grid_2D& operator = (std::vector<ty>&& vec) {
            const size_ty expected_size = std::get<0>(data) * std::get<1>(data);
            if (vec.size() != expected_size) {
               raw_data = std::move(vec);
               }
            return *this;
            }

         bool Valid(size_ty row, size_ty col) const requires is_grid<kind>  {
            return (row < std::get<0>(data) && col < std::get<1>(data) ? true : false);
            }

         bool Valid(size_ty x_, size_ty y_) const requires is_matrix<kind> {
            return (x_ < std::get<1>(data) && y_ < std::get<0>(data) ? true : false);
            }

         bool Valid(coord_ty const& indices) const {
            if constexpr (kind == EKind::grid)
               return Valid(indices.row(), indices.col());
            else
               return Valid(indices.y(), indices.x());
            }

         void CheckBounds(size_ty row, size_ty col) const requires is_grid<kind> {
            if (row >= std::get<0>(data) || col >= std::get<1>(data)) {
               throw std::out_of_range("index for grid2d out of bounds");
               }
            }

         void CheckBounds(size_ty x_, size_ty y_) const requires is_matrix<kind> {
            if (x_ >= std::get<1>(data) || y_ >= std::get<0>(data)) {
               throw std::out_of_range("index for grid2d out of bounds");
               }
            }


         ty& operator[](size_ty row, size_ty col) requires is_grid<kind> {
            CheckBounds(row, col);
            return data_span[row, col];
            }

         ty const& operator[](size_ty row, size_ty col) const requires is_grid<kind> {
            CheckBounds(row, col);
            return data_span[row, col];
            }

         ty& operator[](size_ty x_, size_ty y_) requires is_matrix<kind> {
            CheckBounds(x_, y_);
            return data_span[y_, x_];
            }

         ty const& operator[](size_ty x_, size_ty y_) const requires is_matrix<kind> {
            CheckBounds(x_, y_);
            return data_span[y_, x_];
            }


         ty& operator[](coord_ty const& indices) {
            if constexpr (kind == EKind::grid) {
               size_ty row = indices.row(), col = indices.col();
               CheckBounds(row, col);
               return data_span[row, col];
               }
            else {
               auto const& [x_, y_] = indices;
               CheckBounds(x_, y_);
               return data_span[x_, y_];
               }
            }

         ty const& operator[](coord_ty const& indices) const {
            if constexpr (kind == EKind::grid) {
               size_ty row = indices.row(), col = indices.col();
               CheckBounds(row, col);
               return data_span[row, col];
               }
            else {
               auto const& [x_, y_] = indices;
               CheckBounds(x_, y_);
               return data_span[x_, y_];
               }
            }

         coord_ty GetCoordinates(size_ty first, size_ty second) const {
            CheckBounds(first, second);
            return { *this, first, second };
            }

         coord_ty GetCoordinates(const_iterator it) const {
            if(it < cbegin() || it == cend()) {
               throw std::invalid_argument("iterator for grid2d is out of bounds.");
               }
            if (size_t pos = std::distance(cbegin(), it); pos >= raw_data.size()) {
               throw std::out_of_range("position in grid_2D is out of bounds");
               }
            else [[likely]] {
               if constexpr (kind == EKind::grid)
                  return { *this, pos / cols(), pos % cols() };
               else 
                  return { *this, pos % x_dim(), pos / x_dim() };
               }
            }

         ty& operator()(size_t row, size_t col) requires is_grid<kind> { return data_span(row, col);  }
         ty const& operator()(size_t row, size_t col) const requires is_grid<kind> { return data_span(row, col);  }
         ty& operator()(size_t x_, size_t y_) requires is_matrix<kind> { return data_span(y_, x_); }
         ty const& operator()(size_t x_, size_t y_) const requires is_matrix<kind> { return data_span(y_, x_); }

         ty& operator()(coord_ty pos) requires is_grid<kind> { return data_span(std::get<0>(pos), std::get<1>(pos)); }
         ty const& operator()(coord_ty pos) const requires is_grid<kind> { return data_span(std::get<0>(pos), std::get<1>(pos)); }
         ty& operator()(coord_ty pos) requires is_matrix<kind> { return data_span(std::get<1>(pos), std::get<0>(pos)); }
         ty const& operator()(coord_ty pos) const requires is_matrix<kind> { return data_span(std::get<1>(pos), std::get<0>(pos)); }

         coord_ty operator()(size_t pos) const {
            if (pos >= raw_data.size()) {
               throw std::out_of_range("position in grid_2D is out of bounds");
               }
            else
               if constexpr(kind == EKind::grid)
                  return { *this, pos / cols(), pos % cols() };
               else
                  return { *this, pos / x_dim(), pos % x_dim() };
            }
         
         
         void print(std::ostream& out, size_t width) requires std::is_same_v<ty, char> {
            for (size_t i = 0; i < std::get<0>(data); ++i) {
               for (size_t j = 0; j < std::get<0>(data); ++j) std::print(out, "{:>{}}", (*this)[i, j], width);
               std::println(out, "");
               }
            }

         size_t rows() const requires is_grid<kind> { return std::get<0>(data); }
         size_t cols() const requires is_grid<kind> { return std::get<1>(data); }
         size_t x_dim() const requires is_matrix<kind> { return std::get<1>(data); }
         size_t y_dim() const requires is_matrix<kind> { return std::get<0>(data); }

         iterator       begin()        { return raw_data.begin(); }
         iterator       end()          { return raw_data.end(); }

         const_iterator begin() const  { return raw_data.begin(); }
         const_iterator end() const    { return raw_data.end(); }

         const_iterator cbegin() const { return raw_data.cbegin(); }
         const_iterator cend() const   { return raw_data.cend(); }

      };


      template <typename ty>
      concept my_grid_ty = requires {
           typename ty::value_type;
           typename ty::mdspan2d_t;
           typename ty::iterator;
           typename ty::const_iterator;
           typename ty::coord_ty;
           typename ty::coord_ty::distance_ty;
         }  &&
           std::is_same_v<typename ty::iterator, typename std::vector<typename ty::value_type>::iterator> &&
           std::is_same_v<typename ty::const_iterator, typename std::vector<typename ty::value_type>::const_iterator> &&
           std::is_same_v<typename ty::mdspan2d_t, std::mdspan<typename ty::value_type, std::dextents<size_t, 2>>>;


      // -----------------------------------------------------------------------------------------------------


      template <typename ty, EKind kind = EKind::grid>
      class grid2d_view {
      template <bool>
      friend class iterator_base;
      public:
         using used_grid = grid_2D<ty, kind>;

         template <bool IsConst>
         class iterator_base {
            template <typename ty, EKind kind>
            friend class grid2d_view;
         private:
            using grid2d_ref = std::conditional_t<IsConst, grid2d_view const&, grid2d_view&>;
            grid2d_ref        grid;
            size_t            current_index;
         public:
            using iterator_category = std::random_access_iterator_tag;
            using iterator_type     = iterator_base<IsConst>;
            using value_type        = ty;
            using difference_type   = std::ptrdiff_t;
            using pointer           = ty*;
            using reference         = ty&;
            using const_reference   = ty const&;
            using coord_ty = typename grid_2D<ty, kind>::coord_ty;

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
               if constexpr (kind == EKind::grid) { 
                  auto const& row = grid.path_way[current_index].row();
                  auto const& col = grid.path_way[current_index].col();
                  //std::println(std::cout, "----> {},{}  ({})", x, y, current_index);
                  return grid[row, col];
                  }
               else {
                  auto const& x_ = grid.path_way[current_index].x();
                  auto const& y_ = grid.path_way[current_index].y();
                  //std::println(std::cout, "----> {},{}  ({})", x, y, current_index);
                  return grid[y_, x_];
               }
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

            
            coord_ty get_coords() const {
               //auto const& [x, y] = grid.path_way[current_index];
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

         grid2d_view(used_grid& data, std::vector<typename used_grid::coord_ty> const& coords) : data_grid(data), path_way(coords) {  }
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

         ty& operator[](used_grid::coord_ty const& indices) {
            return data_grid[indices];
            }

         ty const& operator[](used_grid::coord_ty const& indices) const {
            return data_grid[indices];
            }

      private:
         used_grid&                           data_grid;
         std::vector<typename used_grid::coord_ty> const& path_way;
      };

      
      template <typename ty, EKind kind>
      inline std::vector<std::vector<typename grid_2D<ty, kind>::coord_ty>> find_horz_inner_paths(grid_2D<ty, kind> const& grid) {
         using coord_ty = typename grid_2D<ty, kind>::coord_ty;
         std::vector<std::vector<coord_ty>> results;

         for(auto row_idx : std::views::iota(size_t { 1 }, grid.rows() - 1)) {
            results.emplace_back( std::views::zip(std::views::repeat(row_idx) | std::views::take(grid.cols()), 
                                                  std::views::iota(size_t { 1 }, grid.cols() - 1))
                                                 | std::views::transform([&grid](auto val) { return coord_ty(grid, std::move(val)); })
                                                 | std::ranges::to<std::vector>());
            }
         return results;
         }

      template <typename ty, EKind kind>
      std::vector<std::vector<typename grid_2D<ty, kind>::coord_ty>> find_horz_paths(grid_2D<ty, kind> const& grid) {
         using coord_ty = typename grid_2D<ty, kind>::coord_ty;
         std::vector<std::vector<coord_ty>> results;

         for (auto row_idx : std::views::iota(size_t{ 0 }, grid.rows())) {
            results.emplace_back( std::views::zip(std::views::repeat(row_idx) | std::views::take(grid.cols()),
                                                  std::views::iota(size_t{ 0 }, grid.cols()))
                                                | std::views::transform([&grid](auto val) {  return coord_ty(grid, std::move(val)); })
                                                | std::ranges::to<std::vector>());
            }
         return results;
         }

      template <typename ty, EKind kind>
      std::vector<std::vector<typename grid_2D<ty, kind>::coord_ty>> find_vert_paths(grid_2D<ty, kind> const& grid) {
         using coord_ty = typename grid_2D<ty, kind>::coord_ty;
         std::vector<std::vector<coord_ty>> results;

         for (auto col_idx : std::views::iota(size_t{ 0 }, grid.cols())) {
            results.emplace_back(std::views::zip(std::views::iota(size_t{ 0 }, grid.rows()),
                                                 std::views::repeat(col_idx) | std::views::take(grid.rows()))
                                                | std::views::transform([&grid](auto val) {  return coord_ty(grid, std::move(val)); })
                                                | std::ranges::to<std::vector>());
            }
         return results;
         }



      template <typename ty, EKind kind>
      std::vector<std::vector<typename grid_2D<ty, kind>::coord_ty>> find_all_paths(grid_2D<ty, kind> const& grid) {
         using coord_ty = typename grid_2D<ty, kind>::coord_ty;
         std::vector<std::vector<coord_ty>> results;

         for (auto row_idx : std::views::iota(size_t{ 0 }, grid.rows())) {
            results.emplace_back( std::views::zip(std::views::repeat(row_idx) | std::views::take(grid.cols()),
                                                  std::views::iota(size_t{ 0 }, grid.cols()))
                                                | std::views::transform([&grid](auto val) {  return coord_ty(grid, std::move(val)); })
                                                | std::ranges::to<std::vector>());
            }
         
         for (auto col_idx : std::views::iota(size_t{ 0 }, grid.cols())) {
            results.emplace_back(std::views::zip(std::views::iota(size_t{ 0 }, grid.rows()),
                                                 std::views::repeat(col_idx) | std::views::take(grid.rows()))
                                                | std::views::transform([&grid](auto val) {  return coord_ty(grid, std::move(val)); })
                                                | std::ranges::to<std::vector>());
            }

   
         for (auto row_idx : std::views::iota(size_t{ 0 }, grid.rows())) {
            results.emplace_back(std::views::zip(std::views::iota(row_idx, grid.rows()),
                                                 std::views::iota(size_t{ 0 }, grid.cols() - row_idx))
                                                | std::views::transform([&grid](auto val) { return coord_ty(grid, std::move(val)); })
                                                | std::ranges::to<std::vector>());
            } 
   

         for (auto col_idx : std::views::iota(size_t{ 1 }, grid.cols())) {
            results.emplace_back(std::views::zip(std::views::iota(size_t { 0 }, grid.rows()),
                                                 std::views::iota(col_idx, grid.cols()))
                                                | std::views::transform([&grid](auto val) {  return coord_ty(grid, std::move(val)); })
                                                | std::ranges::to<std::vector>());
            }

         for(auto col_idx : std::views::iota(size_t{ 1 }, grid.cols())) {
            auto coords = std::views::zip(std::views::iota(size_t { 0 }, grid.rows()),
                                          std::views::iota(size_t { 0 }, col_idx) | std::views::reverse)
                                          | std::views::transform([&grid](auto val) {  return coord_ty(grid, std::move(val)); })
                                          | std::ranges::to<std::vector>();
            if (coords.size() > 1) results.emplace_back(coords);
            }

         for (auto row_idx : std::views::iota(size_t{ 0 }, grid.rows() - 1)) {
            auto coords = std::views::zip(std::views::iota(row_idx, grid.rows()),
                                          std::views::iota(size_t{ 0 }, grid.cols()) | std::views::reverse)
                                          | std::views::transform([&grid](auto val) {  return coord_ty(grid, std::move(val)); })
                                          | std::ranges::to<std::vector>();
            results.emplace_back(coords);
            }

         return results;
         }

   } // end of namespace grid

}  // end of namespace

/*
template <typename ty>
struct std::formatter<own::grid::grid_2D<typename ty, own::grid::EKind::grid>::coord_ty> : std::formatter<std::string_view> {
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

   auto format(own::grid::coord_2D_type<kind> const& val, std::format_context& ctx) const {
      std::string temp;

      size_t row_val = []() -> size_t {
         if constexpr (kind == own::grid::EKind::grid) val.row();
         else val.x();
         }();
      size_t col_val = []() -> size_t {
         if constexpr (kind == own::grid::EKind::grid) val.col();
         else val.y();
         }(); 

      std::string fmt = std::format("( {0:}, {0:} )", format_string);
      std::vformat_to(std::back_inserter(temp), fmt, std::make_format_args(row_val, col_val));
      return std::formatter<std::string_view>::format(temp, ctx);
      }
};
*/


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

/*
namespace std {
   template<>
   struct tuple_size<typename own::grid::grid_2D::coord_ty> : std::integral_constant<size_t, 2> {};

   template<size_t N>
   struct tuple_element<N, own::grid::grid_2D<own::grid::EKind::grid>::coord_ty> {
      using type = size_t;
      };

   template<size_t N>
   size_t const& get(own::grid::grid_2D<own::grid::EKind::grid>::coord_ty const& obj) {
      if constexpr (N == 0) return std::get<0>(obj.get_data());
      else if constexpr (N == 1) return std::get<1>(obj.get_data());
      }
   
   template<size_t N>
   size_t& get(own::grid::grid_2D<own::grid::EKind::grid>::coord_ty& obj) {
      if constexpr (N == 0) return std::get<0>(obj.get_data());
      else if constexpr (N == 1) return std::get<1>(obj.get_data());
      }

}
*/
