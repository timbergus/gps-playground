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

class GPS
{
private:
  RMC *rmc;

  void read_data_stream(std::string, std::function<void(std::string)>);

public:
  GPS();
  ~GPS();

  void get_data_stream();
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
  while (file.eof() == false)
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

  std::string type = core_data[0].substr(core_data[0].size() - 3);

  core_data.push_back(initial_split[1]);

  if (type == "RMC" && rmc->is_valid(initial_split[0], initial_split[1]))
  {
    rmc = new RMC(core_data);

    std::system("clear");

    std::cout << "RMC Sample Data" << std::endl;
    std::cout << "===============" << std::endl;

    std::cout << "Time: " << rmc->get_time() << std::endl;
    std::cout << "Date: " << rmc->get_date() << std::endl;
    std::cout << "Latitude: " << rmc->get_latitude() << std::endl;
    std::cout << "Longitude: " << rmc->get_longitude() << std::endl;
    std::cout << "Speed: " << rmc->get_speed("MS") << "m/s" << std::endl;
  }
}

void GPS::get_data_stream()
{
  read_data_stream("./src/data/samples.txt", [this](std::string sample)
                   { this->parse_sample(sample); });
}

#endif /* GPS_H */
