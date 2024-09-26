#include "hash_map.h"
#include <iostream>

int main() {

  hash_map<int, float> map(10, 0.1, 0.2);
  for (int i = 0; i < 10; i++) {
    map.insert(i, i);
  }
  int *keys = (int *)malloc(500 * sizeof(int));
  for (int i = 0; i < 10; i++) {
    std::cout << map.get_value(i).value() << std::endl;
  }
}