package networks;

network abilene_network extends base_network{

    parameters:
        //Number of ccn nodes
    	n = 16;

connections allowunconnected:
    node[1].face++ <--> { delay = 3.80ms; } <--> node[0].face++;
	node[2].face++ <--> { delay = 3.80ms; } <--> node[0].face++;
	node[3].face++ <--> { delay = 3.80ms; } <--> node[0].face++;
	node[4].face++ <--> { delay = 3.80ms; } <--> node[1].face++;
	node[5].face++ <--> { delay = 3.80ms; } <--> node[4].face++;
	node[6].face++ <--> { delay = 3.80ms; } <--> node[4].face++;
	node[7].face++ <--> { delay = 3.80ms; } <--> node[4].face++;
	node[8].face++ <--> { delay = 3.80ms; } <--> node[2].face++;
	node[9].face++ <--> { delay = 3.80ms; } <--> node[8].face++;
	node[10].face++ <--> { delay = 3.80ms; } <--> node[8].face++;
	node[11].face++ <--> { delay = 3.80ms; } <--> node[8].face++;	
	node[12].face++ <--> { delay = 3.80ms; } <--> node[3].face++;
	node[13].face++ <--> { delay = 3.80ms; } <--> node[12].face++;
	node[14].face++ <--> { delay = 3.80ms; } <--> node[12].face++;
	node[15].face++ <--> { delay = 3.80ms; } <--> node[12].face++;
	
	
	
}