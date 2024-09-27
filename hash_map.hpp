#ifndef HASH_MAP_HPP
#define HASH_MAP_HPP
#include "hash_list.h"
#include "hash_map.h"
#include <cstddef>
#include <optional>

template <typename K, typename V>
hash_map<K, V>::hash_map(size_t capacity, float upper_load_factor,
                         float lower_load_factor)
    : _capacity(capacity), _upper_load_factor(upper_load_factor),
      _lower_load_factor(lower_load_factor) {
  _size = 0;
  _head = new hash_list<K, V>[_capacity];
}

/**
 * @brief Construct a new hash map object
 *
 * @param other
 *  The map to create a copy of
 */
template <typename K, typename V>
hash_map<K, V>::hash_map(const hash_map<K, V> &other) {
  _size = other._size;
  _capacity = other._capacity;
  _head = new hash_list<K, V>[_capacity];
  for (size_t i = 0; i < _capacity; i++) {
    _head[i] = other._head[i];
  }
}

/**
 * @brief Constructs a new hash map from other
 *
 * @param other
 *  The map to create a copy of
 * @return hash_map&
 *  Returns a reference to the newly constructed hash map. This ensures that
 *  a = b = c works
 */
template <typename K, typename V>
hash_map<K, V> &hash_map<K, V>::operator=(const hash_map<K, V> &other) {
  if (this == &other) {
    return *this;
  }
  hash_map<K, V> co = other;
  std::swap(_size, co._size);
  std::swap(_capacity, co._capacity);
  std::swap(_head, co._head);
  return *this;
}

/**
 * @brief Insert the key/value pair into the map. If the specified key already
 * exists in the map update the associated value, otherwise insert a new key
 * value pair
 *
 * @param key
 *  The key to insert
 * @param value
 *  The value to insert
 */
template <typename K, typename V> void hash_map<K, V>::insert(K key, V value) {
  size_t hash = _hash(key) % _capacity;
  int oldSize = _head[hash].get_size();
  _head[hash].insert(key, value);
  int newSize = _head[hash].get_size();
  _size += newSize - oldSize;

  if (_size > _upper_load_factor * _capacity) {
    int new_cap = get_larger_capacity();
    if (new_cap == -1) {
      return;
    }
    hash_list<K, V> *new_head = new hash_list<K, V>[new_cap];
    for (size_t i = 0; i < _capacity; i++) {
      hash_list<K, V> curr_list = _head[i];
      curr_list.reset_iter();
      while (!curr_list.iter_at_end()) {

        K key_to_move = *std::get<0>(curr_list.get_iter_value().value());
        V value_to_move = *std::get<1>(curr_list.get_iter_value().value());
        size_t new_hash = _hash(key_to_move) % new_cap;
        new_head[new_hash].insert(key_to_move, value_to_move);

        curr_list.increment_iter();
      }
    }
    delete[] _head;
    _head = new_head;
    _capacity = new_cap;
  }
}

/**
 * @brief Return an optional containing the value associated with the
 * specified key. If the key isn't in the map return an empty optional.
 *
 * @param key
 *  The key to search for
 * @return
 *  An empty optional (if the key isn't in the map), otherwise return an
 * optional containing the value associated with the specified key
 */
template <typename K, typename V>
std::optional<V> hash_map<K, V>::get_value(K key) const {
  size_t hash = _hash(key) % _capacity;
  return _head[hash].get_value(key);
}

/**
 * @brief Remove the key and corresponding value from the map and return true.
 * If the key isn't in the map return false.
 *
 * @param key
 *  The key to remove from the list
 * @return
 *  True if key was present
 *  False otherwise
 */
template <typename K, typename V> bool hash_map<K, V>::remove(K key) {
  size_t hash = _hash(key) % _capacity;
  bool res = _head[hash].remove(key);
  _size -= res;

  if (_size < _lower_load_factor * _capacity) {
    int new_cap = get_smaller_capacity();
    if (new_cap == -1) {
      return res;
    }
    hash_list<K, V> *new_head = new hash_list<K, V>[new_cap];
    for (size_t i = 0; i < _capacity; i++) {
      hash_list<K, V> curr_list = _head[i];
      curr_list.reset_iter();
      while (!curr_list.iter_at_end()) {

        K key_to_move = *std::get<0>(curr_list.get_iter_value().value());
        V value_to_move = *std::get<1>(curr_list.get_iter_value().value());
        size_t new_hash = _hash(key_to_move) % new_cap;
        new_head[new_hash].insert(key_to_move, value_to_move);

        curr_list.increment_iter();
      }
    }
    delete[] _head;
    _capacity = new_cap;
    _head = new_head;
  }

  return res;
}

/**
 * @brief Return the number of key/value pairs in the map
 */
template <typename K, typename V> size_t hash_map<K, V>::get_size() const {
  return _size;
}

/**
 * @brief Returns the capacity of the map
 *
 * @return
 *  The capacity of the hash map
 */
template <typename K, typename V> size_t hash_map<K, V>::get_capacity() const {
  return _capacity;
}

/**
 * @brief Copies all the keys from the hash_map into the specified array
 *
 * @param keys
 *  A pointer to an array that has enough space to store all the keys
 *  in the hash_map.
 */
template <typename K, typename V> void hash_map<K, V>::get_all_keys(K *keys) {
  size_t j = 0;
  for (size_t i = 0; i < _capacity; i++) {
    _head[i].reset_iter();
    while (!(_head[i].iter_at_end())) {
      std::optional<std::pair<const int *, float *>> o =
          _head[i].get_iter_value();
      if (o.has_value()) {
        keys[j++] = *(o.value().first);
      }
      _head[i].increment_iter();
    }
  }
}

/**
 * @brief Copies all the keys from the hash_map into the specified array
 * and sorts the array by key value. The smallest key value should be at the
 * front of the array, and the largest key value should be at the end of the
 * array
 *
 * @param keys
 *  A pointer to an array that has enough space to store all the keys
 *  in the hash_map.
 */
template <typename K, typename V>
void hash_map<K, V>::get_all_sorted_keys(K *keys) {
  get_all_keys(keys);
  std::sort(keys, keys + (_size * sizeof(K)));
}

/**
 * @brief Get the number of elements in each hash_list pointed to by _head.
 * So buckets[0] should be the number of elements in the hash list at index 0
 *    buckets[1] should be the number of elements in the hash list at index 1
 *    etc
 * In order for this to work you MUST use the hash function that we specified
 * in the handout. To remind you again here, the hash function is simply the
 * absolute value of the key
 *
 * @param buckets
 *  A pointer to an array that has at least _capacity elements
 */
template <typename K, typename V>
void hash_map<K, V>::get_bucket_sizes(size_t *buckets) {
  for (size_t i = 0; i < _capacity; i++) {
    buckets[i] = _head[i].get_size();
  }
}

/**
 * @brief Frees all memory associated with the map
 */
template <typename K, typename V> hash_map<K, V>::~hash_map() {
  delete[] _head;
}

#endif