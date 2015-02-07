#ifndef MODULE_WRAPPER
#define MODULE_WRAPPER


#include <iostream>
#include <vector>
#include <dlfcn.h>
#include <thread>
#include <string>
#include <sstream>
#include <map>
#include "module_base.hpp"


// dev
#include <unistd.h>


// @TODO: dodelat konstruktor, destruktor a operatory...
typedef struct module_struct {


  /**
   * Basic info about module
   */
  int             id;             // id of the module
  std::string     name;           // name of the module
  std::string     config_path;    // path to config file
  std::string     module_path;    // path to module

  /**
   *  Status : @TODO: predelat do enumu
   */
  bool            initialized;
  bool            active;

  /**
   * Dynamic linking
   */
  destroy_t *     destroy;    // pointer to destroy function
  module_base *   module;
  void *          library;

  /**
   * Threading
   */
  int             thread_index;


  /**
   * Permission and group
   */
  int permission;
  int group_id;


  module_struct() :
    id(-1),
    name("NONE"),
    config_path("NONE"),
    module_path("NONE"),
    initialized(false),
    active(false),
    destroy(NULL),
    module(NULL),
    library(NULL),
    thread_index(-1),
    permission(-1),
    group_id(-1){};


  // method returns string with summary about this structure
  std::string str() {
    std::stringstream ss;

    ss <<
    "id=" << id << " " <<
    "name=" << name << " " <<
    "active=" << active << " " <<
    "initialized=" << initialized << " " <<
    "thread_index=" << thread_index << " " <<
    "config=" << config_path << " " <<
    "module_path=" << module_path << " " <<
    "permission=" << permission << " " <<
    "group_id=" << group_id;

    return ss.str();
  }

} modul_struct;

// unclear.
typedef struct group_struct {

  std::string           name;
  std::vector<int>      subscribers; // id modulu, kam se preposilaji prikazy.

} group_struct;

typedef std::map<int, module_struct> MODUL_CONT;  // module_id -> module_struct

class module_wrapper {

private:
  int                           m_id;        // posledni pouzite id modulu
  int                           m_thread_id; // posledni pouzite id vlakna
  MODUL_CONT                    m_modules;
  std::map<int, std::thread>    m_threads;
  std::map<int, group_struct>   m_groups;

public:
  module_wrapper();
  ~module_wrapper();

  int add(
    std::string module_path,
    std::string module_name,
    int permission,
    int group_id);

  void init(module_struct & module, std::string path_to_config);

  void run(module_struct & module, int type = 0);

  void remove(module_struct & module);

  void stop(module_struct & module);

  void create_thread(module_struct * module);

  MODUL_CONT::iterator find(int module_id);

  int get_unique_id();
  int get_thread_id();

  void print_info();
  void get_info();

};

#endif /* MODULE_WRAPPER */
