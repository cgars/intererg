/*
  intererg/patch2led.h
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

#ifndef _RELACS_ERG_PATCH2LED_H_
#define _RELACS_ERG_PATCH2LED_H_ 1

#include <relacs/repro.h>
#include <relacs/intererg/ledarray.h>
#include <relacs/multiplot.h>
#include <relacs/tablekey.h>

using namespace relacs;

namespace intererg {


/*!
\class Patch2LED
\brief [RePro] Clamps the erg amplitude by adjusting one led
\author Christian Garbersdouble
\version 1.0 (Jan 19, 2012)
*/


    class Patch2LED : public RePro {
        Q_OBJECT

    public:

        Patch2LED(void);

        virtual int main(void);

        /*
         * Dynamically change the pwm1 value such that arg signal difference
         * defined in the getSignal member is minimized.
         *
         * pwm1,pwm2: the two last pwms tested
         * potential1,potential2: there respective erg amplitudes
         * InTrace: The erg trace
         * triggerBegin,triggerEnd: Traces of the triggers
         * *larray: the Led Array
         * getSignal: Member of Patch2Led used to define the signal and convergence
         * criteria (either &getErgDiff, or &getErgVar )
         *
         */
        int getPwmDynamic(int pwm1,
                          std::pair<double, bool> &potential1,
                          int pwm2, std::pair<double, bool> &potential2,
                          LEDArray *larray,
                          const InData &InTrace,
                          const EventData &triggerBegin,
                          const EventData &triggerEnd,
                          const double baseVariance,
                          std::pair<double, bool> (Patch2LED::*getSignal)
                                  (int, LEDArray *, const InData &,
                                   const EventData &, const EventData &,
                                   const double &));

        /*
         * Measure the difference in Erg amplitude for the value pwm2 and
         * the pwm reference  value defined in the RePro Gui. The erg amplitude
         * is taken as the difference between the mean value in a time interval
         * (defined in the gui) before the on_triggers and the mean value in an
         * interval after the end off the on triggers
         * Return a pair of (difference,  should stop)
         */
        std::pair<double, bool> getErgDiff(int pwm2, LEDArray *larray,
                                           const InData &InTrace,
                                           const EventData &triggerBegin,
                                           const EventData &triggerEnd,
                                           const double &baseVariance);

        /*
         * Measure the difference in Erg amplitude for the value pwm2 and
         * the pwm reference  value defined in the RePro Gui. The erg amplitude
         * is taken as the variance of the signal during the measurement
         */
        std::pair<double, bool> getErgVar(int pwm2, LEDArray *larray,
                                          const InData &InTrace,
                                          const EventData &trigger_begin,
                                          const EventData &trigger_end,
                                          const double &baseVariance);

        /*
         * Estimate the pwm value for which the potential (erg signals) would be
         * equal assuming linear interpolation
         */
        int getEqualPotentialPwm(int pwm1, double potential1,
                                 int pwm2, double potential2);


        vector<double> ergAmplitudes;
        vector<double> pwms;
        vector<double> ampDesired;
        vector<double> ampMeasured;
        int defaultGuess;

    protected:
        MultiPlot plots;
    };


}; /* namespace intererg */

#endif /* ! _RELACS_ERG_PATCH2LED_H_ */
