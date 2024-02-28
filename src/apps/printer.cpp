#include "printer.h"

Printer::Printer()
{
}

Printer::~Printer()
{
}

void Printer::printTitle(std::string_view label)
{
  fmt::print(fmt::emphasis::underline | fg(fmt::color::cyan), "{}\n\n", label);
}

void Printer::printSubtitle(std::string_view label)
{
  fmt::print(fmt::emphasis::underline | fg(fmt::color::yellow), "\n{}\n\n", label);
}

void Printer::printInfo(std::string_view label, auto value)
{
  fmt::print("{} {}\n", fmt::format(fg(fmt::color::yellow_green), "{}:", label), value);
}

void Printer::printNumber(std::string_view label, auto value, std::string_view units)
{
  fmt::print(
      "{} {:.7f} {}\n",
      fmt::format(fg(fmt::color::yellow_green), "{}:", label),
      value,
      fmt::format(fg(fmt::color::blue_violet), "{}", units));
}

void Printer::printSatellite(Satellite satellite)
{
  fmt::print(
      "{1} {2} {0} {3} {4} {0} {5} {6} {0} {7} {8}\n",
      fmt::format(fg(fmt::color::red), "|"),
      fmt::format(fg(fmt::color::yellow_green), "Id: "),
      satellite.id,
      fmt::format(fg(fmt::color::yellow_green), "Elevation: "),
      satellite.elevation,
      fmt::format(fg(fmt::color::yellow_green), "Azimuth: "),
      satellite.azimuth,
      fmt::format(fg(fmt::color::yellow_green), "SNR: "),
      satellite.snr);
}

std::string Printer::formatUtcTime(std::tuple<std::string, std::string, std::string> utcTime)
{
  auto [hours, minutes, seconds] = utcTime;
  return fmt::format("{}:{}:{}", hours, minutes, seconds);
}

std::string Printer::formatUtcDate(std::tuple<std::string, std::string, std::string> utcDate)
{
  auto [day, month, year] = utcDate;
  return fmt::format("{}/{}/{}", day, month, year);
}

void Printer::print(std::string_view sample)
{
  auto type = sample.substr(0, 6);

  if (type.find("GGA") != std::string::npos)
  {
    auto data{std::any_cast<GGA>(GPS::parse(sample))};

    printTitle(fmt::format("{} Sample Data", data.type.substr(1)));
  }
  else if (type.find("GLL") != std::string::npos)
  {
    auto data{std::any_cast<GLL>(GPS::parse(sample))};

    printTitle(fmt::format("{} Sample Data", data.type.substr(1)));
    printInfo("Time", formatUtcTime(GPS::parseUtcTime(data.utcTime)));
    printNumber("Latitude", GPS::parseLatitude(data.latitude));
    printNumber("Longitude", GPS::parseLongitude(data.longitude, data.longitudeDirection));
    printInfo("Status", data.status);
  }
  else if (type.find("GSA") != std::string::npos)
  {
    auto data{std::any_cast<GSA>(GPS::parse(sample))};

    printTitle(fmt::format("{} Sample Data", data.type.substr(1)));
    printInfo("Mode", data.mode);
    printInfo("Fix Type", data.fixType);
    printInfo("Satellite", GPS::stringifyVector(data.satellites));
    printInfo("PDOP", data.PDOP);
    printInfo("HDOP", data.HDOP);
    printInfo("VDOP", data.VDOP);
  }
  else if (type.find("GSV") != std::string::npos)
  {
    auto data{std::any_cast<GSV>(GPS::parse(sample))};

    printTitle(fmt::format("{} Sample Data", data.type.substr(1)));
    printInfo("Number of Messages", data.numberOfMessages);
    printInfo("Sequence Number", data.sequenceNumber);
    printInfo("Satellites in View", data.satellitesInView);

    printSubtitle("Satellites");

    for (int i = 0; i < std::stoi(data.numberOfMessages); i++)
    {
      printSatellite(data.satellites[i]);
    }
  }
  else if (type.find("RMC") != std::string::npos)
  {
    auto data{std::any_cast<RMC>(GPS::parse(sample))};

    printTitle(fmt::format("{} Sample Data", data.type.substr(1)));
    printInfo("Time", formatUtcTime(GPS::parseUtcTime(data.utcTime)));
    printInfo("Date", formatUtcDate(GPS::parseUtcDate(data.utcDate)));
    printNumber("Latitude", GPS::parseLatitude(data.latitude));
    printNumber("Longitude", GPS::parseLongitude(data.longitude, data.longitudeDirection));
    printNumber("Speed", GPS::parseSpeed(data.speed, GPS::ms), "m/s");
  }
  else if (type.find("VTG") != std::string::npos)
  {
    auto data{std::any_cast<VTG>(GPS::parse(sample))};

    printTitle(fmt::format("{} Sample Data", data.type.substr(1)));
  }
  else if (type.find("ZDA") != std::string::npos)
  {
    auto data{std::any_cast<ZDA>(GPS::parse(sample))};

    printTitle(fmt::format("{} Sample Data", data.type.substr(1)));
  }
  else
  {
    throw std::runtime_error("Sample type not supported");
  }
}

void Printer::clearScreen()
{
#ifdef _WIN32
  std::system("cls");
#else
  std::system("clear");
#endif
}
