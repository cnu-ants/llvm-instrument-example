#ifndef AST_TRAVERSAL_INVALID_FILE_EXCEPTION_H_
#define AST_TRAVERSAL_INVALID_FILE_EXCEPTION_H_

#include <stdexcept>
#include <string>

namespace instrumentor {
class InvalidFileException : public std::runtime_error {
  public:
    InvalidFileException(std::string msg);
};

}  // namespace instrumentor

#endif  // AST_INSTRUMENT_INVALID_FILE_EXCEPTION_H_
