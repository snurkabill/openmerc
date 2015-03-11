#include "mercore.hpp"
#include "commands.hpp"

using namespace mongo;


/**
 * tento program dostane informace o zpusobu komunikace
 * inicializuje vsechny objekty (modul_wrapper, market_data, orders)
 */
int main(/*int argc, char ** argv*/) {

  module_wrapper mw;

  // Prepare our context and socket
  zmq::context_t context(1);
  zmq::socket_t socket(context, ZMQ_REP);
  socket.bind("tcp://*:5555");

  //pid_t = getpid(void);


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

    commands com(&mw);
    std::string reply_message = com.process_command(bson).jsonString();
    //std::string reply_message = process_command(bson, mw);


    std::cout << "reply_message=" << reply_message << std::endl;

    zmq::message_t reply(reply_message.length());
    memcpy ((void *) reply.data(), reply_message.c_str(), reply_message.length());
    socket.send(reply);
    if(reply_message == "{\"return\":\"EXIT\"}") return EXIT_SUCCESS;
  }

  return EXIT_SUCCESS;
}
