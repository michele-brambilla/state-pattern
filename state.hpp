#include <memory>
#include <thread>
#include <chrono>
#include <iostream>

template <class T> class State {
  using machine_type = T;

public:
  virtual bool start(machine_type &machine) { return false; }
  virtual bool stop(machine_type &machine) { return false; }
  virtual void report(machine_type &machine) { /*todo*/
  }
  virtual const size_t counted() { return 0; }
};

template <class T> class Idle;
template <class T> class Start;
template <class T> class Stop;

template <class T> class Idle : public State<T> {
  using machine_type = T;

public:
  bool start(machine_type &machine) override {
    machine.current_.reset(new Start<machine_type>);
    return true;
  }
  bool stop(machine_type &machine) override { return false; }
  void report(machine_type &machine) override { /*todo*/
  }
  const size_t counted() override { return 0; }

};

template <class T> class Start : public State<T> {
  using machine_type = T;

public:
  Start() {
    executer = std::thread([&]{this->counter();});
  }
  Start(Start&& other) : countee{other.countee} {
    other.stop_ = true;
    other.executer.join();
    executer = std::thread([&]{this->counter();});
  };
  ~Start() {
    if(executer.joinable()) {
      stop_=true;
      executer.join();
    }
  }
  bool start(machine_type &machine) override {
    return false; }
  bool stop(machine_type &machine) override {
    stop_ = true;
    executer.join();
    machine.current_.reset(new Stop<machine_type>);
    return true;
  }
  void report(machine_type &machine) override { /*todo*/
  }
  const size_t counted() override {
    return countee;
  }
  
private:
  void counter() {
    while(!stop_) {
      std::this_thread::sleep_for(std::chrono::milliseconds{10});
      ++countee;
    }
  }

  bool stop_{false};
  size_t countee{0};
  std::thread executer;
};

template <class T> class Stop : public State<T> {
  using machine_type = T;

public:
  bool start(machine_type &machine) override { return false; }
  bool stop(machine_type &machine) override { return false; }
  void report(machine_type &machine) override { /*todo*/
  }
  const size_t counted() override { return 0; }
};
