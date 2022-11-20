// gll.h

#ifndef GLL_H
#define GLL_H

#include <iostream>
#include <string>
#include <vector>

#include "color.h"
#include "print.h"
#include "utils.h"

class GLL
{
private:
  Color palette;
  Print printer;

  struct gll_t
  {
    std::string type;
    std::string latitude;
    std::string latitude_direction;
    std::string longitude;
    std::string longitude_direction;
    std::string utc_time;
    std::string status;
    std::string mode;
    std::string checksum;
  } data;

public:
  GLL(std::vector<std::string>);
  ~GLL();

  std::string get_time();

  double get_latitude();
  double get_longitude();

  void print_raw_data();
  void print_formatted_data();
};

GLL::GLL(std::vector<std::string> core_data)
{
  data.type = core_data[0].substr(1);
  data.latitude = core_data[1];
  data.latitude_direction = core_data[2];
  data.longitude = core_data[3];
  data.longitude_direction = core_data[4];
  data.utc_time = core_data[5];
  data.mode = core_data[6];
  data.checksum = core_data[7];
}

GLL::~GLL()
{
}

std::string GLL::get_time()
{
  std::string hours = data.utc_time.substr(0, 2);
  std::string minutes = data.utc_time.substr(2, 2);
  std::string seconds = data.utc_time.substr(4, 2);

  return hours + ":" + minutes + ":" + seconds;
}

double GLL::get_latitude()
{
  return std::stod(data.latitude) / 100.0;
}

double GLL::get_longitude()
{
  double sign = data.longitude_direction == "W" ? -1.0 : 1.0;
  return sign * std::stod(data.longitude) / 100.0;
}

void GLL::print_raw_data()
{
  printer.print_title("GLL Sample Data");
  printer.print_info("Type", data.type);
  printer.print_info("UTC Time", data.utc_time);
  printer.print_info("Status", data.status);
  printer.print_info("Latitude", data.latitude);
  printer.print_info("Latitude Direction", data.latitude_direction);
  printer.print_info("Longitude", data.longitude);
  printer.print_info("Longitude Direction", data.longitude_direction);
  printer.print_info("Mode", data.mode);
}

void GLL::print_formatted_data()
{
  printer.print_title("GLL Sample Data");
  printer.print_info("Time", Utils::get_time(data.utc_time));
  printer.print_number("Latitude", Utils::get_latitude(data.latitude));
  printer.print_number("Longitude", Utils::get_longitude(data.longitude, data.longitude_direction));
}

#endif /* GLL_H */
