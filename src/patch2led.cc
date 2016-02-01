/*
  erg/patch2led.cc
  Clamps the erg amplitude by adjusting one led

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

#include <relacs/intererg/patch2led.h>
#include <relacs/plot.h>

using namespace relacs;

namespace intererg {


    Patch2LED::Patch2LED(void)
            : RePro("Patch2LED", "erg", "Christian Garbers", "1.0",
                    "Jan 19, 2012") {
        addNumber("Repeats", "Nr of ERG to measure", 5.0, 1.0, 1000.0, 1.0,
                  "times",
                  "times");
        addNumber("LED1", "Number of the first LED", 1.0, 1.0, 48.0, 1.0,
                  "LEDNr.",
                  "LEDNr");
        addNumber("PWM1", "Pulsewidth of the first LED", 1000.0, 1.0, 4095.0,
                  1.0, "bit", "bit");
        addNumber("Current1", "Current for the first LED", 4095.0, 1.0, 4095.0,
                  1.0, "bit",
                  "bit");
        addNumber("LED2", "Number of the second LED", 1.0, 1.0, 48.0, 1.0,
                  "LEDNr.",
                  "LEDNr");
        addNumber("Current2", "Current for the second LED", 4095.0, 1.0, 4095.0,
                  1.0, "bit",
                  "bit");
        addNumber("OnDuration", "Duration of the on phase", 500.0, 1.0, 10000.0,
                  1.0, "ms", "ms");
        addNumber("OffDuration", "Duration of the off phase", 500.0, 1.0,
                  10000.0,
                  1.0, "ms", "ms");
        addNumber("Before", "MS for the Baseline", 100.0, 1.0, 1000.0,
                  1.0, "ms", "ms");
        addNumber("After", "MS for the Signal", 100.0, 1.0, 1000.0,
                  1.0, "ms", "ms");
        addNumber("EThreshold", "Threshold for convergence", 0.1, 0.001, 10.0,
                  0.001, "ms", "ms");
        addNumber("EThreshold2", "Threshold for convergence2", 0.1, 0.001, 10.0,
                  0.001, "ms", "ms");
        addSelection("SigType", "Signal Type ",
                     "diff|var");
        addText("filename");

        plots.lock();
        plots.resize(2, 1, true);
        plots.unlock();
        plots[0].setTitle("ERG");
        plots[0].setXLabel("PWM [BIT]");
        plots[0].setYLabel("ERG Amplitude [mv]");
        setWidget(&plots);
    }


    int Patch2LED::main(void) {
        LEDArray *larray;
        larray = dynamic_cast< LEDArray * >( device("led-1"));
        int result = 0;
        const InData &InTrace = trace("V-1");
        const EventData &triggerBegin = events("Trigger-1");
        const EventData &triggerEnd = events("Trigger-2");
        const int led1 = int(number("LED1"));
        int pwm1 = int(number("PWM1"));
        const double onDuration = number("OnDuration");
        const double offDuration = number("OffDuration");
        defaultGuess = 10;
        plots[0].clear();
        plots[1].clear();
        tracePlotContinuous(10);

        if (index("SigType") == 0) {
            std::pair<double, bool> potentialM1 = getErgDiff(4000, larray,
                                                             InTrace,
                                                             triggerBegin,
                                                             triggerEnd, 0);
            std::pair<double, bool> potentialM2 = getErgDiff(1, larray,
                                                             InTrace,
                                                             triggerBegin,
                                                             triggerEnd, 0);
            result = getPwmDynamic(4000, potentialM1, 1., potentialM2,
                                   larray, InTrace, triggerBegin,
                                   triggerEnd, 0, &Patch2LED::getErgDiff);
        }
        if (index("SigType") == 1) {
            const int begin = InTrace.currentIndex();
            sleep((onDuration + offDuration) / 1000.0);
            const int end = InTrace.currentIndex();
            double baseVariance = InTrace.variance(begin, end);
            std::pair<double, bool> varianceM1 = getErgVar(4000, larray,
                                                           InTrace,
                                                           triggerBegin,
                                                           triggerEnd,
                                                           baseVariance);
            std::pair<double, bool> varianceM2 = getErgVar(1, larray, InTrace,
                                                           triggerBegin,
                                                           triggerEnd,
                                                           baseVariance);
            result = getPwmDynamic(4000, varianceM1, 1, varianceM2,
                                   larray, InTrace, triggerBegin,
                                   triggerEnd,
                                   baseVariance, &Patch2LED::getErgVar);
        }

        // plot results --------------------------------------------------------------
        Plot::PointStyle p_style(Plot::Circle, 10, Plot::Red);
        Plot::LineStyle l_style(Plot::Transparent, 1);
        plots.lock();
        plots[0].plot(pwms, ergAmplitudes, l_style, p_style);
        plots[0].draw();
        plots.unlock();

        // saving pwm table ----------------------------------------------------
        char resultFileName[100];
        sprintf(resultFileName, "pwm_steps_%s", text("filename").c_str());
        ofstream resultFile(addPath(resultFileName).c_str(),
                    ofstream::out | ofstream::app);
        TableKey datakey;
        datakey.addNumber("PWM", "bit", "%6.3f");
        datakey.addNumber("ERG Amplitude", "mv", "%6.3f");
        datakey.addNumber("Reference ERG Amplitude", "mv", "%6.3f");
        datakey.addNumber("Measured ERG Amplitude", "mv", "%6.3f");
        datakey.saveKey(resultFile);
        vector<double>::iterator it2 = ergAmplitudes.begin();
        vector<double>::iterator it3 = ampDesired.begin();
        vector<double>::iterator it4 = ampMeasured.begin();
        for (vector<double>::iterator it = pwms.begin();
             it != pwms.end(); ++it) {
            datakey.save(resultFile, *it, 0);
            datakey.save(resultFile, *it2, 1);
            datakey.save(resultFile, *it3, 2);
            datakey.save(resultFile, *it4, 3);
            ++it2;
            ++it3;
            ++it4;
            resultFile << "\n";
        }
        resultFile << endl;
        resultFile.close();
        ergAmplitudes.clear();
        ampDesired.clear();
        ampMeasured.clear();
        pwms.clear();

        // saving result Summary -----------------------------------------------
        ofstream resultSummaryFile(addPath("results").c_str(),
                     ofstream::out | ofstream::app);
        TableKey datakey2;
        datakey2.addNumber("Ref PWM", "bit", "%6.3f");
        datakey2.addNumber("Ref LED", "number", "%6.3f");
        datakey2.addNumber("Result", "pwm", "%6.3f");
        datakey2.saveKey(resultSummaryFile);
        datakey2.save(resultSummaryFile, pwm1, 0);
        datakey2.save(resultSummaryFile, led1, 1);
        datakey2.save(resultSummaryFile, result, 2);
        resultSummaryFile << endl;
        resultSummaryFile.close();

        return Completed;
    }

    int Patch2LED::getPwmDynamic(int pwm1,
                                 std::pair<double, bool> &potential1,
                                 int pwm2,
                                 std::pair<double, bool> &potential2,
                                 LEDArray *larray,
                                 const InData &InTrace,
                                 const EventData &triggerBegin,
                                 const EventData &triggerEnd,
                                 const double baseVariance,
                                 std::pair<double, bool> (Patch2LED::*getSignal)
                                         (int, LEDArray *, const InData &,
                                          const EventData &, const EventData &,
                                          const double &)) {
        if (potential1.second) {
            return int(round((potential1.first + potential2.first) / 2.));
        }
        else {
            int pwmGuess = getEqualPotentialPwm(pwm1, potential1.first, pwm2,
                                                potential2.first);
            vector<double> tmpPwm;
            tmpPwm.push_back(pwm1);
            tmpPwm.push_back(pwm2);
            vector<double> tmpPot;
            tmpPot.push_back(potential1.first);
            tmpPot.push_back(potential2.first);
            Plot::PointStyle tmpPStyle(Plot::Circle, 10, Plot::Red);
            Plot::LineStyle tmpLStyle(Plot::Blue, 1);
            plots[1].clear();
            plots.lock();
            plots[1].plot(tmpPwm, tmpPot, tmpLStyle, tmpPStyle);
            plots[1].draw();
            plots.unlock();
            tmpPwm.clear();
            tmpPot.clear();

            if (pwmGuess <
                1) {//in guess the estimate is below 1 we need to intercept
                pwmGuess = defaultGuess;
                defaultGuess++;
            }
            if (pwmGuess > 4095) {
                pwmGuess = 4010 - defaultGuess;
                defaultGuess++;
            }
            std::pair<double, bool> guessPotential = (this->*getSignal)(
                    pwmGuess, larray, InTrace, triggerBegin,
                    triggerEnd, 0);
            return getPwmDynamic(pwmGuess, guessPotential, pwm1, potential1,
                                 larray, InTrace, triggerBegin, triggerEnd,
                                 baseVariance, getSignal);
        }
    }

    std::pair<double, bool> Patch2LED::getErgDiff(int pwm2,
                                                  LEDArray *larray,
                                                  const InData &InTrace,
                                                  const EventData &triggerBegin,
                                                  const EventData &triggerEnd,
                                                  const double &baseVariance) {
        const int led1 = int(number("LED1"));
        const int pwm1 = int(number("PWM1"));
        const int current1 = int(number("Current1"));
        const int led2 = int(number("LED2"));
        const int current2 = int(number("Current2"));
        const int repeats = int(number("Repeats"));
        const double onDuration = number("OnDuration");
        const double offDuration = number("OffDuration");
        const double before = number("Before");
        const double after = number("After");

        double desired = 0;
        double current_signal = 0;
        // start the flickering
        larray->setLEDParameter(led1, led2, pwm1, pwm2, current1, current2,
                                onDuration, onDuration, offDuration,
                                offDuration);
        larray->start(repeats + 1);
        sleep((onDuration + offDuration) * repeats);
        for (int counter = 0; counter < repeats; counter++) {
            desired += InTrace.mean(
                    triggerBegin.back(2 * counter) - before / 1000.0,
                    triggerBegin.back(2 * counter)) -
                       InTrace.mean(triggerBegin.back(2 * counter) +
                                    onDuration / 1000.0 - after / 1000.0,
                                    triggerBegin.back(2 * counter) +
                                    onDuration / 1000.0);
            current_signal += InTrace.mean(
                    triggerBegin.back(2 * counter + 1) - before / 1000.0,
                    triggerBegin.back(2 * counter + 1)) -
                              InTrace.mean(triggerBegin.back(2 * counter + 1) +
                                           onDuration / 1000.0 -
                                           after / 1000.0,
                                           triggerBegin.back(2 * counter + 1) +
                                           onDuration / 1000.0);
        }
        const double result = (current_signal - desired) / repeats;
        const double lastResult = ergAmplitudes[ergAmplitudes.size() - 1];
        ergAmplitudes.push_back(result);
        pwms.push_back(pwm2);
        ampDesired.push_back(desired);
        ampMeasured.push_back(current_signal);
        bool done = false;
        if (abs(result - lastResult) <
            number("EThreshold") * desired / repeats) {
            if (abs(result) <
                number("EThreshold2") *
                abs(ampDesired[ampDesired.size() - 2])) {
                done = true;
            }
        }
        if (pwm1 == pwm2) {
            done = true;
        }
        return std::make_pair(result, done);
    }

    std::pair<double, bool> Patch2LED::getErgVar(const int pwm2,
                                                 LEDArray *larray,
                                                 const InData &InTrace,
                                                 const EventData &trigger_begin,
                                                 const EventData &trigger_end,
                                                 const double &baseVariance) {
        const int led1 = int(number("LED1"));
        const int pwm1 = int(number("PWM1"));
        const int current1 = int(number("Current1"));
        const int led2 = int(number("LED2"));
        const int current2 = int(number("Current2"));
        const int repeats = int(number("Repeats"));
        const double onDuration = number("OnDuration");
        const double offDuration = number("OffDuration");

        // start the flickering
        larray->setLEDParameter(led1, led2, pwm1, pwm2, current1, current2,
                                onDuration, onDuration, offDuration,
                                offDuration);
        larray->start(repeats + 1);
        sleep((onDuration + offDuration) * repeats);
        double result = InTrace.variance(trigger_begin.back(repeats - 1),
                                         trigger_end.back());
        const double oldResult = ergAmplitudes[ergAmplitudes.size() - 1];
        ergAmplitudes.push_back(result);
        pwms.push_back(pwm2);
        ampDesired.push_back(result);
        ampMeasured.push_back(result);
        bool done = false;
        if ((result - oldResult) <
            number("EThreshold") * baseVariance) {
            if (abs(result) < number("EThreshold2") * baseVariance) {
                done = true;
            }
        }
        if (pwm1 == pwm2) {
            done = true;
        }
        return std::make_pair(result, done);
    }

    int Patch2LED::getEqualPotentialPwm(const int pwm1, const double potential1,
                                        const int pwm2,
                                        const double potential2) {
        return int(round(pwm1 -
                         (((pwm1 - pwm2) / (potential1 - potential2)) *
                          potential1)));
    }

    addRePro(Patch2LED, intererg);
};
/* namespace intererg */

#include "moc_patch2led.cc"
