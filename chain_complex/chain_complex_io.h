#ifndef CTLIB_CHAIN_COMPLEX_IO_H
#define CTLIB_CHAIN_COMPLEX_IO_H
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
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program in a file entitled COPYING; if not, write to the 
* Free Software Foundation, Inc., 
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*******************************************************************************
*******************************************************************************/
#include <fstream>
#include "io/io.h"
#include "chain_complex/chain_complex.h"


template< typename Stream, typename Cell, typename Boundary, 
	   typename Data, typename Hash>
Stream& operator<<( Stream& out, 
		    const ctl::Chain_complex< Cell, Boundary, Data, Hash> & c){ 
	for(auto i = c.begin(); i != c.end(); ++i){
		      out << i->second.id() <<": " << i->first << " --> {" 
		  	  << i->second << "}" << std::endl;
	}
	return out;
}



template< typename Stream, typename Cell, 
	  typename Boundary, typename Data_, typename Hash>
Stream& operator>>( Stream& in, 
		    ctl::Chain_complex< Cell, Boundary, Data_, Hash> & c){ 
	typedef typename ctl::Chain_complex< Cell, Boundary, 
					     Data_, Hash> Chain_complex;  
	typedef typename Chain_complex::Data Data;
	std::size_t line_num = 0;
	std::string line;
	std::size_t id=0;
        char the_first_character = in.peek();
        if( the_first_character == 's') {
                ctl::get_line( in, line, line_num);
                std::istringstream ss( line);
                std::string the_word_size;
                ss >> the_word_size;
                std::size_t the_number_of_cells;
                ss >> the_number_of_cells;
		c.reserve( the_number_of_cells);
        }
	while( ctl::get_line(in, line, line_num)){
		std::istringstream ss( line);
		Cell cell;
		ss >> id;
		ss >> cell;
		Data d( id);
		c.insert_open_cell( cell, d);
	}
	return in;
}

namespace ctl{
template<typename String, typename Complex>
void read_complex(String & complex_name, Complex & complex){
	std::ifstream in;
	std::cout << "File IO ..." << std::flush;
	ctl::open_file( in, complex_name.c_str());
	in >> complex;
	ctl::close_file( in);
	std::cout << "completed!" << std::endl;
}
} //namespace ctl

#endif //CTLIB_CHAIN_COMPLEX_IO_H
