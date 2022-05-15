#include "../include/gps.h"

#define SAMPLE_TYPE "GNRMC"

int main()
{
  GPS gps;

  const auto gpsCallback = [&gps](std::string sample)
  {
    if (Utils::get_type(sample) == SAMPLE_TYPE)
    {
      gps.parse_sample(sample);
    }
  };

  gps.get_data_stream("./src/data/samples.txt", gpsCallback);

  return EXIT_SUCCESS;
}
