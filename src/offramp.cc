/*
  erg/offramp.cc
  Ramps the Duration of the off pulse

  RELACS - Relaxed ELectrophysiological data Acquisition, Control, and Stimulation
  Copyright (C) 2002-2012 Jan Benda <benda@bio.lmu.de>

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

#include <relacs/erg/offramp.h>
#include <string>

using namespace relacs;

namespace erg {


    OffRamp::OffRamp(void)
            : RePro("OffRamp", "erg", "Christian Garbers", "1.0",
                    "Jan 25, 2012") {
        addNumber("Repeats", "Nr of ERGs to measure", 10.0, 1.0, 1000.0, 1.0,
                  "times",
                  "times");
        addNumber("LED", "Number of the LED", 1.0, 1.0, 48.0, 1.0, "LEDNr.",
                  "LEDNr");
        addNumber("Current", "Current for the LED", 1.0, 1.0, 4095.0, 1.0,
                  "bit",
                  "bit");
        addNumber("PWM", "PWM for the LED", 1.0, 1.0, 4095.0, 1.0, "bit",
                  "bit");
        addNumber("OnDuration", "Duration of the LED on phase", 50.0, 1.0,
                  1000.0,
                  1.0, "ms", "ms");
        addNumber("StartDur", "Staring Off Duration of the second LED", 0.0,
                  1.0, 5000.0, 1.0, "ms", "ms");
        addNumber("IncrDur", "Off Duration Increment for the of the second LED",
                  400.0, 1.0, 5000.0, 1.0, "ms", "ms");
        addNumber("StopDur", "Upper bund for Off Duration", 4000.0, 1.0, 10000,
                  1.0, "ms", "ms");
        addNumber("InitialWait", "Time to wait before measurement", 4000.0, 1.0,
                  4095.0, 1.0, "ms", "ms");
        addText("filename.dat");
    }


    int OffRamp::main(void) {
        LEDArray *larray;
        larray = dynamic_cast< LEDArray * >( device("led-1"));
        int repeats = int(number("Repeats"));
        int led = int(number("LED"));
        int current = int(number("Current"));
        int pwm = int(number("PWM"));
        double on_duration = number("OnDuration");
        double start_dur = number("StartDur");
        double stop_dur = number("StopDur");
        double incr_dur = number("IncrDur");
        double initial_wait = number("InitialWait");
        const EventData &trigger = events("Trigger-1");

        char command[100];
        sprintf(command, "%.4i%.4i%.2i", 4000, 4000, led);
        larray->sendCommand(command);
        sleep(initial_wait / 1000);
        larray->sendCommand("0000000000");

        //Measurement  ---------------------------------------------------------------
        for (double off_duration = start_dur;
             off_duration < stop_dur; off_duration += incr_dur) {
            int counter;
            int index1 = trace("V-1").currentIndex();
            for (counter = 1; counter < repeats; counter++) {
                char command[100];
                sprintf(command, "%.4i%.4i%.2i", pwm, current, led);
                larray->sendCommand(command);
                sleep(on_duration / 1000.0);
                larray->sendCommand("0000000000");
                sleep(off_duration / 1000.0);
            }
            int index2 = trace("V-1").currentIndex();
            string basename = text("filename.dat");
            char filename[100];
            sprintf(filename, "off_dur%i%s", int(off_duration),
                    basename.c_str());
            ofstream df(filename, ofstream::out | ofstream::app);
            TableKey datakey;
            datakey.addNumber("ERG", "mv", "%6.3f");
            datakey.addNumber("Trigger", "v", "%6.3f");
            datakey.saveKey(df);
            for (int index = index1; index < index2; index++) {
                datakey.save(df, trace("V-1").at(index), 0);
                datakey.save(df, trace("StimulusTrigger").at(index), 1);
                df << "\n";
            }
            df.close();
            cout << off_duration << endl;
        }
        larray->sendCommand("0000000000");
        return Completed;
    }


    addRePro(OffRamp, erg);

}; /* namespace erg */

#include "moc_offramp.cc"
