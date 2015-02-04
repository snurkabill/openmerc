#include "cppzmq/zmq.hpp"
#include <string>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <sstream>

#include "mongo/client/dbclient.h"

using namespace mongo;


std::string process_command(BSONObj b) {

  if (b["module"].str() == "True") {

    if (b["add"].str() == "True") {

      return "{'command':\"adding module\"}";

    } else if (b["set"].str() == "True") {

      return "{'command':\"setting module\"}";

    } else if (b["remove"].str() == "True") {

      return "{'command':\"removing module\"}";

    } else if (b["run"].str() == "True") {

      return "{'command':\"running module\"}";
    }

  } else if (b["core"].str() == "True") {

    if (b["stop"].str() == "True") {

      return "{'command':\"Stopping mercore.\"}";

    } else if (b["exit"].str() == "True") {

      return "{'command':\"Mercore quit. Thank you for using it.\"}";

    } else if (b["run"].str() == "True") {

      return "{'command':\"Mercore is up and running!\"}";

    } else if (b["status"].str() == "True") {

      return "{'command':\"Mercore update fb status!\"}";
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

    std::cout << bson.toString() << std::endl;

    std::string reply_message = process_command(bson);
    std::cout << "reply_message=" << reply_message << std::endl;

    zmq::message_t reply(reply_message.length());
    memcpy ((void *) reply.data(), reply_message.c_str(), reply_message.length());
    socket.send(reply);

  }

  return EXIT_SUCCESS;
}
