#include <iostream>

#include "../include/gps.h"

int main()
{
  GPS gps;

  gps.get_data_stream("./src/data/samples.txt", [&gps](std::string sample)
                      {
                        if (sample.substr(1, 5) == "GNRMC")
                        {
                          gps.parse_sample(sample);
                        } });

  return EXIT_SUCCESS;
}
