#ifndef AST_TRAVERSAL_NODE_VISITOR_H_
#define AST_TRAVERSAL_NODE_VISITOR_H_

#include <experimental/filesystem>

#include <cstdio>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <utility>

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

using namespace clang;
using namespace std;

namespace instrumentor {

class NodeVisitor : public RecursiveASTVisitor<NodeVisitor> {
public:
    NodeVisitor(Rewriter &R, const LangOptions &langOptions);
    bool VisitDecl(Decl *d);
    bool VisitStmt(Stmt *s);
    bool VisitFunctionDecl(FunctionDecl *f);
private:
    Rewriter& rewriter;
    const LangOptions &lang_opt;
};

class NodeConsumer : public ASTConsumer {
public:
    NodeConsumer(Rewriter &R, const LangOptions &langOpts);
    virtual void HandleTranslationUnit(clang::ASTContext &Context);
private:
    NodeVisitor visitor;
};

}  // namespace instrumentor

#endif  // AST_TRAVERSAL_NODE_VISITOR_H_
