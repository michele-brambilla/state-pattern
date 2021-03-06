#include "state.hpp"
#include "helper.hpp"

#include <gtest/gtest.h>

class Machine {
  using State = State<Machine>;
  using Idle = Idle<Machine>;

public:
  Machine() : current_{new Idle} {}

  template <class T = Idle> void set(const T &) {
    current_ -> stop(*this);
    current_ = std::move(make_unique<T>(T()));
  }

  bool start() { return current_->start(*this); }
  bool stop() { return current_->stop(*this); }
  void report() { current_->report(*this); }
  const size_t counted() { return current_->counted(); }
  
  std::unique_ptr<State> current_;
};


TEST(state_machine, idle) {
  Machine machine;
  machine.set(Idle<Machine>());
  EXPECT_TRUE(machine.start());
  machine.set(Idle<Machine>());
  EXPECT_FALSE(machine.stop());
}

TEST(state_machine, start) {
  Machine machine;
  machine.set(Start<Machine>());
  EXPECT_FALSE(machine.start());
  machine.set(Start<Machine>());
  EXPECT_TRUE(machine.stop());
}

TEST(state_machine, stop) {
  Machine machine;
  machine.set(Stop<Machine>());
  EXPECT_FALSE(machine.start());
  machine.set(Stop<Machine>());
  EXPECT_FALSE(machine.stop());
}

TEST(state_machine, transition_idle_to_start) {
  Machine machine;
  ASSERT_FALSE(machine.stop());
  ASSERT_TRUE(machine.start());

  EXPECT_FALSE(machine.start());
  EXPECT_TRUE(machine.stop());
}

TEST(state_machine, transition_start_to_stop) {
  Machine machine;
  machine.set(Start<Machine>());
  ASSERT_FALSE(machine.start());
  ASSERT_TRUE(machine.stop());

  EXPECT_FALSE(machine.start());
  EXPECT_FALSE(machine.stop());
}

TEST(state_machine, run_method_in_thread) {
  Machine machine;
  std::this_thread::sleep_for(std::chrono::milliseconds{100});
  EXPECT_TRUE(machine.counted() == 0 );
  
  machine.start();
  std::this_thread::sleep_for(std::chrono::milliseconds{100});
  EXPECT_GT(machine.counted(), 0 );

  machine.stop();
  EXPECT_TRUE(machine.counted() == 0 );
}
