/*
 * ccnSim is a scalable chunk-level simulator for Content Centric
 * Networks (CCN), that we developed in the context of ANR Connect
 * (http://www.anr-connect.org/)
 *
 * People:
 *    Giuseppe Rossini (lead developer, mailto giuseppe.rossini@enst.fr)
 *    Andrea Araldo (developer, mailto andrea.araldo@gmail.com)
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
#include "strategy_layer.h"
#include <sstream>
#include "error_handling.h"
ifstream strategy_layer::fdist;
ifstream strategy_layer::frouting;



void strategy_layer::initialize()
{
    for (int i = 0; i<getParentModule()->gateSize("face$o");i++)
    {
	int index ;
	if (!__check_client(i))
		//<aa> If the module attached to the ith interface is a client, 
		//get the index that identifies that module</aa>
	    index = getParentModule()->gate("face$o",i)->getNextGate()->getOwnerModule()->getIndex();
        gatelu[index] = i;
    }
    
    
    string fileradix = par("routing_file").stringValue();
    string filerout = fileradix+".rou";
    string filedist = fileradix+".dist";
    if (fileradix!= ""){
	if (!fdist.is_open()){
	    fdist.open(filedist.c_str());
	    frouting.open(filerout.c_str());
	}
	populate_from_file(); //Building forwarding table 
    }else 
	populate_routing_table(); //Building forwarding table 
	
	//<aa>
//	max_FIB_entries = (int) par("max_FIB_entries").longValue();
	//</aa>
}

void strategy_layer::finish(){
    fdist.close();
    frouting.close();
}

//Common to each strategy layer. Populate the host-centric routing table.
//That comes from a centralized process based on the ctopology class.
void strategy_layer::populate_routing_table(){

    deque<int> p;
    cTopology topo;
    vector<string> types;
    //Extract topology map
    types.push_back("modules.node.node");
    topo.extractByNedTypeName( types );
    cTopology::Node *node = topo.getNode( getParentModule()->getIndex() ); //iterator node
    //int rand_out; //<aa> I disabled this line</aa>
    //As the node topology is defined as a vector of nodes (see Omnet++ manual), cTopology 
    //associates the node i with the node whose Index is i.

	//<aa> Find the paths toward all the other nodes <aa>
    for (int dest = 0; dest < topo.getNumNodes(); dest++)
    {
    	//<aa>If the module d is not myself</aa>
		if (dest != getParentModule()->getIndex())
		{
			cTopology::Node *to   = topo.getNode( dest ); //destination node
			topo.weightedMultiShortestPathsTo( to );
			if ( node->getNumPaths() == 0)
			{
				cout << "strategy_layer.cc:"<<__LINE__<<": ERROR: No paths connecting"
					<<" node "<<getParentModule()->getIndex() <<" to node "<< dest <<
					" have been found"<<endl;
				exit(-5);
			}

			//<aa>
			// Choose the paths toward dest
			vector<int> paths = choose_paths(node->getNumPaths() );
			//here is the problem
			for (unsigned int i=0; i<paths.size(); i++){
				// The output gate and the distance to reach dest
				int output_gate = node->getPath( i )->getLocalGate()->getIndex();
				int distance = node->getDistanceToTarget();
				add_FIB_entry(dest, output_gate, distance);
			}
			//</aa>
			

			//cout<<getParentModule()->gate("face$o",FIB[d].id)->getNextGate()->getOwnerModule()->getIndex()+1<<" ";
			//cout<<FIB[d].len<<" ";
		}else
			;//cout<<getParentModule()->getIndex()+1<<" ";
			//cout<<0<<" ";
    }

}

void strategy_layer::populate_from_file(){
    string rline, dline;
    getline(frouting, rline);
    getline(fdist, dline);

    istringstream diis (dline);
    istringstream riis (rline);
    int n = getAncestorPar("n");

    int cell1, cell2;
    int k = 0;

    while (k<n){
	riis>>cell1;
	diis>>cell2;
	int out_interface = gatelu[cell1-1];
	int distance = cell2;
	add_FIB_entry(k, out_interface, distance);
	k++;
    }
}

//<aa>
/**
 * distance: the distance of the path to reach the destination node passing through
 * the specified interface
 */
void strategy_layer::add_FIB_entry(
	int destination_node_index, int interface_index, int distance)
{
	int_f FIB_entry;
	FIB_entry.id = interface_index;
	FIB_entry.len = distance;
	//Inserimento roba
	FIB[destination_node_index].push_back(FIB_entry);
	
	#ifdef SEVERE_DEBUG
	vector<int_f> entry_vec = FIB[destination_node_index];
	int_f entry_just_added = entry_vec.back();
	int output_gates = getParentModule()->gateSize("face$o");
	if (entry_just_added.id >= output_gates){
		std::stringstream msg; msg<<"gate "<<entry_just_added.id<<" is invalid"<<
				". gate_size is "<< output_gates;
		severe_error(__FILE__,__LINE__, msg.str().c_str() );
	}
	#endif
}

const vector<int_f> strategy_layer::get_FIB_entries(
		int destination_node_index)
{
	vector<int_f> entries = FIB[destination_node_index] ;
	return entries;
	//return FIB[destination_node_index];
}


//</aa>
