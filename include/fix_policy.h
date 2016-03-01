/*
 * ccnSim is a scalable chunk-level simulator for Content Centric
 * Networks (CCN), that we developed in the context of ANR Connect
 * (http://www.anr-connect.org/)
 *
 * People:
 *    Giuseppe Rossini (lead developer, mailto giuseppe.rossini@enst.fr)
 *    Raffaele Chiocchetti (developer, mailto raffaele.chiocchetti@gmail.com)
 *    Dario Rossi (occasional debugger, mailto dario.rossi@enst.fr)
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef FIX_POLICY_H_
#define FIX_POLICY_H_

#include "decision_policy.h"

//<aa>
#include "error_handling.h"
//</aa>

/* Fixed probability policy: store a given chunk with a given (fixed)
 * probability. 
 */
class Fix: public DecisionPolicy{
    public:
	Fix(double pc):p(pc){;}//Store the caching probability

	virtual bool data_to_cache(ccn_data *){

	    double x = dblrand();

	    if (x < p)
			return true;
	    return false;
	}
    private:
	double p;
};
#endif
	
