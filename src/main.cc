#include <iostream>

#include "instrumentor.h"
#include "invalid_file_exception.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    llvm::errs() << "Usage: kcov-branch-identify <filename>\n";
    return -1;
  }

  instrumentor::Instrumentor inst;

  try {
    inst.Instrument(argv[1]);
  } catch(instrumentor::InvalidFileException& e) {
    std::cerr << "[IFF] " << e.what() << std::endl;
    return -2;
  }
}
