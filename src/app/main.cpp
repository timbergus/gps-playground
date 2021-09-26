#include <iostream>

#include "gps.h"

int main()
{
  GPS gps;

  gps.get_data_stream("./src/data/samples.txt");

  return EXIT_SUCCESS;
}
