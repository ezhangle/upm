/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <unistd.h>
#include <iostream>
#include <signal.h>

#include "t8100.hpp"

using namespace std;
using namespace upm;

bool shouldRun = true;

void sig_handler(int signo)
{
  if (signo == SIGINT)
    shouldRun = false;
}

int main(int argc, char **argv)
{
  signal(SIGINT, sig_handler);

//! [Interesting]
  // You will need to edit this example to conform to your site and your
  // devices, specifically the Device Object Instance ID passed to the
  // constructor, and the arguments to initMaster() that are
  // appropriate for your BACnet network.

  string defaultDev = "/dev/ttyUSB0";

  // if an argument was specified, use it as the device instead
  if (argc > 1)
    defaultDev = string(argv[1]);

  cout << "Using device " << defaultDev << endl;
  cout << "Initializing..." << endl;

  // Instantiate an T8100 object for an T8100 device that has 568000
  // as it's unique Device Object Instance ID.  NOTE: You will
  // certainly want to change this to the correct value for your
  // device(s).
  T8100 *sensor = new T8100(568000);

  // Initialize our BACnet master, if it has not already been
  // initialized, with the device and baudrate, choosing 1000001 as
  // our unique Device Object Instance ID, 2 as our MAC address and
  // using default values for maxMaster and maxInfoFrames
  sensor->initMaster(defaultDev, 38400, 1000001, 2);

  // Uncomment to enable debugging output
  // sensor->setDebug(true);

  cout << endl;
  cout << "Device Description: " << sensor->getDeviceDescription() << endl;
  cout << "Device Location: " << sensor->getDeviceLocation() << endl;
  cout << endl;

  // update and print a few values every 5 seconds
  while (shouldRun)
    {
      // update our values
      sensor->update();

      cout << "CO2 Concentration: "
           << sensor->getCO2()
           << " ppm"
           << endl;

      // we show both C and F for temperature
      cout << "Temperature: " << sensor->getTemperature()
           << " C / "
           << sensor->getTemperature(true)
           << " F"
           << endl;

      cout << "Humidity: " << sensor->getHumidity()
           << " %RH"
           << endl;

      cout << "Relay State: " << sensor->getRelayState()
           << endl;

      cout << endl;
      sleep(5);
    }

  cout << "Exiting..." << endl;

  delete sensor;

//! [Interesting]

  return 0;
}
