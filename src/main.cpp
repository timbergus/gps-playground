#include <iostream>
#include <raylib.h>
#include <fstream>
#include <chrono>
#include <thread>

#include "gpsTools.h"
#include "printer.h"

#include "gps.h"

#include "gngll.h"
#include "gngsa.h"
#include "gnrmc.h"
#include "gpgsv.h"

std::vector<std::tuple<double, double>> getCoordinates(
    std::string_view fileName,
    bool isRealtime = false)
{
  int samples = 11163;
  double time = 1666.71;

  std::fstream measures;
  std::string measure;

  std::vector<std::tuple<double, double>> coordinates;

  measures.open(fileName, std::ios::in);

  while (std::getline(measures, measure))
  {
    auto type = measure.substr(0, 6);

    if (type == "$GNRMC")
    {
      if (GPSTools::isValidSample(measure))
      {
        auto sample = GNRMC::parse(measure);

        coordinates.push_back(std::make_tuple(
            GPSTools::parseLatitude(sample.latitude),
            GPSTools::parseLongitude(sample.longitude, sample.longitudeDirection)));
      }
    }
    if (isRealtime)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds((int)std::ceil(time * 1000 / samples)));
    }
  }

  return coordinates;
}

int main(int, char **)
{
  std::vector<std::tuple<double, double>> coordinates{
      getCoordinates("./src/data/today.txt"),
  };

  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(400, 400, "GPS Data Explorer");

  SetTargetFPS(120);

  const int ZOOM = 7000000;

  auto [refLatitude, refLongitude] = coordinates.at(0);

  while (!WindowShouldClose())
  {
    BeginDrawing();
    ClearBackground(GRAY);
    DrawText("GPS Data", 10, 10, 20, WHITE);

    for (auto coord : coordinates)
    {
      auto [latitude, longitude] = coord;
      DrawCircle((latitude * ZOOM) + 200 - (refLatitude * ZOOM), (longitude * ZOOM) + 200 - (refLongitude * ZOOM), 5, YELLOW);
    }

    DrawCircle(200, 200, 5, BLACK);
    DrawCircle(200 + refLatitude, 200 + refLongitude, 5, RED);
    EndDrawing();
  }

  CloseWindow();

  return EXIT_SUCCESS;
}
