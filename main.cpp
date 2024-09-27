#include "hash_map.h"
#include <iostream>

int main() {

  hash_map<float, float> map(0, 0.9, 0.2);
  for (int i = 0; i < 500; i++) {
    map.insert(i, i);
  }
  for (int i = 0; i < 300; i++) {
    map.remove(i);
  }

  for (int i = 300; i < 300 + 180; i++) {
    std::cout << i << std::endl;
    map.remove(i);
  }
  for (int i = 480; i < 500; i++) {
    std::cout << map.get_value(i).value() << std::endl;
  }
}