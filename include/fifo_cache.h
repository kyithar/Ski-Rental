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
#ifndef FIFO_CACHE_H_
#define FIFO_CACHE_H_

#include "base_cache.h"
#include <deque>
#include <boost/unordered_map.hpp>
using namespace std;
using boost::unordered_map;


/*
 * FIFO replacement cache: each new chunk is pushed in front of the cache and
 * the back element is evicted.
 */
class fifo_cache: public base_cache{
    public:
	//Polymorphic methods
    virtual void data_store (ccn_data *);
	virtual bool data_lookup (chunk_t);
	virtual bool full();
	virtual double get_caching_cost();
    private:
	deque<chunk_t> deq;//Deque for the order 
	unordered_map<chunk_t,bool> cache;//Map for a look up


};
#endif
