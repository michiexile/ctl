#ifndef CTLIB_HASH_H
#define CTLIB_HASH_H
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

#define CTL_USE_CITY

//STL
#include <cstring>

//CTL
namespace ctl{
namespace hash{
#include <ctl/hash/city.hpp>
#include <ctl/hash/MurmurHash3.hpp>
} //end namespace hash
} //end namespace ctl
//non-exported functionality 
namespace ctl{
namespace detail{ 
namespace cth = ctl::hash;
template< typename Container>
inline 
std::size_t murmur3_hash( const Container & key, std::false_type){
	typedef typename Container::value_type T;
	std::size_t out;
	//MurmurHash3_x86_32 
	//MurmurHash3_x86_128
	cth::MurmurHash3_x64_128 ( (const char*)&(*(key.begin())), 
				    sizeof(T)*key.size(), key.size(), &out );
	return out;
}

template< typename T>
inline std::size_t murmur3_hash( const T & t, std::true_type){
	std::size_t out;
	cth::MurmurHash3_x86_128( (const char*)&t, sizeof(T), 1, &out);
	return out;
}

template< typename Container>
inline std::size_t city_hash( const Container & key, std::false_type){
	typedef typename Container::value_type T;
	return cth::CityHash64WithSeed( (const char*)&(*(key.begin())), 
					 sizeof(T)*key.size(),key.size()); 
}

template< typename T>
inline std::size_t city_hash( const T & t, std::true_type){
	return cth::CityHash64WithSeed( (const char*)&(t),
					 sizeof(T),1);
}

template< typename T>
inline std::size_t pjw_hash( const T & key){
    std::size_t h, g;
    h = 0;
    for( auto i : key){
      h = (h << 4) + i;
      if ((g = h & 0xf0000000)) {
	h = h^(g >> 24);
	h = h^g;
      }
    }
   return h;
} 
template< typename T>
inline std::size_t jenkins_hash( const T & key){
    std::size_t hash=0;
    for(auto i : key){
        hash += i;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

} //detail namespace
} //ctl namespace

namespace ctl {

	template< typename T>
	std::size_t hash_function( const T & key){
	#ifdef  CTL_USE_MURMUR
		return detail::murmur3_hash( key, std::is_integral< T>());
	#elif defined( CTL_USE_CITY)
		return detail::city_hash( key, std::is_integral< T>());
	#elif defined( CTL_USE_JENKINS)
		return detail::jenkins_hash( key);
	#else 
		return detail::pjw_hash( key);
	#endif
	}

	template< typename T>
	struct Hash{
		std::size_t operator()( const T & key) const{
			return hash_function( key);	
		}
	}; //class Hash
} //namespace ctl
#endif //CTLIB_HASH_H
