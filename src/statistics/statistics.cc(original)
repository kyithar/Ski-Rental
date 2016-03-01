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
#include <cmath>
#include "statistics.h"
#include "core_layer.h"
#include "base_cache.h"
#include "content_distribution.h"
#include "lru_cache.h"

//<aa>
#include "error_handling.h"
//</aa>


Register_Class(statistics);


using namespace std;

void statistics::initialize(){
    //Node property
    num_nodes 	= getAncestorPar("n");
    num_clients = getAncestorPar("num_clients");

   
    //Statistics parameters
    ts          = par("ts"); //sampling time
    window      = par("window");
    time_steady = par("steady");
    partial_n 	= par("partial_n");
	//<aa>
	variance_threshold = par("variance_threshold");
	//</aa>

    if (partial_n == -1)
		partial_n = num_nodes;


    cTopology topo;
    //Extracting clients
    clients = new client* [num_clients];
    vector<string> clients_vec(1,"modules.clients.client");
    topo.extractByNedTypeName(clients_vec);
    int k = 0;
    for (int i = 0;i<topo.getNumNodes();i++){
		    int c = ((client *)topo.getNode(i)->getModule())->getNodeIndex();
		if ( find (content_distribution::clients, content_distribution::clients + num_clients, c) 
			!= content_distribution::clients + num_clients)  {
			clients[k++] = (client *)topo.getNode(i)->getModule();
		}
    }
    
	total_replicas = *(content_distribution::total_replicas_p );

	//<aa> {REPO POPULARITY
	if ( content_distribution::repo_popularity_p != NULL )
	{

		for (unsigned repo_idx =0; repo_idx < content_distribution::repo_popularity_p->size();
					 repo_idx++)
		{
		    char name[30];

			sprintf ( name, "repo_popularity[%u]",repo_idx);
			double repo_popularity = 
				(*content_distribution::repo_popularity_p)[repo_idx];
			recordScalar(name,repo_popularity);
		}


		#ifdef SEVERE_DEBUG
			double repo_popularity_sum = 0;
			for (unsigned repo_idx =0; repo_idx < content_distribution::repo_popularity_p->size();
				 repo_idx++)
			{
				repo_popularity_sum += (*content_distribution::repo_popularity_p)[repo_idx];	
			}

			if ( ! double_equality(repo_popularity_sum, 1) )
			{
				std::stringstream ermsg; 
				ermsg<<"pop_indication_sum="<<repo_popularity_sum<<
					". It must be 1";
				severe_error(__FILE__,__LINE__,ermsg.str().c_str() );
			}
		#endif
	} //else the info about the repo popularity has not been calculated
	//</aa> }REPO POPULARITY



    //Extracting nodes (caches and cores)
    caches = new base_cache*[num_nodes];
    cores = new core_layer*[num_nodes];
    vector<string> nodes_vec(1,"modules.node.node");
    topo.extractByNedTypeName(nodes_vec);

    for (int i = 0;i<topo.getNumNodes();i++){
		caches[i] = (base_cache *) (topo.getNode(i)->getModule()->getModuleByRelativePath("content_store"));
		cores [i] = (core_layer *) (topo.getNode(i)->getModule()->getModuleByRelativePath("core_layer"));
    }

    //Store samples for stabilization
    samples.resize(num_nodes);

    full_check = new cMessage("full_check", FULL_CHECK);
    stable_check = new cMessage("stable_check",STABLE_CHECK);
    end = new cMessage("end",END);

	cout<<endl;

    //Start checking for full
    scheduleAt(simTime() + ts, full_check);
    
}



void statistics::handleMessage(cMessage *in){
    //Handle simulation timers

    int full = 0;
    int stables = 0;

    switch (in->getKind()){
        case FULL_CHECK:
            for (int i = 0; i < num_nodes;i++)
        	full += (int)caches[i]->full();

            if (full >= partial_n || simTime()>=10*3600){
        	cout<<"Caches filled at time "<<simTime()<<endl;
        	clear_stat();
        	scheduleAt(simTime() + ts, stable_check);
        	delete full_check;
            }else
        	scheduleAt(simTime() + ts, in);

            break;

        case STABLE_CHECK:

            for (int i = 0;i<num_nodes;i++)
        	stables += (int) stable(i);

            if ( stables >= partial_n )
            {
				delete stable_check;
				scheduleAt(simTime() + time_steady, end );
				stability_has_been_reached();
            } else 
        		scheduleAt(simTime() + ts, in);
		    break;
        case END:
            delete in;
            endSimulation();
    }

}

bool statistics::stable(int n){

    bool stable = false;
    double var = 0.0;
    double rate = caches[n]->hit * 1./ ( caches[n]->hit + caches[n]->miss );

    //Only hit rates matter, not also the misses
    if (caches[n]->hit != 0 ){
        samples[n].push_back( rate );
    }else 
        samples[n].push_back(0);

    if ( fabs( samples[n].size() - window * 1./ts ) <= 0.001 )
	{ //variance each window seconds
		var =variance(samples[n]); 
        // cout<<n<<"] variance = "<<var<<endl; //<aa> I disabled this line </aa>
        if ( var <= variance_threshold){
            stabilization_time = simTime().dbl();
            stable = true;
        }
        samples[n].clear();

    }
    return stable;

}


void statistics::finish(){

    char name[30];

    uint32_t global_hit = 0;
    uint32_t global_miss = 0;
    uint32_t global_interests = 0;
    uint32_t global_data      = 0;

    //<aa>
    uint32_t global_repo_load = 0;
	long total_cost = 0;
    //</aa>

    double global_avg_distance = 0;
    simtime_t global_avg_time = 0;
    uint32_t global_tot_downloads = 0;

    //<aa>
    #ifdef SEVERE_DEBUG
    unsigned int global_interests_sent = 0;
    #endif
    //</aa>

    for (int i = 0; i<num_nodes; i++)
	{
			//TODO: do not always compute cost. Do it only when you want to evaluate the cost in your
			// network
			total_cost += cores[i]->repo_load * cores[i]->get_repo_price();

		if (cores[i]->interests){
			//Check if the given node got involved within the interest/data process
			global_hit  += caches[i]->hit;
			global_miss += caches[i]->miss;
			global_data += cores[i]->data;
			global_interests += cores[i]->interests;
			global_repo_load += cores[i]->repo_load;

			//<aa>
			#ifdef SEVERE_DEBUG
				if (	caches[i]->decision_yes + caches[i]->decision_no +  
						(unsigned) cores[i]->unsolicited_data
						!=  (unsigned) cores[i]->data + cores[i]->repo_load
				){
					std::stringstream ermsg; 
					ermsg<<"caches["<<i<<"]->decision_yes="<<caches[i]->decision_yes<<
						"; caches[i]->decision_no="<<caches[i]->decision_no<<
						"; cores[i]->data="<<cores[i]->data<<
						"; cores[i]->repo_load="<<cores[i]->repo_load<<
						"; cores[i]->unsolicited_data="<<cores[i]->unsolicited_data<<
						". The sum of "<< "decision_yes and decision_no must be data";
					severe_error(__FILE__,__LINE__,ermsg.str().c_str() );
				}
			#endif
			//</aa>
		}
    }

    //Print and store global statistics

    //global_hit is the sum of the hits of each cache
    sprintf (name, "p_hit");
    recordScalar(name,global_hit * 1./(global_hit+global_miss));
    cout<<"p_hit/cache: "<<global_hit *1./(global_hit+global_miss)<<endl;

    sprintf ( name, "interests");
    recordScalar(name,global_interests * 1./num_nodes);

    sprintf ( name, "data" );
    recordScalar(name,global_data * 1./num_nodes);

    for (int i = 0;i<num_clients;i++){
		global_avg_distance += clients[i]->get_avg_distance();
		global_tot_downloads += clients[i]->get_tot_downloads();
		global_avg_time  += clients[i]->get_avg_time();
		//<aa>
		#ifdef SEVERE_DEBUG
		global_interests_sent += clients[i]->get_interests_sent();
		#endif
		//</aa>

    }

    sprintf ( name, "hdistance");
    recordScalar(name,global_avg_distance * 1./num_clients);
    cout<<"Distance/client: "<<global_avg_distance * 1./num_clients<<endl;

    sprintf ( name, "avg_time");
    recordScalar(name,global_avg_time * 1./num_clients);
    cout<<"Time/client: "<<global_avg_time * 1./num_clients<<endl;


	//<aa> Sum of the download of all users//</aa>
    sprintf ( name, "downloads");
    recordScalar(name,global_tot_downloads);

    sprintf ( name, "total_cost");
    recordScalar(name,total_cost);
    cout<<"total_cost: "<<total_cost<<endl;


    sprintf ( name, "total_replicas");
    recordScalar(name,total_replicas);
    cout<<"total_replicas: "<<total_replicas<<endl;

    //<aa>
    // It is the fraction of traffic that is satisfied by some cache inside
    // the network and thus does not exit the network </aa>
    sprintf (name, "inner_hit");
    recordScalar(name , (double) (global_tot_downloads - global_repo_load) / global_tot_downloads) ;

    #ifdef SEVERE_DEBUG
	if (global_tot_downloads == 0)
	{
	       	std::stringstream ermsg;
		ermsg<<"global_tot_downloads == 0";
		severe_error(__FILE__,__LINE__,ermsg.str().c_str() );
	}

		sprintf ( name, "interests_sent");
		recordScalar(name,global_interests_sent);
		cout<<"interests_sent: "<<global_interests_sent<<endl;

		if (global_interests_sent != global_tot_downloads){
			std::stringstream ermsg; 
			ermsg<<"interests_sent="<<global_interests_sent<<"; tot_downloads="<< 
				global_tot_downloads<<
				". If **.size==1 in omnetpp.ini and all links have 0 delay, this "<<
				" is an error. Otherwise, it is not";
			debug_message(__FILE__,__LINE__,ermsg.str().c_str() );
		}
	#endif
    //</aa>

    
    //TODO per content statistics
    //double hit_rate;
    // for (uint32_t f = 1; f <=content_distribution::perfile_bulk; f++){
    //     hit_rate = 0;
    //     if(hit_per_file[f]!=0)
    //         hit_rate = hit_per_file[f] / ( hit_per_file[f] +miss_per_file[f] );
    //     hit_per_fileV.recordWithTimestamp(f, hit_rate);
    //}
}

void statistics::clear_stat()
{
    for (int i = 0;i<num_clients;i++)
	if (clients[i]->is_active() )
	    clients[i]->clear_stat();

    for (int i = 0;i<num_nodes;i++)
        cores[i]->clear_stat();

    for (int i = 0;i<num_nodes;i++)
	    caches[i]->clear_stat();
}

void statistics::stability_has_been_reached(){
	//<aa>
	char name[30];
	sprintf (name, "stabilization_time");
	recordScalar(name,stabilization_time);
	cout<<"stabilization_time: "<< stabilization_time <<endl;
	//</aa>

	clear_stat();
}

void statistics::registerIcnChannel(cChannel* icn_channel){
	#ifdef SEVERE_DEBUG
	if ( std::find(icn_channels.begin(), icn_channels.end(), icn_channel)
			!=icn_channels.end()
	){
        std::stringstream ermsg; 
		ermsg<<"Trying to add to statistics object an icn channel already added"<<endl;
	    severe_error(__FILE__,__LINE__,ermsg.str().c_str() );
	}
	#endif
	icn_channels.push_back(icn_channel);
}
//</aa>
