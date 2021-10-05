// rmc.h

#ifndef RMC_H
#define RMC_H

#include <iostream>
#include <string>
#include <vector>

#include "color.h"

// TODO: Fix the library to use it. I have asked here.
// https://stackoverflow.com/questions/56608684/how-to-use-the-fmt-library-without-getting-undefined-symbols-for-architecture-x

/**
 * FMT: https://github.com/fmtlib/fmt.
 *
 * To install it use brew.
 * https://fmt.dev/latest/usage.html#homebrew
 */

// #include <fmt/format.h>

#define KNTOMS 0.514444444
#define KNTOKMH 1.85

class RMC
{
private:
  Color palette;

  struct rmc_t
  {
    std::string type;
    std::string utc_time;
    std::string status;
    std::string latitude;
    std::string latitude_direction;
    std::string longitude;
    std::string longitude_direction;
    std::string speed;
    std::string course;
    std::string utc_date;
    std::string mode;
    std::string checksum;
  } data;

public:
  RMC(std::vector<std::string>);
  ~RMC();

  bool is_valid(std::string, std::string);

  std::string get_time();
  std::string get_date(std::string);

  double get_latitude();
  double get_longitude();
  double get_speed(std::string);

  void print_data();
};

RMC::RMC(std::vector<std::string> core_data)
{
  data.type = core_data[0].substr(1);
  data.utc_time = core_data[1];
  data.status = core_data[2];
  data.latitude = core_data[3];
  data.latitude_direction = core_data[4];
  data.longitude = core_data[5];
  data.longitude_direction = core_data[6];
  data.speed = core_data[7];
  data.course = core_data[8];
  data.utc_date = core_data[9];
  data.mode = core_data[12];
  data.checksum = core_data[13];
}

RMC::~RMC()
{
}

// https://www.tigoe.com/pcomp/code/Processing/127

bool RMC::is_valid(std::string core_data, std::string checksum)
{
  std::string data = core_data.substr(1);

  int check = 0;

  for (int i = 0; i < (int)data.size(); i++)
  {
    check = char(check ^ data.at(i));
  }

  char hex_check[20];

  sprintf(hex_check, "%X", check);

  return hex_check == checksum;
}

std::string RMC::get_time()
{
  std::string hours = data.utc_time.substr(0, 2);
  std::string minutes = data.utc_time.substr(2, 2);
  std::string seconds = data.utc_time.substr(4, 2);

  return hours + ":" + minutes + ":" + seconds;
}

std::string RMC::get_date(std::string language = "sp")
{
  std::string day = data.utc_date.substr(0, 2);
  std::string month = data.utc_date.substr(2, 2);
  std::string year = data.utc_date.substr(4, 2);

  if (language == "en")
  {
    return month + "/" + day + "/" + year;
  }
  else
  {
    return day + "/" + month + "/" + year;
  }
}

double RMC::get_latitude()
{
  return std::stod(data.latitude) / 100.0;
}

double RMC::get_longitude()
{
  double sign = data.longitude_direction == "W" ? -1.0 : 1.0;
  return sign * std::stod(data.longitude) / 100.0;
}

double RMC::get_speed(std::string units)
{
  double speed = 0.0;

  if (units == "MS")
  {
    speed = std::stod(data.speed) * KNTOMS;
  }
  else if (units == "KMH")
  {
    speed = std::stod(data.speed) * KNTOKMH;
  }

  return speed;
}

void RMC::print_data()
{
  std::cout << palette.set_color("Type: ", "green") << data.type << std::endl;
  std::cout << palette.set_color("UTC Time: ", "green") << data.utc_time << std::endl;
  std::cout << palette.set_color("Status: ", "green") << data.status << std::endl;
  std::cout << palette.set_color("Latitude: ", "green") << data.latitude << std::endl;
  std::cout << palette.set_color("Latitude Direction: ", "green") << data.latitude_direction << std::endl;
  std::cout << palette.set_color("Longitude: ", "green") << data.longitude << std::endl;
  std::cout << palette.set_color("Longitude Direction: ", "green") << data.longitude_direction << std::endl;
  std::cout << palette.set_color("Speed: ", "green") << data.speed << std::endl;
  std::cout << palette.set_color("Course: ", "green") << data.course << std::endl;
  std::cout << palette.set_color("UTC Date: ", "green") << data.utc_date << std::endl;
  std::cout << palette.set_color("Mode: ", "green") << data.mode << std::endl;
}

#endif /* RMC_H */
