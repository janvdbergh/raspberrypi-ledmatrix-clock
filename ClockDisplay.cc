/*
 * Copyright (C) 2016 Jan Van den Bergh <jan.vdbergh@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ClockDisplay.h"

#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace rgb_matrix;

const char *BDF_FONT_FILE = "fonts/6x9.bdf";
const char *DAYS[] = { "ZONDAG", "MAANDAG", "DINSDAG", "WOENSDAG", "DONDERDAG", "VRIJDAG", "ZATERDAG"};
const int DISPLAY_WIDTH = 64;
const int FONT_WIDTH = 6;
const int TEXT_START_Y = 9;
const int BDF_FONT_FILE_DAY = 7;
const int DIGIT_START_X = 7;
const int DIGIT_START_Y = 12;

ClockDisplay::ClockDisplay(): _digitColor(127, 0, 0), _dayColor(0, 127, 0), _minutesSinceStartOfWeek(-1) {
}

ClockDisplay::~ClockDisplay() {
  if (_rgbMatrix) {
    delete _rgbMatrix;
  }
  if (_font) {
    delete _font;
  }
}

bool ClockDisplay::initialize(int argc, char *argv[]) {
  RGBMatrix::Options options;
  options.hardware_mapping = "adafruit-hat-pwm";
  options.rows = 32;
  options.chain_length = 2;
  options.parallel = 1;

  RuntimeOptions runtimeOptions;
  runtimeOptions.gpio_slowdown = 2;
  _rgbMatrix = rgb_matrix::CreateMatrixFromFlags(&argc, &argv, &options, &runtimeOptions);

  if (_rgbMatrix == NULL) {
    std::cerr <<  "Could not initialize matrix" << std::endl;
    rgb_matrix::PrintMatrixFlags(stderr);
    return false;
  }

  _font = new Font();
  if (!_font->LoadFont(BDF_FONT_FILE)) {
    std::cerr <<  "Could not load font " << BDF_FONT_FILE << std::endl;
    return false;
  }

  _frameCanvas = _rgbMatrix->CreateFrameCanvas();

  return true;
}

int ClockDisplay::getBrightness() const {
  return _digitColor.r;
}

void ClockDisplay::setBrightness(int brightness) {
  _digitColor = Color(brightness, 0, 0);
  _dayColor = Color(0, brightness, 0);
  refreshDisplay();
}

int ClockDisplay::getTime() const {
  return _minutesSinceStartOfWeek;
}

void ClockDisplay::setTime(int minutesSinceStartOfWeek) {
  if (minutesSinceStartOfWeek != _minutesSinceStartOfWeek) {
    _minutesSinceStartOfWeek = minutesSinceStartOfWeek;
    refreshDisplay();
  }
}

void ClockDisplay::clear() {
  _minutesSinceStartOfWeek = -1;
  refreshDisplay();
}

void ClockDisplay::refreshDisplay() {
  _frameCanvas->Clear();

  if (_minutesSinceStartOfWeek >= 0) {
    int day = _minutesSinceStartOfWeek / 1440 % 7;
    int hours = _minutesSinceStartOfWeek % 1440 / 60;
    int minutes = _minutesSinceStartOfWeek % 60;

    // Digits
    drawDigit(0, hours / 10);
    drawDigit(1, hours % 10);
    drawDigit(2, minutes / 10);
    drawDigit(3, minutes % 10);

    // Dots between hours and minutes
    drawDots();

    // Day name
    drawDay(day);
  }

  _rgbMatrix->SwapOnVSync(_frameCanvas);
}

void ClockDisplay::drawRectangle(int x, int y, int width, int height, const Color &color) {
  for(int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      _frameCanvas->SetPixel(x + i, y + j, color.r, color.g, color.b);
    }
  }
}

void ClockDisplay::drawDigit(int position, int digit)  {
  int startX = DIGIT_START_X + position * 12;
  if (position >= 2) {
    startX += 4;
  }

  // top
  if (digit==0 || digit==2 || digit==3 || digit==5 || digit==6 || digit==7 || digit==8 || digit==9) {
    drawRectangle(startX, DIGIT_START_Y, 10, 2, _digitColor);
  }

  // middle
  if (digit==2 || digit==3 || digit==4 || digit==5 || digit==6 || digit==8 || digit==9) {
    drawRectangle(startX, DIGIT_START_Y + 8, 10, 2, _digitColor);
  }

  // bottom
  if (digit==0 || digit==2 || digit==3 || digit==5 || digit==6 || digit==8 || digit==9) {
    drawRectangle(startX, DIGIT_START_Y + 16, 10, 2, _digitColor);
  }

  // left top
  if (digit==0 || digit==4 || digit==5 || digit==6 || digit==8 || digit==9) {
    drawRectangle(startX, DIGIT_START_Y, 2, 10, _digitColor);
  }

  // right top
  if (digit==0 || digit==1 || digit==2 || digit==3 || digit==4 || digit==7 || digit==8 || digit==9) {
    drawRectangle(startX + 8, DIGIT_START_Y, 2, 10, _digitColor);
  }

  // left bottom
  if (digit==0 || digit==2 || digit==6 || digit==8) {
    drawRectangle(startX, DIGIT_START_Y + 8, 2, 10, _digitColor);
  }

  // right bottom
  if (digit==0 || digit==1 || digit==3 || digit==4 || digit==5 || digit==6 || digit==7 || digit==8 || digit==9) {
    drawRectangle(startX + 8, DIGIT_START_Y + 8, 2, 10, _digitColor);
  }
}

void ClockDisplay::drawDots() {
  drawRectangle(DIGIT_START_X + 24, DIGIT_START_Y +  4, 2, 2, _digitColor);
  drawRectangle(DIGIT_START_X + 24, DIGIT_START_Y + 12, 2, 2, _digitColor);
}

void ClockDisplay::drawDay(int dayOfWeek) {
  const char* dayName = DAYS[dayOfWeek];
  int textX = (DISPLAY_WIDTH / 2) - strlen(dayName) * FONT_WIDTH / 2;
  DrawText(_frameCanvas, *_font, textX, TEXT_START_Y, _dayColor, dayName);
}
