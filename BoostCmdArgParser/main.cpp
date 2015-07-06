// STD
#include <iostream>
#include <string>

// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

#define ACCEL_TYPE_STR    "acceltype"
#define ACCEL_DIMS_STR    "accel.dims"
#define ACCEL_MAX_MEM_STR "accel.max_mem_space"


void parse_command_arguments(int ac, char** av)
{
  // Declare the supported options.
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help", "produce help message")
    (ACCEL_TYPE_STR,    po::value<std::string>(), "Type of acceleration structure used in stream surface tracing. Depending on the acceleration type, choose the acceleration memory size or dimensions.")
    (ACCEL_DIMS_STR,    po::value<std::string>(), "Dimensions of the acceleration structure.")
    (ACCEL_MAX_MEM_STR, po::value<std::size_t>(), "Size of the accleration structure in memory.")
    ;

  po::variables_map vm;
  po::store(po::parse_command_line(ac, av, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << "\n";
    return;
  }

  if (vm.count(ACCEL_TYPE_STR)) {
    std::cout << "Acceleration Structure Type is " << vm[ACCEL_TYPE_STR].as<std::string>() << std::endl;
  }
  else {
    std::cout << "Acceleration Structure Type is not set.\n";
  }

  if (vm.count(ACCEL_DIMS_STR)) {
    std::cout << "Acceleration Structure Dimensions is " << vm[ACCEL_DIMS_STR].as<std::string>() << std::endl;
  }
  else {
    std::cout << "Acceleration Structure Dimensions is not set.\n";
  }

  if (vm.count(ACCEL_MAX_MEM_STR)) {
    std::cout << "Acceleration Structure Max Memory size is " << vm[ACCEL_MAX_MEM_STR].as<std::size_t>() << std::endl;
  }
  else {
    std::cout << "Acceleration Structure Max Memory size is not set.\n";
  }

}

int main(int ac, char** av)
{
  parse_command_arguments(ac, av);

  return 0;
}