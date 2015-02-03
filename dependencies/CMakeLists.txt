set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/dependencies/CMakeModules")
set(CMAKE_PREFIX_PATH /usr/lib64/openmpi)

include( ${CMAKE_SOURCE_DIR}/build_dependencies.txt RESULT_VARIABLE bdvars)

message("Configure GTest Build.")
execute_process( COMMAND cmake . 
		 WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies/external/gtest)

message("Building GTest")
execute_process( COMMAND make
		 WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies/external/gtest)

set(GTEST_ROOT ${CMAKE_SOURCE_DIR}/dependencies/gtest)
find_package(GTest)


add_custom_target(ann COMMAND cmake . -DANN_Version=${ANN_Version} 
		      COMMAND make WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/ann)
add_custom_target(boost COMMAND cmake . -DBoost_Version_Underscore=${Boost_Version_Underscore} -DBoost_Version=${Boost_Version} 
		        COMMAND make WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/boost)
add_custom_target(metis COMMAND cmake . -DMetis_Version=${Metis_Version} COMMAND make WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/metis)


include_directories(/usr/local/include)
link_directories(/usr/local/lib)
#if( bdvars NOTFOUND)
##TODO: Check if not found
#endif()
if( NOT Boost_FOUND)
	find_package(Boost COMPONENTS program_options serialization mpi QUIET)
	if( NOT Boost_FOUND)
	 message( "--- Boost not found. Install locally by typing 'make boost'")
	endif() 
else(Boost_FOUND)
	message("-- Using Boost specified by user")
endif()

if( Boost_FOUND)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})
set(Boost_LIBRARIES ${Boost_LIBRARIES} PARENT_SCOPE)
endif()

if( NOT MPI_FOUND)
	find_package(MPI REQUIRED)
elseif( MPI_FOUND)
	message( "-- Using MPI specified by user")
endif()
if( MPI_FOUND)
include_directories( ${MPI_INCLUDE_DIR})
link_directories( ${MPI_LIBRARY_DIR})
set(MPI_LIBRARIES ${MPI_LIBRARIES} PARENT_SCOPE)
endif()

if( NOT TBB_FOUND)
	find_package(TBB) 
elseif( TBB_FOUND)
	message( "-- Using TBB specified by user")
endif()
if(TBB_FOUND)
include_directories(${TBB_INCLUDE_DIR})
link_directories(${TBB_LIBRARY_DIR})
set(TBB_LIBRARIES ${TBB_LIBRARIES} PARENT_SCOPE)
endif()
if( NOT ANN_FOUND)
	find_package(ANN)
	if( NOT ANN_FOUND)
	 message( "--- ANN not found. Install locally by typing 'make ann'")
	endif()
elseif( ANN_FOUND)
	message( "-- Using ANN specified by user")
endif()
if( ANN_FOUND)
include_directories(${ANN_INCLUDE_DIR})
link_directories(${ANN_LIBRARY_DIR})
set(ANN_LIBRARIES ${ANN_LIBRARIES} PARENT_SCOPE)
endif()

if( NOT METIS_FOUND)
	find_package(METIS QUIET)
	if( NOT METIS_FOUND)
	 message( "--- METIS not found. Install locally by typing 'make metis'")
	endif()
elseif( METIS_FOUND)
	message( "-- Using METIS specified by user")
endif()

if( METIS_FOUND)
include_directories(${METIS_INCLUDE_DIR})
link_directories(${METIS_LIBRARY_DIR})
set(METIS_LIBRARIES ${METIS_LIBRARIES} PARENT_SCOPE)
endif()

if( NOT DOXYGEN_FOUND)
	find_package(Doxygen)
elseif( DOXYGEN_FOUND)
	message( "-- Using Doxygen specified by user")
endif() 

include_directories(${GTEST_INCLUDE_DIR})
link_directories( ${GTEST_LIBRARY_DIR})
set(GTEST_LIBRARIES ${GTEST_LIBRARIES} PARENT_SCOPE)

if(DOXYGEN_FOUND)
configure_file(${CMAKE_SOURCE_DIR}/doc/Doxyfile.in 
	       ${CMAKE_BINARY_DIR}/Doxyfile @ONLY)
add_custom_target(doc ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
COMMENT "Generating API documentation with Doxygen" VERBATIM
)
endif(DOXYGEN_FOUND)