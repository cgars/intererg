/*
  intererg/offramp.h
  Ramps the Duration of the off pulse

  Intererg (C) 2012-2016 Christian Garbers <christian@stuebeweg50.de>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  Intererg is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _RELACS_ERG_OFFRAMP_H_
#define _RELACS_ERG_OFFRAMP_H_ 1

#include <relacs/repro.h>
#include <relacs/intererg/ledarray.h>
#include <relacs/multiplot.h>
#include <relacs/tablekey.h>

using namespace relacs;

namespace intererg {


    /*!
    \class OffRamp
    \brief [RePro] Ramps the Duration of the off pulse
    \author Christian Garbers
    \version 1.0 (Jan 25, 2012)
     This repro iteratively increaes the duration of the time after the selected
     led is switched on
    */
    class OffRamp : public RePro {
        Q_OBJECT

    public:
        OffRamp(void);

        virtual int main(void);

    };


}; /* namespace intererg */

#endif /* ! _RELACS_ERG_OFFRAMP_H_ */
