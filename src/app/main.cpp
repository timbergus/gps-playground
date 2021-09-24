#include <iostream>

#include "gps.h"

int main()
{
  GPS gps;

  std::cout << gps.get_sample() << std::endl;

  return EXIT_SUCCESS;

  return 0;
}
