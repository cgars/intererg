/*
  erg/triggerdetector.h
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

#ifndef _RELACS_ERG_TRIGGERDETECTOR_H_
#define _RELACS_ERG_TRIGGERDETECTOR_H_ 1

#include <relacs/filter.h>
#include <relacs/detector.h>

using namespace relacs;

namespace erg {


/*!
\class TriggerDetector
\brief [Filter] detects a rising trigger pulse
\author Christian Garbers
\version 1.0 (Jan 24, 2012)
*/


    class TriggerDetector : public Filter {
        Q_OBJECT

    public:

        TriggerDetector(const string &ident = "", int mode = 0);

        ~TriggerDetector(void);

        virtual int init(const InData &data, EventData &outevents,
                         const EventList &other, const EventData &stimuli);

        virtual int detect(const InData &data, EventData &outevents,
                           const EventList &other, const EventData &stimuli);

        int checkEvent(InData::const_iterator first,
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
                       double &time, double &size, double &width);


    protected:

        Detector<InData::const_iterator, InData::const_range_iterator> D;

    };


}; /* namespace erg */

#endif /* ! _RELACS_ERG_TRIGGERDETECTOR_H_ */
