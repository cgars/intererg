/*
  intererg/lightramp.cc
  Measured ERG amplitude for increasing light levels

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

#include <relacs/intererg/lightramp.h>
#include <relacs/plot.h>
#include <string>

using namespace relacs;

namespace intererg {


    LightRamp::LightRamp(void)
            : RePro("LightRamp", "erg", "Christian Garbers", "1.0",
                    "Jan 23, 2012") {
        addNumber("Repeats", "Nr of ERGs to measure", 10.0, 1.0, 1000.0, 1.0,
                  "times",
                  "times");
        addNumber("LED", "Number of the LED", 1.0, 1.0, 48.0, 1.0, "LEDNr.",
                  "LEDNr");
        addNumber("Current", "Current for the LED", 1.0, 1.0, 4095.0, 1.0,
                  "bit",
                  "bit");
        addNumber("OnDuration", "Duration of the LED on phase", 50.0, 1.0,
                  10000.0,
                  1.0, "ms", "ms");
        addNumber("OffDuration", "Duration of the LED off phase", 50.0, 1.0,
                  10000.0,
                  1.0, "ms", "ms");
        addNumber("StartPWM", "Staring Pulsewidth of the second LED", 0.0, 1.0,
                  4095.0, 1.0, "bit", "bit");
        addNumber("IncrPWM", "PWM Increment for the of the second LED", 400.0,
                  1.0, 4000.0, 1.0, "bit", "bit");
        addNumber("StopPWM", "Upper bund for Pulsewidth of the second LED",
                  10000.0, 1.0, 4095.0, 1.0, "bit", "bit");
        addNumber("InitialWait", "Time to wait before measurement", 4000.0, 1.0,
                  4095.0, 1.0, "ms", "ms");
        addNumber("InitPWM", "PWM while waiting", 4000.0, 1.0, 4095.0, 1.0,
                  "ms", "ms");
        addText("filename");
        addNumber("Before", "MS for the Baseline", 100.0, 1.0, 1000.0,
                  1.0, "ms", "ms");
        addNumber("After", "MS for the Signal", 100.0, 1.0, 1000.0,
                  1.0, "ms", "ms");

        plots.lock();
        plots.resize(2, 1, true);
        plots.unlock();
        plots[0].setTitle("ERG");
        plots[1].setXLabel("PWM [BIT]");
        plots[1].setYLabel("ERG Amplitude [mv]");
        plots[0].setXLabel("time [ms]");
        plots[0].setYLabel("ERG Amplitude [mv]");
        setWidget(&plots);
    }


    int LightRamp::main(void) {
        LEDArray *larray;
        larray = dynamic_cast< LEDArray * >( device("led-1"));
        const int repeats = int(number("Repeats"));
        const int led = int(number("LED"));
        const int current = int(number("Current"));
        const double on_duration = number("OnDuration");
        const double off_duration = number("OffDuration");
        const int start_pwm = int(number("StartPWM"));
        const int stop_pwm = int(number("StopPWM"));
        const int incr_pwm = int(number("IncrPWM"));
        const double initial_wait = number("InitialWait");
        const int initial_pwm = int(number("InitPWM"));
        const double before = number("Before");
        const double after = number("After");
        const InData &InTrace = trace("V-1");
        const EventData &trigger_begin = events("Trigger-1");
        const EventData &trigger_end = events("Trigger-2");

        larray->setOneLEDParameter(led, initial_pwm, current, on_duration,
                                   initial_wait);
        larray->start(1);
        sleep((initial_wait + on_duration) / 1000);

        plots.lock();
        plots[0].clear();
        plots.unlock();
        plots[0].clear();
        plots[1].clear();

        //Measurement  ---------------------------------------------------------
        int indexBefore, indexAfter;
        vector<double> erg_amplitudes;
        vector<double> pwms;
        ofstream resultFile(text("filename"), ofstream::out | ofstream::app);
        for (int pwm = start_pwm; pwm <= stop_pwm; pwm += incr_pwm) {
            double amplitude = 0.0;
            indexBefore = trace("V-1").currentIndex();
            for (int counter = 0; counter < repeats; counter++) {
                larray->setOneLEDParameter(led, pwm, current, on_duration,
                                           off_duration);
                larray->start(1);
                sleep((on_duration + off_duration) / 1000);
                amplitude += InTrace.mean(trigger_begin.back(),
                                          trigger_begin.back() +
                                          before / 1000.0) -
                             InTrace.mean(trigger_end.back() - after / 1000.0,
                                          trigger_end.back());
            }
            indexAfter = trace("V-1").currentIndex();
            erg_amplitudes.push_back(amplitude / repeats);
            pwms.push_back(pwm);
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

        // plot results --------------------------------------------------------
        Plot::PointStyle p_style(Plot::Circle, 10, Plot::Red);
        Plot::LineStyle l_style(Plot::Blue, 1);
        plots.lock();
        plots[1].plot(pwms, erg_amplitudes, l_style, p_style);
        plots[1].draw();
        plots.unlock();

        // save data -----------------------------------------------------------
        TableKey datakey;
        datakey.addNumber("PWM", "bit", "%6.3f");
        datakey.addNumber("ERG Amplitude", "mv", "%6.5f");
        datakey.saveKey(resultFile);
        vector<double>::iterator it2 = erg_amplitudes.begin();
        for (vector<double>::iterator it = pwms.begin();
             it != pwms.end(); ++it) {
            datakey.save(resultFile, *it, 0);
            datakey.save(resultFile, *it2, 1);
            ++it2;
            resultFile << "\n";
        }
        resultFile.close();
        return Completed;
    }


    addRePro(LightRamp, intererg);

}; /* namespace intererg */

#include "moc_lightramp.cc"
