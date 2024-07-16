#include "arguments.h"

ArgumentParser::ArgumentParser(int argc, char **argv,
                               std::vector<ArgumentTemplate> usage) {
  this->usage = usage;
  this->argumentString = {};

  this->argv = argv;
  this->argc = argc;

  for (int i = 1; i < argc; ++i) {
    this->argumentString.push_back(std::string(this->argv[i]));
  }
}

ArgumentParser::~ArgumentParser() {
  this->usage = {};
  this->argumentString = {};

  this->argv = NULL;
  this->argc = 0;
}

bool ArgumentParser::parseArguments(std::vector<Argument> &args) {
  std::vector<std::string> tempArgs = this->argumentString;
  size_t argIndex = 0;

  while (tempArgs.size() > 0) {
    std::string currentArg = tempArgs[0];
    tempArgs.erase(tempArgs.begin());

    for (size_t i = 0; i < usage.size(); ++i) {
      ArgumentTemplate temp = usage[i];
      if (currentArg[0] == '-') {
        if (temp.flag == currentArg) {
          Argument arg;
          arg.name = temp.name;
          temp.used = true;

          if (!(temp.argumentType & ARG_IMPLIED)) {
            arg.value = tempArgs[0];
            tempArgs.erase(tempArgs.begin());
          }

          args.push_back(arg);
          break;
        }
      } else if ((temp.argumentType & ARG_POSITIONAL) && argIndex == i) {
        usage[i].used = true;
        args.push_back(Argument{temp.name, currentArg});
        argIndex++;
        break;
      }
    }
  }

  for (ArgumentTemplate temp : usage) {
    if ((temp.argumentType & ARG_REQUIRED) && !temp.used)
      return false;
  }

  return true;
}

bool ArgumentParser::argumentExists(std::vector<Argument> args,
                                    std::string name) {
  for (Argument arg : args) {
    if (arg.name == name)
      return true;
  }

  return false;
}

Argument ArgumentParser::getArgByName(std::vector<Argument> args,
                                      std::string name) {
  for (Argument arg : args) {
    if (arg.name == name)
      return arg;
  }

  return {0};
}
