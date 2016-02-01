/*
  LEDArray.cc
  Class for accessing the LEDArray

  RELACS - Relaxed ELectrophysiological data Acquisition, Control, and Stimulation
  Copyright (C) 2002-2011 Jan Benda <benda@bio.lmu.de>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.
  
  RELACS is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <sstream>
#include <relacs/relacsplugin.h>
#include <relacs/intererg/ledarray.h>

using namespace relacs;
using namespace LibSerial;

namespace intererg {

    static SerialStream SerialStream;

    LEDArray::LEDArray(void)
            : Device("LEDArray") {
    }

    LEDArray::~LEDArray(void) {
        SerialStream.Close();
    }

    int LEDArray::open(const string &device, const Options &opts) {
        return open(device);
    }

    int LEDArray::open(Device &device, const Options &opts) {
        return open(device.deviceIdent());
    }

    int LEDArray::open(const string &device) {
        SerialStream.Open(device);
        SerialStream.SetBaudRate(SerialStreamBuf::BAUD_38400);
        SerialStream.SetCharSize(SerialStreamBuf::CHAR_SIZE_8);
        SerialStream.SetNumOfStopBits(1);
        SerialStream.SetParity(SerialStreamBuf::PARITY_NONE);
        setDeviceFile(device);
        setDeviceType(MiscellaneousType);
        setDeviceClass("LEDArray");
        setDeviceName("LEDArray");
        setDeviceVendor("Presh Electronics");
        return 0;
    }

    void LEDArray::close(void) {
        SerialStream.Close();
    }

    int LEDArray::sendCommand(const char &key, const int &value) {
        char next_byte[8];
        char command[8];
        sprintf(command, "1%c%05i\r", key, value);
        SerialStream << command;
        char return_value[5];
        SerialStream.read(return_value, 1);
        return 1;
    }

    int LEDArray::setOneLEDParameter(const int &led, const int &pwm,
                                     const int &current, const int &ontime,
                                     const int &offtime) {
        LEDArray::setLEDParameter(led, 0, pwm, 0, current, 0, ontime, 0,
                                  offtime, 0);
        return 1;
    }

    bool LEDArray::isOpen(void) const {
        bool r = SerialStream.IsOpen();
        cerr << "ISOPEN? " << r << '\n';
        return r;
    }

    int LEDArray::setLEDParameter(const int &led1, const int &led2,
                                  const int &pwm1, const int &pwm2,
                                  const int &current1, const int &current2,
                                  const int &ontime1, const int &ontime2,
                                  const int &offtime1, const int &offtime2) {
        sendCommand('a', led1);
        sendCommand('b', led2);
        sendCommand('c', pwm1);
        sendCommand('d', pwm2);
        sendCommand('e', current1);
        sendCommand('f', current2);
        sendCommand('g', ontime1);
        sendCommand('h', ontime2);
        sendCommand('i', offtime1);
        sendCommand('j', offtime2);
        return 1;
    }

    int LEDArray::start(const int &cycles) {
        sendCommand('l', 0);
        sendCommand('m', 0);
        sendCommand('k', cycles);
        sendCommand('l', 1);
        return 1;
    }

    addDevice(LEDArray, intererg);

}; /* namespace intererg */
