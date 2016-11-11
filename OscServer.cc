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

const int FADE_STEP_MS = 10;

ClockOscPacketListener::ClockOscPacketListener(ClockDisplay& clockDisplay): clockDisplay(clockDisplay) {
}

void ClockOscPacketListener::ProcessMessage(const osc::ReceivedMessage& message, const IpEndpointName& remoteEndpoint) {
  try {
    osc::ReceivedMessageArgumentStream arguments = message.ArgumentStream();
    if (strcmp(message.AddressPattern(), "/clear") == 0) {
        clear(arguments);
    }
    if (strcmp(message.AddressPattern(), "/time") == 0) {
      setTime(arguments);
    }
    if (strcmp(message.AddressPattern(), "/brightness") == 0) {
        setBrightness(arguments);
    }
    if (strcmp(message.AddressPattern(), "/fade") == 0) {
        fadeBrightnessTo(arguments);
    }
    if (strcmp(message.AddressPattern(), "/forward") == 0) {
        forwardTimeTo(arguments);
    }
    if (strcmp(message.AddressPattern(), "/showAndForward") == 0) {
        showAndForward(arguments);
    }
  } catch( osc::Exception& e ){
    std::cerr << "!!! Error while parsing message: " << message.AddressPattern() << ": " << e.what() << "\n";
  }
}

void ClockOscPacketListener::setTime(osc::ReceivedMessageArgumentStream arguments) {
  osc::int32 time;
  arguments >> time >> osc::EndMessage;
  setTime(time);
}

void ClockOscPacketListener::setTime(int time) {
  std::cout << "Setting time to " << time << std::endl;
  clockDisplay.setTime(time);
}

void ClockOscPacketListener::clear(osc::ReceivedMessageArgumentStream arguments) {
  arguments >> osc::EndMessage;
  clear();
}

void ClockOscPacketListener::clear() {
  std::cout << "Clearing display" << std::endl;
  clockDisplay.clear();
}

void ClockOscPacketListener::setBrightness(osc::ReceivedMessageArgumentStream arguments) {
  osc::int32 brightness;
  arguments >> brightness >> osc::EndMessage;
  setBrightness(brightness);
}

void ClockOscPacketListener::setBrightness(int brightness) {
  std::cout << "Setting brightness to " << brightness << std::endl;
  clockDisplay.setBrightness(brightness);
}

void ClockOscPacketListener::fadeBrightnessTo(osc::ReceivedMessageArgumentStream arguments) {
  osc::int32 newBrightness, durationMs;
  arguments >> newBrightness >> durationMs >> osc::EndMessage;
  fadeBrightnessTo(newBrightness, durationMs);
}

void ClockOscPacketListener::fadeBrightnessTo(int newBrightness, int durationMs) {
  std::cout << "Fading brightness to " << newBrightness << " in " << durationMs << " ms" << std::endl;

  double currentBrightness = clockDisplay.getBrightness();
  double increment = 1.0 * (newBrightness - currentBrightness) / durationMs * FADE_STEP_MS;
  for(int time = 0; time < durationMs; time += FADE_STEP_MS) {
    currentBrightness += increment;
    clockDisplay.setBrightness(currentBrightness);
    usleep(FADE_STEP_MS * 1000);
  }

  clockDisplay.setBrightness(newBrightness);
}

void ClockOscPacketListener::forwardTimeTo(osc::ReceivedMessageArgumentStream arguments) {
  osc::int32 newTime, durationMs;
  arguments >> newTime >> durationMs >> osc::EndMessage;
  forwardTimeTo(newTime, durationMs);
}

void ClockOscPacketListener::forwardTimeTo(int newTime, int durationMs) {
  std::cout << "Forwarding to " << newTime << " in " << durationMs << " ms" << std::endl;
}

void ClockOscPacketListener::showAndForward(osc::ReceivedMessageArgumentStream arguments) {
  osc::int32 startTime, endTime, brightness, fadeTimeMs, forwardTimeMs;
  arguments >> startTime >> endTime >> brightness >> fadeTimeMs >> forwardTimeMs >> osc::EndMessage;

  setBrightness(0);
  setTime(startTime);
  fadeBrightnessTo(brightness, fadeTimeMs);
  forwardTimeTo(endTime, forwardTimeMs);
  fadeBrightnessTo(0, fadeTimeMs);
  clear();
}
