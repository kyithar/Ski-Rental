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
#ifndef TWO_CACHE_H_
#define TWO_CACHE_H_

#include "base_cache.h"
#include <deque>
#include <boost/unordered_map.hpp>

using namespace boost;
using namespace std;

/*Power of two replacement: elements are pushed back into the cache If the
 * cache if filled replacement is fulfilled in this way:
 *    a) two random elements are taken from the cache 
 *    b) the "most popular" (out of the two) element is replaced.
*/
class two_cache: public base_cache{
    public:

    virtual void data_store(ccn_data *);
	virtual bool data_lookup(chunk_t);
	virtual bool full();
	virtual double get_caching_cost();
	virtual double get_tresh();

    private:
	deque<uint64_t> deq;
	unordered_map<uint64_t,bool> cache;
};
#endif
