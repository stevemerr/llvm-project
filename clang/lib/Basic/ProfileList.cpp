//===--- ProfileList.h - ProfileList filter ---------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// User-provided filters include/exclude profile instrumentation in certain
// functions or files.
//
//===----------------------------------------------------------------------===//

#include "clang/Basic/ProfileList.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include "llvm/Support/SpecialCaseList.h"

#include <optional>

using namespace clang;

namespace clang {

class ProfileSpecialCaseList : public llvm::SpecialCaseList {
public:
  static std::unique_ptr<ProfileSpecialCaseList>
  create(const std::vector<std::string> &Paths, llvm::vfs::FileSystem &VFS,
         std::pair<unsigned, std::string> &Error);

  static std::unique_ptr<ProfileSpecialCaseList>
  createOrDie(const std::vector<std::string> &Paths,
              llvm::vfs::FileSystem &VFS);

  bool isEmpty() const { return Sections.empty(); }

  bool hasPrefix(StringRef Prefix) const {
    for (const auto &It : Sections)
      if (It.Entries.count(Prefix) > 0)
        return true;
    return false;
  }
};

std::unique_ptr<ProfileSpecialCaseList>
ProfileSpecialCaseList::create(const std::vector<std::string> &Paths,
                               llvm::vfs::FileSystem &VFS,
                               std::pair<unsigned, std::string> &Error) {
  auto PSCL = std::make_unique<ProfileSpecialCaseList>();
  if (PSCL->createInternal(Paths, VFS, Error))
    return PSCL;
  return nullptr;
}

std::unique_ptr<ProfileSpecialCaseList>
ProfileSpecialCaseList::createOrDie(const std::vector<std::string> &Paths,
                                    llvm::vfs::FileSystem &VFS) {
  std::pair<unsigned, std::string> Error;
  if (auto PSCL = create(Paths, VFS, Error))
    return PSCL;
  // TODO: add init function and use diagnose instead fo report_fatal_error
  llvm::report_fatal_error(llvm::Twine(Error.second));
}

} // namespace clang

ProfileList::ProfileList(ArrayRef<std::string> Paths, SourceManager &SM)
    : SCL(ProfileSpecialCaseList::createOrDie(
          Paths, SM.getFileManager().getVirtualFileSystem())),
      Empty(SCL->isEmpty()), SM(SM) {}

ProfileList::~ProfileList() = default;

static StringRef getSectionName(llvm::driver::ProfileInstrKind Kind) {
  switch (Kind) {
  case llvm::driver::ProfileInstrKind::ProfileNone:
    return "";
  case llvm::driver::ProfileInstrKind::ProfileClangInstr:
    return "clang";
  case llvm::driver::ProfileInstrKind::ProfileIRInstr:
    return "llvm";
  case llvm::driver::ProfileInstrKind::ProfileCSIRInstr:
    return "csllvm";
  case llvm::driver::ProfileInstrKind::ProfileIRSampleColdCov:
    return "sample-coldcov";
  }
  llvm_unreachable("Unhandled llvm::driver::ProfileInstrKind enum");
}

ProfileList::ExclusionType
ProfileList::getDefault(llvm::driver::ProfileInstrKind Kind) const {
  StringRef Section = getSectionName(Kind);
  // Check for "default:<type>"
  if (SCL->inSection(Section, "default", "allow"))
    return Allow;
  if (SCL->inSection(Section, "default", "skip"))
    return Skip;
  if (SCL->inSection(Section, "default", "forbid"))
    return Forbid;
  // If any cases use "fun" or "src", set the default to FORBID.
  if (SCL->hasPrefix("fun") || SCL->hasPrefix("src"))
    return Forbid;
  return Allow;
}

std::optional<ProfileList::ExclusionType>
ProfileList::inSection(StringRef Section, StringRef Prefix,
                       StringRef Query) const {
  if (SCL->inSection(Section, Prefix, Query, "allow"))
    return Allow;
  if (SCL->inSection(Section, Prefix, Query, "skip"))
    return Skip;
  if (SCL->inSection(Section, Prefix, Query, "forbid"))
    return Forbid;
  if (SCL->inSection(Section, Prefix, Query))
    return Allow;
  return std::nullopt;
}

std::optional<ProfileList::ExclusionType>
ProfileList::isFunctionExcluded(StringRef FunctionName,
                                llvm::driver::ProfileInstrKind Kind) const {
  StringRef Section = getSectionName(Kind);
  // Check for "function:<regex>=<case>"
  if (auto V = inSection(Section, "function", FunctionName))
    return V;
  if (SCL->inSection(Section, "!fun", FunctionName))
    return Forbid;
  if (SCL->inSection(Section, "fun", FunctionName))
    return Allow;
  return std::nullopt;
}

std::optional<ProfileList::ExclusionType>
ProfileList::isLocationExcluded(SourceLocation Loc,
                                llvm::driver::ProfileInstrKind Kind) const {
  return isFileExcluded(SM.getFilename(SM.getFileLoc(Loc)), Kind);
}

std::optional<ProfileList::ExclusionType>
ProfileList::isFileExcluded(StringRef FileName,
                            llvm::driver::ProfileInstrKind Kind) const {
  StringRef Section = getSectionName(Kind);
  // Check for "source:<regex>=<case>"
  if (auto V = inSection(Section, "source", FileName))
    return V;
  if (SCL->inSection(Section, "!src", FileName))
    return Forbid;
  if (SCL->inSection(Section, "src", FileName))
    return Allow;
  return std::nullopt;
}
