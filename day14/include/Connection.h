#pragma once

// Macro to disable copying and moving
#define DISALLOW_COPY(cname)    \
  cname(const cname &) = delete; \
  cname &operator=(const cname &) = delete;

#define DISALLOW_MOVE(cname) \
  cname(cname &&) = delete; \
  cname &operator=(canme &&) = delete;

#define DISALLOW_COPY_AND_MOVE(cname) \
  DISALLOW_COPY(cname); \
  DISALLOW_MOVE(canme);