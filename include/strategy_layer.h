/*
 * ccnSim is a scalable chunk-level simulator for Content Centric
 * Networks (CCN), that we developed in the context of ANR Connect
 * (http://www.anr-connect.org/)
 *
 * People:
 *    Giuseppe Rossini (lead developer, mailto giuseppe.rossini@enst.fr)
 *	  Andrea Araldo (developer, mailto andrea.araldo@gmail.com)
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
#ifndef STRATEGY_H_
#define STRATEGY_H_

#include "ccnsim.h"
#include <omnetpp.h>
#include <fstream>

//#include "abstract_node.h"
#include <boost/unordered_map.hpp>

using namespace std;
using namespace boost;


struct int_f{
    int  id; //<aa> Identifies an output interface </aa>
    
    //<aa>the distance of the path to reach the destination node passing through
	//the specified interface </aa>
    int  len;

    bool operator<(int_f other){
	return (other.len > this->len);
    }
};


//
//Basic strategy layer class. In order to "be" a strategy layer 
//a class needs to define its own get_decision function
//which returns an array of booleans. The i-th bool value
//indicates if the message should be sent on the i-th interface.
//
class strategy_layer: public abstract_node{
    public:
		//The only interface function. Cores should be call this function in
		//order to get the interfaces on which sending the current interest
		//<aa>
		/**
		 * It returns an array of boolean values, one for each output gate.
		 * If the i-th element of this array is 1, the message must be forwaded
		 * toward gate i.
		 */
		//</aa>
		virtual bool* get_decision(cMessage *)=0;
		
		static ifstream fdist;
		static ifstream frouting;
    protected:
		//Omnet base functions
		virtual void initialize();
		virtual void finish();

		//FIB initialization stuff
		void populate_routing_table();
		void populate_from_file();
		//<aa>
		void add_FIB_entry(int destination_node_index, int interface_index, 
							int distance);
		const vector<int_f> get_FIB_entries(int destination_node_index);
		virtual vector<int> choose_paths(int num_paths)=0;
		//</aa>


	private:
		//FIB (available to all subclasses, for sake of utilization)
		//<aa> Associates to each destination node,
		// an output interface to reach it</aa>
		unordered_map <int ,vector<int_f> > FIB; 	
		unordered_map <int, int> gatelu;	
		
		//<aa> For each index identifying a client
		//module, associates the interface through
		//which that client can be reached</aa>
		int nodes;

};
#endif
