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
#ifndef CLIENT_H_
#define CLIENT_H_

#include <omnetpp.h>
#include "ccnsim.h"
class statistics;
class ccn_data;
using namespace std;



//Each of these entries contains information about the current downloads
struct download {
    filesize_t chunk; //number of chunks that still miss within the file

    simtime_t start; //start time (for statistic purposes)
    simtime_t last; //last time a chunk has been downloaded

	//<aa>
	#ifdef SEVERE_DEBUG
		int serial_number;
	#endif
	//</aa>

    download (double m = 0,simtime_t t = 0):chunk(m),start(t),last(t){;}
};

//Each of these entries contains statistics for each single file
struct client_stat_entry{
    double avg_distance;
    simtime_t avg_time;
    double tot_downloads;//double type due to the chunkization in CCN. See below.
    unsigned int tot_chunks; //<aa> The number of chunks downloaded so far </aa>

    client_stat_entry():avg_distance(0),avg_time(0),tot_downloads(0),tot_chunks(0){;}

};



class client : public cSimpleModule {
	//<aa>
	public:
		double get_avg_distance();
		double get_tot_downloads();
		simtime_t get_avg_time();
		bool is_active();
		void clear_stat(); //<aa> I moved this function to public</aa>
		int  getNodeIndex(); //<aa> I moved it to public</aa>

		//<aa>
		#ifdef SEVERE_DEBUG
		// Returns true iff the content is among the current_downloads
		bool is_waiting_for (name_t content);
		unsigned int get_interests_sent();
		#endif
		//</aa>

	//</aa>
    //friend class statistics; //<aa> I deactivated this line</aa>
    protected:
		virtual void initialize();
		virtual void handleMessage(cMessage *);
		virtual void finish();

		virtual void handle_incoming_chunk(ccn_data *);
		virtual void request_file();
		virtual void handle_timers(cMessage*);

		void send_interest(name_t, cnumber_t, int);
		void resend_interest(name_t,cnumber_t,int);


    private:
		cMessage *timer;
		cMessage *arrival;

		//List of current downloads for a given file
		multimap < name_t, download > current_downloads;

		//Single file statistics
		client_stat_entry* client_stats;

		//<aa> Number of objects downloaded by the client </aa>
		double tot_downloads; // Here the "double" type arises when you consider
			                  // that a given download might be not yet completed at all. 
		unsigned int tot_chunks;

		//<aa>
		#ifdef SEVERE_DEBUG
		unsigned int interests_sent;
		#endif
		//</aa>

		//Average statistics (on the whole set of files downloaded by this client)
		simtime_t avg_time;
		double avg_distance;

		//INI parameters
		double lambda;
		double RTT;
		simtime_t check_time;

		//Set if the client actively sends interests for files
		bool active;


};
#endif
