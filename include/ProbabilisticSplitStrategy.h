/*
 * ccnSim is a scalable chunk-level simulator for Content Centric
 * Networks (CCN), that we developed in the context of ANR Connect
 * (http://www.anr-connect.org/)
 *
 * People:
 *    Giuseppe Rossini (lead developer, mailto giuseppe.rossini@enst.fr)
 *    Andrea Araldo (developer, mailto andrea.araldo@gmail.com) 
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
//<aa>

#ifndef PROBABILISTICSPLITSTRATEGY_H_
#define PROBABILISTICSPLITSTRATEGY_H_

#include <omnetpp.h>
#include "MultipathStrategyLayer.h"

using namespace std;

class ccn_interest;

class ProbabilisticSplitStrategy: public MultipathStrategyLayer
{
    public:
		bool* get_decision(cMessage *);

	protected:
		void initialize();
		bool* exploit(ccn_interest *);
		void finish();
		vector<int> choose_paths(int num_paths);

	private:
		int decide_target_repository(ccn_interest *interest);
		int decide_out_gate(vector<int_f> FIB_entries);
		vector<double> split_factors;

};
#endif
//</aa>
