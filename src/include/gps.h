// gps.h

// https://www.gpsworld.com/what-exactly-is-gps-nmea-data
// http://navspark.mybigcommerce.com/content/NMEA_Format_v0.1.pdf

#ifndef GPS_H
#define GPS_H

#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
// #include <ranges>
#include <vector>
#include <functional>
// Sleep.
#include <chrono>
#include <thread>
// Maths
#include <cmath>

#include "utils.h"
#include "rmc.h"
#include "gsv.h"
#include "gsa.h"
#include "gll.h"

#include "color.h"

class GPS
{
private:
  Color palette;

  RMC *rmc;
  GSV *gsv;
  GSA *gsa;
  GLL *gll;

  void read_data_stream(std::string, std::function<void(std::string)>);

public:
  GPS();
  ~GPS();

  void get_data_stream(std::string, std::function<void(std::string)> callback);
  void parse_sample(std::string);
};

GPS::GPS()
{
}

GPS::~GPS()
{
}

void GPS::read_data_stream(std::string file_name, std::function<void(std::string)> callback)
{
  std::ifstream file(file_name);
  char buffer[1024];
  while (!file.eof())
  {
    file.getline(buffer, sizeof buffer);
    callback(buffer);

    int samples = 20000;
    double time = 1666.71;

    std::this_thread::sleep_for(std::chrono::milliseconds((int)std::ceil(time * 1000 / samples)));
  }
}

void GPS::parse_sample(std::string sample)
{
  // First we get the checksum.

  std::vector<std::string> initial_split = Utils::clean_split(sample, "*");

  // Then we parse the RMC sentence. Even with the empty tokens.

  std::vector<std::string> core_data = Utils::split(initial_split[0], ",");

  std::string type = core_data[0].substr(1);

  core_data.push_back(initial_split[1]);

#ifdef _WIN32
  std::system("cls");
#else
  std::system("clear");
#endif

  if (type == "GNRMC" && rmc->is_valid(initial_split[0], initial_split[1]))
  {
    rmc = new RMC(core_data);

    std::cout << palette.set_color("RMC Sample Data", "cyan", "underline") << "\n"
              << std::endl;
    std::cout << palette.set_color("Time: ", "green") << rmc->get_time() << std::endl;
    std::cout << palette.set_color("Date: ", "green") << rmc->get_date() << std::endl;
    std::cout << palette.set_color("Latitude: ", "green") << rmc->get_latitude() << std::endl;
    std::cout << palette.set_color("Longitude: ", "green") << rmc->get_longitude() << std::endl;
    std::cout << palette.set_color("Speed: ", "green") << rmc->get_speed("MS") << palette.set_color(" m/s", "magenta") << std::endl;
  }

  if (type == "GPGSV" and gsv->is_valid(initial_split[0], initial_split[1]))
  {
    gsv = new GSV(core_data);

    std::cout << palette.set_color("GSV Sample Data", "cyan", "underline") << "\n"
              << std::endl;
    gsv->print_data();
  }

  if (type == "GNGSA" && gsa->is_valid(initial_split[0], initial_split[1]))
  {
    gsa = new GSA(core_data);

    std::cout << palette.set_color("GSA Sample Data", "cyan", "underline") << "\n"
              << std::endl;
    gsa->print_data();
  }

  if (type == "GNGLL" && gll->is_valid(initial_split[0], initial_split[1]))
  {
    gll = new GLL(core_data);

    std::cout << palette.set_color("GLL Sample Data", "cyan", "underline") << "\n"
              << std::endl;
    std::cout << palette.set_color("Time: ", "green") << gll->get_time() << std::endl;
    std::cout << palette.set_color("Latitude: ", "green") << gll->get_latitude() << std::endl;
    std::cout << palette.set_color("Longitude: ", "green") << gll->get_longitude() << std::endl;
  }
}

void GPS::get_data_stream(std::string file_name, std::function<void(std::string)> callback)
{
  read_data_stream(file_name, callback);
}

#endif /* GPS_H */
