// gsv.h

#ifndef GSV_H
#define GSV_H

#include <iostream>
#include <string>
#include <vector>

#include "color.h"
#include "print.h"

class GSV
{
private:
  Color palette;
  Print printer;

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

  void print_satellite(satellite_t satellite);
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

void GSV::print_satellite(satellite_t satellite)
{
  std::string separator = palette.set_color(" | ", "red");

  std::string idLabel = palette.set_color("Id: ", "green");
  std::string elevationLabel = palette.set_color("Elevation: ", "green");
  std::string azimuthLabel = palette.set_color("Azimuth: ", "green");
  std::string snrLabel = palette.set_color("SNR: ", "green");

  std::cout << fmt::format("{1} {2} {0} {3} {4} {0} {5} {6} {0} {7} {8}\n",
                           separator,
                           idLabel,
                           satellite.id,
                           elevationLabel,
                           satellite.elevation,
                           azimuthLabel,
                           satellite.azimuth,
                           snrLabel,
                           satellite.snr);
}

void GSV::print_data()
{
  printer.print_title("GSV Sample Data");
  printer.print_info("Type", data.type);
  printer.print_info("Number of Satellites", data.number_of_satellites);
  printer.print_info("Sequence Number", data.sequence_number);
  printer.print_info("Satellites in View", data.satellites_in_view);

  printer.print_subtitle("Satellites");

  for (int i = 0; i < std::stoi(data.number_of_satellites); i++)
  {
    print_satellite(data.satellites[i]);
  }
}

#endif /* GSV_H */
