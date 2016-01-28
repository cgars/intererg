/*
  erg/triggerdetector.cc
  detects a rising trigger pulse

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

#include <relacs/erg/triggerdetector.h>

using namespace relacs;

namespace erg {

    TriggerDetector::TriggerDetector(const string &ident, int mode)
            : Filter(ident, mode, SingleAnalogDetector, 1, "TriggerDetector",
                     "erg",
                     "Christian Garbers", "1.0", "Jan 24, 2012") {
        addNumber("threshold", "Threshold", 1.5, 1.0, 5, 1.0, "threshold",
                  "threshold");
        addSelection("detectorType", "Type oclear signal of the detector",
                     "up|down");
    }

    TriggerDetector::~TriggerDetector(void) {
    }


    int TriggerDetector::init(const InData &data, EventData &outevents,
                              const EventList &other,
                              const EventData &stimuli) {
        D.init(data.begin(), data.end(), data.timeBegin());
        return 0;
    }

    int TriggerDetector::detect(const InData &data, EventData &outevents,
                                const EventList &other,
                                const EventData &stimuli) {
        if (index("detectorType") == 0) {
            double Threshold = number("threshold");
            D.rising(data.minBegin(), data.end(), outevents,
                     Threshold, Threshold, Threshold, *this);
        }
        else {
            double Threshold = number("threshold");
            D.falling(data.minBegin(), data.end(), outevents,
                      Threshold, Threshold, Threshold, *this);
        }
        return 0;
    }

    int TriggerDetector::checkEvent(InData::const_iterator first,
                                    InData::const_iterator last,
                                    InData::const_iterator event,
                                    InData::const_range_iterator eventtime,
                                    InData::const_iterator index,
                                    InData::const_range_iterator indextime,
                                    InData::const_iterator prevevent,
                                    InData::const_range_iterator prevtime,
                                    EventData &outevents,
                                    double &threshold,
                                    double &minthresh, double &maxthresh,
                                    double &time, double &size, double &width) {
        return 1;
    }

    addFilter(TriggerDetector, erg);

}; /* namespace erg */

#include "moc_triggerdetector.cc"
