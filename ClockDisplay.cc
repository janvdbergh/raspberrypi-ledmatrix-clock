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
#include "led-matrix.h"

#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace rgb_matrix;

const char *BDF_FONT_FILE = "fonts/6x9.bdf";
const char *DAYS[] = { "ZONDAG", "MAANDAG", "DINSDAG", "WOENSDAG", "DONDERDAG", "VRIJDAG", "ZATERDAG"};
const int DISPLAY_WIDTH = 64;
const int FONT_WIDTH = 6;
const int TEXT_START_Y = 9;

const Color DARK_RED(12, 0, 0);

const int DIGIT_START_X = 7;
const int DIGIT_START_Y = 12;
const int DIGIT_BLOCK_LENGTH = 6;
const int DIGIT_BLOCK_HEIGHT = 2;
const int DIGIT_SEPARATOR_WIDTH = 2;
const int SEPARATOR_DOT_SIZE = 2;
const int DIGIT_WIDTH = DIGIT_BLOCK_LENGTH + 2 * DIGIT_BLOCK_HEIGHT + DIGIT_SEPARATOR_WIDTH;
const int SEPARATOR_WIDTH = SEPARATOR_DOT_SIZE + DIGIT_SEPARATOR_WIDTH;

ClockDisplay::ClockDisplay(): _digitColor(0, 0, 0), _dayColor(0, 0, 0) {
  this->setBrightness(127);
}

ClockDisplay::~ClockDisplay() {
  if (_canvas) {
    delete _canvas;
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
  _canvas = rgb_matrix::CreateMatrixFromFlags(&argc, &argv, &options, &runtimeOptions);

  if (_canvas == NULL) {
    std::cerr <<  "Could not initialize matrix" << std::endl;
    rgb_matrix::PrintMatrixFlags(stderr);
    return false;
  }

  _font = new Font();
  if (!_font->LoadFont(BDF_FONT_FILE)) {
    std::cerr <<  "Could not load font " << BDF_FONT_FILE << std::endl;
    return false;
  }

  return true;
}

void ClockDisplay::setBrightness(int brightness) {
  _digitColor = Color(brightness, 0, 0);
  _dayColor = Color(0, brightness, 0);
}

void ClockDisplay::setTime(int secondsSinceStartOfWeek) {
  clear();

  int day = secondsSinceStartOfWeek / 86400 % 7;
  int hours = secondsSinceStartOfWeek % 86400 / 3600;
  int minutes = secondsSinceStartOfWeek % 3600 / 60;

  // Digits
  drawDigit(0, hours / 10);
  drawDigit(1, hours % 10);
  drawDigit(2, minutes / 10);
  drawDigit(3, minutes % 10);

  // Dots between hours and minutes
  drawRectangle(
    DIGIT_START_X + 2 * DIGIT_WIDTH, DIGIT_START_Y + 2 * DIGIT_BLOCK_HEIGHT,
    2, 2,
    _digitColor
  );
  drawRectangle(
    DIGIT_START_X + 2 * DIGIT_WIDTH, DIGIT_START_Y + 3 * DIGIT_BLOCK_HEIGHT + DIGIT_BLOCK_LENGTH,
    2, 2,
    _digitColor
  );

  // Day name
  drawDay(day);
}

void ClockDisplay::clear() {
  _canvas->Clear();
}

void ClockDisplay::drawRectangle(int x, int y, int width, int height, const Color &color) {
  for(int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      _canvas->SetPixel(x + i, y + j, color.r, color.g, color.b);
    }
  }
}

void ClockDisplay::drawDigit(int position, int digit)  {
  int startX = DIGIT_START_X + DIGIT_WIDTH * position + (position >= 2 ? SEPARATOR_WIDTH : 0);

  // top
  drawRectangle(
    startX + DIGIT_BLOCK_HEIGHT, DIGIT_START_Y,
    DIGIT_BLOCK_LENGTH, DIGIT_BLOCK_HEIGHT,
    (digit==0 || digit==2 || digit==3 || digit==5 || digit==6 || digit==7 || digit==8 || digit==9) ? _digitColor : DARK_RED
  );

  // middle
  drawRectangle(
    startX + DIGIT_BLOCK_HEIGHT, DIGIT_START_Y + DIGIT_BLOCK_HEIGHT + DIGIT_BLOCK_LENGTH,
    DIGIT_BLOCK_LENGTH, DIGIT_BLOCK_HEIGHT,
    (digit==2 || digit==3 || digit==4 || digit==5 || digit==6 || digit==8 || digit==9) ? _digitColor : DARK_RED
  );

  // bottom
  drawRectangle(
    startX + DIGIT_BLOCK_HEIGHT, DIGIT_START_Y + 2 * DIGIT_BLOCK_HEIGHT + 2 * DIGIT_BLOCK_LENGTH,
    DIGIT_BLOCK_LENGTH, DIGIT_BLOCK_HEIGHT,
    (digit==0 || digit==2 || digit==3 || digit==5 || digit==6 || digit==8 || digit==9) ? _digitColor : DARK_RED
  );

  // left top
  drawRectangle(
    startX, DIGIT_START_Y + DIGIT_BLOCK_HEIGHT,
    DIGIT_BLOCK_HEIGHT, DIGIT_BLOCK_LENGTH,
    (digit==0 || digit==4 || digit==5 || digit==6 || digit==8 || digit==9) ? _digitColor : DARK_RED
  );

  // right top
  drawRectangle(
    startX + DIGIT_BLOCK_HEIGHT + DIGIT_BLOCK_LENGTH, DIGIT_START_Y + DIGIT_BLOCK_HEIGHT,
    DIGIT_BLOCK_HEIGHT, DIGIT_BLOCK_LENGTH,
    (digit==0 || digit==1 || digit==2 || digit==3 || digit==4 || digit==7 || digit==8 || digit==9) ? _digitColor : DARK_RED
  );

  // left bottom
  drawRectangle(
    startX, DIGIT_START_Y + 2 * DIGIT_BLOCK_HEIGHT + DIGIT_BLOCK_LENGTH,
    DIGIT_BLOCK_HEIGHT, DIGIT_BLOCK_LENGTH,
    (digit==0 || digit==2 || digit==6 || digit==8) ? _digitColor : DARK_RED
  );

  // right bottom
  drawRectangle(
    startX + DIGIT_BLOCK_HEIGHT + DIGIT_BLOCK_LENGTH, DIGIT_START_Y + 2 * DIGIT_BLOCK_HEIGHT + DIGIT_BLOCK_LENGTH,
    DIGIT_BLOCK_HEIGHT, DIGIT_BLOCK_LENGTH,
    (digit==0 || digit==1 || digit==3 || digit==4 || digit==5 || digit==6 || digit==7 || digit==8 || digit==9) ? _digitColor : DARK_RED
  );
}

void ClockDisplay::drawDay(int dayOfWeek) {
  const char* dayName = DAYS[dayOfWeek];
  int textX = (DISPLAY_WIDTH / 2) - strlen(dayName) * FONT_WIDTH / 2;
  DrawText(_canvas, *_font, textX, TEXT_START_Y, _dayColor, dayName);
}
