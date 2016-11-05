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

ClockOscPacketListener::ClockOscPacketListener(ClockDisplay& clockDisplay): clockDisplay(clockDisplay) {
}

void ClockOscPacketListener::ProcessMessage(const osc::ReceivedMessage& message, const IpEndpointName& remoteEndpoint) {
  if (strcmp(message.AddressPattern(), "/time") == 0) {
      osc::int32 time;
      osc::ReceivedMessageArgumentStream args = message.ArgumentStream();
			args >> time >> osc::EndMessage;

      std::cout << "Setting time to " << time << std::endl;
      clockDisplay.setTime(time);
  }

  if (strcmp(message.AddressPattern(), "/clear") == 0) {
      osc::ReceivedMessageArgumentStream args = message.ArgumentStream();
			args >> osc::EndMessage;
      std::cout << "Clearing display" << std::endl;
      clockDisplay.clear();
  }

}
