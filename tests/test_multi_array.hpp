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
*******************************************************************************
*******************************************************************************/
//STL
#include <iostream>

//CTL 
#include <ctl/chain_complex/detail/data_wrapper.hpp>
#include <ctl/chain_complex/detail/multi_array.hpp>

//Gtest
#include <gtest/gtest.h>

struct Filter_function_value { 
Filter_function_value(): d( 0){}
Filter_function_value( const Filter_function_value & s): d( s.d){}
Filter_function_value( const Filter_function_value && s): d( s.d){}
bool operator==( const Filter_function_value & f) const { return d == f.d; }
Filter_function_value& operator=( const Filter_function_value & f){ 
	d = f.d; 
	return *this;
}
Filter_function_value& operator=( const Filter_function_value && f){ 
	d = f.d; 
	return *this;
}
std::size_t d; 
}; //end struct Filter_function_value

template< typename Stream, typename Vector>
Stream& print_vector( Stream& out, const Vector & v){
	for( auto& i : v){ out << i << " "; }
	return out;
}

template< typename M>
void print_multi_array( const M & m){
std::vector< std::size_t> coordinate;
for( auto& i: m){ 
   std::cout << "d=" << i.d << " : "; 
   print_vector( std::cout,  m.index_to_coordinate( i.d, coordinate)); 
   std::cout << std::endl;
}
}


template< typename M>
bool check_multi_indexing( const M & m){
	typedef std::vector< std::size_t> Coordinate;
	Coordinate c;
	for( auto i = m.begin(); i != m.end(); ++i){
	  std::size_t p = std::distance( m.begin(), i);
	  if( m[ p] != *i){ return false; }
	  if( m.coordinate_to_index( m.index_to_coordinate( p, c)) != p){ return false; }
	}
	return true;
} 


TEST(MultiArray, VanillaMultiIndexing){
	typedef ctl::detail::Data_wrapper< Filter_function_value > Data;
	typedef ctl::detail::multi_array< Data> multi_array;


	std::vector< std::size_t> extents{5,4,3};

	multi_array m( extents.begin(), extents.end());
	ASSERT_EQ( check_multi_indexing( m), true);
}

TEST(MultiArray, OffsetMultiIndexing){
typedef ctl::detail::Data_wrapper< Filter_function_value > Data;
typedef ctl::detail::multi_array< Data> multi_array;


	std::vector< std::size_t> extents{32,32,32};
	std::vector< std::size_t> base{32,32,32};
	
	multi_array m2( extents.begin(), extents.end(), 
		        base.begin(), base.end());
	ASSERT_EQ( base, m2.base());
	std::cout << std::endl;
	ASSERT_EQ( check_multi_indexing( m2), true);
}

TEST(MultiArray, ConstructorTest){
typedef ctl::detail::Data_wrapper< Filter_function_value > Data;
typedef ctl::detail::multi_array< Data> multi_array;
	std::vector< std::size_t> extents{5,4,3};
	multi_array m( extents.begin(), extents.end());
	ASSERT_EQ( extents, m.extents());
}
