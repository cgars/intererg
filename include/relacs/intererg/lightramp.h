/*
  intererg/lightramp.h
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

#ifndef _RELACS_ERG_LIGHTRAMP_H_
#define _RELACS_ERG_LIGHTRAMP_H_ 1

#include <relacs/repro.h>
#include <relacs/intererg/ledarray.h>
#include <relacs/multiplot.h>
#include <relacs/tablekey.h>

using namespace relacs;

namespace intererg {


    /*!
    \class LightRamp
    \brief [RePro] Measured ERG amplitude for increasing light levels
    \author Christian Garbers
    \version 1.0 (Jan 23, 2012)
    */
    class LightRamp : public RePro {
        Q_OBJECT

    public:

        LightRamp(void);

        virtual int main(void);

    protected:
        MultiPlot plots;

    };


}; /* namespace intererg */

#endif /* ! _RELACS_ERG_LIGHTRAMP_H_ */
