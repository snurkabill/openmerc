/**
 * ret  - navrat ciselne hodnoty
 * bson - navrat hodnoty ve forme bsonu
 * msg  - navrat zpravy
 * err  - navrat chybove hlasky
 */


#include "commands.hpp"

/**
 * hlavni metoda, ktera vola nasledujici methody podle prikazu
 * @TODO: dodelat kontrolu existence atributu
 */
mongo::BSONObj commands::process_command(mongo::BSONObj b) {

  if (b["module"].str() == "True") {

    if (b["add"].str() == "True") {

      return module_add(b);

    } else if (b["set"].str() == "True") {

      return module_set(b);

    } else if (b["remove"].str() == "True") {

      return module_remove(b);

    } else if (b["stop"].str() == "True") {

      return module_stop(b);

    } else if (b["run"].str() == "True") {

      return module_run(b);

    } else if (b["status"].str() == "True") {

      return module_status(b);

    } else if (b["find"].str() == "True") {

      return module_find(b);
    }

  } else if (b["core"].str() == "True") {

    if (b["status"].str() == "True") {

      return mercore_status(b);
    }

  }

  return command_not_found();
}

/**
 * return message, for commands, which have not been found
 */
mongo::BSONObj commands::command_not_found() {

  mongo::BSONObjBuilder ret;

  ret.append("err", "Unknown command.");

  return ret.obj();
}

/**
 * 	MERCORE FUNCTIONS
 */

/**
 * method return basic info about openmerc
 */
mongo::BSONObj commands::mercore_status(mongo::BSONObj b) {

  mongo::BSONObjBuilder ret;

  ret.append("msg", "Openmerc is up and running.");

  return ret.obj();
}

/**
 * MODULES FUNCTIONS
 */

/**
 * methods add new module to openmerc
 * @type {[type]}
 */
mongo::BSONObj commands::module_add(mongo::BSONObj b) {


  std::string module_path = b["<module_path>"].str();
  int permission = std::atoi(b["-P"].str().c_str());
  std::string name = b["-n"].str();
  int group_id = 0;

  int module_id = m_modules->add(module_path, name, permission, group_id);

  mongo::BSONObjBuilder ret;
  if(module_id >= 0) ret.append("ret", module_id);
  else ret.append("err", "Unable to add module.");
  return ret.obj();
}

mongo::BSONObj commands::module_set(mongo::BSONObj b) {

  mongo::BSONObjBuilder ret;
  ret.append("msg", "method is not implemented yet");
  return ret.obj();
}

/**
 * method stops module with given MID, if it exists
 * @TODO: chceck existence of MID
 */
mongo::BSONObj commands::module_stop(mongo::BSONObj b) {

  int module_id = std::atoi(b["<mid>"].str().c_str());
  MODUL_CONT::iterator module = m_modules->find(module_id);

  m_modules->stop(module->second);

  mongo::BSONObjBuilder ret;
  ret.append("ret", module_id);
  ret.append("msg", "Module has been stoped.");

  return ret.obj();
}

mongo::BSONObj commands::module_run(mongo::BSONObj b) {

  int module_id = std::atoi(b["<mid>"].str().c_str());
  MODUL_CONT::iterator module = m_modules->find(module_id);

  m_modules->run(module->second);

  mongo::BSONObjBuilder ret;
  ret.append("msg", "Module is up and running");

  return ret.obj();
}

mongo::BSONObj commands::module_status(mongo::BSONObj b) {

  // @TODO: prepsat get_info method na get_status a navrat bsonu
  //        tato metoda bude vracet bson z te metody
  // return mw.get_info();
  mongo::BSONObjBuilder ret;
  ret.append("msg", "method is not implemented yet");
  return ret.obj();
}

mongo::BSONObj commands::module_find(mongo::BSONObj b) {

  mongo::BSONObjBuilder ret;
  ret.append("msg", "method is not implemented yet");
  return ret.obj();
}

mongo::BSONObj commands::module_remove(mongo::BSONObj b) {

  int module_id = std::atoi(b["<mid>"].str().c_str());
  MODUL_CONT::iterator module = m_modules->find(module_id);
  m_modules->remove(module->second);

  mongo::BSONObjBuilder ret;
  ret.append("ret", module_id);
  ret.append("msg", "Module has been removed.");

  return ret.obj();
}
