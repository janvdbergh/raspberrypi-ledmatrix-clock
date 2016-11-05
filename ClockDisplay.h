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

#ifndef RPI_CLOCK_DISPLAY_H
#define RPI_CLOCK_DISPLAY_H
#include <graphics.h>

class ClockDisplay {
public:
  ClockDisplay();
  virtual ~ClockDisplay();

  virtual bool initialize(int argc, char *argv[]);

  virtual void setBrightness(int brightness);
  virtual void setTime(int secondsSinceStartOfWeek);
  virtual void clear();
private:
  rgb_matrix::Canvas* _canvas;
  rgb_matrix::Font* _font;
  rgb_matrix::Color _digitColor, _dayColor;

  virtual void drawRectangle(int x, int y, int width, int height, const rgb_matrix::Color &color);
  virtual void drawDigit(int position, int digit);
  virtual void drawDay(int dayOfWeek);
};

#endif // RPI_CLOCK_DISPLAY_H
