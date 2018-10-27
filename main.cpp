#pragma once

#include "clang/Lex/PPCallbacks.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/Core/Replacement.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/Tooling.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "llvm/Support/CommandLine.h"
#include <cstdio>
#include <fstream>
#include <set>
#include <cmath>
#include <chrono>
#include <iomanip>
#include <sstream>
#include "llvm/Support/Debug.h"
#include "clang/AST/RecursiveASTVisitor.h"

namespace cl = llvm::cl;
namespace ct = clang::tooling;
using namespace clang;

cl::OptionCategory ToolCategory("category");
cl::opt<std::string> OutputFilename("o",
  cl::desc("Output filename"),
  cl::value_desc("filename"),
  cl::cat(ToolCategory));

std::stringstream g_ss;

class ApiparseRAV : public clang::RecursiveASTVisitor<ApiparseRAV> {
private:
  CompilerInstance * CI;
public:
  ApiparseRAV(CompilerInstance * CI) :
    CI(CI)
  {}
  bool VisitDecl(clang::Decl *D)
  {
    /*if (auto const *pFD = llvm::dyn_cast<clang::FunctionDecl>(D))
    {
      if (auto const *pRD = llvm::dyn_cast<clang::RecordDecl>(pFD->getDeclContext()))
      {
        llvm::outs() << pRD->getNameAsString() << "::\n";
      }
      llvm::outs() << pFD->getNameAsString() << "\n";
    }*/
    bool print = false;
    if (auto const *pFD = llvm::dyn_cast<clang::FunctionDecl>(D))
    {
      print = !pFD->getDeclContext()->isRecord() && (
        pFD->getNameAsString().find("D3D11") == 0
        || pFD->getNameAsString().find("ID3D11") == 0
        || pFD->getNameAsString().find("IDXGI") == 0);

    }
    if (auto const *pRD = llvm::dyn_cast<clang::TagDecl>(D))
    {
      print = pRD->getNameAsString().find("D3D11") == 0
        || pRD->getNameAsString().find("ID3D11") == 0
        || pRD->getNameAsString().find("IDXGI") == 0
        || pRD->getNameAsString().find("D3D_") == 0
        || pRD->getNameAsString().find("APP_DEPRECATED_HRESULT") == 0
        || pRD->getNameAsString().find("D3DCOLORVALUE") == 0
        || pRD->getNameAsString().find("DXGI") == 0;
    }
    if (auto const *pRD = llvm::dyn_cast<clang::TypedefDecl>(D))
    {
      print = pRD->getNameAsString().find("D3D11") == 0
        || pRD->getNameAsString().find("ID3D11") == 0
        || pRD->getNameAsString().find("IDXGI") == 0
        || pRD->getNameAsString().find("D3D_") == 0
        || pRD->getNameAsString().find("APP_DEPRECATED_HRESULT") == 0
        || pRD->getNameAsString().find("D3DCOLORVALUE") == 0
        || pRD->getNameAsString().find("DXGI") == 0;
    }
    if (print)
    {
      std::string stmt;
      llvm::raw_string_ostream stream(stmt);

      D->print(stream, PrintingPolicy(LangOptions()));

      stream.flush();
      g_ss << stmt << ";\n";
      //llvm::outs() << stmt << "\n";
    }
    return true;
  }
};

class ApiparseASTCounsumer : public clang::ASTConsumer {
private:
  CompilerInstance * CI;
public:
  ApiparseASTCounsumer(CompilerInstance * CI) :
    CI(CI)
  {}
public:
  void HandleTranslationUnit(clang::ASTContext &Ctx) {
    ApiparseRAV rav(CI);
    rav.TraverseDecl(Ctx.getTranslationUnitDecl());
  }
};

class ApiparseAction : public clang::ASTFrontendAction {
private:
public:
protected:
  std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef InFile) override
  {
    return std::unique_ptr<clang::ASTConsumer>(new ApiparseASTCounsumer(&CI));
  }
};

namespace ct = clang::tooling;

class ApiparseActionFactory : public ct::FrontendActionFactory {
public:
  clang::ASTFrontendAction *create() override {
    return new ApiparseAction;
  }
};

int main(int argc, const char **argv) {
  ct::CommonOptionsParser OptionsParser(argc, argv, ToolCategory, llvm::cl::OneOrMore);
  std::vector<std::string> fileSources = OptionsParser.getSourcePathList();
  std::string dst = OutputFilename;
  if (dst.empty()) {
    llvm::errs() << "[ERROR] Empty output filename\n";
    return 1;
  }

  ct::ClangTool tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());
  int ret = tool.run(new ApiparseActionFactory);
  if (!ret)
  {
    std::ofstream out(dst, std::ios::binary);
    out << g_ss.str();
    out.close();
  }
  else
  {
    getchar();
  }
  return 0;
}