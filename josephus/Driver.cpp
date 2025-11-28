#include <iostream>

#include "Josephus.h"

template <typename T>
T
prompt(char const * const string) {
  T value;
  std::cout << string;
  std::cin >> value;
  return value;
}

int
main ()
{
  const auto N = prompt<int>("N ==> ");
  const auto k = prompt<int>("k ==> ");
  std::cout << "The survivor is " << josephus(N, k) << std::endl;
  return 0;
}
