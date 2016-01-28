/*
  erg/patch2led.h
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
#include <relacs/erg/ledarray.h>
#include <relacs/multiplot.h>
#include <relacs/tablekey.h>

using namespace relacs;

namespace erg {


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
         * recursivly determine and return the PWM in range 0-4000 that results in an
         * an minimal erg amplitude deffirence between the Amplitude measured at pwm
         * and desired
        */

        int get_pwm_dynamic(int pwm_m_1,
                            std::pair<double, double> potential_m_1,
                            int pwm_m_2, std::pair<double, double> potential_m2,
                            LEDArray *larray,
                            const InData &InTrace,
                            const EventData &trigger_begin,
                            const EventData &trigger_end);

        int get_pwm_dynamic_var(int pwm_m_1, double variance_m_1,
                                int pwm_m_2, double variance_m_2,
                                LEDArray *larray,
                                const InData &InTrace,
                                const EventData &trigger_begin,
                                const EventData &trigger_end,
                                double base_variance);

        std::pair<double, double> get_erg_diff(int pwm2, LEDArray *larray,
                                               const InData &InTrace,
                                               const EventData &trigger_begin,
                                               const EventData &trigger_end);

        double get_erg_var(int pwm2, LEDArray *larray, const InData &InTrace,
                           const EventData &trigger_begin,
                           const EventData &trigger_end);

        int get_intercept(int pwm_m_1, double potential_m_1,
                          int pwm_m_2, double potential_m_2);


        vector<double> erg_amplitudes;
        vector<double> pwms;
        vector<double> amp_desired;
        vector<double> amp_measured;
        int default_guess;
    protected:
        MultiPlot P;
    };


}; /* namespace erg */

#endif /* ! _RELACS_ERG_PATCH2LED_H_ */
