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

#ifndef CCN_NODE_H
#define CCN_NODE_H

#include <omnetpp.h>
#include "ccnsim.h"

#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>

using namespace std;
using namespace boost;

class ccn_interest;
class ccn_data;

class strategy_layer;
class base_cache;


//This structure takes care of data forwarding
struct pit_entry {
    interface_t interfaces;
    unordered_set<int> nonces;
    simtime_t time; //<aa> last time this entry has been updated</aa>
    int custo_marking;// to know custodian router or not(0 = not custodian, 1= custodian)
};
struct cstat_entry {
double miss_time;
double pre_miss_t;
double Delta;
double cumu_inter;
double req_cost;
double cache_miss;
int custodian_hit;
int c_decision_check;

cstat_entry():miss_time(0),pre_miss_t(0),Delta(0),cumu_inter(0), req_cost(0), cache_miss(0),custodian_hit(0){;}
};

struct rcs_cstat_entry {
double miss_time;
double pre_miss_t;
double Delta;
double cumu_inter;
double req_cost;
double cache_miss;
int custodian_hit;
int c_decision_check;

rcs_cstat_entry():miss_time(0),pre_miss_t(0),Delta(0),cumu_inter(0), req_cost(0), cache_miss(0),custodian_hit(0){;}
};

class core_layer : public abstract_node{
    friend class statistics;
    
    public:
    	void check_if_correct(int line);
		//<aa>
		#ifdef SEVERE_DEBUG
		bool it_has_a_repo_attached;

		vector<int> get_interfaces_in_PIT(chunk_t chunk);
		bool is_it_initialized;
		#endif

		double get_repo_price();
		//void set_repo_price(double price);
		//</aa>

    protected:
		//Standard node Omnet++ functions
		virtual void initialize();
		virtual void handleMessage(cMessage *);
		virtual void finish();

		//<aa> See ned file
		bool interest_aggregation;
		bool transparent_to_hops;
		double repo_price; //the price of the attached repository.
		void add_to_pit(chunk_t chunk, int gate);
		//</aa>

		//Custom functions
		void handle_interest(ccn_interest *);
		void handle_interest_AR(ccn_interest *);
		void handle_interest_NAR(ccn_interest *);
		void handle_interest_NARSVR(ccn_interest *);
		void handle_forward (ccn_interest *);
		void handle_forward_update (ccn_interest *);
		void handle_statCS (ccn_interest *);
		void handle_statRCS (ccn_interest *);
		void handle_ghost(ccn_interest *);
		void handle_data(ccn_data *);
		void handle_decision(bool *, ccn_interest *);

		void cdecision_cal(ccn_data *);
		void cdecision_CS(ccn_data *);
		void cdecision_RCS(ccn_data *);


		bool check_ownership(vector<int>);
		ccn_data *compose_data(uint64_t);	
		void clear_stat();


    private:

		unsigned long max_pit;
		unsigned short nodes;
		unsigned int my_bitmask;
		double my_btw;
		double RTT;
		static int repo_interest; 	// <aa> total number of interests set to one of the
									// repositories of the network </aa>
        int current_r;//current router
        int cluster_id;
        int client_attached;
        int core_check, core_interest_reset;
        int cache_size, total_cache_size;
        int sigma;
        simtime_t old_Dalta;
        simtime_t tmp_pre_miss;
        double req_cost, max_req_cost, avg_threshold;
        double req_cost_rcs, max_req_cost_rcs, avg_threshold_rcs;


		//<aa> number of chunks satisfied by the repository attached to this node</aa>
		int repo_load; 
	

		//Architecture data structures
		boost::unordered_map <chunk_t, pit_entry > PIT;
		boost::unordered_map <chunk_t, cstat_entry > Cstat;
		boost::unordered_map <chunk_t, rcs_cstat_entry > Rstat;
		boost::unordered_map<int,int>  CHring;
		base_cache *ContentStore;
		base_cache *RContentStore;
		strategy_layer *strategy;

		//Statistics
		int interests;
		int data;

		//<aa>
		#ifdef SEVERE_DEBUG
		int unsolicited_data;	// Data received by the node but not requested by anyone

		int discarded_interests; //number of incoming interests discarded
								 // because their TTL is > max hops
		int unsatisfied_interests;	//number of interests for contents that are neither
									//in the cache nor in the repository of this node	
		int interests_satisfied_by_cache;

		#endif
		int	send_data (ccn_data* msg, const char *gatename, int gateindex, int line_of_the_call);
		//</aa>
};
#endif

