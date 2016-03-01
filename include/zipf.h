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
#ifndef ZIPF_H_
#define ZIPF_H_
#include <vector>

using namespace std;

class zipf_distribution{
    public:
		zipf_distribution(double a, int n):alpha(a),F(n){;};
		zipf_distribution(double a, double p, int n):alpha(a),q(p),F(n){;};
		zipf_distribution(){zipf_distribution(0,0);}
		void zipf_initialize();

		//<aa> 	Return the index of the content y such that the the sum of the 
		//		probabilities of contents from 0 to y is p </aa>
		unsigned int value (double p);

		//<aa>
		double get_normalization_constant();
		//</aa> 
		

    private:
		vector<double> cdfZipf;
		double alpha;
		double q;
		int F;

		//<aa>
		double normalization_constant;
		//</aa>
};
#endif
