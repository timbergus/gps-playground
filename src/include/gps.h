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

#define FMT_HEADER_ONLY
#include <fmt/format.h>

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
  int samples = 20000;
  double time = 1666.71;

  std::fstream measures;
  std::string measure;

  measures.open(file_name, std::ios::in);

  while (std::getline(measures, measure))
  {
    callback(measure);
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
    rmc->print_formatted_data();
  }

  if (type == "GPGSV" and gsv->is_valid(initial_split[0], initial_split[1]))
  {
    gsv = new GSV(core_data);
    gsv->print_data();
  }

  if (type == "GNGSA" && gsa->is_valid(initial_split[0], initial_split[1]))
  {
    gsa = new GSA(core_data);
    gsa->print_data();
  }

  if (type == "GNGLL" && gll->is_valid(initial_split[0], initial_split[1]))
  {
    gll = new GLL(core_data);
    gll->print_formatted_data();
  }
}

void GPS::get_data_stream(std::string file_name, std::function<void(std::string)> callback)
{
  read_data_stream(file_name, callback);
}

#endif /* GPS_H */
