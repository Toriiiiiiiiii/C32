#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#define ARG_REQUIRED 0b00000001
#define ARG_OPTIONAL 0b00000010
#define ARG_POSITIONAL 0b00000100
#define ARG_IMPLIED 0b00001000

/* Contains template information for a command-line argument
 * @param argumentType -> The type of the argument. `ARG_REQUIRED`, `ARG_OPTIONAL`, `ARG_POSITIONAL` and/or `ARG_IMPLIED`
 * @param flag         -> The required flag for the parser to recognise the argument. Should be blank for arguments with type `ARG_POSITIONAL`.
 * @param name         -> The name of the flag. Used in order to fetch the argument value from the parser.
 */
typedef struct {
  uint8_t argumentType;
  std::string flag;
  std::string name;

  bool used;
} ArgumentTemplate;

/* Structure containing argument information.
 * @param name  -> The name of the argument. Always matches the name of the corresponding template.
 * @param value -> The value of the argument. Will be blank if the template has type `ARG_IMPLIED`.
 */
typedef struct {
  std::string name;
  std::string value;
} Argument;

/* Parses command line arguments.
 * @param argc  -> The number of command-line arguments.
 * @param argv  -> A raw array of C-Style strings containing the arguments.
 * @param usage -> A vector of argument templates describing the usage of the program.
 */
class ArgumentParser {
private:
  char **argv;
  int argc;

public:
  std::vector<std::string> argumentString;
  std::vector<ArgumentTemplate> usage;

  ArgumentParser(int argc, char **argv, std::vector<ArgumentTemplate> usage);
  ~ArgumentParser();

  /* Parses the arguments.
   * @param args -> The vector to store the result in. NB : All data in this vector will be overwritten.
   * @returns `true` if the arguments were successfully parsed, otherwise `false`. 
   */
  bool parseArguments(std::vector<Argument> &args);

  /* Checks if an argument is present.
   * @param args -> The vector containing the arguments.
   * @param name -> The name of the argument to search for.
   * 
   * @returns `true` if the argument is found, else `false`.
   */
  bool argumentExists(std::vector<Argument> args, std::string name);

  /* Gets an argument from the vector by name.
   * @param args -> The vector containing the arguments.
   * @param name -> The name of the argument to search for.
   * 
   * @returns The argument structure if found, else `NULL`.
   */
  Argument getArgByName(std::vector<Argument> args, std::string name);
};
