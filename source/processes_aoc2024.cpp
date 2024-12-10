#include "processes_aoc2024.h"

#include "qt_stream.h"
#include "qt_iteratoren.h"
#include "my_common_tools.h"
#include "my_ranges_tools.h"
//#include "my_grid_view.h"
#include "my_grid2d.h"

#include "Day_01.h"
#include "Day_02.h"
#include "Day_03.h"

#include <iostream>
#include <string_view>
#include <vector>
#include <utility>
#include <functional>
#include <regex>
#include <stdexcept>
#include <execution>
#include <atomic>
#include <format>
#include <print>
#include <ranges>

using namespace std::string_literals;

//template <std::ranges::input_range range_ty>
std::pair<std::string, std::string> RiddleDay4(std::string&& text) { //TMyForm& frm) { // range_ty const& values
   std::atomic<size_t> result_1 = 0, result_2 = 0;

   //auto text = frm.GetText("memInput"s);
   auto rows = std::ranges::count(text, '\n');
   auto cols = text.size() / rows - 1;
   text.erase(std::ranges::remove(text, '\n').begin(), text.end());

   own::grid::grid_2D<char> vault(rows, cols);
   vault = text;

    std::vector<std::string> patters = { "XMAS"s, "SAMX"s };
   auto pathways = own::grid::find_all_paths(vault);

   for (auto const& patter : patters) {
      auto searcher = std::boyer_moore_searcher(patter.begin(), patter.end());
      std::for_each(std::execution::par, pathways.begin(), pathways.end(),
                           [&vault, &searcher, &result_1, &patter](auto const& pathway) {
            own::grid::grid2d_view<char> grid(vault, pathway);

            size_t pos = 0u;
            auto result = std::search(grid.begin() + pos, grid.end(), searcher);
            for (; result != grid.end(); result = std::search(grid.begin() + pos, grid.end(), searcher)) {
               ++result_1;
               pos = (result - grid.begin()) + patter.size();
               }
         });
      }

   std::println(std::cout, "the result for the 1st part is {}", result_1.load());

   auto innerPathways = own::grid::find_horz_inner_paths(vault);

   auto xmas_match = [&vault](std::tuple<size_t, size_t> const& val) -> bool {
      auto const& [x, y] = val;
      if (vault[x, y] == 'A') {
         if ((vault[x-1, y-1] == 'M' && vault[x-1, y+1] == 'S' && vault[x+1, y-1] == 'M' && vault[x+1, y+1] == 'S') ||
             (vault[x-1, y-1] == 'M' && vault[x-1, y+1] == 'M' && vault[x+1, y-1] == 'S' && vault[x+1, y+1] == 'S') ||
             (vault[x-1, y-1] == 'S' && vault[x-1, y+1] == 'S' && vault[x+1, y-1] == 'M' && vault[x+1, y+1] == 'M') ||
             (vault[x-1, y-1] == 'S' && vault[x-1, y+1] == 'M' && vault[x+1, y-1] == 'S' && vault[x+1, y+1] == 'M')) return true;
         else [[likely]] return false;
         }
      else [[likely]] return false;

      };

   for(auto const& pathway : innerPathways) {
      result_2 += std::ranges::count_if(pathway, xmas_match);
      }

   std::println(std::cout, "the result for the 2nd part is {}", result_2.load());


   return { to_String(result_1.load()), to_String(result_2.load()) };
   }


// there seems a problem with the iterator, when passed as parameter
//template <std::ranges::input_range range_ty>
std::pair<std::string, std::string> RiddleDay5(TMyForm& frm) { // range_ty const& values
   auto values = frm.GetLines("memInput");
   auto rules        = values | std::views::transform(toString<std::string_view>)
                              | std::views::filter(checkSeparatedIntegersPairs)
                              | std::views::transform([](auto const& p) { return parseSeparatedPairs<size_t>({ p.begin(), p.end() });})
                              | std::ranges::to<std::set>();
   auto update_pages = values | std::views::transform(toString<std::string_view>)
                              | std::views::filter(checkSeparatedIntegers<','>)
                              | std::views::transform([](auto const& p) { return parseSeparatedIntegers<size_t>(std::string { p.begin(), p.end() });})
                              | std::ranges::to<std::vector<std::vector<size_t>>>();
  
   auto check = [&rules](std::vector<size_t> const& line) -> bool {
      for (auto const& pair_vals : line | std::views::slide(2)) {
         std::vector<size_t> slidevals;
         for (auto elem : pair_vals) slidevals.emplace_back(elem);
         if (rules.find(std::make_tuple(slidevals[0], slidevals[1])) == rules.end() &&
            rules.find(std::make_tuple(slidevals[1], slidevals[0])) != rules.end()) return false;
         }
      return true;
      };

   size_t result_1 = 0;
   for(auto const& current_update : update_pages) {
      if (check(current_update) && current_update.size() % 2 != 0) result_1 += current_update[current_update.size() / 2];
      }

   std::println(std::cout, "the result for the 1st part is {}", result_1);

   size_t result_2 = 0;
   static auto swap_elements = [](size_t& val1, size_t& val2) {
      auto tmp = val1;
      val1 = val2;
      val2 = tmp;
      };

   for (std::vector<size_t>& current_update : (update_pages | std::views::filter([&rules, &check](auto const& vec) { return !check(vec); })) ) {
      do {
         for (auto const& vals : std::ranges::views::zip(std::views::iota(1) | std::views::take(current_update.size() - 1), std::views::iota(0))) {
            auto& [idx, prev_idx] = vals;
            size_t& element = current_update[idx];
            size_t& prev = current_update[prev_idx];
            if (rules.find(std::make_tuple(prev, element)) == rules.end() &&
               rules.find(std::make_tuple(element, prev)) != rules.end()) swap_elements(element, prev);
            }
         } 
      while (!check(current_update));
      result_2 += current_update[current_update.size() / 2];
      //std::println(std::cout, "{}", current_update);
      }
   
   std::println(std::cout, "the result for the 2nd part is {}", result_2);

   return { to_String(result_1), to_String(result_2) };
   }

enum class EDirections : char {
   upward = '^', right = '>', downward = 'v', left = '<'
  };


bool is_rectangle(std::array<own::grid::coord_2D_type, 4> const& points) {
   std::array<size_t, 4> rows, cols;
   for (size_t i = 0; i < 4; ++i) {
      //std::tie(rows[i], cols[i]) = points[i];
      rows[i] = std::get<0>(points[i]);
      cols[i] = std::get<1>(points[i]);
   }

   std::ranges::sort(rows);
   std::ranges::sort(cols);

   auto last_row = std::unique(rows.begin(), rows.end());
   auto last_col = std::unique(cols.begin(), cols.end());

   size_t unique_rows = std::distance(rows.begin(), last_row);
   size_t unique_cols = std::distance(cols.begin(), last_col);

   return unique_rows == 2 && unique_cols == 2;
}

template <typename ty, size_t SIZE>
bool next_combination(std::array<ty, SIZE>& indices, size_t n) {
   size_t k = SIZE;
   for (size_t i = k; i-- > 0;) {
      if (indices[i] != i + n - k) {
         ++indices[i];
         for (size_t j = i + 1; j < k; ++j) {
            indices[j] = indices[j - 1] + 1;
            }
         return true;
         }
      }
   return false;
   }

template <typename ty, size_t SIZE>
std::vector<std::array<ty, SIZE>> combination(std::span<ty> values) {
   std::vector<std::array<ty, SIZE>> result;
   if (values.size() < SIZE) return result;

   std::array<size_t, SIZE> indices;
   std::iota(indices.begin(), indices.end(), 0);

   do {
      std::array<ty, SIZE> combination;
      for (size_t i = 0; i < SIZE; ++i) combination[i] = values[indices[i]];
      result.emplace_back(combination);
      } 
   while (next_combination<size_t, SIZE>(indices, values.size()));
   return result;
   }


//template <std::ranges::input_range range_ty>
std::pair<std::string, std::string> RiddleDay6(std::string&& text) { // range_ty const& values
   const auto rows = std::ranges::count(text, '\n');
   const auto cols = text.size() / std::ranges::count(text, '\n') - 1;
   text.erase(std::ranges::remove(text, '\n').begin(), text.end());
   own::grid::grid_2D<char> grid(rows, cols);
   grid = text;

   auto horizontal = own::grid::find_horz_paths(grid);
   auto vertical = own::grid::find_vert_paths(grid);
   std::vector<own::grid::coord_2D_type> path;

   auto turn_right = [](EDirections dir) -> EDirections {
      static std::map<EDirections, EDirections> rules = {
               { EDirections::upward , EDirections::right },
               { EDirections::right,   EDirections::downward },
               { EDirections::downward, EDirections::left },
               { EDirections::left, EDirections::upward }
               };
      return rules[dir];
      };

   using move_ret_type = std::tuple<size_t, size_t, bool>;
   std::map<EDirections, std::function<move_ret_type(size_t row, size_t col)>> moves_func = {
      { EDirections::upward, [&grid, &vertical, &path](size_t row, size_t col) -> move_ret_type {
              own::grid::grid2d_view<char> view(grid, vertical[col]);
              auto start = view.rpos(row);
              if (auto it = std::find(start, view.rend(), '#'); it != view.rend()) {
                 for (auto step = start; step < it; ++step) { *step = 'O';  };
                 auto const& [new_row, new_col] = (it.base() - 1).get_coords();  // base the nearest Element, current is protected
                 return { new_row + 1, new_col, true };
                 }
              else {
                 for (auto step = start; step < it; ++step) { *step = 'O'; };
                 return { 0, col, false };
                 }
              }},
      { EDirections::right, [&grid, &horizontal, &path](size_t row, size_t col) -> move_ret_type {
              own::grid::grid2d_view<char> view(grid, horizontal[row]);
              auto start = view.pos(col);
              if (auto it = std::find(start, view.end(), '#'); it != view.end()) {
                 for (auto step = start; step < it; ++step) { *step = 'O'; };
                 auto const& [new_row, new_col] = it.get_coords();  
                 return { new_row, new_col - 1, true };
                 }
              else {
                 for (auto step = start; step < it; ++step) { *step = 'O'; };
                 return { row, grid.cols(), false };
                 }
              }},
      { EDirections::downward, [&grid, &vertical, &path](size_t row, size_t col) -> move_ret_type {
              own::grid::grid2d_view<char> view(grid, vertical[col]);
              auto start = view.pos(row);
              if (auto it = std::find(start, view.end(), '#'); it != view.end()) {
                 for (auto step = start; step < it; ++step) { *step = 'O'; };
                 auto const& [new_row, new_col] = it.get_coords();  
                 return { new_row - 1, new_col, true };
                 } 
              else {
                 for (auto step = start; step < it; ++step) { *step = 'O'; };
                 return { grid.rows(), col, false };
                 }
              }},
      { EDirections::left, [&grid, &horizontal, &path](size_t row, size_t col) -> move_ret_type {
              own::grid::grid2d_view<char> view(grid, horizontal[row]);
              auto start = view.rpos(col);
              if (auto it = std::find(start, view.rend(), '#'); it != view.rend()) {
                 for (auto step = start; step < it; ++step) { *step = 'O'; };
                 auto const& [new_row, new_col] = (it.base() - 1).get_coords();  // base the nearest Element, current is protected
                 return { new_row, new_col + 1, true };
                 }
              else {
                 for (auto step = start; step < it; ++step) { *step = 'O'; };
                 return { 0, col, false };
                 }
              } }

      };


   if(auto pos = std::find(grid.begin(), grid.end(), '^'); pos != grid.end()) {
      auto const& [c_row, c_col] = grid(std::distance(grid.begin(), pos));
      //*pos = 'O';
      EDirections move = EDirections::upward;
      path.emplace_back(own::grid::coord_2D_type{ c_row, c_col });
      auto [row_, col_, ok_] = moves_func[move](c_row, c_col);
      for(; ok_; std::tie(row_, col_, ok_) = moves_func[move](row_, col_)) {
         path.emplace_back(own::grid::coord_2D_type{ row_, col_ });
         move = turn_right(move);
         }
      path.emplace_back(own::grid::coord_2D_type{ row_, col_ });

      auto res2 = combination<own::grid::coord_2D_type, 4>(path)
                      | std::views::filter([](auto const& v) { return is_rectangle(v); })
                      | std::ranges::to<std::vector>();
      for(auto const& val : res2) 
         std::apply([](auto&&... args) { return std::println(std::cout, "{} {} {} {}", args...); }, val);
      //grid.print(std::cout);
      }



   size_t result_1 = 0, result_2 = 0;
   result_1 = std::count(grid.begin(), grid.end(), 'O');
   std::println(std::cout, "the result for the 1st part is {}", result_1);
   return { to_String(result_1), to_String(result_2) };
   }


template <my_integral_ty ty, typename func_ty>
bool recursion_func_1(std::span<ty> values, ty res, func_ty&& func, ty acc) {
   return values.empty() ? acc == res :
             recursion_func_1<ty>(values.subspan(1), res, std::plus<ty>{}, func(acc, values.front())) ? true :
             recursion_func_1<ty>(values.subspan(1), res, std::multiplies<ty>{}, func(acc, values.front()));
   }

template <my_integral_ty ty, typename func_ty>
bool recursion_func_2(std::span<ty> values, ty res, func_ty&& func, ty acc) {
   return values.empty() ? acc == res :
      recursion_func_2<ty>(values.subspan(1), res, std::plus<ty>{}, func(acc, values.front())) ? true :
      recursion_func_2<ty>(values.subspan(1), res, std::multiplies<ty>{}, func(acc, values.front()))? true :
      recursion_func_2<ty>(values.subspan(1), res, concatenateNumbers_ty<ty>{}, func(acc, values.front()));
   }

template <std::ranges::input_range range_ty>
std::pair<std::string, std::string> RiddleDay7(range_ty const& values) {
   size_t result_1 = 0, result_2 = 0;

   auto tasks = values | std::views::transform([](std::string_view const& p) { return splitString(p, ':'); })
                       | std::views::filter([](auto const& opt) { return opt.has_value(); })
                       | std::views::transform([](auto const& p) {
                             auto result = toInt<size_t>(std::get<0>(*p));
                             auto input = extractNumbers<size_t>(std::get<1>(*p));
                             return std::make_tuple(result, input);
                             })
                       | std::ranges::to<std::vector>();


   for (auto& [result, input] : tasks) {
      result_1 += recursion_func_1<size_t>(input, result, std::plus<size_t>{}, 0u) ? result : 0;
      result_2 += recursion_func_2<size_t>(input, result, std::plus<size_t>{}, 0u) ? result : 0;
      }

   std::println(std::cout, "the result for the 1st part is {}, expected {}", result_1, 20281182715321);
   std::println(std::cout, "the result for the 2nd part is {}, solved but wrong {}", result_2, 32565384255325);

   return { to_String(result_1), to_String(result_2) };
   }
           
template <typename ty>
std::vector<std::pair<ty, ty>> combine(std::vector<ty> const& elements) {
    const size_t n = elements.size();
   const size_t pair_count = (n * (n - 1)) / 2; 
   std::vector<std::pair<own::grid::coord_2D_type, own::grid::coord_2D_type>> result;
   result.reserve(pair_count); 

   for (auto i = 0u; i < elements.size(); ++i) {
      for (auto j = i + 1; j < elements.size(); ++j) {
         result.emplace_back(elements[i], elements[j]);
         }
      }
   return result;
   }

//template <std::ranges::input_range range_ty>
std::pair<std::string, std::string> RiddleDay8(std::string&& text) { //range_ty const& values) {
   const auto rows = std::ranges::count(text, '\n');
   const auto cols = text.size() / std::ranges::count(text, '\n') - 1;
   text.erase(std::ranges::remove(text, '\n').begin(), text.end());
   own::grid::grid_2D<char> grid(rows, cols);
   grid = text;

   std::map<char, std::vector<own::grid::coord_2D_type>> values;
   auto current = grid.begin();
   auto it = std::find_if(current, grid.end(), [](char c) { return c != '.'; });
   while (it != grid.end()) {
      values[*it].emplace_back(grid.GetCoordinates(it));
      it = std::find_if(++it, grid.end(), [](char c) { return c != '.'; });
      }

   for(auto const& [key, vals] : values) {
      auto pairs = combine<own::grid::coord_2D_type>(vals);
      for(auto const& [first, second] : pairs) {
         auto distance = first - second;
         if(distance) {
            auto new_coord_1 = first  - *distance;
            auto new_coord_2 = second + *distance;
            if (new_coord_1) if (grid.Valid(*new_coord_1)) grid[*new_coord_1] = '#';
            if (new_coord_2) if (grid.Valid(*new_coord_2)) grid[*new_coord_2] = '#';
            }
         }
      }

   grid.print(std::cout);

   size_t result_1 = 0, result_2 = 0;
   return { to_String(result_1), to_String(result_2) };
   }

template <std::ranges::input_range range_ty>
std::pair<std::string, std::string> RiddleDay9(range_ty const& values) {
   size_t result_1 = 0, result_2 = 0;
   return { to_String(result_1), to_String(result_2) };
   }

template <std::ranges::input_range range_ty>
std::pair<std::string, std::string> RiddleDay10(range_ty const& values) {
   size_t result_1 = 0, result_2 = 0;
   return { to_String(result_1), to_String(result_2) };
   }

TAOC2024Processes::cntrlDatas TAOC2024Processes::control = {
   {  1, {" 1st day"s,  "Historian Hysteria"s,    "https://adventofcode.com/2024/day/1"s, [](TMyForm& frm) { SetResult(frm, RiddleDay1(frm.GetLines("memInput"))); }, statusRiddle::both_solved }},
   {  2, {" 2nd day"s,  "Red-Nosed Reports"s,     "https://adventofcode.com/2024/day/2"s, [](TMyForm& frm) { SetResult(frm, RiddleDay2(frm.GetLines("memInput"))); }, statusRiddle::both_solved }},
   {  3, {" 3rd day"s,  "Mull It Over"s,          "https://adventofcode.com/2024/day/3"s, [](TMyForm& frm) { SetResult(frm, RiddleDay3(frm.GetLines("memInput"))); }, statusRiddle::both_solved }},
   {  4, {" 4th day"s,  "Ceres Search"s,          "https://adventofcode.com/2024/day/4"s, [](TMyForm& frm) { SetResult(frm, RiddleDay4(frm.GetText("memInput"s))); }, statusRiddle::both_solved }},
   {  5, {" 5th day"s,  "Print Queue"s,           "https://adventofcode.com/2024/day/5"s, [](TMyForm& frm) { SetResult(frm, RiddleDay5(frm)); }, statusRiddle::both_solved }},
   {  6, {" 6th day"s,  "Guard Gallivant"s,       "https://adventofcode.com/2024/day/6"s, [](TMyForm& frm) { SetResult(frm, RiddleDay6(frm.GetText("memInput"))); }, statusRiddle::first_solved }},
   {  7, {" 7th day"s,  "Bridge Repair"s,         "https://adventofcode.com/2024/day/7"s, [](TMyForm& frm) { SetResult(frm, RiddleDay7(frm.GetLines("memInput"))); }, statusRiddle::second_in_progress }},
   {  8, {" 8th day"s,  "Resonant Collinearity"s, "https://adventofcode.com/2024/day/8"s, [](TMyForm& frm) { SetResult(frm, RiddleDay8(frm.GetText("memInput"))); }, statusRiddle::first_in_progress }},
   {  9, {" 9th day"s,  "Disk Fragmenter"s,       "https://adventofcode.com/2024/day/9"s, [](TMyForm& frm) { SetResult(frm, RiddleDay9(frm.GetLines("memInput"))); }, statusRiddle::prepared }},
   //{ 10, {"10th day"s, ""s } }
   //{ 11, {"11th day"s, ""s } }
   //{ 12, {"12th day"s, ""s } }
   //{ 13, {"13th day"s, ""s } }
   //{ 14, {"14th day"s, ""s } }
   //{ 15, {"15th day"s, ""s } }
   //{ 16, {"16th day"s, ""s } }
   //{ 17, {"17th day"s, ""s } }
   //{ 18, {"18th day"s, ""s } }
   //{ 19, {"19th day"s, ""s } }
   //{ 20, {"20nd day"s, ""s } }
   //{ 21, {"21st day"s, ""s } }
   //{ 22, {"22nd day"s, ""s } }
   //{ 23, {"23rd day"s, ""s } }
   //{ 24, {"24th day"s, ""s } }
   //{ 25, {"25th day"s, ""s } }
};

void TAOC2024Processes::Init(TMyForm&& frm) {
   frm.SetCaption(strApplication);
   frm.SetLabel("lblLicense1"s,       "adecc Scholar's Advent of Code 2024 App"s);
   frm.SetLabel("lblLicense2"s,       "copyright ©2024, all rights reserved"s);
   frm.SetLabel("lblLicense3"s,       "adecc Systemhaus GmbH"s);
   frm.SetButton("btnInfo"s,          "information"s);
   frm.SetGroupBox("gbxOldRiddle"s,   "daily tasks"s);
   frm.SetLabel("lblRiddleBox"s,      "choose the daily task");
   frm.InitCheckBox("chkTestData",    "copy test data", false);

   frm.SetButton("btnOldURL"s,        "description for choosen day"s);
   frm.SetButton("btnInputData"s,     "input for the choosen day"s);
   frm.SetButton("btnPasteData"s,     "paste data from clipboard"s);
   frm.SetButton("btnOldRiddles"s,    "solve task for choosen day"s);

   frm.SetGroupBox("gbxResults"s,     "results"s);
   frm.SetEdit("edtResult_1"s,        ""s);
   frm.SetButton("btnCopyResult1"s,   ">"s);
   frm.SetEdit("edtResult_2"s,        ""s);
   frm.SetButton("btnCopyResult2"s,   ">"s);

   frm.SetButton("btnTest"s,          "test case"s);


   auto values = control | std::views::elements<1> 
                         | std::views::filter([](auto& d) {
                                 return std::get<4>(d) != statusRiddle::none && std::get<4>(d) != statusRiddle::added ; 
                                 })
                         | std::views::elements<0>;
   std::ranges::copy(values, make_qt_widget_output_iterator(frm.Find<QComboBox>("cbxOldRiddles")));
   frm.LastItem("cbxOldRiddles"s);
   Info(std::forward<TMyForm>(frm));
   }


void TAOC2024Processes::Info(TMyForm&& frm) {
   std::ostream oss(frm.GetAsStreamBuf("memInput"s, true));
   std::println(oss, "Advent of Code 2024");
   std::println(oss, "-------------------");
   for(auto const& [day, data] : control) {
      std::println(oss, "{:>8} : {:<30} ({:>2}) - {}", std::get<0>(data), std::get<1>(data), day, std::get<4>(data));
      }
   }

void TAOC2024Processes::SetResult(TMyForm& frm, std::pair<std::string, std::string> const& value) {
   frm.SetEdit({ { "edtResult_1"s, value.first }, { "edtResult_2"s, value.second } });
   }



void TAOC2024Processes::CopyTestData(TMyForm&& frm) {
   if (auto it = test.find(GetDay(frm.CurrentText("cbxOldRiddles"))); it != test.end()) {
      std::ostream oss(frm.GetAsStreamBuf("memInput"s, true));
      std::println(oss, "{}", std::get<0>(it->second));
      }
   }

void TAOC2024Processes::ChangeRiddle(TMyForm&& frm) {
   frm.Clear("memOutput"s);
   frm.SetEdit({ "edtResult_1"s, "edtResult_2"s }, ""s);
   std::string choosen = frm.CurrentText("cbxOldRiddles");
   auto day = GetDay(choosen);
   if (auto it = test.find(day); it != test.end()) {
      if(frm.GetCheckBox("chkTestData"s)) {
         std::ostream oss(frm.GetAsStreamBuf("memInput"s, true));
         std::println(oss, "{}", std::get<0>(it->second));
         }
      auto const& data = control[day];
      frm.SetCaption(std::format("{}: {} [{}]", strApplication, std::get<1>(data), std::get<4>(data)));
      std::println(std::clog, "selected daily task for {}, {}", std::get<0>(data), std::get<1>(data));
      }
   else {
      frm.SetCaption(std::format("{}", strApplication));
      std::println(std::clog, "information for the selected {} not found.", choosen);
      }
   }

size_t TAOC2024Processes::GetDay(std::string const& strDay) {
   if (auto it = std::ranges::find_if(control, [&strDay](auto const& elem) {
                                                     return std::get<0>(elem.second) == strDay;
                                                     }); it != control.end()) return it->first;
   else throw std::runtime_error(std::format("element for {} not found in GetDay().", strDay));
   }

TAOC2024Processes::cntrlData const& TAOC2024Processes::GetData(std::string const& strDay) {
   if (auto it = std::ranges::find_if(control, [&strDay](auto const& elem) {
                                                     return std::get<0>(elem.second) == strDay;
                                                     } ); it != control.end()) return it->second;
   else throw std::runtime_error(std::format("element for {} not found in GetData().", strDay));
   }

void TAOC2024Processes::Processes(size_t day, TMyForm&& frm) {
   frm.Clear("memOutput"s);
   try {
      if(auto it = control.find(day); it != control.end()) {
         auto const& [day, cntrl] = *it;
         std::println(std::cout, "{}, Day {}: {}", strApplication, day, std::get<1>(cntrl));
         std::println(std::clog, "starting {} \"{}\"...", std::get<0>(cntrl), std::get<1>(cntrl));
         TMyWait wait("memOutput"s);
         auto func_start = std::chrono::high_resolution_clock::now();
         std::get<3>(cntrl)(frm);
         auto func_ende = std::chrono::high_resolution_clock::now();
         auto func_time = std::chrono::duration_cast<std::chrono::milliseconds>(func_ende - func_start);

         if (auto it = test.find(day); it != test.end() && std::get<1>(it->second) && frm.GetCheckBox("chkTestData"s)) {
            auto & [check_1, check_2] = *std::get<1>(it->second);
            size_t result_1 = toInt<size_t>(frm.GetEditAs("edtResult_1"s));
            size_t result_2 = toInt<size_t>(frm.GetEditAs("edtResult_2"s));
            std::println(std::cout, "");
            std::println(std::cout, "Checking the solution with the test values");
            std::println(std::cout, " 1st part: {} {} {}", result_1, (result_1 == check_1 ? "==" : "!="), check_1);
            std::println(std::cout, " 2nd part: {} {} {}", result_2, (result_2 == check_2 ? "==" : "!="), check_2);
            if(result_1 == check_1 && result_2 == check_2) {
               std::println(std::cout, "Both results correspond to the expected values.");
               }
            }

         std::println(std::cout, "\ndaily task finished successful in {:.3f} sec", func_time.count() / 1000.);
         std::println(std::clog, "{} : {} finished", std::get<0>(cntrl), std::get<1>(cntrl));
         }
      else {
         std::println(std::cout, "{}, Day {}: no informations in the control structure", strApplication, day);
         std::println(std::clog, "missing informations for day {}", day);
         }
      }
   catch(std::exception const& ex) {
      std::println(std::cout, "error in program {}\n{}", strApplication, ex.what());
      }
   }
