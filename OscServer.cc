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

#include "OscServer.h"
#include <iostream>
#include <unistd.h>

ClockOscPacketListener::ClockOscPacketListener(ClockDisplay& clockDisplay): clockDisplay(clockDisplay) {
}

void ClockOscPacketListener::ProcessMessage(const osc::ReceivedMessage& message, const IpEndpointName& remoteEndpoint) {
  try {
    osc::ReceivedMessageArgumentStream arguments = message.ArgumentStream();
    if (strcmp(message.AddressPattern(), "/time") == 0) {
      setTime(arguments);
    }

    if (strcmp(message.AddressPattern(), "/clear") == 0) {
        clear(arguments);
    }
    if (strcmp(message.AddressPattern(), "/brightness") == 0) {
        setBrightness(arguments);
    }
  } catch( osc::Exception& e ){
    std::cerr << "!!! Error while parsing message: " << message.AddressPattern() << ": " << e.what() << "\n";
  }
}

void ClockOscPacketListener::setTime(osc::ReceivedMessageArgumentStream arguments) {
  osc::int32 time;
  arguments >> time >> osc::EndMessage;

  std::cout << "Setting time to " << time << std::endl;
  clockDisplay.setTime(time);
}

void ClockOscPacketListener::clear(osc::ReceivedMessageArgumentStream arguments) {
  arguments >> osc::EndMessage;
  std::cout << "Clearing display" << std::endl;
  clockDisplay.clear();
}

void ClockOscPacketListener::setBrightness(osc::ReceivedMessageArgumentStream arguments) {
  osc::int32 brightness;
  arguments >> brightness >> osc::EndMessage;

  std::cout << "Setting brightness to " << brightness << std::endl;
  clockDisplay.setBrightness(brightness);
}
