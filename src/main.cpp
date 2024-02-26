#include <iostream>
#include <raylib.h>
#include <fstream>
#include <chrono>
#include <thread>
#include <fmt/format.h>

#include "gps.h"

std::vector<std::tuple<double, double, std::string>> getCoordinates(
    std::string_view fileName,
    bool isRealtime = false)
{
  int samplesPerSecond = 12;

  std::fstream measures;
  std::string measure;

  std::vector<std::tuple<double, double, std::string>> coordinates;

  measures.open(fileName, std::ios::in);

  while (std::getline(measures, measure))
  {
    auto type = measure.substr(0, 6);

    if (type.find("RMC") != std::string::npos)
    {
      if (GPS::isValidSample(measure))
      {
        auto sample = GPS::parseRMC(measure);

        auto [hours, minutes, seconds] = GPS::parseUtcTime(sample.utcTime);

        auto time = fmt::format("{}:{}:{}", hours, minutes, seconds);

        coordinates.push_back(std::make_tuple(
            GPS::parseLatitude(sample.latitude),
            GPS::parseLongitude(sample.longitude, sample.longitudeDirection),
            time));
      }
    }
    if (isRealtime)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(1000 / samplesPerSecond));
    }
  }

  return coordinates;
}

int main(int, char **)
{
  std::vector<std::tuple<double, double, std::string>> coordinates{
      getCoordinates("./src/data/today.txt"),
  };

  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(400, 400, "GPS Data Explorer");

  SetTargetFPS(120);

  const int ZOOM = 7000000;

  auto [refLatitude, refLongitude, refTime] = coordinates.at(0);

  int coord = 0;

  while (!WindowShouldClose())
  {
    BeginDrawing();
    ClearBackground(GRAY);
    DrawText("GPS Data", 10, 10, 20, WHITE);

    for (int i = 0; i < coord; i++)
    {
      auto [latitude, longitude, time] = coordinates[i];
      DrawCircle((latitude * ZOOM) + 200 - (refLatitude * ZOOM), (longitude * ZOOM) + 200 - (refLongitude * ZOOM), 5, YELLOW);
    }

    auto [latitude, longitude, time] = coordinates[coord];
    DrawRectangle(10, 40, 150, 20, GRAY);
    DrawText(fmt::format("Time: {}", time).c_str(), 10, 40, 20, GREEN);

    DrawCircle(200, 200, 5, BLACK);
    DrawCircle(200 + refLatitude, 200 + refLongitude, 5, RED);
    EndDrawing();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    if (coord < static_cast<int>(coordinates.size()) - 1)
    {
      coord++;
    }
  }

  CloseWindow();

  return EXIT_SUCCESS;
}
