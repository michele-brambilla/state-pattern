#include <memory>
#include <thread>
#include <chrono>

template <class T> class State {
  using machine_type = T;

public:
  virtual bool start(machine_type &machine) { return false; }
  virtual bool stop(machine_type &machine) { return false; }
  virtual void report(machine_type &machine) { /*todo*/
  }
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
private:
  void run() {
    
  }

};

template <class T> class Start : public State<T> {
  using machine_type = T;

public:
  Start() { executer = std::thread([&]{ counter(); }); }
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

private:
  void counter() {
    while(!stop_) {
      std::this_thread::sleep_for(std::chrono::milliseconds{1000});
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
};
