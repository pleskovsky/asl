project(asl)

include_directories(${asl_SOURCE_DIR}/../include)

set( ASL_SRC
	String.cpp
	Socket.cpp
	SocketServer.cpp
	MulticastSocket.cpp
	HttpServer.cpp
	Http.cpp
	WebSocket.cpp
	Xdl.cpp
	Var.cpp
	Xml.cpp
	IniFile.cpp
	File.cpp
	Directory.cpp
	Path.cpp
	Date.cpp
	Process.cpp
	Matrix4.cpp
	Console.cpp
	Log.cpp
	TabularDataFile.cpp
	CmdArgs.cpp
	SerialPort.cpp
	SharedMem.cpp
	unicodedata.cpp
	util.cpp
	SHA1.cpp
	../include/asl/defs.h
	../include/asl/String.h
	../include/asl/Array.h
	../include/asl/Array_.h
	../include/asl/Stack.h
	../include/asl/Map.h
	../include/asl/HashMap.h
	../include/asl/Vec2.h
	../include/asl/Vec3.h
	../include/asl/Vec4.h
	../include/asl/Quaternion.h
	../include/asl/Matrix3.h
	../include/asl/Matrix4.h
	../include/asl/Pose.h
	../include/asl/File.h
	../include/asl/IniFile.h
	../include/asl/Date.h
	../include/asl/File.h
	../include/asl/Directory.h
	../include/asl/Path.h
	../include/asl/Library.h
	../include/asl/Thread.h
	../include/asl/Mutex.h
	../include/asl/Process.h
	../include/asl/Var.h
	../include/asl/Xdl.h
	../include/asl/Xml.h
	../include/asl/Socket.h
	../include/asl/SocketServer.h
	../include/asl/HttpServer.h
	../include/asl/Http.h
	../include/asl/WebSocket.h
	../include/asl/Console.h
	../include/asl/Singleton.h
	../include/asl/Factory.h
	../include/asl/Log.h
	../include/asl/Pointer.h
	../include/asl/TabularDataFile.h
	../include/asl/CmdArgs.h
	../include/asl/SerialPort.h
	../include/asl/util.h
	../include/asl/TlsSocket.h
	../include/asl/SHA1.h
)

set(ASLS_DEFS ASL_STATIC)
set(ASL_DEFS "")

if(ASL_TLS)
	list(APPEND ASL_SRC TlsSocket.cpp)
	list(APPEND ASL_DEFS ASL_TLS)
	list(APPEND ASLS_DEFS ASL_TLS)
endif()

if( ASL_USE_LOCAL8BIT )
	list(APPEND ASL_DEFS ASL_ANSI)
	list(APPEND ASLS_DEFS ASL_ANSI)
endif()

if(POLICY CMP0022)
	cmake_policy(SET CMP0022 NEW)
endif()

if( ANDROID )
	list(APPEND ASL_DEFS ASL_NOEXCEPT)
	list(APPEND ASLS_DEFS ASL_NOEXCEPT)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fno-exceptions ") # -fno-rtti
endif()

set(TARGETS "")

if(ASL_BUILD_STATIC)
	add_library( asls STATIC ${ASL_SRC} )
	set_target_properties( asls PROPERTIES COMPILE_DEFINITIONS "${ASLS_DEFS}" )
	set_target_properties( asls PROPERTIES INTERFACE_COMPILE_DEFINITIONS "${ASLS_DEFS}" )
	target_include_directories(asls PUBLIC $<BUILD_INTERFACE:${asl_SOURCE_DIR}/../include> $<INSTALL_INTERFACE:$<TARGET_FILE_DIR:asls>/../include> )
	if( WIN32 )
		target_link_libraries(asls ws2_32)
	elseif( ANDROID )
		target_link_libraries(asls dl log c m)
		link_directories( "${ANDROID_SYSROOT}/usr/lib" )
	else()
		target_link_libraries(asls pthread dl)
		if(NOT APPLE)
			target_link_libraries(asls rt)
		endif()

		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
		if(CMAKE_SIZEOF_VOID_P EQUAL 8) # 64 bit linux complains
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
		endif()
	endif()

	if( ASL_TLS )
		target_link_libraries(asls ${mbedTLS_LIB} ${mbedTLSx509_LIB} ${mbedTLScrypto_LIB})
	endif()
	list(APPEND TARGETS asls)
endif()

if(ASL_BUILD_SHARED)
	add_library( asl SHARED ${ASL_SRC} )
	target_include_directories(asl PUBLIC $<BUILD_INTERFACE:${asl_SOURCE_DIR}/../include> $<INSTALL_INTERFACE:$<TARGET_FILE_DIR:asl>/include> )
	set_target_properties( asl PROPERTIES COMPILE_DEFINITIONS "${ASL_DEFS}" )
	set_target_properties( asl PROPERTIES INTERFACE_COMPILE_DEFINITIONS "${ASL_DEFS}" )

	if( WIN32 )
		target_link_libraries(asl LINK_PRIVATE ws2_32) # PRIVATE
	elseif( ANDROID )
		target_link_libraries(asl dl log c m)
		link_directories( "${ANDROID_SYSROOT}/usr/lib" )
	else()
		target_link_libraries(asl LINK_PUBLIC pthread dl)
		if(NOT APPLE)
			target_link_libraries(asl LINK_PUBLIC rt)
		endif()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
		if(CMAKE_SIZEOF_VOID_P EQUAL 8) # 64 bit linux complains
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
		endif()
	endif()

	if( ASL_TLS )
		target_link_libraries(asl LINK_PRIVATE ${mbedTLS_LIB} ${mbedTLSx509_LIB} ${mbedTLScrypto_LIB})
	endif()
	list(APPEND TARGETS asl)
endif()

link_directories( ${LIBRARY_OUTPUT_PATH} )

set( ASL_INSTALL_DIR "" CACHE STRING  "")
if( NOT ASL_INSTALL_DIR )
	if( WIN32 )
		set( ASL_INSTALL_DIR ${CMAKE_BINARY_DIR}/install )
	else()
		set( ASL_INSTALL_DIR ${CMAKE_INSTALL_PREFIX} )
	endif()
endif()

install( TARGETS ${TARGETS} EXPORT aslexport DESTINATION ${ASL_INSTALL_DIR}/lib )
install( DIRECTORY ../include DESTINATION ${ASL_INSTALL_DIR} )
install( EXPORT aslexport DESTINATION ${ASL_INSTALL_DIR} FILE ASLConfig.cmake )
# ... EXPORT_LINK_INTERFACE_LIBRARIES

set(TARGETS ${TARGETS} PARENT_SCOPE)
