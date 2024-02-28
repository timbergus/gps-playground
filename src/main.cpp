#include <raylib.h>
#include <fstream>
#include <tuple>
#include <thread>
#include <fmt/format.h>

#include "gps.h"
#include "printer.h"

std::any measure;

void getSample(std::string_view fileName)
{
  int samplesPerSecond = 12;

  std::fstream samples;
  std::string sample;

  samples.open(fileName, std::ios::in);

  while (std::getline(samples, sample))
  {
    if (GPS::isValidSample(sample) && sample.substr(0, 6).find("RMC") != std::string::npos)
    {
      measure = GPS::parse(sample);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000 / samplesPerSecond));
  }
}

int main()
{
  int scale = 5;

  int DEVICE_WIDTH = 128 * scale;
  int DEVICE_HEIGHT = 128 * scale;

  int TOP = 124;
  int RIGHT = DEVICE_WIDTH;
  int BOTTOM = DEVICE_HEIGHT;
  int LEFT = 0;
  int HEIGHT = DEVICE_HEIGHT - 124;

  int CENTER_X = DEVICE_WIDTH / 2;
  int CENTER_Y = TOP + (HEIGHT / 2);

  int ZOOM = 1000000;

  double REF_LATITUDE = 40.2500666;
  double REF_LONGITUDE = -3.4022613;

  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(DEVICE_WIDTH, DEVICE_HEIGHT, "GPS Data Explorer");

  SetTargetFPS(120);

  // We are going to use a thread to read the GPS samples. It will write
  // the current sample in a global variable shared by the reading thread
  // and the main thread, so the main thread will draw at 120 fps, while
  // the reading thread will get a new sample every second.

  std::thread t(getSample, "./src/data/today.txt");

  std::vector<std::tuple<double, double>> coords;

  while (!WindowShouldClose())
  {
    BeginDrawing();
    ClearBackground(LIGHTGRAY);

    DrawFPS(10, BOTTOM - 30);

    if (measure.has_value())
    {
      auto extractedSample{std::any_cast<RMC>(measure)};

      if (extractedSample.type.find("RMC") != std::string::npos)
      {
        auto latitude = GPS::parseLatitude(extractedSample.latitude);
        auto longitude = GPS::parseLongitude(
            extractedSample.longitude,
            extractedSample.longitudeDirection);

        DrawText(fmt::format(
                     "Data From {} ({})",
                     extractedSample.type.substr(1),
                     Printer::formatUtcDate(GPS::parseUtcDate(extractedSample.utcDate)))
                     .data(),
                 10, 10, 20, BLACK);

        DrawRectangle(0, 35, DEVICE_WIDTH, 3, BLACK);
        DrawText(fmt::format("Latitude: {}", latitude).data(), 10, 50, 20, BLACK);
        DrawText(fmt::format("Longitude: {}", longitude).data(), 10, 70, 20, BLACK);
        DrawText(fmt::format(
                     "Time: {}",
                     Printer::formatUtcTime(GPS::parseUtcDate(extractedSample.utcTime)))
                     .data(),
                 10, 90, 20, BLACK);

        coords.push_back(std::make_tuple(latitude, longitude));

        DrawRectangle(0, 120, DEVICE_WIDTH, 6, BLACK);

        // GRID

        DrawLine(CENTER_X, TOP, CENTER_X, BOTTOM, BLACK);
        DrawLine(LEFT, CENTER_Y, RIGHT, CENTER_Y, BLACK);

        // SAMPLES
        // If we want to have a historic we need to store the coords and the plot the
        // full list of coords.

        for (auto coord : coords)
        {
          auto [lat, lng] = coord;
          DrawCircle(CENTER_X + (lat - REF_LATITUDE) * ZOOM, CENTER_Y + (lng - REF_LONGITUDE) * ZOOM, 5, RED);
        }
      }
    }

    EndDrawing();
  }

  t.join();

  CloseWindow();

  return EXIT_SUCCESS;
}
