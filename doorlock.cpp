#include <iostream>
#include <boost/program_options.hpp>

#include "door.hpp"

namespace po = boost::program_options;

int main(int ac, char *av[])
{
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help"     , "produce help message")
    ("init-gpio", "init the GPIOs")
    ("init-lock", "init the lock after powerloss")
    ("openlock" , "open the lock")
    ("closelock", "close the lock")
    ;

  po::variables_map vm;
  po::store(po::parse_command_line(ac, av, desc), vm);
  po::notify(vm);

  if (vm.count("help") || ac == 0) {
    std::cout << desc << "\n";
    return 1;
  }

  if (wiringPiSetup () == -1)
    return 1;

  if (vm.count("init-gpio")) {
    initPins();
    return 0;
  }
  else if (vm.count("init-lock")) {
    initLock();
    return 1;
  }
  else if (vm.count("openlock")) {
    openLock();
    return 0;
  }
  else if (vm.count("closelock")) {
    closeLock();
    return 0;
  }

  return 1;
}
