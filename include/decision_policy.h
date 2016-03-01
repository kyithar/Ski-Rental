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
#ifndef DECISION_POLICY_H_
#define DECISION_POLICY_H_
#include "ccn_data.h"
/* A Decision policy is an interface with only one function: 
 * 
 * -- data_to_cache()
 *
 * This functions returns a boolean value which says if the given data chunk
 * needs to be cached or not.
 */
class DecisionPolicy {
    public:
		virtual bool data_to_cache (ccn_data *)=0;

		//<aa>
		virtual void finish (int nodeIndex, base_cache* cache_p){};

		// Called by base_cache.cc
		virtual void after_insertion_action(){
			// Do nothing
		};
		//</aa>
};
#endif

