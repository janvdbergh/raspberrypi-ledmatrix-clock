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
#include <led-matrix.h>
#include <graphics.h>

class ClockDisplay {
public:
  ClockDisplay();
  virtual ~ClockDisplay();

  virtual bool initialize(int argc, char *argv[]);

  virtual int getBrightness() const;
  virtual void setBrightness(int brightness);

  virtual int getTime() const;
  virtual void setTime(int minutesSinceStartOfWeek);

  virtual void clear();
private:
  rgb_matrix::RGBMatrix* _rgbMatrix;
  rgb_matrix::FrameCanvas* _frameCanvas;
  rgb_matrix::Color _digitColor, _dayColor;
  rgb_matrix::Font* _font;
  int _minutesSinceStartOfWeek;
  int _brightness;

  virtual void refreshDisplay();
  virtual void drawRectangle(int x, int y, int width, int height, const rgb_matrix::Color &color);
  virtual void drawDigit(int position, int digit);
  virtual void drawDay(int dayOfWeek);
  virtual void drawDots();
};

#endif // RPI_CLOCK_DISPLAY_H
