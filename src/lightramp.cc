/*
  erg/lightramp.cc
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
        addText("filename.dat");
        addNumber("Before", "MS for the Baseline", 100.0, 1.0, 1000.0,
                  1.0, "ms", "ms");
        addNumber("After", "MS for the Signal", 100.0, 1.0, 1000.0,
                  1.0, "ms", "ms");

        P.lock();
        P.resize(2, 1, true);
        P.unlock();
        P[0].setTitle("ERG");
        P[1].setXLabel("PWM [BIT]");
        P[1].setYLabel("ERG Amplitude [mv]");
        P[0].setXLabel("time [ms]");
        P[0].setYLabel("ERG Amplitude [mv]");
        setWidget(&P);
    }


    int LightRamp::main(void) {
        LEDArray *larray;
        larray = dynamic_cast< LEDArray * >( device("led-1"));
        int repeats = int(number("Repeats"));
        int led = int(number("LED"));
        int current = int(number("Current"));
        double on_duration = number("OnDuration");
        double off_duration = number("OffDuration");
        int start_pwm = int(number("StartPWM"));
        int stop_pwm = int(number("StopPWM"));
        int incr_pwm = int(number("IncrPWM"));
        double initial_wait = number("InitialWait");
        int initial_pwm = int(number("InitPWM"));
        double before = number("Before");
        double after = number("After");
        const InData &InTrace = trace("V-1");
        const EventData &trigger_begin = events("Trigger-1");
        const EventData &trigger_end = events("Trigger-2");


        char command[100];
        sprintf(command, "%.4i%.4i%.2i", initial_pwm, 4000, led);
        larray->sendCommand(command);
        sleep(initial_wait / 1000);
        larray->sendCommand("0000000000");

        P.lock();
        P[0].clear();
        P.unlock();
        P[0].clear();
        P[1].clear();
        //Measurement  ---------------------------------------------------------------
        int pwm;
        for (pwm = start_pwm; pwm <= stop_pwm; pwm += incr_pwm) {
            double amplitude = 0.0;
            int index1 = trace("V-1").currentIndex();
            for (int counter = 0; counter < repeats; counter++) {
                char command[100];
                sprintf(command, "%.4i%.4i%.2i", pwm, current, led);
                larray->sendCommand(command);
                sleep(on_duration / 1000);
                larray->sendCommand("0000000000");
                sleep(off_duration / 1000);
                amplitude += InTrace.mean(trigger_begin.back(),
                                          trigger_begin.back() +
                                          before / 1000.0) -
                             InTrace.mean(trigger_end.back() - after / 1000.0,
                                          trigger_end.back());
                //plotting ---------------------------------------------------------
//			double current_time = trace("V-1").currentTime() ;
//			SampleDataF data(0.0, current_time - trigger.back(),
//							trace("V-1").stepsize());
//			trace("V-1").copy(trigger.back(), data);
//		    P.lock();
//			P[0].plot(data, 1.0, Plot::Orange, 2, Plot::Solid);
//			P[0].draw() ;
//			P.unlock() ;
            }
            int index2 = trace("V-1").currentIndex();
            erg_amplitudes.push_back(amplitude / repeats);
            pwms.push_back(pwm);
            cout << "pwm:" << pwm << endl;
            cout << "amplitude:" << amplitude / repeats << endl << endl;
            string basename = text("filename.dat");
            char filename[100];
            sprintf(filename, "pwm%i%s", pwm, basename.c_str());
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
        }
        larray->sendCommand("0000000000");

        // plot results --------------------------------------------------------------
        Plot::PointStyle p_style(Plot::Circle, 10, Plot::Red);
        Plot::LineStyle l_style(Plot::Blue, 1);
        P.lock();
        P[1].plot(pwms, erg_amplitudes, l_style, p_style);
        P[1].draw();
        P.unlock();

        // save data -----------------------------------------------------------------
        ofstream df(text("filename.dat").c_str(),
                    ofstream::out | ofstream::app);
        TableKey datakey;
        datakey.addNumber("PWM", "bit", "%6.3f");
        datakey.addNumber("ERG Amplitude", "mv", "%6.5f");
        datakey.saveKey(df);
        vector<double>::iterator it2 = erg_amplitudes.begin();
        for (vector<double>::iterator it = pwms.begin();
             it != pwms.end(); ++it) {
            datakey.save(df, *it, 0);
            datakey.save(df, *it2, 1);
            ++it2;
            df << "\n";
        }
        return Completed;
    }


    addRePro(LightRamp, intererg);

}; /* namespace intererg */

#include "moc_lightramp.cc"
