// gsv.h

#ifndef GSV_H
#define GSV_H

#include <iostream>
#include <string>
#include <vector>

#include "utils.h"
#include "color.h"

class GSV
{
private:
  Color palette;

  struct satellite_t
  {
    std::string id;
    std::string elevation;
    std::string azimuth;
    std::string snr;
  };

  struct gsv_t
  {
    std::string type;
    std::string number_of_satellites;
    std::string sequence_number;
    std::string satellites_in_view;
    std::vector<satellite_t> satellites;
    std::string checksum;
  } data;

public:
  GSV(std::vector<std::string>);
  ~GSV();

  bool is_valid(std::string, std::string);

  void print_data();
};

GSV::GSV(std::vector<std::string> core_data)
{
  data.type = core_data[0].substr(1);
  data.number_of_satellites = core_data[1];
  data.sequence_number = core_data[2];
  data.satellites_in_view = core_data[3];

  for (int i = 1; i <= std::stoi(data.number_of_satellites); i++)
  {
    satellite_t satellite;

    satellite.id = core_data[i * 4 + 0];
    satellite.elevation = core_data[i * 4 + 1];
    satellite.azimuth = core_data[i * 4 + 2];
    satellite.snr = core_data[i * 4 + 3];

    data.satellites.push_back(satellite);
  }

  data.checksum = core_data[13];
}

GSV::~GSV()
{
}

bool GSV::is_valid(std::string core_data, std::string checksum)
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

void GSV::print_data()
{
  std::cout << palette.set_color("Type: ", "green") << data.type << std::endl;
  std::cout << palette.set_color("Number of Satellites: ", "green") << data.number_of_satellites << std::endl;
  std::cout << palette.set_color("Sequence Number: ", "green") << data.sequence_number << std::endl;
  std::cout << palette.set_color("Satellites in View: ", "green") << data.satellites_in_view << "\n"
            << std::endl;

  std::cout << palette.set_color("Satellites", "yellow", "underline") << "\n"
            << std::endl;

  for (int i = 0; i < std::stoi(data.number_of_satellites); i++)
  {
    std::cout << palette.set_color("Id: ", "green") << data.satellites[i].id << palette.set_color(" | ", "red");
    std::cout << palette.set_color("Elevation: ", "green") << data.satellites[i].elevation << palette.set_color(" | ", "red");
    std::cout << palette.set_color("Azimuth: ", "green") << data.satellites[i].azimuth << palette.set_color(" | ", "red");
    std::cout << palette.set_color("SNR: ", "green") << data.satellites[i].snr << std::endl;
  }
}

#endif /* GSV_H */
