// gll.h

#ifndef GLL_H
#define GLL_H

#include <iostream>
#include <string>
#include <vector>

#include "color.h"

class GLL
{
private:
  Color palette;

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

  bool is_valid(std::string, std::string);

  std::string get_time();

  double get_latitude();
  double get_longitude();

  void print_data();
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

bool GLL::is_valid(std::string core_data, std::string checksum)
{
  std::string data = core_data.substr(1);

  int check = 0;

  for (int i = 0; i < (int)data.size(); i++)
  {
    check = char(check ^ data.at(i));
  }

  char hex_check[20];

  sprintf_s(hex_check, "%X", check);

  return hex_check == checksum;
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

void GLL::print_data()
{
  std::cout << palette.set_color("Type: ", "green") << data.type << std::endl;
  std::cout << palette.set_color("UTC Time: ", "green") << data.utc_time << std::endl;
  std::cout << palette.set_color("Status: ", "green") << data.status << std::endl;
  std::cout << palette.set_color("Latitude: ", "green") << data.latitude << std::endl;
  std::cout << palette.set_color("Latitude Direction: ", "green") << data.latitude_direction << std::endl;
  std::cout << palette.set_color("Longitude: ", "green") << data.longitude << std::endl;
  std::cout << palette.set_color("Longitude Direction: ", "green") << data.longitude_direction << std::endl;
  std::cout << palette.set_color("Mode: ", "green") << data.mode << std::endl;
}

#endif /* GLL_H */
