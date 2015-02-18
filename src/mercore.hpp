#ifndef MERCORE
#define MERCORE

#include "cppzmq/zmq.hpp"
#include <string>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <sstream>
#include <sys/types.h>
#include <unistd.h>

#include "mongo/client/dbclient.h"
#include "module_wrapper/module_wrapper.hpp"



std::string process_command(mongo::BSONObj b, module_wrapper & mw);



#endif
