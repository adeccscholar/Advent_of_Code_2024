#pragma once

#include <cstring>
#include <stdexcept>

template <typename ty>
class Dynamic_Array {
private:
   using used_type = ty;
   ty*    vault = nullptr;
   size_t size_value = 0;
   size_t capacity_value = 0;
public:
   Dynamic_Array() = default;

   ~Dynamic_Array() {
      delete [] vault;
      }

   ty& operator [] (size_t idx) {
      if (idx >= size_value) throw std::invalid_argument("index in dynamic array out of bounds");
      else return *(vault + idx * sizeof(used_type));
      }

   };