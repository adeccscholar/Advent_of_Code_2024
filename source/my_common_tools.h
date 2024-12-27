#pragma once

// Project: Solutions for Advent of Code 2024
// common functions for the project
// example to find and isolate basic functionality for different problems
// 
// copyright © adecc Systemhaus GmbH 2024, All rights reserved.
// This project is released under the MIT License.


#include <string>
#include <stdexcept>
#include <regex>
#include <vector>
#include <set>
#include <map>
#include <span>
#include <array>
#include <chrono>
#include <string_view>
#include <filesystem>
#include <charconv>
#include <system_error>
#include <ranges>
#include <format>
#include <concepts>
#include <source_location>

namespace fs = std::filesystem;
using namespace std::string_literals;


template <typename>
inline constexpr bool always_false = false;

// -----------------------------------------------------------------------------

template <typename ty>
struct is_pair : std::false_type {};

template <typename first_ty, typename second_ty>
struct is_pair<std::pair<first_ty, second_ty>> : std::true_type {};

template <typename ty>
struct is_tuple : std::false_type {};

template <typename... ty>
struct is_tuple<std::tuple<ty...>> : std::true_type {};

// ---------

template <typename ty, typename = void>
struct is_map : std::false_type {};

template <typename key_ty, typename value_ty, typename compare_ty, typename allocator_ty>
struct is_map<std::map<key_ty, value_ty, compare_ty, allocator_ty>> : std::true_type {};

// --------------------------------------------------------------------------
template <typename ty, typename = void>
struct is_vector : std::false_type {};

template <typename ty, typename alloc_ty>
struct is_vector<std::vector<ty, alloc_ty>> : std::true_type {};

// --------------------------------------------------------------------------
template <typename ty, typename = void>
struct is_set : std::false_type {};

template <typename ty, typename compare_ty, typename alloc_ty>
struct is_set<std::set<ty, compare_ty, alloc_ty>> : std::true_type {};

// --------------------------------------------------------------------------
template <typename ty, typename = void>
struct is_span : std::false_type {};

template <typename ty, std::size_t Extent>
struct is_span<std::span<ty, Extent>> : std::true_type {};

// --------------------------------------------------------------------------
template <typename ty>
struct is_array : std::false_type {};

template <typename ty, std::size_t N>
struct is_array<std::array<ty, N>> : std::true_type {};

template <typename ty>
constexpr bool is_pair_v = is_pair<ty>::value;

template <typename ty>
constexpr bool is_tuple_v = is_tuple<ty>::value;

template <typename ty>
constexpr bool is_vector_v = is_vector<ty>::value;

template <typename ty>
constexpr bool is_set_v = is_set<ty>::value;

template <typename ty>
constexpr bool is_map_v = is_map<ty>::value;





template <typename ty>
std::string to_String(ty const& str) {
   return std::format("{}", str);
   }


inline std::string_view trim(std::string_view str) {
   size_t pos1 = str.find_first_not_of(" \t");
   return str.substr(pos1, str.size() - pos1);
   }

inline std::string MyCutPath(std::string const& pathString) {
     if (fs::path path(pathString);  fs::is_regular_file(path)) {
        std::vector<std::string> parts;
        for (const auto& part : path) {
           if (part != path.root_name() && part != "/" && part != "\\") parts.emplace_back(part.string());
        }

        switch (auto size = parts.size(); size) {
        case 0: return ""s;
        case 1: return parts[0];
        case 2: return parts[0] + "/"s + parts[1];
        [[likely]] default: return "../"s + parts[size - 2] + "/"s + parts[size - 1];
        }
     }
     else return pathString;
  }

inline std::string MyTimeStamp(std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now()) {
   auto const cz_ts = std::chrono::current_zone()->to_local(now);
   auto const millis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
   return std::format("{:%d.%m.%Y %X},{:03d}", cz_ts, millis.count());
   }

inline std::string MyPosition(std::source_location loc = std::source_location::current()) {
   return std::format("[in function \"{}\" in file \"{}\" at line {}]", loc.function_name(), MyCutPath(loc.file_name()), loc.line());
   }

inline std::string MyPostionTimeStamp(std::source_location const& loc = std::source_location::current(),
                                      std::chrono::time_point<std::chrono::system_clock>const& now = std::chrono::system_clock::now()) {
   auto const cz_ts = std::chrono::current_zone()->to_local(now);
   auto const millis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
   return std::format(" [at {:%d.%m.%Y %X},{:03d} in function \"{}\" within file \"{}\" in line {}]",
         cz_ts, millis.count(), loc.function_name(), MyCutPath(loc.file_name()), loc.line());
   }



template <typename ty>
concept my_integral_ty = std::is_integral_v<ty> && !std::is_same_v<ty, bool>;

template<my_integral_ty ty1, my_integral_ty ty2>
using biggest_int_of = std::conditional_t<(sizeof(ty1) > sizeof(ty2)) || (sizeof(ty1) == sizeof(ty2) && std::is_unsigned_v<ty1>), ty1, ty2>;


template <typename ty>
concept my_string_ty = std::is_same_v<ty, std::string> || std::is_same_v<ty, std::string_view>;

template <my_integral_ty ty, my_string_ty str_ty>
ty toInt(str_ty str) {
   using used_type = std::remove_cv_t<std::remove_reference_t<decltype(str)>>;
   static auto constexpr toString = [](str_ty str) { 
      if constexpr (std::is_same_v<used_type, std::string>) return str;
      else if constexpr (std::is_same_v<used_type, std::string_view>) {
         return std::string { str.data(), str.size() };
         }
      else static_assert(always_false<used_type>, "unexpected type as parameter for method toInt");
      };

   std::size_t pos{};
   if constexpr (std::is_same_v<ty, long long>) return stoll(toString(str), &pos);
   else if constexpr (std::is_same_v<ty, unsigned long long>) return std::stoull(toString(str), &pos);
   else {
      ty result{};
      auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), result);

      if (ptr == str.data() + str.size()) [[likely]] {
         switch (ec) {
         case std::errc(): return result;
         case std::errc::invalid_argument:  throw std::invalid_argument("The given argument '"s + toString(str) + "' is invalid"s);
         case std::errc::result_out_of_range: throw std::out_of_range("The result for '"s + toString(str) + "' isn't in range that can be represented for int values."s);
         default: throw std::runtime_error("unexpected eror in method from_chars for input '"s + toString(str) + "'."s);
            }
         }
      else throw std::invalid_argument("Not all characters in '"s + toString(str) + "' could be converted."s);
      }
   }


template <typename ty>
concept my_number = std::is_floating_point_v< std::remove_cvref_t<ty>> ||
                    (std::is_integral_v<std::remove_cvref_t<ty>> && !std::is_same_v<ty, bool>);


template <my_number ty>
struct std::formatter<std::vector<ty>> : std::formatter<std::string_view> {
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

   auto format(std::vector<ty> const& val, std::format_context& ctx) const {
      std::string temp;

      std::string fmt = "[" + format_string;
      std::vformat_to(std::back_inserter(temp), fmt, std::make_format_args(val[0]));
      fmt = ", " + format_string;
      for (size_t idx = 1; idx < val.size(); ++idx) std::vformat_to(std::back_inserter(temp), fmt, std::make_format_args(val[idx]));
      temp += "]";
      return std::formatter<std::string_view>::format(temp, ctx);
      }
};


template <my_integral_ty ty>
std::vector<ty> extractNumbers_with(std::string_view para, ty start) {
   std::vector<ty> result;
   result.emplace_back(start);
   static std::regex numberRegex(R"(\d+)");
   std::string input = { para.begin(), para.end() };
   for (auto it = std::sregex_iterator(input.begin(), input.end(), numberRegex); it != std::sregex_iterator(); ++it) {
      result.emplace_back(toInt<ty>(it->str()));
      }
   return result;
   }

template <typename ty>
std::string toString(ty input) { return { input.begin(), input.end() }; }

template <typename ty>
std::string_view toString_view(ty input) { return { input.begin(), input.end() }; }

// -------------------------------------------------------------------------------
template <typename str_ty> requires std::is_same_v<str_ty, std::string> || std::is_same_v<str_ty, std::string_view>
inline std::optional<std::tuple<std::string_view, std::string_view>> splitString(str_ty const& text, char delimiter) {
   if (size_t pos = text.find(delimiter); pos != str_ty::npos) [[likely]] {
      return { { std::string_view(text).substr(0, pos), std::string_view(text).substr(pos + 1) } };
      }
   else return { };
   }


// -------------------------------------------------------------------------------

// functions added with 7th day

inline size_t constexpr bitCount(size_t val) { 
   return val <= 1 ? 0 : static_cast<size_t>(std::log2(val - 1u) + 1u);
   }

// std::pow not constexpr (changed with C++26)
inline size_t variantCount(size_t val) { 
   return std::pow(2u, val);  
   }

inline std::vector<std::vector<uint16_t>> calculateBinaryRules(size_t cntOps) {
   static auto constexpr operationCnt = [](size_t val) -> size_t { return val == 0 ? 0 : val - 1; };
   std::vector<std::vector<uint16_t>> rules;
   for (size_t varCnt = variantCount(cntOps); auto val : std::views::iota(0) | std::views::take(varCnt)) {
      std::vector<uint16_t> results;
      //for (auto [bitPos, bitCnt] = std::make_pair(0u, bitCount(varCnt) - 1u); bitPos < bitCnt; ++bitPos) {
      for (auto [bitPos, bitCnt] = std::make_pair(0u, bitCount(varCnt)); bitPos < bitCnt; ++bitPos) {
         uint16_t bit = (val >> bitPos) & 1;
         results.emplace_back(bit);
         }
      rules.emplace_back(std::move(results));
      }
   return rules;
   }


template <my_integral_ty ty>
ty concatenateNumbers(ty first_val, ty second_val) {
   return first_val * static_cast<ty>(std::pow<ty>(10, static_cast<ty>(std::log10(second_val)) + 1)) + second_val;
   }


template <my_integral_ty ty, typename... Args>  requires (sizeof...(Args) >= 1)
ty concatenateNumbers(ty first_val, ty second_val, Args... args) {
   return concatenateNumbers(concatenateNumbers<ty>(first_val, second_val), args...);
   }

template <my_integral_ty ty>
struct concatenateNumbers_ty {
   ty operator()(ty const& first_val, ty const& second_val) const {
      return concatenateNumbers(first_val, second_val);
      }
};
