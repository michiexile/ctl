#ifndef _CTL_COVER_TEST_H_
#define _CTL_COVER_TEST_H_
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
* Copyright (C) Ryan H. Lewis 2011 <me@ryanlewis.net>
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
#include "gtest/gtest.h"

namespace ctl{
namespace parallel { 
template<typename Complex>
void covered_complex(Complex & complex){
	typedef typename Complex::iterator cell_iterator;
	typedef typename Complex::Cell Cell;
	typedef typename Complex::Data Data;
        for (cell_iterator it = complex.begin(); it != complex.end(); ++it){
                const Cell & cell = it->first;
		Data & data = it->second;
		ASSERT_NEQ(data.data(), Data().data());
        }
}

template<typename Complex>
void sheets_closed(Complex & complex){
        typedef typename Complex::iterator cell_iterator;
        typedef typename Complex::Data Data;
        typedef typename Complex::Cell Cell;
        typedef typename ctl::Complex_boundary< Complex, cell_iterator> 
								Boundary;
	Boundary boundary(complex);
        typedef typename Boundary::const_iterator Boundary_iterator;
	for (cell_iterator i = complex.begin(); i != complex.end(); ++i){
		const Cell & data = i->second.data()->first;	
		for(Boundary_iterator j = boundary.begin( i); 
				      j != boundary.end( i); ++j){
			const Data & face_data = j->get_cell()->second;
			const Cell & face = face_data.data()->first;
			const Cell & face_cell = j->get_cell()->first;
			ASSERT_TRUE(std::includes(face.begin(), face.end(),
				      		  data.begin(), data.end()));
		}
	} 
}

} //namespace parallel
} //namespace ctl

#endif //_CTL_COVER_TEST_H_