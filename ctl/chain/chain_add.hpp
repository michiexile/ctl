#ifndef CTLIB_CHAIN_ADD_H
#define CTLIB_CHAIN_ADD_H
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
//STL
#include <algorithm> 

//CTL
#include <ctl/term/term_tags.hpp>

//exported, but, internal functionality
namespace ctl{
namespace detail {
//this is our equivalent of the BLAS axpy for a=1
//so its a 1xpy
template< typename Chain_iterator, typename OutputIterator, typename Term_less>
OutputIterator chain_add( Chain_iterator x_begin, Chain_iterator x_end, 
		 	  Chain_iterator y_begin, Chain_iterator y_end,
			  OutputIterator result, const Term_less less,   
			  const term_z2_tag t){
	//TODO: optimize set_symmetric_difference
	while( true){ 
	     if(x_begin == x_end){ return std::copy( y_begin, y_end, result); }
	     if(y_begin == y_end){ return std::copy( x_begin, x_end, result); }
	     if( less(*x_begin, *y_begin)) { 
		*result = *x_begin; 
		++result; ++x_begin; 
	     }
	     //expect that Term_less compares position in filtration 
	     //when possible. No derefrence when not necessary
	     else if( less(*y_begin, *x_begin)) { 
		*result = *y_begin; 
		++result; ++y_begin;
	     } else { ++x_begin; ++y_begin; }
	}
	return result;
}

template< typename Chain_iterator, typename Coefficient, 
	  typename OutputIterator, typename Term_less>
OutputIterator chain_add( Chain_iterator x_begin, Chain_iterator x_end, 
			  const Coefficient& not_used, 
		 	  Chain_iterator y_begin, Chain_iterator y_end,
			  OutputIterator result, const Term_less less,   
			  const term_z2_tag t){
	return chain_add( x_begin, x_end, y_begin, y_end, result, less, t);
}	

//and this is an 1xpy call for nonz2 terms
//TODO: should take a coefficient, and a flag
//QUESTION: Currently persistence will not hit this function.
//Is it faster in general to always check if coefficient == 1 then call this
//or is it faster to have the branches in the inner loop?
//any c++11 or better compiler will remove the outer branch when we have a Z2
//term, so this will truly only matter for non_z2 terms.  
template< typename Chain_iterator, typename OutputIterator, typename Term_less>
OutputIterator chain_add( Chain_iterator x_begin, Chain_iterator x_end, 
		  	  Chain_iterator y_begin, Chain_iterator y_end,
		  	  OutputIterator result,
			  const Term_less less,   
		  	  const term_non_z2_tag t){
	typedef typename Chain_iterator::value_type Term;
	typedef typename Term::Coefficient Coefficient;
	
	while( true){ 
	     if(x_begin == x_end){ return std::copy( y_begin, y_end, result); }
	     if(y_begin == y_end){ return std::copy( x_begin, x_end, result); }
	     if( less(*x_begin, *y_begin)) { 
		*result = *x_begin; //optimize chain= operator. 
		 ++x_begin; 
	     }
	     //expect that Term_less compares position in filtration 
	     //when possible so no dereference when not necessary
	     else if( less(*y_begin,*x_begin)) { 
		*result = *y_begin; //optimize chain= operator.
	 	++y_begin;
	     } else {
		const Coefficient c = 2*x_begin->coefficient(); 
		*result = *x_begin; 
		result->coefficient( c);
		++x_begin; ++y_begin; 
	    }
	    ++result;
	}
	return result;
}

//and this is an axpy call 
//since a is in a finite field we could call it a faxpy
template< typename Chain_iterator, typename OutputIterator, typename Term_less>
OutputIterator chain_add( Chain_iterator x_begin, Chain_iterator x_end, 
		  	  const typename Chain_iterator::value_type::
					 Coefficient& a, 
		  	  Chain_iterator y_begin, Chain_iterator y_end,
		  	  OutputIterator  result,
			  const Term_less less,   
		  	  const term_non_z2_tag t){
	typedef typename Chain_iterator::value_type Term;
	typedef typename Term::Coefficient Coefficient;
	
	while( true){ 
	     if(x_begin == x_end){ return std::copy( y_begin, y_end, result); }
	     if(y_begin == y_end){ return std::copy( x_begin, x_end, result); }
	     if( less(*x_begin, *y_begin)) { 
		*result = *x_begin; 
		 ++x_begin; 
	     }
	     //expect that Term_less compares position in filtration 
	     //when possible. No derefrence when not necessary
	     else if( less(*y_begin,*x_begin)) { 
		const Coefficient c = a*y_begin->coefficient(); 
		if( c == 0 ) { continue; } 
		*result = *y_begin; 
		result->coefficient( c);
	 	++y_begin;
	     } else {
		const Coefficient c = (1+a)*y_begin->coefficient(); 
		if( c == 0 ) { continue; } 
		*result = *x_begin; 
		result->coefficient( c);
		++x_begin; ++y_begin; 
	    }
	    ++result;
	}
	return result;
}

//over Z2 the only coeff is 1
template< typename Vector, typename Less>
Vector& chain_term_add( Vector & x, const typename Vector::value_type & y, 
			Less less, const term_z2_tag t){
	auto pos = std::lower_bound( x.begin(), x.end(), y, less);
	//new element, so add it at the end
	if (pos == x.end()) { x.push_back( y); }
	//new element in the middle, add it.
	else if (less(y, *pos)){ x.insert( pos, y); }
	//element exists, in Z2 this means delete!
	else{ x.erase( pos); }
	return x;
}

template< typename Term>
struct is_zero{
	typedef typename Term::Coefficient Coefficient;
	const bool operator()( const Term & t) const{
		return (t.coefficient() == 0);
	}
}; //struct is_zero

//Since x=x+a*y is well defined for y a term we ignore the coefficient
template< typename Vector, typename Less>
Vector& chain_term_add( Vector & x, const typename Vector::value_type & y, 
		        Less less, const term_non_z2_tag t){
	typedef typename Vector::value_type Term;
	//typedef typename Term::Coefficient Coefficient;
	typedef is_zero< Term> Is_zero;
	auto pos = std::lower_bound( x.begin(), x.end(), y, less);
	//new element is larger than all the rest so add it at the end
	if (pos == x.end()) { x.push_back( y); }
	//new element in the middle, add it.
	else if (less(y,*pos)){ x.insert( pos, y); }
	//element exists, but adding y might delete it!
	else{
	 pos->coefficient( y.coefficient(), true);
	 x.erase( std::remove_if( pos, pos+1, Is_zero()));	
	}
	return x;
}
} //namespace detail
} //namespace ctl

#endif //CTL_CHAIN_ADD_H
