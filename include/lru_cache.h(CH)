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

#ifndef LRU_CACHE_H_
#define LRU_CACHE_H_
#include <boost/unordered_map.hpp>
#include "base_cache.h"
#include "ccnsim.h"


using namespace std;
using namespace boost;


//Indicate the position within the lru cache. In order to look-up for an
//element it just suffices removing the element from the current position and
//inserting it within the head of the list
struct lru_pos{
    //older and newer track the lru_position within the 
    //lru cache
    lru_pos* older;
    lru_pos* newer;
    chunk_t k;
    simtime_t hit_time;
	//<aa>
	double cost; //meaningful only with cost aware caching
	//</aa>
};

//Defines a simple lru cache composed by a map and a list of position within the map.
class lru_cache:public base_cache{
    friend class statistics;
    public:
		lru_cache():base_cache(),actual_size(0),lru(0),mru(0){;}
		//<aa>
		lru_pos* get_mru();
		lru_pos* get_lru();
		const lru_pos* get_eviction_candidate();
		//</aa>
	
		bool full(); //<aa> moved from protected to public </aa>

    protected:
		void data_store(chunk_t);
		bool data_lookup(chunk_t);
		bool fake_lookup(chunk_t);

		void dump();


    private:
		uint32_t actual_size; //actual size of the cache
		lru_pos* lru; //least recently used item
		lru_pos* mru; //most recently used item

		unordered_map<chunk_t, lru_pos*> cache; //cache of values

};
#endif
