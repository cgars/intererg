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

    LEDArray::LEDArray(void)
            : Device("LEDArray") {
    }

    LEDArray::~LEDArray(void) {
        serialStream.Close();
    }

    int LEDArray::open(const string &device, const Options &opts) {
        serialStream.Open(device);
        serialStream.SetBaudRate(SerialStreamBuf::BAUD_38400);
        serialStream.SetCharSize(SerialStreamBuf::CHAR_SIZE_8);
        serialStream.SetNumOfStopBits(1);
        serialStream.SetParity(SerialStreamBuf::PARITY_NONE);
        setDeviceFile(device);
        setDeviceType(MiscellaneousType);
        setDeviceClass("LEDArray");
        setDeviceName("LEDArray");
        setDeviceVendor("Presh Electronics");
        return 0;
    }


    int LEDArray::open(Device &device, const Options &opts) {
        serialStream.Open(device.deviceIdent());
        serialStream.SetBaudRate(SerialStreamBuf::BAUD_38400);
        serialStream.SetCharSize(SerialStreamBuf::CHAR_SIZE_8);
        serialStream.SetNumOfStopBits(1);
        serialStream.SetParity(SerialStreamBuf::PARITY_NONE);
        setDeviceFile(device.deviceIdent());
        setDeviceType(MiscellaneousType);
        setDeviceClass("LEDArray");
        setDeviceName("LEDArray");
        setDeviceVendor("Presh Electronics");
        return 0;
    }

    int LEDArray::open(const string &device) {
        serialStream.Open(device);
        serialStream.SetBaudRate(SerialStreamBuf::BAUD_38400);
        serialStream.SetCharSize(SerialStreamBuf::CHAR_SIZE_8);
        serialStream.SetNumOfStopBits(1);
        serialStream.SetParity(SerialStreamBuf::PARITY_NONE);
        setDeviceFile(device);
        setDeviceType(MiscellaneousType);
        setDeviceClass("LEDArray");
        setDeviceName("LEDArray");
        setDeviceVendor("Presh Electronics");
        return 0;
    }

    void LEDArray::close(void) {
        serialStream.Close();
    }

    int LEDArray::sendCommand(const char &key, const int &value) {
        char next_byte[8];
        char command[8];
        sprintf(command, "1%c%05i\r", key, value);
        printf("command is: %s\n", command);
        serialStream << command;
        printf("i have send it\n");
        char return_value[5];
        printf("waiting for reply\n");
        serialStream.read(return_value, 1);
        printf("got a reply t is:%c\n", return_value);
        return 1;
    }

    int LEDArray::setOneLEDParameter(const int &led, const int &pwm,
                                     const int &current, const int &ontime,
                                     const int &offtime) {
        LEDArray::setLEDParameter(led, OFFLED, pwm, 0, current, 0, ontime, 0,
                                  offtime, 0);
        return 1;
    }

    bool LEDArray::isOpen(void) const {
        bool r = serialStream.IsOpen();
        cerr << "ISOPEN? " << r << '\n';
        return r;
    }

    int LEDArray::setLEDParameter(const int &led1, const int &led2,
                                  const int &pwm1,
                                  const int &pwm2, const int &current1,
                                  const int &current2,
                                  const int &ontime1, const int &ontime2,
                                  const int &offtime1,
                                  const int &offtime2) {
        printf("wills end commands now\n");
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
        printf("send all commands\n");
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
