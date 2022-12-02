#include "instrumentor.h"
#include "clang/Lex/PreprocessorOptions.h"

namespace instrumentor {

#if __linux__
static const std::array<std::string, 4> paths = {
  "/usr/include",
  "/usr/include/x86_64-linux-gnu",
  "/usr/lib/gcc/x86_64-linux-gnu/9/include",
  "/usr/lib/jvm/java-1.11.0-openjdk-amd64/include/linux",
};
#elif __APPLE__
static const std::array<std::string, 1> paths = {
  "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include",
};
#endif

void Instrumentor::Instrument(const char* file_path) {
  // CompilerInstance will hold the instance of the Clang compiler for us,
  // managing the various objects needed to run the compiler.
  clang::CompilerInstance comp_inst;
  
  // Set C++ as the target
  comp_inst.getLangOpts().CPlusPlus = 1;

  // Diagnostics manage problems and issues in compile 
  comp_inst.createDiagnostics(NULL, false);

  // Set target platform options 
  // Initialize target info with the default triple for our platform.
  auto to = std::make_shared<clang::TargetOptions>();
  to->Triple = llvm::sys::getDefaultTargetTriple();
  clang::TargetInfo *ti = clang::TargetInfo::CreateTargetInfo(comp_inst.getDiagnostics(), to);
  comp_inst.setTarget(ti);

  // FileManager supports for file system lookup, file system caching, and directory search management.
  comp_inst.createFileManager();
  clang::FileManager &file_manager = comp_inst.getFileManager();
  
  // SourceManager handles loading and caching of source files into memory.
  comp_inst.createSourceManager(file_manager);
  clang::SourceManager &source_manager = comp_inst.getSourceManager();

  // Only if regular_file
  //if (!std::experimental::filesystem::is_regular_file(file_path)) return;
  std::string fname = std::string(file_path);
  
  // Preprocessor runs within a single source file
  comp_inst.createPreprocessor(clang::TU_Module);

  // Add HeaderSearch Path
  clang::Preprocessor &pp = comp_inst.getPreprocessor();

  const llvm::Triple &HeaderSearchTriple = pp.getTargetInfo().getTriple();
  
  clang::HeaderSearchOptions &hso = comp_inst.getHeaderSearchOpts();

  // <Warning!!> -- Platform Specific Code lives here
  // This depends on A) that you're running linux and
  // B) that you have the same GCC LIBs installed that I do.
  /*
  $ gcc -xc -E -v -
  ..
  /usr/lib/gcc/x86_64-linux-gnu/4.9/include
  /usr/local/include
  /usr/lib/gcc/x86_64-linux-gnu/4.9/include-fixed
  /usr/include/x86_64-linux-gnu
  /usr/include
  End of search list.
  */
  
  for (int i = 0; i < paths.size(); i++) {
      hso.AddPath(paths[i], clang::frontend::Angled, false, false);
  }
  // </Warning!!> -- End of Platform Specific Code

  clang::ApplyHeaderSearchOptions(pp.getHeaderSearchInfo(), hso,
                          pp.getLangOpts(), HeaderSearchTriple);
  
  // ASTContext holds long-lived AST nodes (such as types and decls) .
  comp_inst.createASTContext();

  // A Rewriter helps us manage the code rewriting task.
  clang::Rewriter rewriter;
  rewriter.setSourceMgr(source_manager, comp_inst.getLangOpts());

  // Set the main file handled by the source manager to the input file.
  std::cout << "Getting a source file: " << fname << std::endl;
  const clang::FileEntry *fin = file_manager.getFile(fname).get();
  source_manager.setMainFileID(source_manager.createFileID(fin, clang::SourceLocation(), clang::SrcMgr::C_User));
  
  // Inform Diagnostics that processing of a source file is beginning. 
  comp_inst.getDiagnosticClient().BeginSourceFile(comp_inst.getLangOpts(),&comp_inst.getPreprocessor());
  
  // Create an AST consumer instance which is going to get called by ParseAST.
  NodeConsumer consumer(rewriter, comp_inst.getLangOpts());

  // Parse the file to AST, registering our consumer as the AST consumer.
  std::cout << "Instrumenting..." << std::endl;
  clang::ParseAST(pp, &consumer, comp_inst.getASTContext(), clang::TU_Module);

  // .c -> .c ; .cpp -> .cpp
  llvm::StringRef ifname = fin->getName();

  std::string ofname = GetOutputName(ifname);

  const clang::RewriteBuffer *rewrite_buf = rewriter.getRewriteBufferFor(source_manager.getMainFileID());
  if(rewrite_buf != nullptr) {
    std::ofstream ofile(ofname);
    std::cout << "Writing the instrumented source file to " << ofname << std::endl;
    ofile << string(rewrite_buf->begin(), rewrite_buf->end());
    ofile.close();
  }
}

std::string Instrumentor::GetOutputName(llvm::StringRef fname) {
  // Support only three types of files (.c, .cc, .cpp)
  if (fname.endswith(".c"))
    return std::string(fname.drop_back(2/*.c*/)) + "_instrumented.c";
  else if (fname.endswith(".cpp")) 
    return std::string(fname.drop_back(4/*.cpp*/)) + "_instrumented.cpp";
  else if (fname.endswith(".cc"))
    return std::string(fname.drop_back(3/*.cc*/)) + "_instrumented.cc";
  else if (fname.endswith(".h"))
    return std::string(fname.drop_back(3/*.cc*/)) + "_instrumented.h";
  else if (fname.endswith(".hpp"))
    return std::string(fname.drop_back(3/*.cc*/)) + "_instrumented.hpp";
  else 
    throw InvalidFileException(std::string(fname) + 
        ": file extension must be one of .c, .cc, .cpp, .h, or .hpp");
}

}  // namespace instrumentor
