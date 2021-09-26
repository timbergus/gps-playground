#include <iostream>

#include "gps.h"

int main()
{
  GPS gps;

  gps.get_data_stream("./src/data/samples.txt", [&gps](std::string sample)
                      { gps.parse_sample(sample); });

  return EXIT_SUCCESS;
}
