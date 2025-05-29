#pragma once

#include <memory>
#include <queue>
#include <string>
#include <vector>

using namespace std;

typedef queue<string> ArgQueue;

static inline string safePop(ArgQueue &args) {
  if (args.empty()) {
    fprintf(stderr, "unexpected end of command line\n");
    exit(1);
  }

  auto val = args.front();
  args.pop();
  return val;
}

static inline void parseValue(bool &var, ArgQueue &) {
  var = true;
}

static inline void parseValue(string &var, ArgQueue &args) {
  var = safePop(args);
}

template <typename Element>
static inline void parseValue(vector<Element> &var, ArgQueue &args) {
  Element e{};
  parseValue(e, args);
  var.push_back(e);
}

struct OptionHandler {
  void addFlag(string shortName, string longName, bool *pVar, string desc = "") {
    add(shortName, longName, pVar, desc);
  }

  template <typename T>
  void add(string shortName, string longName, T *pVar, string desc = "") {
    auto opt = make_unique<TypedOption<T>>();
    opt->pVar = pVar;
    opt->shortName = "-" + shortName;
    opt->longName = "--" + longName;
    opt->desc = desc;
    m_Options.push_back(move(opt));
  }

  vector<string> parse(int argc, const char *argv[]);
  void printHelp(FILE *);

private:
  struct AbstractOption {
    virtual ~AbstractOption() = default;
    string shortName, longName;
    string desc;
    virtual void parse(ArgQueue &args) = 0;
  };

  vector<unique_ptr<AbstractOption>> m_Options;

  template <typename T>
  struct TypedOption : AbstractOption {
    T *pVar;
    void parse(ArgQueue &args) { parseValue(*pVar, args); }
  };
};
