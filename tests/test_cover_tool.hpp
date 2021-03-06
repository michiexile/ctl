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
* Copyright (C) Ryan H. Lewis 2010 <me@ryanlewis.net>
*******************************************************************************
* NOTES
*******************************************************************************/

//CTL
#include <ctl/abstract_simplex/abstract_simplex.hpp>
#include <ctl/abstract_simplex/simplex_boundary.hpp>
#include <ctl/chain_complex/complex_boundary.hpp>
#include <ctl/parallel/filtration/filtration.hpp>
#include <ctl/chain_complex/chain_complex.hpp>
#include <ctl/io/io.hpp>
#include <ctl/term/term.hpp>

//Local Project Deps
#include <ctl/parallel/partition_covers/covers.hpp>
#include <ctl/parallel/partition_covers/cover_tests.hpp>
#include <ctl/parallel/partition_covers/cover_data.hpp>
#include <ctl/parallel/chain_complex/chain_complex.hpp>
#include <ctl/parallel/build_blowup_complex/build_blowup_complex.hpp>

//BOOST
#include <boost/program_options.hpp>

//STL
#include <sstream>
#include <iostream>
#include <fstream>

//TBB
#include <tbb/task_scheduler_init.h>

namespace po = boost::program_options;

// Complex type
typedef ctl::Abstract_simplex< int> Cell;
typedef ctl::Finite_field< 2> Z2;
typedef ctl::Simplex_boundary< Cell, Z2> Simplex_boundary;
typedef ctl::parallel::Chain_complex< Cell, Simplex_boundary, 
			    	      ctl::parallel::Nerve_data> Nerve;
typedef ctl::parallel::Filtration< Nerve, ctl::Cell_less> Nerve_filtration;

typedef Nerve::iterator Nerve_iterator;
typedef ctl::parallel::Cover_data< Nerve_iterator > Cover_data;
typedef ctl::Chain_complex< Cell, Simplex_boundary, Cover_data> Complex;

typedef Complex::iterator Complex_iterator;
typedef Complex::const_iterator Complex_const_iterator;
typedef ctl::Cell_less Complex_less;

typedef ctl::parallel::Filtration< Complex, Complex_less> Complex_filtration;

typedef ctl::Timer Timer;
typedef ctl::parallel::Cover_stats< Timer> Stats;

template<typename Variable_map>
void process_args( int & argc, char *argv[],Variable_map & vm){
  //parse command line options
  po::options_description desc( "Usage: tp [options] input-file num-parts");
  desc.add_options()
  ( "help", "Display this message")
  ( "input-file", "input .asc file to parse")
  ( "num-parts", "specify partition size")
  ( "num-threads", po::value<int>()->default_value(-1),
			 "specify number of threads");
  po::positional_options_description p;
  p.add( "input-file",1);
  p.add( "num-parts",1);
  po::store( po::command_line_parser( argc,argv)
	    .options( desc)
	    .positional( p)
	    .run(),
	     vm);
  po::notify( vm);
  if ( vm.count( "help")){
	std::cout << desc << std::endl;
	std::exit( 0);
  }
  if ( vm.count( "input-file") != 1 || vm.count( "num-parts") != 1){
	std::cout << desc << std::endl;	
	std::exit( -1);
  }
}

int main( int argc, char *argv[]){
  po::variables_map vm;
  process_args( argc, argv, vm);
  size_t num_parts = atoi( vm[ "num-parts"].as< std::string>().c_str());

  //setup some variables
  std::string full_complex_name = vm[ "input-file"].as< std::string>();
  std::string complex_name( full_complex_name);
  std::string binary_name( argv[ 0]);

  int num_threads = vm[ "num-threads"].as< int>();
  size_t found = complex_name.rfind( '/');
  if ( found != std::string::npos){
        complex_name.replace( 0, found+1, "");
  }

  Complex complex;
  Nerve nerve;
  Stats stats;
  // Read the chain_complex in
  ctl::read_complex( full_complex_name, complex);
  if (num_threads != -1){
  	tbb::task_scheduler_init init( num_threads);
  }else{
  	tbb::task_scheduler_init init; 
  }
  
  stats.timer.start();
  Complex_filtration complex_filtration( complex);
  stats.timer.stop();
  double filtration_time = stats.timer.elapsed();
  std::cout << "time to filter complex: " << filtration_time << std::endl;

  stats.timer.start();
  ctl::parallel::init_cover_complex( nerve, num_parts);
  ctl::parallel::graph_partition_cover( complex_filtration, nerve);
  stats.timer.stop();
  double cover_time = stats.timer.elapsed();
  std::cout << "cover time: " << cover_time << std::endl;
  Nerve_filtration ordered_nerve( nerve);
  
  //typedef Nerve_filtration::iterator Iterator;
  for(const auto & cell:  ordered_nerve){ 
 	std::cout << cell->first << ": " << cell->second.count() << std::endl;
  }

  return 0;
}
