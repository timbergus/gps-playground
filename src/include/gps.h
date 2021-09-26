// gps.h

// https://www.gpsworld.com/what-exactly-is-gps-nmea-data
// http://navspark.mybigcommerce.com/content/NMEA_Format_v0.1.pdf

#ifndef GPS_H
#define GPS_H

#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <ranges>
#include <vector>
#include <functional>
// Sleep.
#include <chrono>
#include <thread>

#include "utils.h"
#include "rmc.h"
#include "gsv.h"
#include "gsa.h"
#include "gll.h"

class GPS
{
private:
  RMC *rmc;
  GSV *gsv;
  GSA *gsa;
  GLL *gll;

  void read_data_stream(std::string, std::function<void(std::string)>);

public:
  GPS();
  ~GPS();

  void get_data_stream(std::string);
  void parse_sample(std::string);
};

GPS::GPS()
{
}

GPS::~GPS()
{
}

void GPS::read_data_stream(std::string filename, std::function<void(std::string)> callback)
{
  std::ifstream file(filename);
  char buffer[1024];
  while (!file.eof())
  {
    file.getline(buffer, sizeof buffer);
    callback(buffer);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
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

  std::system("clear");

  if (type == "GNRMC" && rmc->is_valid(initial_split[0], initial_split[1]))
  {
    rmc = new RMC(core_data);

    std::cout << "RMC Sample Data" << std::endl;
    std::cout << "===============" << std::endl;
    std::cout << "Time: " << rmc->get_time() << std::endl;
    std::cout << "Date: " << rmc->get_date() << std::endl;
    std::cout << "Latitude: " << rmc->get_latitude() << std::endl;
    std::cout << "Longitude: " << rmc->get_longitude() << std::endl;
    std::cout << "Speed: " << rmc->get_speed("MS") << "m/s" << std::endl;
  }

  if (type == "GPGSV" and gsv->is_valid(initial_split[0], initial_split[1]))
  {
    gsv = new GSV(core_data);

    std::cout << "GSV Sample Data" << std::endl;
    std::cout << "===============" << std::endl;
    gsv->print_data();
  }

  if (type == "GNGSA" && gsa->is_valid(initial_split[0], initial_split[1]))
  {
    gsa = new GSA(core_data);

    std::cout << "GSA Sample Data" << std::endl;
    std::cout << "===============" << std::endl;
    gsa->print_data();
  }

  if (type == "GNGLL" && gll->is_valid(initial_split[0], initial_split[1]))
  {
    gll = new GLL(core_data);

    std::cout << "GLL Sample Data" << std::endl;
    std::cout << "===============" << std::endl;
    std::cout << "Time: " << gll->get_time() << std::endl;
    std::cout << "Latitude: " << gll->get_latitude() << std::endl;
    std::cout << "Longitude: " << gll->get_longitude() << std::endl;
  }
}

void GPS::get_data_stream(std::string filename)
{
  read_data_stream(filename, [this](std::string sample)
                   { this->parse_sample(sample); });
}

#endif /* GPS_H */
