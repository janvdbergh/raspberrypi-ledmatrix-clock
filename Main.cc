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

#include <iostream>
#include <ip/UdpSocket.h>

#include "ClockDisplay.h"
#include "OscServer.h"

#define PORT 7000

int main(int argc, char *argv[]) {
  ClockDisplay clockDisplay;
  if (!clockDisplay.initialize(argc, argv)) {
    return 1;
  }

  ClockOscPacketListener oscPacketListener(clockDisplay);
  UdpListeningReceiveSocket udpReceiveSocket(
          IpEndpointName(IpEndpointName::ANY_ADDRESS, PORT),
          &oscPacketListener);

  std::cout << "Press ctrl-c to end" << std::endl;

  udpReceiveSocket.RunUntilSigInt();

  return 0;
}
