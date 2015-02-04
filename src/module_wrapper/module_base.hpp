#ifndef MODUL_BASE
#define MODUL_BASE

class module_base {

public:
  module_base(){}

  virtual ~module_base() {}

  virtual int init() = 0;
  virtual int update() = 0;
  virtual int deinit() = 0;
};

// the types of the class factories
typedef module_base* create_t();
typedef void destroy_t(module_base*);

#endif
