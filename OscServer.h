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
  virtual void ProcessMessage(const osc::ReceivedMessage& m, const IpEndpointName& remoteEndpoint);

private:
  ClockDisplay& clockDisplay;
};

#endif //RPI_OSC_SERVER_H