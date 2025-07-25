//===-- SpecialCaseList.h - special case list for sanitizers ----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//===----------------------------------------------------------------------===//
//
// This file implements a Special Case List for code sanitizers.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_SUPPORT_SPECIALCASELIST_H
#define LLVM_SUPPORT_SPECIALCASELIST_H

#include "llvm/ADT/StringMap.h"
#include "llvm/Support/Compiler.h"
#include "llvm/Support/GlobPattern.h"
#include "llvm/Support/Regex.h"
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace llvm {
class MemoryBuffer;
class StringRef;

namespace vfs {
class FileSystem;
}

/// This is a utility class used to parse user-provided text files with
/// "special case lists" for code sanitizers. Such files are used to
/// define an "ABI list" for DataFlowSanitizer and allow/exclusion lists for
/// sanitizers like AddressSanitizer or UndefinedBehaviorSanitizer.
///
/// Empty lines and lines starting with "#" are ignored. Sections are defined
/// using a '[section_name]' header and can be used to specify sanitizers the
/// entries below it apply to. Section names are globs, and
/// entries without a section header match all sections (e.g. an '[*]' header
/// is assumed.)
/// The remaining lines should have the form:
///   prefix:glob_pattern[=category]
/// If category is not specified, it is assumed to be empty string.
/// Definitions of "prefix" and "category" are sanitizer-specific. For example,
/// sanitizer exclusion support prefixes "src", "mainfile", "fun" and "global".
/// "glob_pattern" defines source files, main files, functions or globals which
/// shouldn't be instrumented.
/// Examples of categories:
///   "functional": used in DFSan to list functions with pure functional
///                 semantics.
///   "init": used in ASan exclusion list to disable initialization-order bugs
///           detection for certain globals or source files.
/// Full special case list file example:
/// ---
/// [address]
/// # Excluded items:
/// fun:*_ZN4base6subtle*
/// global:*global_with_bad_access_or_initialization*
/// global:*global_with_initialization_issues*=init
/// type:*Namespace::ClassName*=init
/// src:file_with_tricky_code.cc
/// src:ignore-global-initializers-issues.cc=init
/// mainfile:main_file.cc
///
/// [dataflow]
/// # Functions with pure functional semantics:
/// fun:cos=functional
/// fun:sin=functional
/// ---
class SpecialCaseList {
public:
  static constexpr std::pair<unsigned, unsigned> NotFound = {0, 0};
  /// Parses the special case list entries from files. On failure, returns
  /// std::pair Error, Error.first is error enum, Error.second is error message.
  LLVM_ABI static std::unique_ptr<SpecialCaseList>
  create(const std::vector<std::string> &Paths, llvm::vfs::FileSystem &FS,
         std::pair<unsigned, std::string> &Error);
  /// Parses the special case list entries from files. On failure, returns
  /// 0 and writes an error message to string.
  LLVM_ABI static std::unique_ptr<SpecialCaseList>
  create(const std::vector<std::string> &Paths, llvm::vfs::FileSystem &FS,
         std::string &Error);
  /// Parses the special case list from a memory buffer. On failure, returns
  /// 0 and writes an error message to string.
  LLVM_ABI static std::unique_ptr<SpecialCaseList>
  create(const MemoryBuffer *MB, std::string &Error);
  /// Parses the special case list entries from files. On failure, reports a
  /// fatal error.
  LLVM_ABI static std::unique_ptr<SpecialCaseList>
  createOrDie(const std::vector<std::string> &Paths, llvm::vfs::FileSystem &FS);

  LLVM_ABI ~SpecialCaseList();

  /// Returns true, if special case list contains a line
  /// \code
  ///   @Prefix:<E>=@Category
  /// \endcode
  /// where @Query satisfies the glob <E> in a given @Section.
  LLVM_ABI bool inSection(StringRef Section, StringRef Prefix, StringRef Query,
                          StringRef Category = StringRef()) const;

  /// Returns the file index and the line number <FileIdx, LineNo> corresponding
  /// to the special case list entry if the special case list contains a line
  /// \code
  ///   @Prefix:<E>=@Category
  /// \endcode
  /// where @Query satisfies the glob <E> in a given @Section.
  /// Returns (zero, zero) if there is no exclusion entry corresponding to this
  /// expression.
  LLVM_ABI std::pair<unsigned, unsigned>
  inSectionBlame(StringRef Section, StringRef Prefix, StringRef Query,
                 StringRef Category = StringRef()) const;

protected:
  // Implementations of the create*() functions that can also be used by derived
  // classes.
  LLVM_ABI bool createInternal(const std::vector<std::string> &Paths,
                               vfs::FileSystem &VFS,
                               std::pair<unsigned, std::string> &Error);
  LLVM_ABI bool createInternal(const MemoryBuffer *MB, std::string &Error);

  SpecialCaseList() = default;
  SpecialCaseList(SpecialCaseList const &) = delete;
  SpecialCaseList &operator=(SpecialCaseList const &) = delete;

  /// Represents a set of globs and their line numbers
  class Matcher {
  public:
    LLVM_ABI Error insert(StringRef Pattern, unsigned LineNumber,
                          bool UseRegex);
    // Returns the line number in the source file that this query matches to.
    // Returns zero if no match is found.
    LLVM_ABI unsigned match(StringRef Query) const;

    struct Glob {
      std::string Name;
      unsigned LineNo;
      GlobPattern Pattern;
      // neither copyable nor movable because GlobPattern contains
      // Glob::StringRef that points to Glob::Name.
      Glob(Glob &&) = delete;
      Glob() = default;
    };

    std::vector<std::unique_ptr<Matcher::Glob>> Globs;
    std::vector<std::pair<std::unique_ptr<Regex>, unsigned>> RegExes;
  };

  using SectionEntries = StringMap<StringMap<Matcher>>;

  struct Section {
    Section(StringRef Str, unsigned FileIdx)
        : SectionStr(Str), FileIdx(FileIdx) {};

    std::unique_ptr<Matcher> SectionMatcher = std::make_unique<Matcher>();
    SectionEntries Entries;
    std::string SectionStr;
    unsigned FileIdx;
  };

  std::vector<Section> Sections;

  LLVM_ABI Expected<Section *> addSection(StringRef SectionStr,
                                          unsigned FileIdx, unsigned LineNo,
                                          bool UseGlobs = true);

  /// Parses just-constructed SpecialCaseList entries from a memory buffer.
  LLVM_ABI bool parse(unsigned FileIdx, const MemoryBuffer *MB,
                      std::string &Error);

  // Helper method for derived classes to search by Prefix, Query, and Category
  // once they have already resolved a section entry.
  LLVM_ABI unsigned inSectionBlame(const SectionEntries &Entries,
                                   StringRef Prefix, StringRef Query,
                                   StringRef Category) const;
};

} // namespace llvm

#endif // LLVM_SUPPORT_SPECIALCASELIST_H
