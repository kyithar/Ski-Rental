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
#include "fifo_cache.h"
#include "ccn_data.h"

//Register_Class(fifo_cache);

void fifo_cache::data_store(ccn_data *data_msg){
    //chunk_t chunk = data_msg->getChunk();
   chunk_t chunk = data_msg->getChunk();
   cache[chunk] = true;
   deq.push_back(chunk);

   if ( deq.size() > get_size() ) {
   //Eviction of the last element
       chunk_t toErase = deq.front();
       deq.pop_front();
       cache.erase(toErase);
   }

}
double fifo_cache::get_caching_cost(){}


bool fifo_cache::data_lookup(chunk_t chunk){
    return (cache.find(chunk)!=cache.end());
}


bool fifo_cache::full(){
    return (cache.size() == get_size());
}
