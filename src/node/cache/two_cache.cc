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
#include "two_cache.h"
#include "ccn_data.h"
Register_Class(two_cache);

void two_cache::data_store(ccn_data *data_msg){
    chunk_t chunk =data_msg->getChunk();

   cache[chunk] = true;

   if (deq.size() == get_size()){

       //Random extraction of two elements
       unsigned int  pos1 = intrand( deq.size() );
       unsigned int  pos2 = intrand( deq.size() );
       unsigned int  pos;

       chunk_t  toErase1 = deq.at(pos1);
       chunk_t  toErase2 = deq.at(pos2);
       chunk_t  toErase;

       name_t name1 = __id(toErase1);
       name_t name2 = __id(toErase2);


       //Comparing content popularity (a realistic implementation can employ a the freq map
       if (name1 > name2){

	   toErase = toErase2;
	   pos = pos2;

       }else if (name1 == name2){
	   if ( intrand(2) == 0 ){
	       toErase = toErase1;
	       pos=pos1;
	   }else{
	       toErase=toErase2;
	       pos=pos2;
	   }
       }else{
	   toErase = toErase1;
	   pos = pos1;
       }

       //Erase the more popular elements among the two
       deq.at(pos)=chunk;
       cache.erase(toErase);
   }else
       deq.push_back(chunk);


}
double two_cache::get_caching_cost(){}
double two_cache::get_tresh(){}

bool two_cache::data_lookup(chunk_t chunk){
    return (cache.find(chunk)!=cache.end());
}

bool two_cache::full(){
    return (deq.size()==get_size());
}
