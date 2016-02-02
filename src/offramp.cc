/*
  intererg/offramp.cc
  Ramps the Duration of the off pulse

  Intererg (C) 2012-2016 Christian Garbers <christian@stuebeweg50.de>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  Intererg is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <relacs/intererg/offramp.h>
#include <string>

using namespace relacs;

namespace intererg {


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
        addText("filename");
    }


    int OffRamp::main(void) {
        LEDArray *larray = dynamic_cast< LEDArray * >( device("led-1"));
        const int repeats = int(number("Repeats"));
        const int led = int(number("LED"));
        const int current = int(number("Current"));
        const int pwm = int(number("PWM"));
        const double on_duration = number("OnDuration");
        const double start_dur = number("StartDur");
        const double stop_dur = number("StopDur");
        const double incr_dur = number("IncrDur");
        const double initial_wait = number("InitialWait");
        const EventData &trigger = events("Trigger-1");

        larray->setOneLEDParameter(led, pwm, current, on_duration,
                                   initial_wait);
        larray->start(1);
        sleep((initial_wait + on_duration) / 1000);


        int indexBefore, indexAfter;
        //Measurement  ---------------------------------------------------------
        for (double off_duration = start_dur;
             off_duration < stop_dur; off_duration += incr_dur) {
            int counter;
            indexBefore = trace("V-1").currentIndex();
            for (counter = 1; counter < repeats; counter++) {
                larray->setOneLEDParameter(led, pwm, current, on_duration,
                                           off_duration);
                larray->start(1);
                sleep((on_duration + off_duration) / 1000.0);
            }
            indexAfter = trace("V-1").currentIndex();
            ofstream resultFile(text("filename"),
                                ofstream::out | ofstream::app);
            TableKey datakey;
            datakey.addNumber("ERG", "mv", "%6.3f");
            datakey.addNumber("Trigger", "v", "%6.3f");
            datakey.saveKey(resultFile);
            for (int index = indexBefore; index < indexAfter; index++) {
                datakey.save(resultFile, trace("V-1").at(index), 0);
                datakey.save(resultFile, trace("StimulusTrigger").at(index), 1);
                resultFile << "\n";
            }
            resultFile.close();
        }
        return Completed;
    }


    addRePro(OffRamp, intererg);

}; /* namespace erg */

#include "moc_offramp.cc"
