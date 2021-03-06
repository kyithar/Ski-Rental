/*
 * ccnSim is a scalable chunk-level simulator for Content Centric
 * Networks (CCN), that we developed in the context of ANR Connect
 * (http://www.anr-connect.org/)
 *
 * People:
 *    Giuseppe Rossini (lead developer)
 *    Raffaele Chiocchetti (developer)
 *    Dario Rossi (occasional debugger)
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
#include "core_layer.h"
#include "ccnsim.h"
#include <algorithm>
#include <math.h>
#include "content_distribution.h"
#include "strategy_layer.h"
#include "ccn_interest.h"
#include "ccn_data.h"
#include "base_cache.h"

//<aa>
#include "error_handling.h"
//</aa>

Register_Class(core_layer);
int core_layer::repo_interest = 0;


void  core_layer::initialize(){
    RTT = par("RTT");
    //<aa>
    interest_aggregation = par("interest_aggregation");
    transparent_to_hops = par("transparent_to_hops");
    // Notice that repo_price has been initialized by WeightedContentDistribution
    //</aa>
    repo_load = 0;
    nodes = getAncestorPar("n"); //Number of nodes
    my_btw = getAncestorPar("betweenness");
    int num_repos = getAncestorPar("num_repos");
    req_cost =0;
    max_req_cost=0;
    avg_threshold =0;
    max_miss_cs=0;
    max_miss_rcs=0;
    max_cost_cs=0;
    max_cost_rcs=0;
    arrival_cs=0;
    arrival_rcs=0;




    cTopology topo;
    vector<string> types;
    types.push_back("modules.node.node");
    topo.extractByNedTypeName( types );
    cTopology::Node *node = topo.getNode( getIndex());

    if(node->getCluster_id()==0){
           cluster_id=0;
           CHring=CH0;
          total_cache_size=Tsize[0];
               }
    if(node->getCluster_id()==1){
           cluster_id=1;
           CHring=CH1;
           total_cache_size=Tsize[1];
                     }
    if(node->getCluster_id()==2){
           cluster_id=2;
           CHring=CH2;
           total_cache_size=Tsize[2];
                          }
    if(node->getCluster_id()==3){
           cluster_id=3;
           CHring=CH3;
           total_cache_size=Tsize[3];
                          }
    //cout<<"total cache size "<<total_cache_size<<endl;

    if(getIndex()==0){ core_check=1;}//to identify the core routers

    if(getIndex()==2||getIndex()==3||getIndex()==4||getIndex()==5||getIndex()==7||getIndex()==8||getIndex()==9||getIndex()==10||getIndex()==12||getIndex()==13||getIndex()==14||getIndex()==15){
           client_attached=1;
       }//to identify the access router
   // cache_size=cacheini[getIndex()];
   //cout<<"cache size "<<cache_size<<endl;


    //<aa>
    #ifdef SEVERE_DEBUG
        is_it_initialized = false;
        it_has_a_repo_attached = false;
    #endif
    //</aa>

    int i = 0;
    my_bitmask = 0;
    for (i = 0; i < num_repos; i++)
    {
        if (content_distribution::repositories[i] == getIndex() ){
            //<aa>
            #ifdef SEVERE_DEBUG
                it_has_a_repo_attached = true;
            #endif

            repo_price = content_distribution::repo_prices[i];
            //</aa>
            break;
        } else
                repo_price = 0;
    }
    my_bitmask = (1<<i);//recall that the width of the repository bitset is only num_repos

    //Getting the content store
    ContentStore = (base_cache *) gate("cache_port$o")->getNextGate()->getOwner();//original content store
    RContentStore = (base_cache *) gate("rcache_port$o")->getNextGate()->getOwner();// replicate content store
    strategy = (strategy_layer *) gate("strategy_port$o")->getNextGate()->getOwner();

    //Statistics
    //interests = 0; //<aa> Disabled this. The reset is inside clear_stat() </aa>
    //data = 0; //<aa> Disabled this. The reset is inside clear_stat() </aa>

    //<aa>
    clear_stat();
    check_time=1;
    //arrival = new cMessage("arrival", ARRIVAL );
    timer = new cMessage("timer", TIMER);
    //scheduleAt( simTime() + exponential(1./lambda), arrival);
    scheduleAt( simTime() + check_time, timer  );


}



/*
 * Core layer core function. Here the incoming packet is classified,
 * determining if it is an interest or a data packet (the corresponding
 * counters are increased). The two auxiliary functions handle_interest() and
 * handle_data() have the task of dealing with interest and data processing.
 *
 *
 */



void core_layer::handleMessage(cMessage *in){
    if (in->isSelfMessage()){
        handle_timers(in);
        return;
    }
    //cout<<getIndex()<<"_C_"<<ContentStore->get_size()<<endl;
   // cout<<getIndex()<<"_R_"<<RContentStore->get_size()<<endl;
    current_r = getParentModule()->getIndex();//get router name

    ccn_data *data_msg;
    ccn_interest *int_msg;
    int type = in->getKind();//get msg type(Interest or Data)
    switch(type){
    //On receiving interest
    case CCN_I:
        interests++;

        int_msg = (ccn_interest *) in;
        //Cstat[int_msg->getChunk()].access_freq +=1;
        //<aa>
        if (!transparent_to_hops)
        //</aa>
            int_msg->setHops(int_msg -> getHops() + 1);

        if (int_msg->getHops() == int_msg->getTTL())
        {
            //<aa>
            #ifdef SEVERE_DEBUG
            discarded_interests++;
            check_if_correct(__LINE__);
            #endif
            //</aa>
            break;
        }
        int_msg->setCapacity (int_msg->getCapacity() + ContentStore->get_size());
        handle_interest (int_msg);
        break;

    //On receiving data
    case CCN_D:
        data++;

        data_msg = (ccn_data* ) in; //One hop more from the last caching node (useful for distance policy)

        //<aa>
        if (!transparent_to_hops)
        //</aa>
            data_msg->setHops(data_msg -> getHops() + 1);

        handle_data(data_msg);

        break;
    }

    delete in;

    //<aa>
    #ifdef SEVERE_DEBUG
    check_if_correct(__LINE__);
    #endif
    //</aa>
}

//Per node statistics printing
void core_layer::finish(){
    //<aa>
    #ifdef SEVERE_DEBUG
    check_if_correct(__LINE__);
//        if (    data+repo_load != \
//                (int) (ContentStore->get_decision_yes() +
//                        ContentStore->get_decision_no() )
//        ){
//            std::stringstream msg;
//            msg<<"node["<<getIndex()<<"]: "<<
//                "decision_yes=="<<ContentStore->get_decision_yes()<<
//                "; decision_no=="<<ContentStore->get_decision_no()<<
//                "; repo_load=="<<repo_load<<
//                "; data="<<data<<
//                ". The sum of decision_yes+decision_no MUST be equal to data+repo_load";
//            severe_error(__FILE__, __LINE__, msg.str().c_str() );
//        }
    #endif
    //</aa>

    char name [30];

    //Total interests
    // <aa> Parts of these interests will be satisfied by the local cache; the remaining part will be sent to the local repo (if present) and partly sarisfied there. For the remaining part, a FIB entry will be searched to forward the intereset. If no FIB entry is found, the interest will be discarded </aa>
    sprintf ( name, "interests[%d]", getIndex());
    recordScalar (name, interests);

    if (repo_load != 0){
        sprintf ( name, "repo_load[%d]", getIndex());
        recordScalar(name,repo_load);
    }

    //Total data
    sprintf ( name, "data[%d]", getIndex());
    recordScalar (name, data);

    //<aa> Interests sent to the repository attached to this node</aa>
    if (repo_interest != 0){
    sprintf ( name, "repo_int[%d]", getIndex());
    recordScalar(name, repo_interest);
    repo_interest = 0;
    }


}

void core_layer::handle_timers(cMessage *timer){
arrival_cs=0;
arrival_rcs=0;
scheduleAt( simTime() + check_time, timer );
}

void core_layer::handle_interest(ccn_interest *int_msg){

if(current_r == 0){int_msg->setI_type(0);}//when the core router receives the Interest, change the Interest type to "original" Interest.
if(current_r==1||current_r==6 ||current_r==11){
    handle_interest_8 (int_msg);
}else{
    if(client_attached==1){
        handle_interest_AR (int_msg);
    }else if(current_r==0){
        //handle_interest_NARSVR (int_msg);
        handle_interest_Core (int_msg);
    }else{
        handle_interest_NAR (int_msg);
    }
}








}

void core_layer::handle_statCS(ccn_interest *int_msg){
    arrival_cs +=1;
    //cout<<arrival_cs<<endl;
    chunk_t chunk = int_msg->getChunk();//get the chunk number
    unordered_map <chunk_t, cstat_entry >::iterator CstatIt = Cstat.find(chunk);
    if(CstatIt==Cstat.end()){
            Cstat[chunk].uni=uniform(0, 1, 0);
    }
    Cstat[chunk].cache_miss +=1;

                if(Cstat[chunk].cache_miss>1){// to update the request cost when the Interest is arrrived at the router

                    Cstat[chunk].pre_miss_t=Cstat[chunk].miss_time;
                    Cstat[chunk].miss_time=(simTime().dbl()*1000.0);
                    Cstat[chunk].Delta += Cstat[chunk].miss_time -Cstat[chunk].pre_miss_t;
                    Cstat[chunk].cumu_inter=Cstat[chunk].Delta/(Cstat[chunk].cache_miss-1);
                    Cstat[chunk].miss_cost =Cstat[chunk].cache_miss/Cstat[chunk].cumu_inter;

                 }else {
                     Cstat[chunk].miss_time=(simTime().dbl()*1000.0);
                     Cstat[chunk].miss_cost=0;
                 }
                //if(Cstat[chunk].cache_miss>1){

                    if(ContentStore->full()==true){
                        Cstat[chunk].cache_cost=ContentStore->getTresh();
                        //cout<<"full-cache cost "<< Cstat[chunk].cache_cost<<endl;
                        //double a =pow((1+1/Cstat[chunk].cache_cost),Cstat[chunk].cache_cost)-1;
                        Cstat[chunk].x +=(1/Cstat[chunk].cache_cost)-(ContentStore->getcaching_cost()-Cstat[chunk].miss_cost);//(1/(a*Cstat[chunk].cache_cost));
                        //Cstat[chunk].x +=(1/Cstat[chunk].cache_cost);
                        if(Cstat[chunk].x>=Cstat[chunk].uni){
                            Cstat[chunk].x=1;
                        }
                    }else{//rcs is free
                        if(Cstat[chunk].x<1){
                            if(max_miss_cs<arrival_cs){
                                double cs_size=ContentStore->get_size();
                                max_miss_cs=arrival_cs;
                                //max_cost_cs=(arrival_cs*ceil(exp(1/(cs_size*0.01))));//pow(max_miss_cs/(cs_size*0.01),max_miss_cs);//((max_miss_cs/(cs_size*0.01))*(max_miss_cs+1));//ceil(pow(max_miss_cs/cs_size,max_miss_cs));
                                max_cost_cs= arrival_cs*floor(total_cache_size/cs_size);
                                //cout<<"max_miss_cs "<< (exp(1/(cs_size*0.00001)))<<endl;
                            }
                            Cstat[chunk].cache_cost=max_cost_cs;
                            //cout<<"max_cost_cs "<<max_cost_cs<<endl;
                            //double a =pow((1+1/Cstat[chunk].cache_cost),Cstat[chunk].cache_cost)-1;
                            Cstat[chunk].x +=Cstat[chunk].miss_cost+(1/Cstat[chunk].cache_cost);//(1+Cstat[chunk].miss_cost)///(1/a*Cstat[chunk].cache_cost);
                            if(Cstat[chunk].x>=Cstat[chunk].uni){
                                //Cstat[chunk].thresh=Cstat[chunk].x;
                                Cstat[chunk].x=1;
                            }
                        }

                    }


                //}
               //cout<<"cs x "<<Cstat[chunk].x<<endl;
                    //cout<<"cache miss ratio "<<Cstat[chunk].miss_cost+(1/Cstat[chunk].cache_cost)<<endl;
}

void core_layer::handle_statRCS(ccn_interest *int_msg){
    arrival_rcs +=1;
    chunk_t chunk = int_msg->getChunk();//get the chunk number
    unordered_map <chunk_t, rcs_cstat_entry >::iterator RstatIt = Rstat.find(chunk);
    if(RstatIt==Rstat.end()){
            Rstat[chunk].uni=uniform(0, 1, 0);
    }

    Rstat[chunk].cache_miss +=1;


    if(Rstat[chunk].cache_miss>1){// to update the request cost when the Interest is arrrived at the router

        Rstat[chunk].pre_miss_t=Rstat[chunk].miss_time;
        Rstat[chunk].miss_time=(simTime().dbl()*1000.0);
        Rstat[chunk].Delta += Rstat[chunk].miss_time -Rstat[chunk].pre_miss_t;
        Rstat[chunk].cumu_inter=Rstat[chunk].Delta/(Rstat[chunk].cache_miss-1);
        Rstat[chunk].miss_cost =Rstat[chunk].cache_miss/Rstat[chunk].cumu_inter;

     }else {
         Rstat[chunk].miss_time=(simTime().dbl()*1000.0);
         Rstat[chunk].miss_cost=0;
     }
   // if(Rstat[chunk].cache_miss>1){
        if(RContentStore->full()==true){
            Rstat[chunk].cache_cost=RContentStore->getTresh();
            //cout<<"full-cache cost RCS "<< Rstat[chunk].cache_cost<<endl;
            //double a = pow((1+1/Rstat[chunk].cache_cost),Rstat[chunk].cache_cost)-1;
            Rstat[chunk].x +=(1/Rstat[chunk].cache_cost)-(RContentStore->getcaching_cost()-Rstat[chunk].miss_cost);//(Rstat[chunk].x*(1+(1/Rstat[chunk].cache_cost)))+(1/(a*Rstat[chunk].cache_cost));

            if(Rstat[chunk].x>=Rstat[chunk].uni){

                Rstat[chunk].x=1;
            }
        }else{//rcs is free
            if(Rstat[chunk].x<1){
                if(max_miss_rcs<arrival_rcs){
                    double rcs_size=RContentStore->get_size();
                    max_miss_rcs=arrival_rcs;
                    max_cost_rcs= arrival_rcs*floor(total_cache_size/rcs_size);//pow(max_miss_rcs/(rcs_size*0.01),max_miss_rcs);//150;//ceil(pow(2,max_miss_rcs/rcs_size));
                   // cout<<"max_miss_rcs "<<max_cost_rcs <<endl;
                    cout<<"max_miss_rcs "<< max_cost_rcs<<endl;
                }
                Rstat[chunk].cache_cost=max_cost_rcs;
                //double a =pow((1+1/Rstat[chunk].cache_cost),Rstat[chunk].cache_cost)-1;
                //cout<<"a "<<a<<endl;
                Rstat[chunk].x +=Rstat[chunk].miss_cost+(1/Rstat[chunk].cache_cost);//(Rstat[chunk].x*(1+(1/Rstat[chunk].cache_cost)))+(1/(a*Rstat[chunk].cache_cost));
                //Rstat[chunk].x +=(1/Rstat[chunk].cache_cost);
                //cout<<"x "<<Rstat[chunk].x<<endl;
              // cout<<"uni "<<Rstat[chunk].uni<<endl;
                if(Rstat[chunk].x>=Rstat[chunk].uni){
                    //cout<<"x "<<Rstat[chunk].x<<endl;
                   // cout<<"uni "<<Rstat[chunk].uni<<endl;
                    //Rstat[chunk].thresh=Rstat[chunk].x;
                    Rstat[chunk].x=1;

                }
            }

        }


   // }
       // cout<<"Rcs x "<<Rstat[chunk].x<<endl;
   // cout<<"R cache cost "<<Rstat[chunk].cache_cost<<endl;


}
//void core_layer::handle_forward_update(ccn_interest *int_msg){
//
//    bool i_will_forward_interest=true;
//    if (int_msg->getTarget() == getIndex() )
//    {
//        int_msg->setAggregate(false);
//    }
//
//    if ( !interest_aggregation || int_msg->getAggregate()==false )
//        i_will_forward_interest = true;
//
//    if (i_will_forward_interest)
//    {   bool * decision = strategy->get_decision(int_msg);
//        handle_decision(decision,int_msg);
//        delete [] decision;//free memory for the decision array
//    }
//
//}
void core_layer::handle_forward(ccn_interest *int_msg){
    chunk_t chunk = int_msg->getChunk();//get the chunk number
    unordered_map < chunk_t , pit_entry >::iterator pitIt = PIT.find(chunk);

    if(client_attached==1){
        if(current_r==int_msg->getCusto()){//custodian-interest come from CS
            if(pitIt != PIT.end()){
               int_msg->setI_type(0);
            }else{
            int_msg->setI_type(1);
            PIT[chunk].custo_marking =1;//to store the chunk at custodian
            }
        }else{
            if(pitIt != PIT.end()){
                int_msg->setI_type(2);
            }else{
            PIT[chunk].custo_marking=2;
            int_msg->setI_type(0);
            }
    }
    }else{
        if(current_r==int_msg->getCusto()){//custodian-interest come from CS
            if(pitIt != PIT.end()){
                int_msg->setI_type(0);
            }else{
           int_msg->setI_type(1);
           PIT[chunk].custo_marking =1;//to store the chunk at custodian
            }
        }

    }
   // cout<<"current r "<<current_r<<" custodian "<<int_msg->getCusto()<<"->interest type "<<int_msg->getI_type()<<endl;
                             bool i_will_forward_interest = false;
                             if (pitIt==PIT.end() || int_msg->getI_type()!=0
                                     || (pitIt != PIT.end() && int_msg->getNfound() )
                                     || simTime() - PIT[chunk].time > 2*RTT){
                                 i_will_forward_interest = true;

                                 if(client_attached==1){
                                     if (pitIt!=PIT.end()){
                                         PIT.erase(chunk);

                                     }
                                     PIT[chunk].time = simTime();
                                 }



//                                 if ((pitIt!=PIT.end())&& (int_msg->getI_type()==0))
//                                  PIT.erase(chunk);
//
//                                  PIT[chunk].time = simTime();
                                }

                       if (int_msg->getTarget() == getIndex() )
                       {
                           int_msg->setAggregate(false);
                       }

                       if ( !interest_aggregation || int_msg->getAggregate()==false )
                           i_will_forward_interest = true;

                       if (i_will_forward_interest)
                       {   bool * decision = strategy->get_decision(int_msg);
                           handle_decision(decision,int_msg);
                           delete [] decision;//free memory for the decision array
                       }

                       if(client_attached==1){
                           add_to_pit( chunk, int_msg->getArrivalGate()->getIndex() );
                       }else{
                           if(int_msg->getI_type()!=2){
                               add_to_pit( chunk, int_msg->getArrivalGate()->getIndex() );
                           }
                       }


}

void core_layer::handle_interest_AR(ccn_interest *int_msg){
    int custo_r;//custodian router
              int c_name = int_msg->get_name();//get the chunk name
              chunk_t chunk = int_msg->getChunk();//get the chunk number
              double int_btw = int_msg->getBtw();

              custo_r=CHring[c_name % num_of_vr];//get custodian router name
              //cout<<"custodian router "<<custo_r<<endl;
              int_msg->setCusto(custo_r);//add the custodian router name to Interest
              if (custo_r==current_r){
                  if(int_msg->getI_type()!=2){// interest type is 0 and 1
                  if(ContentStore->lookup(chunk)){
                                   ccn_data* data_msg = compose_data(chunk);
                                             data_msg->setHops(0);
                                             data_msg->setBtw(int_btw); //Copy the highest betweenness
                                             data_msg->setTarget(getIndex());
                                             data_msg->setFound(true);
                                             data_msg->setCapacity(int_msg->getCapacity());
                                             data_msg->setTSI(int_msg->getHops());
                                             data_msg->setTSB(1);
                                             if (core_check!=1){data_msg->setCusto_check(1);}//to know whether the content is already cached at the custodian router or not

                                          send_data(data_msg,"face$o", int_msg->getArrivalGate()->getIndex(), __LINE__);

                              } else{
                                  handle_statCS(int_msg);//miss stat
                                  handle_forward(int_msg);
                  }
              }else{//interest type is 2
                  if(ContentStore->lookup(chunk)){

                  }else{
                      handle_statCS(int_msg);//miss stat
                  }

              }
              }else{//the current router is access router but not custodian

            if (RContentStore->lookup(chunk)){//the requested content is inside the cache
                   ccn_data* data_msg = compose_data(chunk);
                             data_msg->setHops(0);
                             data_msg->setBtw(int_btw); //Copy the highest betweenness
                             data_msg->setTarget(getIndex());
                             data_msg->setFound(true);
                             data_msg->setCapacity(int_msg->getCapacity());
                             data_msg->setTSI(int_msg->getHops());
                             data_msg->setTSB(1);
                             send_data(data_msg,"face$o", int_msg->getArrivalGate()->getIndex(), __LINE__);

                }else{//the requested content is not inside the cache
                    handle_statRCS(int_msg);
                    handle_forward(int_msg);

            }

        }

    }
void core_layer::handle_interest_Core(ccn_interest *int_msg){
    int c_name = int_msg->get_name();//get the chunk name
                  chunk_t chunk = int_msg->getChunk();//get the chunk number
                  double int_btw = int_msg->getBtw();
                  if (ContentStore->lookup(chunk)){
                      //
                      //a) Check in your Content Store
                      //
                      ccn_data* data_msg = compose_data(chunk);

                      data_msg->setHops(0);
                      data_msg->setBtw(int_btw); //Copy the highest betweenness
                      data_msg->setTarget(getIndex());
                      data_msg->setFound(true);

                      data_msg->setCapacity(int_msg->getCapacity());
                      data_msg->setTSI(int_msg->getHops());
                      data_msg->setTSB(1);

                      //<aa> I transformed send in send_data</aa>
                      send_data(data_msg,"face$o", int_msg->getArrivalGate()->getIndex(), __LINE__);



                  } else if ( my_bitmask & __repo(int_msg->get_name() ) ){
                     // handle_statCS(int_msg);
                  //
                  //b) Look locally (only if you own a repository)
                  // we are mimicking a message sent to the repository
                  //
                      ccn_data* data_msg = compose_data(chunk);

                      //<aa>
                      data_msg->setPrice(repo_price);     // I fix in the data msg the cost of the object
                                                      // that is the price of the repository
                      //</aa>


                      repo_interest++;
                      repo_load++;

                      data_msg->setHops(1);
                      data_msg->setTarget(getIndex());
                      data_msg->setBtw(std::max(my_btw,int_btw));

                      data_msg->setCapacity(int_msg->getCapacity());
                      data_msg->setTSI(int_msg->getHops() + 1);
                      data_msg->setTSB(1);
                      data_msg->setFound(true);
                 data_msg->setC_decision(1);
                 //cdecision_CS(data_msg);
                      ContentStore->store(data_msg);

                      //<aa> I transformed send in send_data</aa>
                      send_data(data_msg,"face$o",int_msg->getArrivalGate()->getIndex(),__LINE__);


                 } else {
                      //
                      //c) Put the interface within the PIT (and follow your FIB)
                      //



                      unordered_map < chunk_t , pit_entry >::iterator pitIt = PIT.find(chunk);

                      //<aa>
                      bool i_will_forward_interest = false;
                      //</aa>

                      //<aa> Insert a new PIT entry for this object, if not present. If present and invalid, reset the
                      // old entry. If present and valid, do nothing </aa>
                      if (
                          //<aa> there is no such an entry in the PIT thus I have to forward the interest</aa>
                          pitIt==PIT.end()

                          //<aa> There is a PIT entry but it is invalid (the PIT entry has been invalidated by client
                          // because a timer expired and the object has not been found </aa>
                          || (pitIt != PIT.end() && int_msg->getNfound() )

                          //<aa> Too much time has been passed since the old PIT entry was added </aa>
                          || simTime() - PIT[chunk].time > 2*RTT
                      ){
                          //<aa> Replaces the lines
                          //      bool * decision = strategy->get_decision(int_msg);
                          //      handle_decision(decision,int_msg);
                          //      delete [] decision;//free memory for the decision array
                          i_will_forward_interest = true;
                          //</aa>

                          if (pitIt!=PIT.end())
                              PIT.erase(chunk);
                          //<aa>Last time this entry has been updated is now</aa>
                          PIT[chunk].time = simTime();
                      }

                      //<aa>
                      if (int_msg->getTarget() == getIndex() )
                      {   // I am the target of this interest but I have no more the object
                          // Therefore, this interest cannot be aggregated with the others
                          int_msg->setAggregate(false);
                      }

                      if ( !interest_aggregation || int_msg->getAggregate()==false )
                          i_will_forward_interest = true;

                      if (i_will_forward_interest)
                      {   bool * decision = strategy->get_decision(int_msg);
                          handle_decision(decision,int_msg);
                          delete [] decision;//free memory for the decision array
                      }


                      //<aa> The following line will add the origin interface of the interest
                      //      msg to the PIT </aa>
                      add_to_pit( chunk, int_msg->getArrivalGate()->getIndex() );

                  }



}

void core_layer::handle_interest_NARSVR(ccn_interest *int_msg){
    int custo_r;//custodian router
              int c_name = int_msg->get_name();//get the chunk name
              chunk_t chunk = int_msg->getChunk();//get the chunk number
              double int_btw = int_msg->getBtw();

              custo_r=CHring[c_name % num_of_vr];//get custodian router name
              //cout<<"custodian router "<<custo_r<<endl;
              int_msg->setCusto(custo_r);//add the custodian router name to Interest

       if(custo_r==current_r){//current router is custodian
           if(int_msg->getI_type()==2){
               ContentStore->lookup(chunk);
           }else if(ContentStore->lookup(chunk)){
               ccn_data* data_msg = compose_data(chunk);
                                         data_msg->setHops(0);
                                         data_msg->setBtw(int_btw); //Copy the highest betweenness
                                         data_msg->setTarget(getIndex());
                                         data_msg->setFound(true);
                                         data_msg->setCapacity(int_msg->getCapacity());
                                         data_msg->setTSI(int_msg->getHops());
                                         data_msg->setTSB(1);
                                         if (core_check!=1){data_msg->setCusto_check(1);}//to know whether the content is already cached at the custodian router or not
                                         send_data(data_msg,"face$o", int_msg->getArrivalGate()->getIndex(), __LINE__);


            }else if(my_bitmask & __repo(int_msg->get_name())){
                //The current router is connected with the content server.
                //While the requested chunk is not located at CS, the Interest is forwarded to the connected content server.
                handle_statCS(int_msg);

                ccn_data* data_msg = compose_data(chunk);

                data_msg->setPrice(repo_price);

                repo_interest++;
                repo_load++;

                data_msg->setHops(1);
                data_msg->setTarget(getIndex());
                data_msg->setBtw(std::max(my_btw,int_btw));
                data_msg->setCapacity(int_msg->getCapacity());
                data_msg->setTSI(int_msg->getHops() + 1);
                data_msg->setTSB(1);
                data_msg->setFound(true);
                data_msg->setCusto_check(1);

                cdecision_CS(data_msg);
if(data_msg->getC_decision()==1){
                Cstat.erase(chunk);
                ContentStore->store(data_msg);
}
                send_data(data_msg,"face$o",int_msg->getArrivalGate()->getIndex(),__LINE__);


            }

        }else{//non access router 0 and it is not custodian
if(int_msg->getI_type()==1){
    if( my_bitmask & __repo(int_msg->get_name())){//current router 0 is non custodian but the receiving interest type is 1(DF)

                          ccn_data* data_msg = compose_data(chunk);
                          data_msg->setPrice(repo_price);

                          repo_interest++;
                          repo_load++;

                          data_msg->setHops(1);
                          data_msg->setTarget(getIndex());
                          data_msg->setBtw(std::max(my_btw,int_btw));
                          data_msg->setCapacity(int_msg->getCapacity());
                          data_msg->setTSI(int_msg->getHops() + 1);
                          data_msg->setTSB(1);
                          data_msg->setFound(true);
                          if (core_check!=1){data_msg->setCusto_check(1);}
                          send_data(data_msg,"face$o",int_msg->getArrivalGate()->getIndex(),__LINE__);
                              int i = 0;
                              interface_t interfaces = 0;
                              unordered_map < chunk_t , pit_entry >::iterator pitIt = PIT.find(chunk);

                              //If someone had previously requested the data
                                  if ( pitIt != PIT.end() )
                                      {
                                       interfaces = (pitIt->second).interfaces;//get interface list
                                       i = 0;
                                       while (interfaces){
                                       if ( interfaces & 1 ){

                                        send_data(data_msg->dup(), "face$o", i,__LINE__ ); //follow bread crumbs back

                                                       }
                                                       i++;
                                                       interfaces >>= 1;
                                                  }
                                        }PIT.erase(chunk); //erase pending interests for that data file

              }


}else if(int_msg->getI_type()==0){//Interest type is 0 and forward the interest to custodian router
    handle_statCS(int_msg);
    handle_forward(int_msg);


                     }
        }

}
void core_layer::handle_interest_8(ccn_interest *int_msg){
    int custo_r;//custodian router
    int c_name = int_msg->get_name();//get the chunk name
    chunk_t chunk = int_msg->getChunk();//get the chunk number
    double int_btw = int_msg->getBtw();
    custo_r=CHring[c_name % num_of_vr];//get custodian router name
    //cout<<"custodian router "<<custo_r<<endl;
    int_msg->setCusto(custo_r);//add the custodian router name to Interest

    if (custo_r==current_r){
        if(int_msg->getI_type()!=2){// interest type is 0 and 1
              if(ContentStore->lookup(chunk)){
                        ccn_data* data_msg = compose_data(chunk);
                                  data_msg->setHops(0);
                                  data_msg->setBtw(int_btw); //Copy the highest betweenness
                                  data_msg->setTarget(getIndex());
                                  data_msg->setFound(true);
                                  data_msg->setCapacity(int_msg->getCapacity());
                                  data_msg->setTSI(int_msg->getHops());
                                  data_msg->setTSB(1);
                                  //if (core_check!=1){data_msg->setCusto_check(1);}//to know whether the content is already cached at the custodian router or not
                                  send_data(data_msg,"face$o", int_msg->getArrivalGate()->getIndex(), __LINE__);

                              } else{
                                  handle_statCS(int_msg);//miss stat


                                  chunk_t chunk = int_msg->getChunk();//get the chunk number
                                  unordered_map < chunk_t , pit_entry >::iterator pitIt = PIT.find(chunk);

                                         // if(pitIt != PIT.end()){
                                             //int_msg->setI_type(0);
                                          //}else{
                                          int_msg->setI_type(1);
                                          PIT[chunk].custo_marking =1;//to store the chunk at custodian
                                         // }

                                 // cout<<"current r "<<current_r<<" custodian "<<int_msg->getCusto()<<"->interest type "<<int_msg->getI_type()<<endl;
                                                           bool i_will_forward_interest = false;
                                                           if (pitIt==PIT.end() //|| int_msg->getI_type()!=0
                                                                   //|| (pitIt != PIT.end() && int_msg->getNfound() )
                                                                   || simTime() - PIT[chunk].time > 2*RTT){
                                                               i_will_forward_interest = true;


                                                                   if (pitIt!=PIT.end()){// && int_msg->getI_type()!=1){
                                                                       PIT.erase(chunk);

                                                                   }
                                                                   PIT[chunk].time = simTime();


                                                              }

                                                     if (int_msg->getTarget() == getIndex() )
                                                     {
                                                         int_msg->setAggregate(false);
                                                     }

                                                     if ( !interest_aggregation || int_msg->getAggregate()==false )
                                                         i_will_forward_interest = true;

                                                     if (i_will_forward_interest)
                                                     {   bool * decision = strategy->get_decision(int_msg);
                                                         handle_decision(decision,int_msg);
                                                         delete [] decision;//free memory for the decision array
                                                     }



                                                             add_to_pit( chunk, int_msg->getArrivalGate()->getIndex() );



                  }
              }else if(int_msg->getI_type()==2){
                  if(ContentStore->fake_lookup(chunk)){
                      ContentStore->lookup(chunk);//to count hit
                  }else{
                      ContentStore->lookup(chunk);//to count miss
                      handle_statCS(int_msg);//miss stat
                  }
              }


              }else{
                  handle_forward(int_msg);
              }
}

void core_layer::handle_interest_NAR(ccn_interest *int_msg){
    int custo_r;//custodian router
              int c_name = int_msg->get_name();//get the chunk name
              chunk_t chunk = int_msg->getChunk();//get the chunk number
              double int_btw = int_msg->getBtw();

              custo_r=CHring[c_name % num_of_vr];//get custodian router name
              //cout<<"custodian router "<<custo_r<<endl;
              int_msg->setCusto(custo_r);//add the custodian router name to Interest
//if Type 0  check CS
//if Type 2, update the statistic no data return
              if (custo_r==current_r){
                  if(int_msg->getI_type()!=2){// interest type is 0 and 1
                  if(ContentStore->lookup(chunk)){
                                   ccn_data* data_msg = compose_data(chunk);
                                             data_msg->setHops(0);
                                             data_msg->setBtw(int_btw); //Copy the highest betweenness
                                             data_msg->setTarget(getIndex());
                                             data_msg->setFound(true);
                                             data_msg->setCapacity(int_msg->getCapacity());
                                             data_msg->setTSI(int_msg->getHops());
                                             data_msg->setTSB(1);
                                             if (core_check!=1){data_msg->setCusto_check(1);}//to know whether the content is already cached at the custodian router or not

                                          send_data(data_msg,"face$o", int_msg->getArrivalGate()->getIndex(), __LINE__);

                              } else{
                                  handle_statCS(int_msg);//miss stat


                                                                   chunk_t chunk = int_msg->getChunk();//get the chunk number
                                                                   unordered_map < chunk_t , pit_entry >::iterator pitIt = PIT.find(chunk);
                                  int_msg->setI_type(1);
                                                                        PIT[chunk].custo_marking =1;//to store the chunk at custodian
                                                                       // }

                                                               // cout<<"current r "<<current_r<<" custodian "<<int_msg->getCusto()<<"->interest type "<<int_msg->getI_type()<<endl;
                                                                                         bool i_will_forward_interest = false;
                                                                                         if (pitIt==PIT.end()//|| int_msg->getI_type()!=0
                                                                                                 || (pitIt != PIT.end() && int_msg->getNfound() )
                                                                                                 || simTime() - PIT[chunk].time > 2*RTT){
                                                                                             i_will_forward_interest = true;


                                                                                                 if (pitIt!=PIT.end()){// && int_msg->getI_type()!=1){
                                                                                                     PIT.erase(chunk);

                                                                                                 }
                                                                                                 PIT[chunk].time = simTime();


                                                                                            }

                                                                                   if (int_msg->getTarget() == getIndex() )
                                                                                   {
                                                                                       int_msg->setAggregate(false);
                                                                                   }

                                                                                   if ( !interest_aggregation || int_msg->getAggregate()==false )
                                                                                       i_will_forward_interest = true;

                                                                                   if (i_will_forward_interest)
                                                                                   {   bool * decision = strategy->get_decision(int_msg);
                                                                                       handle_decision(decision,int_msg);
                                                                                       delete [] decision;//free memory for the decision array
                                                                                   }



                                                                                           add_to_pit( chunk, int_msg->getArrivalGate()->getIndex() );

                  }
              }else if(int_msg->getI_type()==2){
                  if(ContentStore->fake_lookup(chunk)){
                      ContentStore->lookup(chunk);//to count hit
                  }else{
                      ContentStore->lookup(chunk);//to count miss
                      handle_statCS(int_msg);//miss stat
                  }
              }


              }else{
                  handle_forward(int_msg);
              }
    }





/*
 * Handle incoming data packets. First check within the PIT if there are
 * interfaces interested for the given content, then (try to) store the object
 * within your content store. Finally propagate the interests towards all the
 * interested interfaces.
 */
void core_layer::handle_data(ccn_data *data_msg)
{
    int incoming_data_face=data_msg->getArrivalGate()->getIndex();//get incoming data interface to solve the return data to the sent node
    int i = 0;
    interface_t interfaces = 0;
    chunk_t chunk = data_msg -> getChunk(); //Get information about the file

    unordered_map < chunk_t , pit_entry >::iterator pitIt = PIT.find(chunk);

    //<aa>
    #ifdef SEVERE_DEBUG
        int copies_sent = 0;
    #endif
    //</aa>


    //If someone had previously requested the data
    if ( pitIt != PIT.end() )
    {
        if(current_r==0 && PIT[chunk].custo_marking==1){
           // cdecision_CS(data_msg);
            data_msg->setC_decision(1);
            ContentStore->store(data_msg);
    }else{

        if(PIT[chunk].custo_marking==1){

                cdecision_CS(data_msg);

            if(data_msg->getC_decision()==1){

                Cstat.erase(chunk);
                ContentStore->store(data_msg);
                 }
        }

        if(PIT[chunk].custo_marking==2){
            cdecision_RCS(data_msg);
            if(data_msg->getC_decision()==1){
                Rstat.erase(chunk);
              RContentStore->store(data_msg);
          }



        }

    }





        interfaces = (pitIt->second).interfaces;//get interface list
        i = 0;
        while (interfaces){
            if ( interfaces & 1 ){
                //<aa> I transformed send in send_data</aa>
                if(i!=incoming_data_face){
                send_data(data_msg->dup(), "face$o", i,__LINE__ ); //follow bread crumbs back
                }

                //<aa>
                #ifdef SEVERE_DEBUG
                    copies_sent++;
                #endif
                //</aa>
            }
            i++;
            interfaces >>= 1;
        }
    }
    //<aa>
    // Otherwise the data are unrequested
    #ifdef SEVERE_DEBUG
        else unsolicited_data++;
    #endif


    PIT.erase(chunk); //erase pending interests for that data file

    //<aa>
    #ifdef SEVERE_DEBUG
    check_if_correct(__LINE__);
    #endif
    //</aa>
}


void core_layer::handle_decision(bool* decision,ccn_interest *interest){
    //<aa>
    #ifdef SEVERE_DEBUG
    bool interest_has_been_forwarded = false;
    #endif
    //</aa>

    if (my_btw > interest->getBtw())
        interest->setBtw(my_btw);

    for (int i = 0; i < __get_outer_interfaces(); i++)
    {
        //<aa>
        #ifdef SEVERE_DEBUG
            if (decision[i] == true && __check_client(i) )
            {
                std::stringstream msg;
                msg<<"I am node "<< getIndex()<<" and the interface supposed to give"<<
                    " access to chunk "<< interest->getChunk() <<" is "<<i
                    <<". This is impossible "<<
                    " since that interface is to reach a client and you cannot access"
                    << " a content from a client ";
                severe_error(__FILE__, __LINE__, msg.str().c_str() );
            }
        #endif
        //</aa>

        if (decision[i] == true && !__check_client(i)
            //&& interest->getArrivalGate()->getIndex() != i
        ){
            sendDelayed(interest->dup(),interest->getDelay(),"face$o",i);
            #ifdef SEVERE_DEBUG
            interest_has_been_forwarded = true;
            #endif
        }
    }
    //<aa>
    #ifdef SEVERE_DEBUG
        if (! interest_has_been_forwarded)
        {
            int affirmative_decision_from_arrival_gate = 0;
            int affirmative_decision_from_client = 0;
            int last_affermative_decision = -1;

            for (int i = 0; i < __get_outer_interfaces(); i++)
            {
                if (decision[i] == true)
                {
                    if ( __check_client(i) ){
                        affirmative_decision_from_client++;
                        last_affermative_decision = i;
                    }
                    if ( interest->getArrivalGate()->getIndex() == i ){
                        affirmative_decision_from_arrival_gate++;
                        last_affermative_decision = i;
                    }
                }
            }
            std::stringstream msg;
            msg<<"I am node "<< getIndex()<<" and interest for chunk "<<
                interest->getChunk()<<" has not been forwarded. "<<
                ". One of the possible repositories of this chunk is "<<
                interest->get_repos()[0] <<" and the target of the interest is "<<
                interest->getTarget() <<
                ". affirmative_decision_for_client = "<<
                affirmative_decision_from_client<<
                ". affirmative_decision_for_arrival_gate = "<<
                affirmative_decision_from_arrival_gate<<
                ". I would have sent the interest to interface "<<
                last_affermative_decision;
            severe_error(__FILE__, __LINE__, msg.str().c_str() );
        }
    #endif
}


bool core_layer::check_ownership(vector<int> repositories){
    bool check = false;
    if (find (repositories.begin(),repositories.end(),getIndex()) != repositories.end())
    check = true;
    return check;
}



/*
 * Compose a data response packet
 */
ccn_data* core_layer::compose_data(uint64_t response_data){
    ccn_data* data = new ccn_data("data",CCN_D);
    data -> setChunk (response_data);
    data -> setHops(0);
    data->setTimestamp(simTime());
    return data;
}
/*
 * Cache decision caculation
 */
void core_layer::cdecision_CS(ccn_data *data_msg){
    chunk_t chunk = data_msg -> getChunk(); //Get information about the file

       if(Cstat[chunk].x==1){
          // cout<<"cached"<<endl;
           data_msg->setC_decision(1);
           data_msg->setPre_reqcost(Cstat[chunk].miss_cost);
           data_msg->setTresh(Cstat[chunk].cache_miss);
           if(Rstat[chunk].cache_miss<0){
               cout<<"error r miss "<<Rstat[chunk].cache_miss<<endl;
           }

       }else{//cout<<"not-cached"<<endl;
                  data_msg->setC_decision(0);
                  data_msg->setPre_reqcost(0);
                  data_msg->setTresh(Cstat[chunk].cache_miss);
              }
       //cout<<"--------------"<<endl;
       //cout<<"C miss "<<Cstat[chunk].cache_miss<<endl;
}

void core_layer::cdecision_RCS(ccn_data *data_msg){
    chunk_t chunk = data_msg -> getChunk(); //Get information about the file
if(RContentStore->full()==true){
    // cout<<"RCS is full"<<endl;
    if(data_msg->getCusto_check()!=1){
        Rstat.erase(chunk);
        data_msg->setC_decision(0);
    } else{

       // cout<<"req "<< Rstat[chunk].miss_cost << " caching "<<RContentStore->getcaching_cost()<<endl;
        if(Rstat[chunk].x==1){
            //cout<<"not-cached"<<endl;
                 data_msg->setC_decision(1);
                 //cout<<"miss cost rcs decision "<<Rstat[chunk].miss_cost<<endl;
                 data_msg->setPre_reqcost(Rstat[chunk].miss_cost);
                 data_msg->setTresh(Rstat[chunk].cache_miss);
                 if(Rstat[chunk].cache_miss<0){
                     cout<<"error r miss "<<Rstat[chunk].cache_miss<<endl;
                 }
             }else{
                 //cout<<"cached"<<endl;
                 data_msg->setC_decision(0);
                 data_msg->setPre_reqcost(0);
                 data_msg->setTresh(Rstat[chunk].cache_miss);
             }

    }

}else{

    if(Rstat[chunk].x==1){
        //cout<<"not-cached"<<endl;
             data_msg->setC_decision(1);
             //cout<<"miss cost rcs decision "<<Rstat[chunk].miss_cost<<endl;
             data_msg->setPre_reqcost(Rstat[chunk].miss_cost);
             data_msg->setTresh(Rstat[chunk].cache_miss);
         }else{
             //cout<<"cached"<<endl;
             data_msg->setC_decision(0);
             data_msg->setPre_reqcost(0);
             data_msg->setTresh(Rstat[chunk].cache_miss);
         }

}
//cout<<"--------------"<<endl;
//cout<<"R miss "<<Rstat[chunk].cache_miss<<endl;
}

/*
 * Clear local statistics
 */
void core_layer::clear_stat(){
    repo_interest = 0;
    interests = 0;
    data = 0;

    //<aa>
    repo_interest = 0;
    repo_load = 0;
    ContentStore->set_decision_yes(0);
    ContentStore->set_decision_no(0);
    Cstat.clear();


       #ifdef SEVERE_DEBUG
    unsolicited_data = 0;
    discarded_interests = 0;
    unsatisfied_interests = 0;
    interests_satisfied_by_cache = 0;
    check_if_correct(__LINE__);
    #endif
    //</aa>
}

//<aa>
#ifdef SEVERE_DEBUG
void core_layer::check_if_correct(int line)
{
    if (repo_load != interests - discarded_interests - unsatisfied_interests
        -interests_satisfied_by_cache)
    {
            std::stringstream msg;
            msg<<"node["<<getIndex()<<"]: "<<
                "repo_load="<<repo_load<<"; interests="<<interests<<
                "; discarded_interests="<<discarded_interests<<
                "; unsatisfied_interests="<<unsatisfied_interests<<
                "; interests_satisfied_by_cache="<<interests_satisfied_by_cache;
            severe_error(__FILE__, line, msg.str().c_str() );
    }

    if (!it_has_a_repo_attached && repo_load>0 )
    {
            std::stringstream msg;
            msg<<"node["<<getIndex()<<"] has no repo attached. "<<
                "repo_load=="<<repo_load<<
                "; repo_interest=="<<repo_interest;
            severe_error(__FILE__, line, msg.str().c_str() );
    }

    if (    ContentStore->get_decision_yes() + ContentStore->get_decision_no() +
                        (unsigned) unsolicited_data
                        !=  (unsigned) data + repo_load
    ){
                    std::stringstream ermsg;
                    ermsg<<"caches["<<getIndex()<<"]->decision_yes="<<ContentStore->get_decision_yes()<<
                        "; caches[i]->decision_no="<< ContentStore->get_decision_no()<<
                        "; cores[i]->data="<< data<<
                        "; cores[i]->repo_load="<< repo_load<<
                        "; cores[i]->unsolicited_data="<< unsolicited_data<<
                        ". The sum of "<< "decision_yes + decision_no + unsolicited_data must be data";
                    severe_error(__FILE__,line,ermsg.str().c_str() );
    }
} //end of check_if_correct(..)
#endif
//</aa>

//<aa>
double core_layer::get_repo_price()
{
    #ifdef SEVERE_DEBUG
    if (!is_it_initialized)
    {
            std::stringstream msg;
            msg<<"I am node "<< getIndex()<<". Someone called this method before I was"
                " initialized";
            severe_error(__FILE__, __LINE__, msg.str().c_str() );
    }
    #endif

    return repo_price;
}

void core_layer::add_to_pit(chunk_t chunk, int gateindex)
{
    #ifdef SEVERE_DEBUG
    check_if_correct(__LINE__);

    if (gateindex > gateSize("face$o")-1 )
    {
        std::stringstream msg;
        msg<<"You are inserting a pit entry related to interface "<<gateindex<<
            ". But the number of ports is "<<gateSize("face$o");
        severe_error(__FILE__, __LINE__, msg.str().c_str() );
    }

    if (gateindex > (int) sizeof(interface_t)*8-1 )
    {
        std::stringstream msg;
        msg<<"You are inserting a pit entry related to interface "<<gateindex<<
            ". But the maximum interface "
            <<"number manageable by ccnsim is "<<sizeof(interface_t)*8-1 <<" beacause the type of "
            <<"interface_t is of size "<<sizeof(interface_t)<<". You can change the definition of "
            <<"interface_t (in ccnsim.h) to solve this issue and recompile";
        severe_error(__FILE__, __LINE__, msg.str().c_str() );
    }
    #endif

    __sface( PIT[chunk].interfaces , gateindex );

    #ifdef SEVERE_DEBUG

    unsigned long long bit_op_result = (interface_t)1 << gateindex;
    if ( bit_op_result > pow(2,gateSize("face$o")-1) )
    {
                printf("ATTTENZIONE bit_op_result %llX\n", bit_op_result);
                std::stringstream ermsg;
                ermsg<<"I am node "<<getIndex()<<", bit_op_result="<<bit_op_result <<
                    " while the number of ports is "<<
                    gateSize("face$o")<<" and the max number that I should observe is "<<
                    pow(2,gateSize("face$o") )-1;
                ermsg<<". (1<<34)="<< (1<<34);
                severe_error(__FILE__,__LINE__,ermsg.str().c_str() );
    }

    check_if_correct(__LINE__);
    #endif
}

int    core_layer::send_data(ccn_data* msg, const char *gatename, int gateindex, int line_of_the_call)
{
    if (gateindex > gateSize("face$o")-1 )
    {
        std::stringstream msg;
        msg<<"I am node "<<getIndex() <<". Line "<<line_of_the_call<<
            " commands you to send a packet to interface "<<gateindex<<
            ". But the number of ports is "<<gateSize("face$o");
        severe_error(__FILE__, __LINE__, msg.str().c_str() );
    }

    #ifdef SEVERE_DEBUG
    if ( gateindex > (int) sizeof(interface_t)*8-1 )
    {
        std::stringstream msg;
        msg<<"You are trying to send a packet through the interface gateindex. But the maximum interface "
            <<"number manageable by ccnsim is "<<sizeof(interface_t)*8-1 <<" beacause the type of "
            <<"interface_t is of size "<<sizeof(interface_t)<<". You can change the definition of "
            <<"interface_t (in ccnsim.h) to solve this issue and recompile";
        severe_error(__FILE__, __LINE__, msg.str().c_str() );
    }

    client* c = __get_attached_client(gateindex);
    if (c)
    {    //There is a client attached to that port
        if ( !c->is_waiting_for( msg->get_name() ) )
        {
            std::stringstream msg;
            msg<<"I am node "<< getIndex()<<". I am sending a data to the attached client that is not "<<
                " waiting for it. This is not necessarily an error, as this data could have been "
                <<" requested by the client and the client could have retrieved it before and now"
                <<" it may be fine and not wanting the data anymore. If it is the case, "<<
                "ignore this message ";
            debug_message(__FILE__, __LINE__, msg.str().c_str() );
        }

        if ( !c->is_active() )
        {
            std::stringstream msg;
            msg<<"I am node "<< getIndex()<<". I am sending a data to the attached client "<<
                ", that is not active, "<<
                " through port "<<gateindex<<". This was commanded in line "<< line_of_the_call;
            severe_error(__FILE__, __LINE__, msg.str().c_str() );
        }
    }
    #endif
    return send (msg, gatename, gateindex);
}
//</aa>

