#ifndef CTL_ALL_PAIRS_H
#define CTL_ALL_PAIRS_H
/*******************************************************************************
* -Academic Honesty-
* Plagarism: The unauthorized use or close imitation of the language and 
* thoughts of another author and the representation of them as one's own 
* original work, as by not crediting the author. 
* (Encyclopedia Britannica, 2008.)
*
* You are free to use the code according to the license below, but, please
* do not commit acts of academic dishonesty. We strongly encourage and request 
* that for any [academic] use of this source code one should cite one the 
* following works:
* 
* \cite{hatcher, z-ct-10}
* 
* See ct.bib for the corresponding bibtex entries. 
* !!! DO NOT CITE THE USER MANUAL !!!
*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
* ********** BSD-3 License ****************
* Redistribution and use in source and binary forms, with or without 
* modification, are permitted provided that the following conditions are met:
* 
* 1. Redistributions of source code must retain the above copyright notice, 
* this list of conditions and the following disclaimer.
* 
* 2. Redistributions in binary form must reproduce the above copyright notice, 
* this list of conditions and the following disclaimer in the documentation 
* and/or other materials provided with the distribution.
* 
* 3. Neither the name of the copyright holder nor the names of its contributors 
* may be used to endorse or promote products derived from this software without 
* specific prior written permission.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
* POSSIBILITY OF SUCH DAMAGE.
********************************************************************************
*******************************************************************************
*******************************************************************************
* NOTES
* We use the all pairs approach to constuct a graph. 
*
*******************************************************************************/

//STL 
#include <vector> 
#include <algorithm>

//BOOST
#include <boost/graph/adjacency_list.hpp>

//CTL
#include <ctl/metrics/metric.hpp>

namespace ctl{
namespace all_pairs{
template<typename Points, typename Graph>
void construct_graph( const Points& points, 
		      const double epsilon, 
		      Graph& graph) {
    typedef typename boost::graph_traits<Graph> graph_traits;
    typedef typename graph_traits::vertex_iterator vertex_iterator;
    typedef typename boost::graph_traits< Graph>::vertex_descriptor vertex_descriptor; 
    typedef typename boost::property_map< Graph,
					  boost::vertex_name_t>::type name_map_t;
    typedef typename boost::property_traits< name_map_t>::value_type  vertex_name_t;
    typedef std::unordered_map< vertex_name_t, vertex_descriptor> Name_to_descriptor_map;

    name_map_t name_map = boost::get( boost::vertex_name, graph);
    Name_to_descriptor_map descriptor( points.size());

    // add vertices
    for( std::size_t i = 0; i < points.size(); ++i) {
	vertex_descriptor v_descriptor = boost::add_vertex( graph);
	name_map[ v_descriptor] = i;
	descriptor.emplace( i, v_descriptor);
    }

    // add edges
    vertex_iterator vi, vj, vlast;
    double epsilon_squared = epsilon*epsilon;
    for ( std::tie( vi, vlast) = boost::vertices( graph); vi != vlast; ++vi) {
      for ( std::tie( vj, vlast) = boost::vertices (graph); vj != vi; ++vj) {
       if( lp(points[name_map[*vi]], points[name_map[*vj]]) < epsilon_squared) {
        boost::add_edge(*vi, *vj, graph);
       }
      } 
    }
} 
} //end namespace all_pairs
} //end namespace CTL


#endif //CTL_ALL_PAIRS_H
