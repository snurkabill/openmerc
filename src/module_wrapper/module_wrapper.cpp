#include "module_wrapper.hpp"

module_wrapper::module_wrapper() {

  m_id        = 0;
  m_thread_id = 0;

}

module_wrapper::~module_wrapper() {}


MODUL_CONT::iterator module_wrapper::find(int module_id) {

  for (auto itr = m_modules.begin(); itr != m_modules.end(); ++itr) {
    if(itr->second.id == module_id) return itr;
  }

  return m_modules.end();
}


std::string module_wrapper::get_info() {

  /* make a bit more complex object with some nesting
   * { x : 'asdf', y : true, subobj : { z : 3, q : 4 } }
   */
  // BSONObj y = BSON( "x" << "asdf" << "y" << true << "subobj" << BSON( "z" << 3 << "q" << 4 ) );

  /**
   * {number_of_modules: ..., modules: [{module info}, ...] ,number_of_threads: ..., threads: [{threads info}, ...] }
   */

  //BSONObj b = BSON("number_of_modules" << m_modules.size() << "modules:" <<);

  std::stringstream ss;
  ss << "{ \"number_of_modules\":" << m_modules.size() << " \"modules\": [";
  for (auto itr = m_modules.begin(); itr != m_modules.end(); ++itr) {
    ss << itr->second.toBSON().jsonString() << ',';
  }
  ss << "{\"last\": \"foo\"}]}";
  return ss.str();
}

// in json format
// std::string module_wrapper::get_info() {
//
//   std::stringstream ss;
//
//   ss <<
//   "_________________________________________________________________________" << '\n' <<
//   "MODULES:" << '\n' <<
//   "number_of_modules=" << m_modules.size() << '\n' <<
//   "_________________________________________________________________________" << '\n';
//
//   for (auto itr = m_modules.begin(); itr != m_modules.end(); ++itr) {
//     ss << itr->second.str() << '\n';
//   }
//
//   ss <<
//   "__________________________________________________________________________\n";
//   /* Threads */
//
//   ss << "THREADS: " << '\n' <<
//   "number_of_threads=" << m_threads.size() << '\n' <<
//   "active thread_id: ";
//   for (auto itr = m_threads.begin(); itr != m_threads.end(); ++itr) {
//     ss << itr->first << ", ";
//   }
//   ss << '\n';
//
//   ss << "___________________________________________________________________________\n";
//
//
//   /* Groups */
//
//   ss << "GROUPS: " << '\n' <<
//   "number_of_groups=" << m_threads.size() << '\n';
//   for (auto itr = m_groups.begin(); itr != m_groups.end(); ++itr) {
//     ss << itr->first << ": " << itr->second.str() << '\n';
//   }
//
//   ss << "__________________________________________________________________________\n\n\n";
//
//   return ss.str();
//
// }

/**
 * Metoda vytiskne info o aktualnich modulech
 * @type {}
 */
void module_wrapper::print_info() {

  std::cout << get_info() << std::endl;
}

/**
 * metoda vrati unikatni id pro modul
 * @type {[type]}
 * @TODO: vyuzivat jiz zahozena id
 */
int module_wrapper::get_unique_id() {
  m_id++;
  return m_id;
}

/**
 * metoda vratu unikatni id pro vlakno
 * @type {[type]}
 * @TODO: vyuzivat jiz zahozena id
 */
int module_wrapper::get_thread_id() {
  m_thread_id++;
  return m_thread_id++;
}


// @TODO: pridat i nepovinne parametry a nastavit jejich default!
//        check group_id if exists
// @return module_id
int module_wrapper::add(
  std::string module_path,
  std::string module_name,
  int permission,
  int group_id) {

  // load the module library
  void* module = dlopen(module_path.c_str(), RTLD_LAZY);
  if (!module) {
    std::cerr << "Cannot load module: " << dlerror() << '\n';
    return (-1); // @TODO: throw exception
  }

  // reset errors
  dlerror();

  // load the symbols
  create_t* create_module = (create_t*) dlsym(module, "create");
  const char* dlsym_error = dlerror();
  if (dlsym_error) {
    std::cerr << "Cannot load symbol create: " << dlsym_error << '\n';
    return (-2); // @TODO: throw exception
  }

  destroy_t* destroy_module = (destroy_t*) dlsym(module, "destroy");
  dlsym_error = dlerror();
  if (dlsym_error) {
    std::cerr << "Cannot load symbol destroy: " << dlsym_error << '\n';
    return (-3); // @TODO: throw exception
  }

  module_struct module_tmp;

  module_tmp.id           = get_unique_id();
  module_tmp.name         = module_name;
  module_tmp.destroy      = destroy_module;
  module_tmp.module       = create_module();
  module_tmp.library      = module;
  module_tmp.module_path  = module_path;
  module_tmp.permission   = permission;
  module_tmp.group_id     = group_id;


  m_modules.insert( std::pair<int, module_struct>(module_tmp.id, module_tmp));

  return module_tmp.id;
}


/**
 * metoda zavola init() z modulu a hodi ji config.
 * @type {[type]}
 * @TODO: pridat path_to_config jako argument
 * 				zkontrolovat navratovou hodnotu init()
 */
void module_wrapper::init(module_struct & module, std::string path_to_config) {

  std::cout << "path_to_config=" << path_to_config << std::endl;
  module.config_path = path_to_config;
  module.module->init();

  module.initialized=true;
}

void module_wrapper::stop(module_struct & module) {

  std::cout << "stoping module" << std::endl;
  module.active = false;
  //if (m_threads[module.thread_index].joinable())
  //  m_threads[module.thread_index].join();

}

/**
 * metoda pusti v novem vlakne update
 * @type {[type]}
 * @TODO:
 */
void module_wrapper::run(module_struct & module, int type) {

  std::cout << "run_type=" << type << std::endl;
  module.active = true;
  module.thread_index = get_thread_id();

  m_threads.insert(
    std::pair<int, std::thread>(
      module.thread_index,
      std::thread(
        &module_wrapper::create_thread,
        this,
        &module
      )
    )
  );

}

void module_wrapper::remove(module_struct & module) {

  std::cout << "removing module" << std::endl;
  module.active = false;

  if (m_threads[module.thread_index].joinable())
  {
    m_threads[module.thread_index].join();
  }


  module.destroy(module.module);
  dlclose(module.library);

  // @TODO remove element from containers

}


void module_wrapper::create_thread(module_struct * module) {

  module->active = true;

  while(module->active)
  {
    module->module->update();
  }

  std::cout << "module id=" << module->id << " finished!" << std::endl;
}
