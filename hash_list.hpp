#ifndef HASH_LIST_HPP
#define HASH_LIST_HPP
#include "hash_list.h"
#include <csignal>
#include <optional>

template <typename K, typename V> hash_list<K, V>::hash_list() {
  size = 0;
  head = nullptr;
  iter_ptr = nullptr;
}

/**-----------------------------------------------------------------------------------
 * START Part 1
 *------------------------------------------------------------------------------------*/

/**
 * @brief Insert a node with the corresponding key value pair into the list.
 * If a node with the associated key already exists, update that node with the
 * new value instead of adding a new node to the list.
 *
 * @param key
 *  The key to insert into the list
 * @param value
 *  The value to insert into the list
 */
template <typename K, typename V> void hash_list<K, V>::insert(K key, V value) {
  node<K, V> *iter = head;
  for (int i = 0; i < (int)size; i++) {
    if (iter->key == key) {
      iter->value = value;
      return;
    }
    iter = iter->next;
  }
  iter = new node<K, V>{key, value, head};
  head = iter;
  size++;
}

/**
 * @brief Return an optional containing the value associated with the
 * specified key. If the key isn't in the list return an empty optional.
 *
 * @param key
 *  The key to search the list for
 * @return
 *  If the key isn't in the list returns an empty optional
 *  If the key is in the list returns the corresponding value
 */
template <typename K, typename V>
std::optional<V> hash_list<K, V>::get_value(K key) const {
  node<K, V> *iter = head;
  for (int i = 0; i < (int)size; i++) {
    if (iter->key == key) {
      return iter->value;
    }
    iter = iter->next;
  }
  return std::nullopt;
}

/**
 * @brief Remove the node containing the specified key from the list and
 * return true. If the key isn't in the list return false.
 *
 * @param key
 *  The key to remove from the list
 * @return
 *  True if the key was removed from the list
 *  False if the key wasn't in the list
 */
template <typename K, typename V> bool hash_list<K, V>::remove(K key) {
  node<K, V> *iter = head;
  if (head && head->key == key) {
    head = head->next;
    delete iter;
    size--;
    return true;
  }
  for (int i = 0; i < (int)size - 1; i++) {
    if (iter->next->key == key) {
      node<K, V> *temp = iter->next;
      iter->next = iter->next->next;
      delete temp;
      size--;
      return true;
    }
    iter = iter->next;
  }
  return false;
}

/**
 * @brief Return the number of nodes in the list.
 * This function must run in constant time
 *
 * @return
 *  the number of nodes in the list
 */
template <typename K, typename V> size_t hash_list<K, V>::get_size() const {
  return size;
}

/**
 * @brief Free all memory associated with the nodes.
 * This must not free the nodes recursively
 */
template <typename K, typename V> hash_list<K, V>::~hash_list() {
  node<K, V> *iter;
  for (int i = 0; i < (int)size; i++) {
    iter = head;
    head = head->next;
    delete iter;
  }
}

/**-----------------------------------------------------------------------------------
 * END Part 1
 *------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------
 * START Part 2
 *------------------------------------------------------------------------------------*/
/**
 * @brief The copy constructor
 *
 * @param other
 *  The list to create a copy of
 */
template <typename K, typename V>
hash_list<K, V>::hash_list(const hash_list<K, V> &other) {
  node<K, V> *prev;
  node<K, V> *curr;

  node<K, V> *ocurr = other.head;
  size = other.size;
  if (size > 0) {
    head = new node<K, V>{other.head->key, other.head->value, nullptr};
    prev = head;
    if (other.iter_ptr && head->key == other.iter_ptr->key) {
      iter_ptr = head;
    }
  }
  reset_iter();
  for (int i = 0; i < (int)size - 1; i++) {
    ocurr = ocurr->next;
    curr = new node<K, V>{ocurr->key, ocurr->value, nullptr};
    prev->next = curr;
    prev = curr;
    if (other.iter_ptr && curr->key == other.iter_ptr->key) {
      iter_ptr = curr;
    }
  }
}

/**
 * @brief Assignment operator
 *
 * @param other
 *  The list to create a copy of
 * @return
 *  A reference to the list that was created. This allows for code like
 *  a = b = c to work
 */
template <typename K, typename V>
hash_list<K, V> &hash_list<K, V>::operator=(const hash_list &other) {
  if (this == &other)
    return *this;
  hash_list co = other;
  std::swap(size, co.size);
  std::swap(head, co.head);
  std::swap(iter_ptr, co.iter_ptr);
  return *this;
}

/**
 * @brief Resets the iterator back to point to the first element in the list.
 * If the list is empty then the iterator is set to NULL.
 */
template <typename K, typename V> void hash_list<K, V>::reset_iter() {
  if ((int)size == 0) {
    iter_ptr = NULL;
    return;
  }
  iter_ptr = head;
}

/**
 * @brief Moves the iterator to the next element. If the iterator points to
 * the last element of the list when this is called the iterator is set to
 * NULL. If the iterator is NULL when this function is called then this
 * function does nothing
 */
template <typename K, typename V> void hash_list<K, V>::increment_iter() {
  if (!(iter_at_end())) {
    iter_ptr = iter_ptr->next;
  }
}

/**
 * @brief Return an optional that contains a pointer to the key and a pointer
 * to the value of the node pointed to by the iterator. If the iterator is
 * NULL this returns an empty optional
 *
 * @return
 *  If the iterator is NULL returns an empty optional
 *  Otherwise returns a pointer to the key/value pointed to by the current
 * iterator
 */
template <typename K, typename V>
std::optional<std::pair<const K *, V *>> hash_list<K, V>::get_iter_value() {
  if (!(iter_at_end())) {
    return std::make_pair(&(iter_ptr->key), &(iter_ptr->value));
  }
  return std::nullopt;
}

/**
 * @brief Returns true if the iterator is NULL
 *
 * @return
 *  True if the iterator is NULL
 *  False otherwise
 */
template <typename K, typename V> bool hash_list<K, V>::iter_at_end() {
  return iter_ptr == NULL;
}
/**-----------------------------------------------------------------------------------
 * END Part 2
 *------------------------------------------------------------------------------------*/

#endif