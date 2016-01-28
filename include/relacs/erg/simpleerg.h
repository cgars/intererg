/*
  erg/simpleerg.h
  safdsadsa

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

#ifndef _RELACS_ERG_SIMPLEERG_H_
#define _RELACS_ERG_SIMPLEERG_H_ 1

#include <relacs/repro.h>
#include <relacs/erg/ledarray.h>
#include <relacs/sampledata.h>
#include <relacs/multiplot.h>

using namespace relacs;

namespace erg {


/*!
\class simpleErg
\brief [RePro] A Repro measuring a set of ergs with one led flickering
\author Christian Garbers
\version 1.0 (Jan, 2016)
*/
    class simpleErg : public RePro {
        Q_OBJECT

    public:
        simpleErg(void);

        virtual int main(void);

    protected:
        MultiPlot P;
    };


}; /* namespace erg */

#endif /* ! _RELACS_ERG_SIMPLEERG_H_ */
