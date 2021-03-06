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
#include <math.h>

const int STEP_IN_MS = 20;

ClockOscPacketListener::ClockOscPacketListener(ClockDisplay& clockDisplay): clockDisplay(clockDisplay) {
}

void ClockOscPacketListener::ProcessMessage(const osc::ReceivedMessage& message, const IpEndpointName& remoteEndpoint) {
  try {
    osc::ReceivedMessageArgumentStream arguments = message.ArgumentStream();
    if (strcmp(message.AddressPattern(), "/clear") == 0) {
        clear(arguments);
    } else if (strcmp(message.AddressPattern(), "/time") == 0) {
      setTime(arguments);
    } else if (strcmp(message.AddressPattern(), "/brightness") == 0) {
        setBrightness(arguments);
    } else if (strcmp(message.AddressPattern(), "/fade") == 0) {
        fadeBrightnessTo(arguments);
    } else if (strcmp(message.AddressPattern(), "/forward") == 0) {
        forwardTimeTo(arguments);
    } else if (strcmp(message.AddressPattern(), "/show") == 0) {
        show(arguments);
    } else if (strcmp(message.AddressPattern(), "/hide") == 0) {
        hide(arguments);
    } else if (strcmp(message.AddressPattern(), "/showForwardAndHide") == 0) {
        showForwardAndHide(arguments);
    } else {
      std::cerr << "!!! Unknown command: " << message.AddressPattern() << std::endl;
    }
  } catch( osc::Exception& e ){
    std::cerr << "!!! Error while parsing message: " << message.AddressPattern() << ": " << e.what() << std::endl;
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
  float brightness;
  arguments >> brightness >> osc::EndMessage;
  setBrightness(brightness);
}

void ClockOscPacketListener::setBrightness(double brightness) {
  std::cout << "Setting brightness to " << brightness << std::endl;
  clockDisplay.setBrightness(brightness);
}

void ClockOscPacketListener::fadeBrightnessTo(osc::ReceivedMessageArgumentStream arguments) {
  osc::int32 durationMs;
  float newBrightness;
  arguments >> newBrightness >> durationMs >> osc::EndMessage;
  fadeBrightnessTo(newBrightness, durationMs);
}

void ClockOscPacketListener::fadeBrightnessTo(double newBrightness, int durationMs) {
  std::cout << "Fading brightness to " << newBrightness << " in " << durationMs << " ms" << std::endl;

  double currentBrightness = clockDisplay.getBrightness();
  double increment = 1.0 * (newBrightness - currentBrightness) / durationMs * STEP_IN_MS;
  for(int stepper = 0; stepper < durationMs; stepper += STEP_IN_MS) {
    currentBrightness += increment;
    clockDisplay.setBrightness(currentBrightness);
    usleep(STEP_IN_MS * 1000);
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

  int startTime = clockDisplay.getTime();
  double steepness = 2 * log(2.0 * (newTime - startTime) - 1);
  for(int stepper = 0; stepper <= durationMs; stepper += STEP_IN_MS) {
    int timeValue = startTime + (newTime - startTime)/(1 + exp(-steepness * (stepper - durationMs / 2.0) / durationMs));
    clockDisplay.setTime(timeValue);

    usleep(STEP_IN_MS * 1000);
  }

  clockDisplay.setTime(newTime);
}

void ClockOscPacketListener::show(osc::ReceivedMessageArgumentStream arguments) {
  osc::int32 time, fadeTimeMs;
  float brightness;
  arguments >> time >> brightness >> fadeTimeMs >> osc::EndMessage;

  setBrightness(0.0);
  setTime(time);
  fadeBrightnessTo(brightness, fadeTimeMs);
}

void ClockOscPacketListener::hide(osc::ReceivedMessageArgumentStream arguments) {
  osc::int32 fadeTimeMs;
  arguments >> fadeTimeMs >> osc::EndMessage;

  fadeBrightnessTo(0.0, fadeTimeMs);

  clear();
}

void ClockOscPacketListener::showForwardAndHide(osc::ReceivedMessageArgumentStream arguments) {
  osc::int32 startTime, endTime, fadeTimeMs, forwardTimeMs, waitTimeMs;
  float brightness;
  arguments >> startTime >> endTime >> brightness >> fadeTimeMs >> forwardTimeMs >> waitTimeMs >> osc::EndMessage;

  setBrightness(0.0);
  setTime(startTime);
  fadeBrightnessTo(brightness, fadeTimeMs);
  forwardTimeTo(endTime, forwardTimeMs);
  usleep(waitTimeMs * 1000);
  fadeBrightnessTo(0.0, fadeTimeMs);

  clear();
}
