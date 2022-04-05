// Copyright (C) 2022, Antoine Basset
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "ProgramOptions.h"

int main(int argc, const char* const argv[]) {

  // Declare
  Kast::ProgramOptions options("Here's a dummy option parser!");
  options.positional<int>("positional", "Positional option");
  options.named<int>("named,n", "Named option with short form");
  options.named("optional,o", "Named option with default value", 42);
  options.flag("flag,f", "Flag with short form");
  
  // Parse
  options.parse(argc, argv);
  const auto positional = options.has("positional") ? options.as<int>("positional") : -1;
  const auto named = options.has("named") ? options.as<int>("named") : -1;
  const auto optional = options.as<int>("optional");
  const auto flag = options.as<bool>("flag");
  
  // Print
  printf("positional = %d\n", positional);
  printf("     named = %d\n", named);
  printf("  optional = %d\n", optional);
  printf("      flag = %d\n", flag);
}

