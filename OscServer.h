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

#ifndef RPI_OSC_SERVER_H
#define RPI_OSC_SERVER_H

#include <osc/OscPacketListener.h>
#include "ClockDisplay.h"

class ClockOscPacketListener: public osc::OscPacketListener {
public:
  ClockOscPacketListener(ClockDisplay& clockDisplay);

protected:
  virtual void ProcessMessage(const osc::ReceivedMessage& message, const IpEndpointName& remoteEndpoint);

private:
  ClockDisplay& clockDisplay;

  virtual void clear(const osc::ReceivedMessageArgumentStream argumentsStream);
  virtual void clear();

  virtual void setTime(const osc::ReceivedMessageArgumentStream argumentsStream);
  virtual void setTime(int time);

  virtual void setBrightness(const osc::ReceivedMessageArgumentStream argumentsStream);
  virtual void setBrightness(int brightness);

  virtual void fadeBrightnessTo(const osc::ReceivedMessageArgumentStream argumentsStream);
  virtual void fadeBrightnessTo(int newBrightness, int durationMs);

  virtual void forwardTimeTo(const osc::ReceivedMessageArgumentStream argumentsStream);
  virtual void forwardTimeTo(int newTime, int durationMs);

  virtual void show(const osc::ReceivedMessageArgumentStream argumentsStream);
  virtual void showAndForward(const osc::ReceivedMessageArgumentStream argumentsStream);
};

#endif //RPI_OSC_SERVER_H
