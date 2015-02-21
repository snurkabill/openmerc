cmake_minimum_required (VERSION 2.6)
PROJECT (mercore.out)
ADD_EXECUTABLE (mercore.out ../src/mercore.cpp ../src/module_wrapper/module_wrapper.cpp)

SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lzmq -pthread -ldl -lmongoclient -lboost_thread -lboost_system -lboost_regex")

INSTALL (FILES mercore.out 
	 DESTINATION bin
	 PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                    GROUP_EXECUTE GROUP_READ)
