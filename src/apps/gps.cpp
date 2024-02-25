#include "gps.h"

GPS::GPS(GPSType type)
{
  gpsType = type;
}

GPS::~GPS()
{
}

void GPS::parse(std::string_view sample)
{
  if (gpsType == T_GNGLL)
  {
    gngll.parse(sample);
  }
  else if (gpsType == T_GNGSA)
  {
    gngsa.parse(sample);
  }
  else if (gpsType == T_GNRMC)
  {
    gnrmc.parse(sample);
  }
  else if (gpsType == T_GPGSV)
  {
    gpgsv.parse(sample);
  }
  else
  {
    fmt::print("Not working at all");
  }
}
