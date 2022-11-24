#include "node_visitor.h"

namespace instrumentor {

NodeVisitor::NodeVisitor(Rewriter &R, const LangOptions &langOptions) :
  rewriter(R), lang_opt(langOptions) {}

bool NodeVisitor::VisitDecl(Decl *d) {
  return true;
}

bool NodeVisitor::VisitStmt(Stmt *s) {
  if (auto* decl_stmt = llvm::dyn_cast<DeclStmt>(s)) {
    assert(decl_stmt->isSingleDecl()); // temporary - for multiple decls : TBD
    Decl* decl = decl_stmt->getSingleDecl();
    if (auto* var_decl = llvm::dyn_cast<VarDecl>(decl)){ 
      string var_name = var_decl->getNameAsString();
      string var_type = var_decl->getType().getAsString();
      if (var_type == "int" && var_decl->hasInit()) { // int <var> = <expr>;
        char* str;
        asprintf(&str, "printf(\"Var %s=%%d is declared\\n\", %s);", var_name.c_str(), var_name.c_str());
        SourceLocation end_loc = s->getEndLoc().getLocWithOffset(1);
        rewriter.InsertTextAfter(end_loc, str);
        free(str);
      } 
    }
  }
  return true;
}

bool NodeVisitor::VisitFunctionDecl(FunctionDecl *f) {
  return true;
}

NodeConsumer::NodeConsumer(Rewriter &R, const LangOptions &langOpts) :
  visitor(R, langOpts) {}

void NodeConsumer::HandleTranslationUnit(clang::ASTContext &Context) {
  visitor.TraverseDecl(Context.getTranslationUnitDecl());
}

}  // instrumentor
