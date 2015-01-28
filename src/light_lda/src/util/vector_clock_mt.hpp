// author: jinliang

#pragma once

#include "util/vector_clock.hpp"
#include "util/lock.hpp"

#include <boost/noncopyable.hpp>
#include <glog/logging.h>
#include <boost/thread.hpp>
#include <vector>
#include <cstdint>

namespace petuum {

// VectorClock is a thread-safe extension of VectorClockST.
class VectorClockMT : public VectorClock, boost::noncopyable {
public:
  VectorClockMT();
  explicit VectorClockMT(const std::vector<int32_t>& ids);

  // Override VectorClock
  void AddClock(int32_t id, int32_t clock = 0);
  int32_t Tick(int32_t id);

  // Accessor to a particular clock.
  int32_t get_clock(int32_t id) const;
  int32_t get_min_clock() const;

private:
  // Lock for slowest record
  mutable SharedMutex mutex_;
};

}  // namespace petuum
