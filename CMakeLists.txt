cmake_minimum_required(VERSION 2.8.11)

set(CMAKE_DEBUG_POSTFIX "d")

project(asl)

include_directories(include)

set( OLD_CMAKE_ARCHIVE ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY} ) 
set( OLD_CMAKE_LIBRARY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ) 
set( OLD_CMAKE_RUNTIME ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ) 

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib/ )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lbin/ )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/ )
set(CMAKE_MACOSX_RPATH ON)

option( ASL_USE_LOCAL8BIT "Treat char strings as local 8 bit instead of UTF8 when converting wchar_t strings" OFF)

if(NOT MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
	if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "4.4")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
	endif()
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option( ASL_TLS "TLS Sockets" OFF)

if( ASL_TLS )
	find_path(mbedTLS_DIR "include/mbedtls")
	if(mbedTLS_DIR STREQUAL mbedTLS_DIR-NOTFOUND )
		message(FATAL_ERROR "TLS needs the directory of mbedTLS" )
	endif()
	include_directories( ${mbedTLS_DIR}/include )
	find_library( mbedTLS_LIB mbedtls ${mbedTLS_DIR}/lib )
	find_library( mbedTLSx509_LIB mbedx509 ${mbedTLS_DIR}/lib )
	find_library( mbedTLScrypto_LIB mbedcrypto ${mbedTLS_DIR}/lib )
endif()

set(TARGETS "")


if(CMAKE_SOURCE_DIR STREQUAL ${CMAKE_CURRENT_SOURCE_DIR} AND NOT ANDROID)
	set(ASL_BUILD_SHARED ON)
endif()

option(ASL_BUILD_STATIC "Build static library" ON)
option(ASL_BUILD_SHARED "Build shared library" ${ASL_BUILD_SHARED})

add_subdirectory( src )

option( ASL_SAMPLES "Build samples" OFF)

if( ASL_SAMPLES )
	add_subdirectory(samples)
endif()

option( ASL_TESTS "Build tests" OFF)

if( ASL_TESTS )
	enable_testing()
	add_subdirectory(tests)
endif()

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OLD_CMAKE_ARCHIVE} CACHE PATH "Output directory for libraries." FORCE ) 
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OLD_CMAKE_LIBRARY} CACHE PATH "Output directory for libraries." FORCE ) 
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OLD_CMAKE_RUNTIME} CACHE PATH "Output directory for executables." FORCE ) 

file( WRITE "${PROJECT_BINARY_DIR}/ASLConfigVersion.cmake"
	"if(NOT \${CMAKE_GENERATOR} STREQUAL \"${CMAKE_GENERATOR}\")\n"
	"message( \"Skipping incompatible ASL build\" )\n"
	"set(PACKAGE_VERSION_UNSUITABLE TRUE)\nendif()\n" )

export( TARGETS ${TARGETS} FILE ASLConfig.cmake )

if(CMAKE_SOURCE_DIR STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
	export( PACKAGE ASL )
	find_package(Doxygen)
	if(DOXYGEN_FOUND AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/doc)
		set(ASL_DOC_DIR "${PROJECT_BINARY_DIR}/doc" CACHE DIR  "Directory for documentation")
		configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doc/doxy.ini ${PROJECT_BINARY_DIR}/doxy2.ini)
		add_custom_target(DOC
			${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/doxy2.ini
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc
			COMMENT "Generating API documentation with Doxygen" VERBATIM
		)
	endif()
endif()
