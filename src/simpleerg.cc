/*
 erg/simpleerg.cc

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

#include <relacs/erg/simpleerg.h>
#include <relacs/tablekey.h>
#include <typeinfo>


using namespace relacs;

namespace erg {

    simpleErg::simpleErg(void) :
            RePro("simpleErg", "erg", "Christian Garbers", "0.1",
                  "Nov 30, 2011") {
        // add some options:
        addNumber("Repeats", "Nr of ERG to measure", 1.0, 1.0, 1000.0, 1.0,
                  "times", "times");
        addNumber("Duration", "Duration of each individual ERG", 10.0, 1.0,
                  100000.0,
                  1.0, "ms", "ms");
        addNumber("LED", "Number of the LED", 1.0, 1.0, 48.0, 1.0, "LEDNr.",
                  "LEDNr");
        addNumber("PWM", "Pulsewidth of the LED", 1.0, 1.0, 4095.0, 1.0, "bit",
                  "bit");

        addNumber("Current", "Current for the LED", 1.0, 1.0, 4095.0, 1.0,
                  "bit",
                  "bit");
        addText("Filename");
        P.lock();
        P.resize(2, 1, true);
        P.unlock();
        P[0].setTitle("ERG");
        P[1].setTitle("mean ERG");
        P[1].setXLabel("time [s]");
        setWidget(&P);
    }

    int simpleErg::main(void) {
        // get options -------------------------------------------------------------
        const int repeats = int(number("Repeats"));
        const int current = int(number("Current"));
        const int pwm = int(number("PWM"));
        const int led = int(number("LED"));
        const int duration = int(number("Duration"));
        P[0].clear();
        P[1].clear();
        tracePlotContinuous(10);

        // initialize hardware -----------------------------------------------------
        LEDArray *larray;
        larray = dynamic_cast<LEDArray *> (device("led-1"));
        printf("LEDArray open: %i\n", larray->isOpen());
        printf("LEDArray open: %i\n", larray->isOpen());
        larray->setLEDParameter(led, led, pwm, pwm, current, current,
                                duration, duration, duration, duration);
        larray->start(repeats / 2);

        const InData &InTrace = trace("V-1");
        const InData &TriggerTrace = trace("StimulusTrigger");
        const EventData &trigger_begin = events("Trigger-1");
        const EventData &trigger_end = events("Trigger-2");

        //do ergs here -------------------------------------------------------------
        const int startIndex = InTrace.currentIndex();
        int counter;
        larray->setLEDParameter(led, led, pwm, pwm, current, current,
                                duration, duration, duration, duration);
        larray->start(repeats / 2);
        sleep((duration * 2 * repeats) / 1000);
        const int endIndex = InTrace.currentIndex();

        //plot the individual ergs -------------------------------------------------
        vector<SampleDataF> ergVector;
        for (counter = repeats / 2 - 1; counter > -1; counter--) {
            SampleDataF data(0.0, duration, InTrace.stepsize());
            InTrace.copy(trigger_begin.back(counter), data);
            ergVector.push_back(data);
            P.lock();
            //P.clear();
            P[0].plot(data, 1.0, Plot::Orange, 2, Plot::Solid);
            P[0].draw();
            P.unlock();
        }

        //plot mean erg-------------------------------------------------------------
        SampleDataF meanerg(0.0, duration, InTrace.stepsize());
        average(meanerg, ergVector);
        P.lock();
        P[1].plot(meanerg, 1.0, Plot::Orange, 2, Plot::Solid);
        P[1].draw();
        P.unlock();

        //save data ----------------------------------------------------------------
        ofstream df(addPath(text("filename")).c_str(),
                    ofstream::out | ofstream::app);
        //ofstream df("/home/erg/test.dat", ofstream::out | ofstream::app);
        df << '\n';
        TableKey datakey;
        datakey.addNumber("ERG", "mv", "%6.3f");
        datakey.addNumber("Trigger", "v", "%6.3f");
        datakey.saveKey(df);
        for (int i = startIndex; i < endIndex; i++) {
            datakey.save(df, InTrace.at(i), 0);
            datakey.save(df, TriggerTrace.at(i), 1);
            df << '\n';
        }
        df << "\n";
        return Completed;
    }

    addRePro(simpleErg, erg);

}; /* namespace erg */

#include "moc_simpleerg.cc"
