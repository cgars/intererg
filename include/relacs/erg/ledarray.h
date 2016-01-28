/*
  device.h
  Virtual class for accessing a device.

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

#ifndef _RELACS_ERG_LEDARRAY_H_
#define _RELACS_ERG_LEDARRAY_H_1

//#pragma once
#include <relacs/device.h>
#include <iostream>
#include <relacs/options.h>
#include <SerialStream.h>

using namespace relacs;
using namespace LibSerial;

namespace erg {
/*!
\class LEDArray
\brief Basic interface to the LEDArray.
\author Christian Garbers
\version 1.3
*/


    class LEDArray : public Device {

    public:
        /*! Construct a Device of type \a type.
            \sa setDeviceType() */
        LEDArray(void);

        /*! Destroy a Device. In case the associated device is open, close it. */
        ~LEDArray(void);

        /*! Open the device specified by \a device with options \a opts.
        On success, in your implementation
        \a device should be passed to setDeviceFile()
        and the name and the vendor of the device should be set
        with setDeviceName() and setDeviceVendor().
            The info() should be set in the implementation of open()
        by the info().addNumber(), info().addText(), etc. functions.
            For adding some default information to info() you may use addInfo().
        Returns zero on success, or InvalidDevice (or any other negative number
        indicating the error).
            \sa isOpen(), close(), reset() */
        int open(const string &device, const Options &opts);

        /*! Open the device specified by \a device with options \a opts.
        On success, in your implementation
        \a device should be passed to setDeviceFile()
        and the name and the vendor of the device should be set
        with setDeviceName() and setDeviceVendor().
            The info() should be set in the implementation of open()
        by the info().addNumber(), info().addText(), etc. functions.
            For adding some default information to info() you may use addInfo().
        Returns zero on success, or InvalidDevice (or any other negative number
        indicating the error).
            \sa isOpen(), close(), reset() */
        int open(Device &device, const Options &opts);

        int open(const string &device);

        void close(void);

        int sendCommand(const string &command);

        int sendCommand(const char &key, const int &value);

        bool isOpen(void) const;

        /*
         * Set all the parameter to the led (stop it if running)
         */
        int setLEDParameter(const int &led1, const int &led2, const int &pwm1,
                            const int &pwm2, const int &current1,
                            const int &current2,
                            const int &ontime1, const int &ontime2,
                            const int &offtime1,
                            const int &offtime2);

        /*
         * Start the array and do cycle cycles
         */

        int start(const int &cycles);


    };
}; /* namespace relacs */

#endif /* _RELACS_ERG_LEDARRAY */
