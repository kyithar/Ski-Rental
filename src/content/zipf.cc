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
#include "zipf.h"
#include <iostream>
#include <cmath>

using namespace std;

//Initialize the vector representing the zipf cdf distribution. As this vector can be of really high dimension 
//the initialization is done just one time for every request generator who needs it
void zipf_distribution::zipf_initialize(){
    //Return if the Zipf cdf has been already initialized
    if (cdfZipf.size() != 0)
	return;

    double c = 0;
    //int q = 0;

    cout<<"Initializing Zipf..."<<endl;
    //Otherwise initialize it
    cdfZipf.resize(F + 1);
    cdfZipf[0] = -1;

    //Normalization constant computation
    c = 0;          
    for (int i=1; i<=F; i++){
	c += (1.0 /  pow(i+q,alpha));
	cdfZipf[i] = c; 
    }
    c = 1.0 / c;

	//<aa>
	normalization_constant = c;
	//</aa>

    //Normalize Zipf distribution
    for (vector<double>::iterator it = cdfZipf.begin() + 1; it!=cdfZipf.end();it++)
	(*it) *= c;

    cout<<"Zipf initialization completed"<<endl;



}

//<aa>
double zipf_distribution::get_normalization_constant(){
	return normalization_constant;
}
//</aa>


//Binary search zipf
unsigned int zipf_distribution::value(double p){

    unsigned int upper, lower,atry, last_try;
    
    lower = -1;
    upper = cdfZipf.size()-1;
    atry = -1;
    last_try = -1;

    //binary search to find the nearest value y whose cdf is x
    while (1){
		atry = floor((lower+upper+1)/2);

		if (last_try == atry)
			break;

		if (cdfZipf[atry] >= p)
			upper=atry;
		else
			lower = atry-1;

		last_try = atry;
    }

    return upper;

}
