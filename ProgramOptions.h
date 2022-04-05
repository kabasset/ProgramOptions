// Copyright (C) 2022, Antoine Basset
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef _PROGRAMOPTIONS_H
#define _PROGRAMOPTIONS_H

#include <boost/program_options.hpp>
#include <sstream>

namespace Kast {

/**
 * @brief Helper class to declare positional, named and flag options, as well as some help message.
 * @details
 * Here is an example use case for the following command line:
 * \verbatim Program <positional> --named1 <value1> --flag --named2 <value2> \endverbatim
 * 
 * Here's an example program to handle it:
 * \code
 * std::pair<OptionsDescription, PositionalOptionsDescription> defineProgramArguments() override {
 *   auto options = ProgramOptions::fromAuxdir("help.txt");
 *   options.positional("positional", value<std::string>(), "Positional option");
 *   options.named("named1", value<int>(), "Named option 1");
 *   options.named("named2", value<int>(), "Named option 2");
 *   return options.asPair();
 * }
 * \endcode
 */
class ProgramOptions {

public:

  using OptionsDescription = boost::program_options::options_description;
  using PositionalOptionsDescription = boost::program_options::positional_options_description;
  using ValueSemantics = boost::program_options::value_semantic;

  /**
   * @brief Make a `ProgramOptions` with optional description string.
   */
  ProgramOptions(const std::string& description = "") :
    m_named(makeDesc(description)),
    m_add(m_named.add_options()),
    m_positional(),
    m_variables() {}

  /**
   * @brief Declare a positional option.
   */
  template <typename T>
  void positional(const char* name, const char* description) {
    positional(name, boost::program_options::value<T>(), description);
  }

  /**
   * @brief Declare a positional option with default value.
   */
  template <typename T>
  void positional(const char* name, const char* description, T defaultValue) {
    positional(name, boost::program_options::value<T>()->default_value(defaultValue), description);
  }

  /**
   * @brief Declare a positional option with custom semantics.
   */
  void positional(const char* name, const ValueSemantics* value, const char* description) {
    m_add(name, value, description);
    const int maxArgs = value->max_tokens();
    m_positional.add(name, maxArgs);
  }

  /**
   * @brief Declare a named option.
   * @details
   * A short form (1-character) of the option can be provided, separated by a comma.
   */
  template <typename T>
  void named(const char* name, const char* description) {
    named(name, boost::program_options::value<T>(), description);
  }

  /**
   * @brief Declare a named option with default value.
   * @details
   * A short form (1-character) of the option can be provided, separated by a comma.
   */
  template <typename T>
  void named(const char* name, const char* description, T defaultValue) {
    named(name, boost::program_options::value<T>()->default_value(defaultValue), description);
  }

  /**
   * @brief Declare a named option with custom semantics.
   * @details
   * A short form (1-character) of the option can be provided, separated by a comma.
   */
  void named(const char* name, const ValueSemantics* value, const char* description) {
    m_add(name, value, description);
  }

  /**
   * @brief Declare a flag option.
   */
  void flag(const char* name, const char* description) {
    named(name, boost::program_options::value<bool>()->default_value(false)->implicit_value(true), description);
  }

  
  /**
   * @brief Get the named (flags included) and positional options as a pair.
   */
  std::pair<OptionsDescription, PositionalOptionsDescription> asPair() const {
    return std::make_pair(m_named, m_positional);
  }

  /**
   * @brief Parse a command line (`main`-like).
   */
  void parse(int argc, const char* const argv[]) {
    boost::program_options::store(
        boost::program_options::command_line_parser(argc, argv).options(m_named).positional(m_positional).run(),
        m_variables);
    boost::program_options::notify(m_variables);
  }

  /**
   * @brief Parse a command line (vector of `const char*` arguments).
   */
  void parse(const std::vector<const char*>& args) {
    parse(args.size(), args.data());
  }

  /**
   * @brief Parse a command line (vector of string arguments).
   */
  void parse(const std::vector<std::string>& args) {
    std::vector<const char*> cstr(args.size());
    std::transform(args.begin(), args.end(), cstr.begin(), [](const auto& a) { return a.c_str(); });
    parse(cstr);
  }

  /**
   * @brief Parse a command line (space-separated arguments as a single string).
   */
  void parse(const std::string& args) {
    std::istringstream iss(args);
    parse(std::vector<std::string>(
        std::istream_iterator<std::string>{iss},
        std::istream_iterator<std::string>()));
  }

  /**
   * @brief Check whether a given option is set.
   */
  bool has(const char* name) const {
    return m_variables.count(name);
  }

  /**
   * @brief Get the value of a given option.
   * @details
   * Throws if the option is not set.
   */
  template <typename T>
  T as(const char* name) const {
    return m_variables[name].as<T>();
  }

private:
  
  static std::string makeDesc(const std::string& description) {
    const std::string optionsGroup = "Options:";
    if (description.length() > 0) {
      return description + "\n\n" + optionsGroup;
    }
    return optionsGroup;
  }

  OptionsDescription m_named;
  boost::program_options::options_description_easy_init m_add;
  PositionalOptionsDescription m_positional;
  boost::program_options::variables_map m_variables;

};

}

#endif

