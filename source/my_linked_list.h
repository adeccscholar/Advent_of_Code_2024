#pragma once

#include <ranges>

template <typename ty>
struct linked_list_type {
   using value_type = ty;

   struct node_ty {
      value_type node_value = { };
      node_ty*   next_node  = nullptr;
      };

   node_ty*  head_of_list;
   size_t elements_in_list;

   // node* current difficult, in std here interators used

   linked_list_type() : head_of_list{ nullptr }, elements_in_list { 0 } { }
   ~linked_list_type() { erase_list();  }

   node_ty* next_node_in_list(node_ty* position) {
      if (position == nullptr) return nullptr;
      else return position->next_node;
      }

   node_ty* insert_first(ty const& value) {
      node_ty* node = new node_ty;
      node->node_value = value;
      node->next_node  = head_of_list;
      head_of_list = node;
      ++elements_in_list;
      return node;
      }

   node_ty* insert_last(ty const& value) {
      node_ty* node = new node_ty;
      node->node_value = value;
      node->next_node  = nullptr;
      if(head_of_list == nullptr) {
         head_of_list = node;
         ++elements_in_list;
         }
      else {
         node_ty* last = get_last_element();
         if(last != nullptr)  {
            last->next_node = node;
            ++elements_in_list;
            }
         else {
            delete node;
            node = nullptr;
            }
         }
      return node;
      }
      
   node_ty* insert_into_list_before(ty const& value, node_ty* position) {
      if (head_of_list == nullptr) { 
         position = new node_ty;
         position->node_value = value;
         position->next_node  = nullptr;
         head_of_list = position; 
         ++elements_in_list;
         return position;
         }
      else if(position == head_of_list) {
         node_ty* element = new node_ty;
         element->node_value = value;
         element->next_node = head_of_list;
         head_of_list = element;
         ++elements_in_list;
         return position;
         }
      else if (position != nullptr) {
         node_ty* current = nullptr;
         for (current = head_of_list; current != nullptr && current->next_node != position; current = current->next_node);
         if (current->next_node == position) {
            node_ty* element = new node_ty;
            element->node_value = value;
            element->next_node = position;
            current->next_node = element;
            ++elements_in_list;
            return position;
            }
         else {
            return nullptr;
            }
         }
      else return nullptr;
      }
   
   node_ty* delete_from_list(node_ty* position) {
      if (position != nullptr) {
         if(position == head_of_list) {
            head_of_list = head_of_list->next_node;
            delete position;
            --elements_in_list;
            return head_of_list;
            }
         else {
            for(node_ty* current = head_of_list; current != nullptr && current->next_node != position; ++current) {
               if (current->next_node == position) {
                  current->next_node = position->next_node;
                  delete position;
                  --elements_in_list;
                  return current->next_node;
                  }
               else return position;
               }
            }
         }
      else return nullptr;
      }
   
   void erase_list() {
      for(; head_of_list != nullptr;) {
         node_ty* current = head_of_list;
         head_of_list = head_of_list->next_node;
         delete current;
         }
      elements_in_list = 0;
      }

   node_ty* get_last_element() {
      if (head_of_list == nullptr) return nullptr;
      else {
         node_ty* current = head_of_list;
         for (; current->next_node != nullptr; current = current->next_node);
         return current;
         }
      }

   /*
   template <typename... Args>
   void add_to_list(ty first, Args... args) {
      node_ty 
      (result.push_back(first), ..., result.push_back(args));
      return result;
   }
   */

};

