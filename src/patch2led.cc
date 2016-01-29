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
// add some options:
//  addNumber( "duration", "Stimulus duration", 1.0, 0.001, 100000.0, 0.001, "s", "ms" );
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
        addText("filename.dat");

        P.lock();
        P.resize(2, 1, true);
        P.unlock();
        P[0].setTitle("ERG");
        P[0].setXLabel("PWM [BIT]");
        P[0].setYLabel("ERG Amplitude [mv]");
        setWidget(&P);
    }


    int Patch2LED::main(void) {
        LEDArray *larray;
        larray = dynamic_cast< LEDArray * >( device("led-1"));
        int result = 0;
        const InData &InTrace = trace("V-1");
        const EventData &trigger_begin = events("Trigger-1");
        const EventData &trigger_end = events("Trigger-2");
        int led1 = int(number("LED1"));
        int pwm1 = int(number("PWM1"));
        double on_duration = number("OnDuration");
        double off_duration = number("OffDuration");
        default_guess = 10;
        P[0].clear();
        P[1].clear();
        tracePlotContinuous(10);

        if (index("SigType") == 0) {
            std::pair<double, double> potential_m_1 = get_erg_diff(4000, larray,
                                                                   InTrace,
                                                                   trigger_begin,
                                                                   trigger_end);
            std::pair<double, double> potential_m_2 = get_erg_diff(1, larray,
                                                                   InTrace,
                                                                   trigger_begin,
                                                                   trigger_end);
            result = get_pwm_dynamic(4000, potential_m_1, 1, potential_m_2,
                                     larray, InTrace, trigger_begin,
                                     trigger_end);
        }
        if (index("SigType") == 1) {
            int begin = InTrace.currentIndex();
            sleep((on_duration + off_duration) / 1000.0);
            int end = InTrace.currentIndex();
            double base_variance = InTrace.variance(begin, end);
            double variance_m_1 = get_erg_var(4000, larray, InTrace,
                                              trigger_begin, trigger_end);
            double variance_m_2 = get_erg_var(1, larray, InTrace,
                                              trigger_begin, trigger_end);
            result = get_pwm_dynamic_var(4000, variance_m_1, 1, variance_m_2,
                                         larray, InTrace, trigger_begin,
                                         trigger_end,
                                         base_variance);
        }
        cout << "i know its is:" << result << endl;

        // plot results --------------------------------------------------------------
        Plot::PointStyle p_style(Plot::Circle, 10, Plot::Red);
        Plot::LineStyle l_style(Plot::Transparent, 1);
        P.lock();
        P[0].plot(pwms, erg_amplitudes, l_style, p_style);
        P[0].draw();
        P.unlock();

        // saving pwm table ----------------------------------------------------------
        char result_file_name[100];
        sprintf(result_file_name, "pwm_steps_%s", text("filename.dat").c_str());
        ofstream df(addPath(result_file_name).c_str(),
                    ofstream::out | ofstream::app);
        TableKey datakey;
        datakey.addNumber("PWM", "bit", "%6.3f");
        datakey.addNumber("ERG Amplitude", "mv", "%6.3f");
        datakey.addNumber("Reference ERG Amplitude", "mv", "%6.3f");
        datakey.addNumber("Measured ERG Amplitude", "mv", "%6.3f");
        datakey.saveKey(df);
        vector<double>::iterator it2 = erg_amplitudes.begin();
        vector<double>::iterator it3 = amp_desired.begin();
        vector<double>::iterator it4 = amp_measured.begin();
        for (vector<double>::iterator it = pwms.begin();
             it != pwms.end(); ++it) {
            datakey.save(df, *it, 0);
            datakey.save(df, *it2, 1);
            datakey.save(df, *it3, 2);
            datakey.save(df, *it4, 3);
            ++it2;
            ++it3;
            ++it4;
            df << "\n";
        }
        df << endl;
        df.close();
        erg_amplitudes.clear();
        amp_desired.clear();
        amp_measured.clear();
        pwms.clear();


//	// saving raw ERGS -----------------------------------------------------------
//	char erg_file_name [100] ;
//	sprintf(erg_file_name, "ergs_%s", text("filename.dat").c_str()) ;
//  ofstream df1(addPath(erg_file_name).c_str(), ofstream::out | ofstream::app);
//	TableKey datakey1;
//	datakey1.addNumber("ERG", "mv", "%6.3f");
//	datakey1.addNumber("Stimulus", "v", "%6.3f");
//	datakey1.saveKey(df1);
//	for (int i=index_begin;i<index_end;i++)
//	{
//		datakey1.save(df1, InTrace.at(i), 0);
//		datakey1.save(df1, TriggerTrace.at(i), 1);
//		df1<<endl;
//	}
//	df1<<endl ;
//	df1.close() ;

        // saving result -------------------------------------------------------------
        ofstream df2(addPath("results.dat").c_str(),
                     ofstream::out | ofstream::app);
        TableKey datakey2;
        datakey2.addNumber("Ref PWM", "bit", "%6.3f");
        datakey2.addNumber("Ref LED", "number", "%6.3f");
        datakey2.addNumber("Result", "pwm", "%6.3f");
        datakey2.saveKey(df2);
        datakey2.save(df2, pwm1, 0);
        datakey2.save(df2, led1, 1);
        datakey2.save(df2, result, 2);
        df2 << endl;
        df2.close();

        return Completed;
    }

    int Patch2LED::get_pwm_dynamic(int pwm_m_1,
                                   std::pair<double, double> potential_m_1,
                                   int pwm_m_2,
                                   std::pair<double, double> potential_m_2,
                                   LEDArray *larray,
                                   const InData &InTrace,
                                   const EventData &trigger_begin,
                                   const EventData &trigger_end) {
        //cout<<"comparing:"<<abs(potential_m_1)<<"and "<<abs(potential_m_2) <<endl;
        if (abs(potential_m_1.first - potential_m_2.first) <
            number("EThreshold") * abs(potential_m_1.second)) {
            if (abs(potential_m_1.first) <
                number("EThreshold2") * abs(potential_m_1.second)) {
                return (pwm_m_1 + pwm_m_2) / 2;
            }
        }
        if (pwm_m_1 == pwm_m_2) {
            return pwm_m_1;
        }
        else {
            int guess = get_intercept(pwm_m_1, potential_m_1.first, pwm_m_2,
                                      potential_m_2.first);
            vector<double> tmp_pwm;
            tmp_pwm.push_back(pwm_m_1);
            tmp_pwm.push_back(pwm_m_2);
            vector<double> tmp_pot;
            tmp_pot.push_back(potential_m_1.first);
            tmp_pot.push_back(potential_m_2.first);
            Plot::PointStyle tmpp_style(Plot::Circle, 10, Plot::Red);
            Plot::LineStyle tmpl_style(Plot::Blue, 1);
            P[1].clear();
            P.lock();
            P[1].plot(tmp_pwm, tmp_pot, tmpl_style, tmpp_style);
            P[1].draw();
            P.unlock();
            tmp_pwm.clear();
            tmp_pot.clear();

            if (guess < 1) {
                guess = default_guess;
                default_guess++;
            }
            if (guess > 4095) {
                guess = 4010 - default_guess;
                default_guess++;
            }
            cout << "i am guessing its:" << guess << endl << endl;
            std::pair<double, double> guess_potential = get_erg_diff(guess,
                                                                     larray,
                                                                     InTrace,
                                                                     trigger_begin,
                                                                     trigger_end);
            return get_pwm_dynamic(guess, guess_potential, pwm_m_1,
                                   potential_m_1,
                                   larray, InTrace,
                                   trigger_begin, trigger_end);
        }
    }

    int Patch2LED::get_pwm_dynamic_var(int pwm_m_1, double variance_m_1,
                                       int pwm_m_2, double variance_m_2,
                                       LEDArray *larray,
                                       const InData &InTrace,
                                       const EventData &trigger_begin,
                                       const EventData &trigger_end,
                                       double base_variance) {
        //cout<<"comparing:"<<abs(potential_m_1)<<"and "<<abs(potential_m_2) <<endl;
        if ((variance_m_1 - variance_m_2) <
            number("EThreshold") * base_variance) {
            if (abs(variance_m_1) < number("EThreshold2") * base_variance) {
                return (pwm_m_1 + pwm_m_2) / 2;
            }
        }
        if (pwm_m_1 == pwm_m_2) {
            return pwm_m_1;
        }
        else {
            int guess = get_intercept(pwm_m_1, variance_m_1, pwm_m_2,
                                      variance_m_2);
            vector<double> tmp_pwm;
            tmp_pwm.push_back(pwm_m_1);
            tmp_pwm.push_back(pwm_m_2);
            vector<double> tmp_pot;
            tmp_pot.push_back(variance_m_1);
            tmp_pot.push_back(variance_m_2);
            Plot::PointStyle tmpp_style(Plot::Circle, 10, Plot::Red);
            Plot::LineStyle tmpl_style(Plot::Blue, 1);
            P[1].clear();
            P.lock();
            P[1].plot(tmp_pwm, tmp_pot, tmpl_style, tmpp_style);
            P[1].draw();
            P.unlock();
            tmp_pwm.clear();
            tmp_pot.clear();

            if (guess < 1) {
                guess = default_guess;
                default_guess++;
            }
            if (guess > 4095) {
                guess = 4010 - default_guess;
                default_guess++;
            }
            cout << "i am guessing its:" << guess << endl << endl;
            double guess_variance = get_erg_var(guess, larray, InTrace,
                                                trigger_begin, trigger_end);
            return get_pwm_dynamic_var(guess, guess_variance, pwm_m_1,
                                       variance_m_1,
                                       larray, InTrace, trigger_begin,
                                       trigger_end,
                                       base_variance);
        }
    }

//double Patch2LED::get_erg_diff(int pwm2, LEDArray *larray, const InData &InTrace,
//															 const EventData &trigger_begin,
//															 const EventData &trigger_end)
//{
//	int led1 = int(number("LED1")) ;
//	int pwm1 = int(number("PWM1")) ;
//	int current1 = int(number("Current1")) ;
//	int led2 = int(number("LED2")) ;
//	int current2 = int(number("Current2"));
//	int repeats = int(number( "Repeats" )) ;
//	double on_duration = number("OnDuration") ;
//	double off_duration = number("OffDuration") ;
//	double before = number("Before") ;
//	double after = number("After") ;
//
//	double desired = 0;
//	double current_signal = 0;
//	// start the flickering
//	larray->setLEDParameter(led1,led2,pwm1,pwm2,current1,current2,
//	on_duration, on_duration, off_duration, off_duration);
//	larray->start(repeats+1);
//	for(int counter=0; counter < repeats; counter++)
//	{
//		desired += InTrace.mean(trigger_begin.back(2*counter)- before/1000.0, trigger_begin.back(2*counter)) -
//				InTrace.mean(trigger_begin.back(2*counter)+on_duration/1000.0-after/1000.0
//				, trigger_begin.back(2*counter)+on_duration/1000.0) ;
//		current_signal += InTrace.mean(trigger_begin.back(2*counter+1)- before/1000.0, trigger_begin.back(2*counter+1)) -
//				InTrace.mean(trigger_begin.back(2*counter+1)+on_duration/1000.0-after/1000.0
//				, trigger_begin.back(2*counter+1)+on_duration/1000.0) ;
//	}
//	double result = (current_signal - desired) / repeats  ;
//	erg_amplitudes.push_back(result) ;
//	pwms.push_back(pwm2) ;
//	amp_desired.push_back(desired) ;
//	amp_measured.push_back(current_signal) ;
//	cout << "pwm:"<<pwm2<<endl;
//	cout << "desired:"<<desired<<endl;
//	cout << "current:"<<current_signal<<endl;
//	cout << "diff:"<<result<<endl;
//	return  result ;
//}

    std::pair<double, double> Patch2LED::get_erg_diff(int pwm2,
                                                      LEDArray *larray,
                                                      const InData &InTrace,
                                                      const EventData &trigger_begin,
                                                      const EventData &trigger_end) {
        int led1 = int(number("LED1"));
        int pwm1 = int(number("PWM1"));
        int current1 = int(number("Current1"));
        int led2 = int(number("LED2"));
        int current2 = int(number("Current2"));
        int repeats = int(number("Repeats"));
        double on_duration = number("OnDuration");
        double off_duration = number("OffDuration");
        double before = number("Before");
        double after = number("After");

        double desired = 0;
        double current_signal = 0;
        // start the flickering
        larray->setLEDParameter(led1, led2, pwm1, pwm2, current1, current2,
                                on_duration, on_duration, off_duration,
                                off_duration);
        larray->start(repeats + 1);
        for (int counter = 0; counter < repeats; counter++) {
            desired += InTrace.mean(
                    trigger_begin.back(2 * counter) - before / 1000.0,
                    trigger_begin.back(2 * counter)) -
                       InTrace.mean(trigger_begin.back(2 * counter) +
                                    on_duration / 1000.0 - after / 1000.0,
                                    trigger_begin.back(2 * counter) +
                                    on_duration / 1000.0);
            current_signal += InTrace.mean(
                    trigger_begin.back(2 * counter + 1) - before / 1000.0,
                    trigger_begin.back(2 * counter + 1)) -
                              InTrace.mean(trigger_begin.back(2 * counter + 1) +
                                           on_duration / 1000.0 -
                                           after / 1000.0,
                                           trigger_begin.back(2 * counter + 1) +
                                           on_duration / 1000.0);
        }
        double result = (current_signal - desired) / repeats;
        erg_amplitudes.push_back(result);
        pwms.push_back(pwm2);
        amp_desired.push_back(desired);
        amp_measured.push_back(current_signal);
        cout << "pwm:" << pwm2 << endl;
        cout << "desired:" << desired << endl;
        cout << "current:" << current_signal << endl;
        cout << "diff:" << result << endl;
        return std::make_pair(result, desired / repeats);
    }

    double Patch2LED::get_erg_var(int pwm2, LEDArray *larray,
                                  const InData &InTrace,
                                  const EventData &trigger_begin,
                                  const EventData &trigger_end) {
        int led1 = int(number("LED1"));
        int pwm1 = int(number("PWM1"));
        int current1 = int(number("Current1"));
        int led2 = int(number("LED2"));
        int current2 = int(number("Current2"));
        int repeats = int(number("Repeats"));
        double on_duration = number("OnDuration");
        double off_duration = number("OffDuration");

        // start the flickering
        larray->setLEDParameter(led1, led2, pwm1, pwm2, current1, current2,
                                on_duration, on_duration, off_duration,
                                off_duration);
        larray->start(repeats + 1);
        double result = InTrace.variance(trigger_begin.back(repeats - 1),
                                         trigger_end.back());
        erg_amplitudes.push_back(result);
        pwms.push_back(pwm2);
        amp_desired.push_back(result);
        amp_measured.push_back(result);
        cout << "pwm:" << pwm2 << endl;
        cout << "var:" << result << endl;
        return result;
    }

    int Patch2LED::get_intercept(int pwm_m_1, double potential_m_1,
                                 int pwm_m_2, double potential_m_2) {
        //cout<<pwm_m_1<<" "<<pwm_m_2<<endl;
        //cout << potential_m_1<<" "<< potential_m_2<<endl<<endl;

        return pwm_m_1 -
               (((pwm_m_1 - pwm_m_2) / (potential_m_1 - potential_m_2)) *
                potential_m_1);
    }

    addRePro(Patch2LED, intererg);
}; /* namespace intererg */

#include "moc_patch2led.cc"
