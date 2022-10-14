#include "invalid_file_exception.h"

namespace instrumentor {

InvalidFileException::InvalidFileException(std::string msg) : std::runtime_error(msg) {}

}  // namespace instrumentor
