#ifndef SIMPLEX_BOUNDARY_H
#define SIMPLEX_BOUNDARY_H
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
#include "term/term.h"
#include "finite_field/finite_field.h"
#include <cassert>
//non-exported functionality
namespace {

template< typename Term_>
class const_boundary_iterator : 
	public std::iterator< std::bidirectional_iterator_tag,
			      Term_,
			      std::ptrdiff_t,
			      const Term_*,
			      const Term_>{
	private:
	  typedef const_boundary_iterator< Term_> Self;
	  typedef Term_ Term;
	  typedef typename Term::Cell Cell;
	  typedef typename Cell::value_type Vertex;
	public:
	  //default constructor
	const_boundary_iterator(): cellptr( 0), pos( 0), face(){}
	const_boundary_iterator( const Cell & s): pos( 0), face(){ 
		if( s.dimension()){
		    //begin by removing first vertex
		    cellptr = &s;
		    removed = s.vertices[ 0];
		    face.coefficient( 1);
		    face.cell().vertices.resize( s.dimension());
		    std::copy( s.begin()+1, s.end(), 
			       face.cell().vertices.begin()); 
		}else{ cellptr = 0; } //\partial(vertex) = 0
	}
	//copy constructor
	const_boundary_iterator( const Self & from): cellptr( from.cellptr), 
	pos( from.pos), face(from.face), removed( from.removed){}
	//move constructor
	const_boundary_iterator( Self && from): 
	cellptr( std::move(from.cellptr)), 
	pos( std::move(from.pos)), 
	face( std::move(from.face)), 
	removed( std::move(from.removed)){ from.cellptr = 0; }	
	//equality
	const_boundary_iterator& operator==( const Self & b) const { 
		return (b.cellptr == cellptr) && (b.pos == pos); 
	} 
	const_boundary_iterator& operator=(const Self & b){
		cellptr = b.cellptr;
		remove = b.removed;
		face = b.face;
		pos = b.pos;
		return *this;
	}
	const_boundary_iterator& operator=(Self && b){
		cellptr = std::move(b.cellptr);
		remove  = std::move(b.removed);
		face    = std::move(b.face);
		pos     = std::move(b.pos);
		return *this;
	}
	//We modify normal behavior for performance reasons.
	//When we do prefetching we wish to prefetch the next element
	//in the boundary and not experience a branch to check for the end
	//therefore we use assert here so that in debug mode it will complain
	//if your software has bugs, but in release mode we assume only
	//correct usage of this software so we will "silently fail".
	//this way the prefetch will work, and this bound checking wont
	//happen on any dereference, saving a branch. 
	Term& operator*() {
		assert( cellptr != 0); 
		return face; 
	}
	const Term* operator->() const { return &face; }
	bool operator!=( const const_boundary_iterator & b) const { 
		return (b.cellptr != cellptr) || (b.pos != pos); 
	}

	const_boundary_iterator& operator++(){
		if( pos == face.cell().size()){
			cellptr = 0;
			pos = 0;
			return *this;
		}
		//return removed vertex, get rid of another one
		std::swap( face.cell().vertices[ pos++], removed);
		face.coefficient( -1*face.coefficient());
		return *this;	
	}

	const_boundary_iterator& operator--(){
		if(pos == 0){
			cellptr = 0;
			return *this;
		}
		//return removed vertex, get rid of another one
		std::swap( face.cell().vertices[ --pos], removed);
		face.coefficient( -1*face.coefficient());
		return *this;	
	}

	const_boundary_iterator operator++( int){
	 	const_boundary_iterator tmp( *this); 
		++(*this); //now call previous operator
		return tmp;
	}
	const_boundary_iterator operator--( int){
	 	const_boundary_iterator tmp( *this); 
		--(*this); //now call previous operator
		return tmp;
	}
	 private:
		const Cell* cellptr;
		typename Cell::size_t pos;
		Term face;
		Vertex removed;
}; // END const_boundary_iterator
} //END private namespace

namespace ctl { 
template< typename Simplex_, typename Coefficient_>
class Simplex_boundary {
public:
	typedef Simplex_ Simplex;
	typedef Coefficient_ Coefficient;
	typedef ctl::Term< Simplex, Coefficient> Term;
	typedef const_boundary_iterator< Term> const_iterator;
	//default constructor
	Simplex_boundary(){};	

	Simplex_boundary& operator=( const Simplex_boundary& from){ 
		return *this;
	}
	//It only makes sense for const iterators
	const_iterator begin( const Simplex & s) const { return const_iterator( s); }
	const_iterator end( const Simplex & s) const   { return const_iterator(); }
	std::size_t length( const Simplex & s) const { 
		return (s.size()>1)? s.size():0;
	}
}; //Simplex_boundary

} // namespace ctl

#endif //SIMPLEX_BOUNDARY_H
