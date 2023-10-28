#pragma once
#ifndef _application_hpp
#define _application_hpp

#include "filesystem.hpp"
#include "internal.hpp"
#include "pixmappool.hpp"
#include "window.hpp"

class application {
public:
  application(int argc, char **argv);
  ~application();

  int run();
};

#endif