#ifndef CTLIB_FILTRATION_ITERATOR_H
#define CTLIB_FILTRATION_ITERATOR_H
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
*******************************************************************************
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
*******************************************************************************/
/**
 * NOTES: This header is DEPRECATED!
 */ 
//STL
#include <iterator>

//private functionality 
//the multiplier gracefully handles reverse iterators.
namespace ctl{
namespace detail{
template< typename Iterator, int multiplier>
class _filtration_iterator : 
		public std::iterator< std::random_access_iterator_tag, 
					typename Iterator::value_type> {
	public:
	//Does not matter to be defined since we inherit
		typedef typename Iterator::difference_type difference_type;
		typedef typename Iterator::value_type value_type;
		typedef typename Iterator::pointer pointer;
		typedef typename Iterator::reference reference;
		typedef std::random_access_iterator_tag iterator_category;
	private:
		typedef typename Iterator::difference_type diff_type;
		typedef _filtration_iterator< Iterator, multiplier> Self;
	public:
		
	//default
	_filtration_iterator(): i(), p( 0) {}
	//copy
	_filtration_iterator( const Self & f): i( f.i), p( f.p) {};
	//move
	_filtration_iterator( Self && f): i( std::move( f.i)),
					  p( std::move(f.p)) {};
	//special
	_filtration_iterator( const Iterator & i_, 
			      const std::size_t p_): i( i_), p( p_) {}    
	Self& operator++(){ 
		p+=multiplier; i++;	
		return *this; 
	}
	Self operator++(int){ 
		Self r( *this);
		++(*this);	
		return r; 
	}
	Self& operator--(){ 
		p-=multiplier; i--;	
		return *this; 
	}
	Self operator--(int){ 
		Self r( *this);
		--(*this);	
		return r; 
	}
	//deref
	pointer operator->() const { return &(*i); }
	reference operator*(){ return *i; }
	//comparisons
 	bool operator!=(const Self& r) const { return (i != r.i); }
 	bool operator==(const Self& r) const { return !operator!=(r); }
 	bool operator<(const Self& r) const { return (i < r.i); }
 	bool operator<=(const Self& r) const { return (i <= r.i); }
 	bool operator>(const Self& r) const { return (i > r.i); }
 	bool operator>=(const Self& r) const { return (i >= r.i); }
	//arithmetic
 	Self operator+(const diff_type n) const { return Self(i+n, p+n); }
	Self operator-(const diff_type n) const { return Self(i-n, p-n); }
	diff_type operator-(const Self& a) const { return i-a.i; }
	Self& operator=(const Self& r) { i = r.i; p = r.p; return *this; } 
 	Self& operator+=(const difference_type n) { i+=n; p+=n; return *this;}
 	Self& operator-=(const difference_type n) { i-=n; p-=n; return *this;}
	reference operator[](const std::size_t n) const { return *(i+n); } 	
	std::size_t pos() const { return p; } 
	//the details
	private: 
	Iterator i;
	std::size_t p;
}; //class _filtration_iterator
} //namespace detail
} //namespace ctl
#endif //CTLIB_FILTRATION_ITERATOR_H
