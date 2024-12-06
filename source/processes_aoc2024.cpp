#include "processes_aoc2024.h"

#include "qt_stream.h"
#include "qt_iteratoren.h"
#include "my_common_tools.h"
#include "my_ranges_tools.h"
#include "my_grid_view.h"

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

template <std::ranges::input_range range_ty>
std::pair<std::string, std::string> RiddleDay1(range_ty const& values) {
   auto result = values | std::ranges::views::filter([](auto const& p) { return p.size() > 0; })
                        | std::ranges::views::transform(parsePair<size_t>)
                        | std::ranges::to<std::vector>();

   auto first_vals = result | own::views::first | std::ranges::to<std::vector>();
   auto second_vals = result | own::views::second | std::ranges::to<std::vector>();

   std::ranges::sort(first_vals);
   std::ranges::sort(second_vals);

   // total distance
   auto result_1 = std::ranges::fold_left(std::ranges::views::zip(first_vals, second_vals), 0,
         [](int acc, auto const& pair) {
            auto const& [a, b] = pair;
            return acc + Difference(a, b);
         });
   std::println(std::cout, "the result for the 1st part is {}", result_1);  // 1889772

   // similarity score
   auto result_2 = std::ranges::fold_left(first_vals, 0,
         [&second_vals](int acc, auto const& val) {
            return acc + val * std::ranges::count(second_vals, val);
         });
   std::println(std::cout, "the result for the 2nd part is {}", result_2);   // 23228917
   std::println(std::clog, "1st day finished");
   return { to_String(result_1), to_String(result_2) };
   }


template <std::ranges::input_range range_ty>
std::pair<std::string, std::string> RiddleDay2(range_ty const& values) {

      size_t result_1 = 0, result_2 = 0;
      for (auto const& data : values) {
         auto row = extractNumbers<size_t>(data);
         // std::print(std::cout, "{:>3} ->", row);
         std::vector<int> differences;
         for (auto const& diffs : row | std::views::slide(2)) {
            std::vector<size_t> slidevals;
            for (auto elem : diffs) {
               slidevals.emplace_back(elem);
            }
            differences.emplace_back(static_cast<int>(static_cast<long long>(slidevals[1]) - static_cast<long long>(slidevals[0])));
         }

         // std::print(std::cout, "   {:>3}\t", differences);

         bool allIncreasing = std::ranges::all_of(differences, [](int diff) { return diff > 0 && diff <= 3; });
         bool allDecreasing = std::ranges::all_of(differences, [](int diff) { return diff < 0 && diff >= -3; });
         bool allChecks = allIncreasing || allDecreasing;

         if (allChecks) {
            result_1 += 1;
            result_2 += 1;
            // std::println(std::cout, "korrekt");
         }
         else [[likely]] {
            for (size_t i = 0; i < row.size(); ++i) {
               auto subrow = row | std::views::enumerate
                  | std::views::filter([i](auto const& p) {
                  auto const& [idx, val] = p;
                  return idx != i;
                     })
                  | std::views::transform([](auto const& p) {
                  auto const& [idx, val] = p;
                  return val;
                     })
                  | std::ranges::to<std::vector>();

               std::vector<int> subdiffs;
               for (auto const& diffs : subrow | std::views::slide(2)) {
                  std::vector<size_t> slidevals;
                  for (auto elem : diffs) {
                     slidevals.emplace_back(elem);
                     }
                  subdiffs.emplace_back(static_cast<int>(static_cast<long long>(slidevals[1]) - static_cast<long long>(slidevals[0])));
                  }

               bool allIncreasing = std::ranges::all_of(subdiffs, [](int diff) { return diff > 0 && diff <= 3; });
               bool allDecreasing = std::ranges::all_of(subdiffs, [](int diff) { return diff < 0 && diff >= -3; });
               bool allChecks = allIncreasing || allDecreasing;
               if (allChecks) {
                  result_2 += 1;
                  break;
                  }
               }
            }
            /*
               auto cntIncreasing = std::ranges::count_if(differences, [](int diff) { return diff >= 0; });
               auto cntDecreasing = std::ranges::count_if(differences, [](int diff) { return diff <= 0; });

               if(cntIncreasing > cntDecreasing) {
                  if(auto it = std::ranges::find_if(differences, [](int diff) {  return diff <= 0 || diff > 3; }); it != differences.end()) {
                     if (auto next = it + 1; it != differences.begin() && next != differences.end()) *next += *it;
                     differences.erase(it);
                     }
                  allIncreasing = std::ranges::all_of(differences, [](int diff) { return diff > 0 && diff <= 3; });
                  allDecreasing = false;
                  }
               else {
                  if (auto it = std::ranges::find_if(differences, [](int diff) {  return diff >= 0 || diff < -3; }); it != differences.end()) {
                     if (auto next = it + 1; it != differences.begin() && next != differences.end()) *next += *it;
                     differences.erase(it);
                     }
                  allIncreasing = false;
                  allDecreasing = std::ranges::all_of(differences, [](int diff) { return diff < 0 && diff >= -3; });
                  }
               result_2 += allIncreasing || allDecreasing ? 1 : 0;
               std::println(std::cout, "{:3}\t{}", differences, (allIncreasing || allDecreasing ? "korrigiert" : "ungültig"));
               }
               */
      }

      std::println(std::cout, "the result for the 1st part is {}", result_1);
      std::println(std::cout, "the result for the 2nd part is {}", result_2);
      return { to_String(result_1), to_String(result_2) };
   }

template <std::ranges::input_range range_ty>
std::pair<std::string, std::string> RiddleDay3(range_ty const& values) {
   static std::regex pattern1(R"(mul\(([1-9][0-9]{0,2}),([1-9][0-9]{0,2})\))");
   std::vector<std::pair<size_t, size_t>> results;
   for (auto const& data : values) {
      std::string input = { data.begin(), data.end() };
      auto begin = std::sregex_iterator(input.begin(), input.end(), pattern1);
      auto end = std::sregex_iterator();

      for (std::sregex_iterator it = begin; it != end; ++it) {
         std::smatch match = *it;
         results.emplace_back(toInt<size_t>(match[1].str()), toInt<size_t>(match[2].str()));
         }
      }

   auto result_1 = std::accumulate(results.begin(), results.end(), size_t { 0 }, 
                                   [](size_t acc, std::pair<size_t, size_t>& p) {
                                       return acc + (p.first * p.second);
                                       }
                                   );
   //*
   results.clear();
   static std::regex pattern2(R"((mul\(([1-9][0-9]{0,2}),([1-9][0-9]{0,2})\))|(do\(\))|(don't\(\)))");
   for (bool boActive = true; auto const& data : values) {
      std::string input = { data.begin(), data.end() };
      auto begin = std::sregex_iterator(input.begin(), input.end(), pattern2);
      auto end = std::sregex_iterator();

      for (std::sregex_iterator it = begin; it != end; ++it) {
         std::smatch const& match = *it;


         if (match[4].matched) boActive = true;
         else if (match[5].matched) boActive = false;
         else if (match[1].matched && boActive) results.emplace_back(toInt<size_t>(match[2].str()), toInt<size_t>(match[3].str()));
         }
      }
   //*/
   /*
   bool boActive = true;
   static std::string strStart = "do()"s;
   static std::string strStop  = "don't()"s;

   for (results.clear(); auto const& data : values) {
      std::string input = { data.begin(), data.end() };

      for (size_t pos = (boActive ? 0u : input.find(strStart) + strStart.size()); pos != std::string::npos ; ) {
         size_t endpos = input.find(strStop, pos);
         auto begin = std::sregex_iterator(input.begin() + pos, (endpos == std::string::npos ? input.end() : input.begin() + endpos), pattern1);
         auto end   = std::sregex_iterator();
         boActive = true;
         for (std::sregex_iterator it = begin; it != end; ++it) {
            std::smatch match = *it;
            results.emplace_back(toInt<size_t>(match[1].str()), toInt<size_t>(match[2].str()));
            }

         if (endpos == std::string::npos) {
            pos = endpos;
            continue;
            }
         else { 
            pos = input.find(strStart, endpos + strStop.size());
            if(pos != std::string::npos) pos += strStart.size();
            else boActive = false;
            }
         }
      }
   */

   auto result_2 = std::accumulate(results.begin(), results.end(), size_t{ 0 },
                                       [](size_t acc, std::pair<size_t, size_t>& p) {
                                            return acc + (p.first * p.second);
                                            }
                                  );


   std::println(std::cout, "the result for the 1st part is {}", result_1);
   std::println(std::cout, "the result for the 2nd part is {}", result_2);
   return { to_String(result_1), to_String(result_2) };
   }

//template <std::ranges::input_range range_ty>
std::pair<std::string, std::string> RiddleDay4(TMyForm& frm) { // range_ty const& values
   std::atomic<size_t> result_1 = 0, result_2 = 0;

   auto text = frm.GetText("memInput"s);
   auto rows = std::ranges::count(text, '\n');
   auto cols = text.size() / std::ranges::count(text, '\n') - 1;
   text.erase(std::ranges::remove(text, '\n').begin(), text.end());
   std::mdspan<char, std::extents<size_t, std::dynamic_extent, std::dynamic_extent>> vault(text.data(), rows, cols);

   std::vector<std::string> patters = { "XMAS"s, "SAMX"s };
   auto pathways = own::find_all_paths(vault);

   for (auto const& patter : patters) {
      auto searcher = std::boyer_moore_searcher(patter.begin(), patter.end());
      //for (auto const& pathway : pathways) {
      std::for_each(std::execution::par, pathways.begin(), pathways.end(),
         [&vault, &searcher, &result_1, &patter](auto const& pathway) {
            own::grid_view grid(vault, pathway);

            size_t pos = 0u;
            auto result = std::search(grid.begin() + pos, grid.end(), searcher);
            for (; result != grid.end(); result = std::search(grid.begin() + pos, grid.end(), searcher)) {
               ++result_1;
               pos = (result - grid.begin()) + patter.size();
               }
         });
      }

   std::println(std::cout, "the result for the 1st part is {}", result_1.load());

   auto innerPathways = own::find_horz_inner_paths(vault);

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


//template <std::ranges::input_range range_ty>
std::pair<std::string, std::string> RiddleDay5(TMyForm& frm) { // range_ty const& values
   auto values = frm.GetLines("memInput");
   auto rules        = values | std::views::transform(toString<std::string_view>)
                              | std::views::filter(checkSeparatedIntegersPairs)
                              | std::views::transform([](auto const& p) { return parseSeparatedPairs<size_t>({ p.begin(), p.end() });})
                              | std::ranges::to<std::set>();
   auto update_pages = values | std::views::transform(toString<std::string_view>)
                              | std::views::filter(checkCommaSeparatedIntegers)
                              | std::views::transform([](auto const& p) { return parseCommaSeparatedIntegers<size_t>({ p.begin(), p.end() });})
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

template <std::ranges::input_range range_ty>
std::pair<std::string, std::string> RiddleDay6(range_ty const& values) {
   size_t result_1 = 0, result_2 = 0;
   return { to_String(result_1), to_String(result_2) };
   }

template <std::ranges::input_range range_ty>
std::pair<std::string, std::string> RiddleDay7(range_ty const& values) {
   size_t result_1 = 0, result_2 = 0;
   return { to_String(result_1), to_String(result_2) };
}

TAOC2024Processes::cntrlDatas TAOC2024Processes::control = {
   {  1, {" 1st day"s,  "Historian Hysteria"s, "https://adventofcode.com/2024/day/1"s, [](TMyForm& frm) { SetResult(frm, RiddleDay1(frm.GetLines("memInput"))); } }},
   {  2, {" 2nd day"s,  "Red-Nosed Reports"s,  "https://adventofcode.com/2024/day/2"s, [](TMyForm& frm) { SetResult(frm, RiddleDay2(frm.GetLines("memInput"))); } }},
   {  3, {" 3rd day"s,  "Mull It Over"s,       "https://adventofcode.com/2024/day/3"s, [](TMyForm& frm) { SetResult(frm, RiddleDay3(frm.GetLines("memInput"))); } }},
   {  4, {" 4th day"s,  "Ceres Search"s,       "https://adventofcode.com/2024/day/4"s, [](TMyForm& frm) { SetResult(frm, RiddleDay4(frm)); } }},
   {  5, {" 5th day"s,  "Print Queue"s,        "https://adventofcode.com/2024/day/5"s, [](TMyForm& frm) { SetResult(frm, RiddleDay5(frm)); } }}
   //{  6, {"6th day"s,  ""s } }
   //{  7, {"7th day"s,  ""s } }
   //{  8, {"8th day"s,  ""s } }
   //{  9, {"9th day"s,  ""s } }
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


   auto values = (control | own::views::second) | std::views::elements<0>;
   std::ranges::copy(values, make_qt_widget_output_iterator(frm.Find<QComboBox>("cbxOldRiddles")));
   frm.LastItem("cbxOldRiddles"s);
   Info(std::forward<TMyForm>(frm));
   }


void TAOC2024Processes::Info(TMyForm&& frm) {
   std::ostream oss(frm.GetAsStreamBuf("memInput"s, true));
   std::println(oss, "Advent of Code 2024");
   std::println(oss, "-------------------");
   for(auto const& [day, data] : control) {
      std::println(oss, "{:>8} : {} ({})", std::get<0>(data), std::get<1>(data), day);
      }
   }

void TAOC2024Processes::SetResult(TMyForm& frm, std::pair<std::string, std::string> const& value) {
   frm.SetEdit({ { "edtResult_1"s, value.first }, { "edtResult_2"s, value.second } });
   }



void TAOC2024Processes::CopyTestData(TMyForm&& frm) {
   if (auto it = test.find(GetDay(frm.CurrentText("cbxOldRiddles"))); it != test.end()) {
      std::ostream oss(frm.GetAsStreamBuf("memInput"s, true));
      std::println(oss, "{}", it->second);
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
         std::println(oss, "{}", it->second);
         }
      auto const& data = control[day];
      frm.SetCaption(std::format("{}: {}", strApplication, std::get<1>(data)));
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
