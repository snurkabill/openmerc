#include "module_base.hpp"
#include <iostream>

class module : public module_base {
public:

  virtual int init() {
    std::cout << "hello world! I am init function." << std::endl;
    return 0;
  }

  virtual int update() {
    std::cout << "hello world! I am update function." << std::endl;
    return 0;
  }

  virtual int deinit() {
    std::cout << "hello world! I am deinit function." << std::endl;
    return 0;
  }

};


/* The class factories */
extern "C" module_base* create() {
  return new module;
}

extern "C" void destroy(module_base* mb) {
  delete mb;
}
