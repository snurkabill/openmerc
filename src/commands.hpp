#ifndef COMMANDS
#define COMMANDS


#include <iostream>
#include "cppzmq/zmq.hpp"
#include "mongo/client/dbclient.h"
#include "module_wrapper/module_wrapper.hpp"



/**
 * class for processing commands in openmerc core
 */
class commands {

private:
  module_wrapper * m_modules; // pointer to instance module_wrapper

public:
  commands(){};
  commands(module_wrapper * mw): m_modules(mw){}
  ~commands(){};


  mongo::BSONObj process_command(mongo::BSONObj b);
  mongo::BSONObj command_not_found();

  /**
   * 	OPENMERC CORE FUNCTIONS
   */

  mongo::BSONObj mercore_status(mongo::BSONObj b);

   /**
    * MODULES FUNCTIONS
    */

  mongo::BSONObj module_add(mongo::BSONObj b);
  mongo::BSONObj module_set(mongo::BSONObj b);
  mongo::BSONObj module_stop(mongo::BSONObj b);
  mongo::BSONObj module_run(mongo::BSONObj b);
  mongo::BSONObj module_status(mongo::BSONObj b);
  mongo::BSONObj module_find(mongo::BSONObj b);
  mongo::BSONObj module_remove(mongo::BSONObj b);

  /**
   * MODULES CONNECTION
   * propojeni modulu
   */

  /**
   * MARKET DATA
   * zobrazovani aktualnich cenovy dat
   *
   */

  /**
   * HISTORICAL DATA
   */

  /**
   * PORTFOLIO
   */

  /**
   * ACCOUNTS
   */

  /**
   * CONTRACTS
   */

  /**
   * NEWS
   */


};

#endif
