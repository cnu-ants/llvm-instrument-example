#ifndef AST_TRAVERSAL_INSTRUMENTOR_H_
#define AST_TRAVERSAL_INSTRUMENTOR_H_

//#include <experimental/filesystem>

#include <cstdio>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <utility>
#include <array>

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Parse/ParseAST.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Rewrite/Frontend/Rewriters.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/IntrusiveRefCntPtr.h"
#include "clang/Lex/HeaderSearch.h"
#include "clang/Frontend/Utils.h"

#include "invalid_file_exception.h"
#include "node_visitor.h"

namespace instrumentor {

class Instrumentor {
  public:
    void Instrument(const char* file_path); 

  private:
    // If the file extension is not one of .c, .cc, .cpp, throw an
    // InvalidFileException.
    std::string GetOutputName(llvm::StringRef fname);
};

}  // namespace instrumentor

#endif  // AST_TRAVERSAL_INSTRUMENTOR_H_
