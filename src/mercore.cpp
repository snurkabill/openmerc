#include "cppzmq/zmq.hpp"
#include <string>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <sstream>

#include "mongo/client/dbclient.h"
#include "module_wrapper/module_wrapper.hpp"

using namespace mongo;


std::string process_command(BSONObj b, module_wrapper & mw) {

  if (b["module"].str() == "True") {

    if (b["add"].str() == "True") {

      // seber vsechny informace...
      std::string module_path = b["<module_path>"].str();
      int permission = std::atoi(b["-P"].str().c_str());
      std::string name = b["-n"].str();
      int group_id = 0;
      int module_id = mw.add(module_path, name, permission, group_id);

      // create return message
      std::stringstream ss;
      ss << "{'return':\"" << module_id << "\"}";
      return ss.str();

    } else if (b["set"].str() == "True") {

      return "{'return':\"setting module is not defined yet\"}";

    } else if (b["remove"].str() == "True") {

      int module_id = std::atoi(b["<mid>"].str().c_str());
      MODUL_CONT::iterator module = mw.find(module_id);
      mw.remove(module->second);
      return "OK";

    } else if (b["stop"].str() == "True") {

      int module_id = std::atoi(b["<mid>"].str().c_str());
      MODUL_CONT::iterator module = mw.find(module_id);
      mw.stop(module->second);

      return "OK";

    } else if (b["run"].str() == "True") {

      int module_id = std::atoi(b["<mid>"].str().c_str());
      MODUL_CONT::iterator module = mw.find(module_id);
      mw.run(module->second);


    } else if (b["status"].str() == "True") {

      return mw.get_info();

    } else if (b["find"].str() == "True") {

      return "{'return': \"The method find is not implemented yet.\"}";
    }

  } else if (b["core"].str() == "True") {

    if (b["stop"].str() == "True") {

      return "{'command':\"Stopping mercore.\"}";

    } else if (b["exit"].str() == "True") {

      exit(EXIT_SUCCESS);
      return "{'command':\"Mercore quit. Thank you for using it.\"}";

    } else if (b["run"].str() == "True") {

      return "{'command':\"Mercore is up and running!\"}";

    } else if (b["status"].str() == "True") {

      return "{'command':\"Mercore update status!\"}";
    }

  }

  // pokud se dostal az se, pak command neni validni nebo je chyba v kodu...

  return "{'command':\"Unknown command.\"}";
}

/**
 * tento program dostane informace o zpusobu komunikace
 * inicializuje vsechny objekty (modul_wrapper, market_data, orders)
 */
int main(int argc, char ** argv) {

  module_wrapper mw;

  // Prepare our context and socket
  zmq::context_t context(1);
  zmq::socket_t socket(context, ZMQ_REP);
  socket.bind("tcp://*:5555");

  while (true) {

    zmq::message_t request;

    // Wait for next request from client
    socket.recv(&request);
    std::cout << "Socket has been received." << std::endl;


    std::string command = std::string(
            static_cast<char*>(request.data()),
            request.size());

    BSONObj bson = fromjson(command.c_str());

    //std::cout << bson.toString() << std::endl;

    std::string reply_message = process_command(bson, mw);
    std::cout << "reply_message=" << reply_message << std::endl;

    zmq::message_t reply(reply_message.length());
    memcpy ((void *) reply.data(), reply_message.c_str(), reply_message.length());
    socket.send(reply);

  }

  return EXIT_SUCCESS;
}
