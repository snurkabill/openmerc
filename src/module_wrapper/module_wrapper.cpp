#include "module_wrapper.hpp"

module_wrapper::module_wrapper() {

  m_id = 0;

}

module_wrapper::~module_wrapper() {

}




MODUL_CONT::iterator module_wrapper::find(int module_id) {

  for (auto itr = m_modules.begin(); itr != m_modules.end(); ++itr) {
    if(itr->second.id == module_id) return itr;
  }

  return m_modules.end();
}


/**
 * Metoda vytiskne info o aktualnich modulech
 * @type {}
 */
void module_wrapper::print_info() {

  /* Modules */

  std::cout <<
  "MODULES:" << std::endl <<
  "number_of_modules=" << m_modules.size() << std::endl <<
  "____________________________________________________________________________________" <<
  std::endl;

  for (auto itr = m_modules.begin(); itr != m_modules.end(); ++itr) {
    std::cout << itr->second.str() << std::endl;
  }



  /* Threads */

  /* Groups */

  std::cout << std::endl << std::endl;

}

int module_wrapper::get_unique_id() {
  m_id++;
  return m_id;
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
    exit(EXIT_FAILURE); // @TODO: throw exception
  }

  // reset errors
  dlerror();

  // load the symbols
  create_t* create_module = (create_t*) dlsym(module, "create");
  const char* dlsym_error = dlerror();
  if (dlsym_error) {
    std::cerr << "Cannot load symbol create: " << dlsym_error << '\n';
    exit(EXIT_FAILURE); // @TODO: throw exception
  }

  destroy_t* destroy_module = (destroy_t*) dlsym(module, "destroy");
  dlsym_error = dlerror();
  if (dlsym_error) {
    std::cerr << "Cannot load symbol destroy: " << dlsym_error << '\n';
    exit(1); // @TODO: throw exception
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


  m_modules.insert( std::pair<int, module_struct>(group_id, module_tmp));

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

void module_wrapper::run(module_struct & module, int type) {

  std::cout << "run_type=" << type << std::endl;

  module.thread_index = -1;
  // module.thread_index = m_threads.size()-1;
}

void module_wrapper::remove(module_struct & module) {

  std::cout << "removing module" << std::endl;
  module.active = false;

  // if (m_threads[module.thread_index].joinable())
  // {
  //   m_threads[module.thread_index].join();
  // }


  module.destroy(module.module);
  dlclose(module.library);

  // @TODO remove element from containers

}


int main() {

  module_wrapper mw;

  int module = mw.add("/home/zahrada/Dropbox/openmerc/IPC/0.0.1/src/module_wrapper/module.so", "fooo", 0, 1);

  auto itr = mw.find(module);

  mw.print_info();


  mw.init(itr->second, "config");
  itr = mw.find(module);
  mw.run(itr->second, 42);
  itr = mw.find(module);
  mw.stop(itr->second);
  itr = mw.find(module);
mw.print_info();
  mw.remove(itr->second);

  mw.print_info();
  return 0;
}
