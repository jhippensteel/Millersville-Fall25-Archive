#include <cstdlib>
#include <iostream>
#include <regex>
#include <vector>

#include <catch2/catch_all.hpp>

#include "TestDivideConquer.hpp"


static std::regex const regex{
  "[ \t]*(When|Then|Scenario|Given|And):[ \t]*\\[([.0-9]*)\\].*",
  std::regex::optimize};

struct AutolabListener : Catch::EventListenerBase
{
  static std::string json;
  static bool passedFlag;
  static bool failedFlag;
  std::vector<std::string> passed;
  std::vector<std::string> failed;
  std::vector<std::string> testStack;
  double correct = 0;
  double points = 0;

  using EventListenerBase::EventListenerBase; // inherit constructor

  virtual void
  sectionStarting (Catch::SectionInfo const& sectionInfo) override
  {
    testStack.push_back (sectionInfo.name);
  }

  std::string
  to_string (std::string const& last) const
  {
    std::string str;
    for (auto const& s : testStack)
    {
      str += s;
      str += '\n';
    }
    str += last;
    return str;
  }

  virtual void
  sectionEnded (Catch::SectionStats const& sectionStats) override
  {
    std::string top = testStack.back ();
    testStack.pop_back ();

    std::smatch sm;
    if (std::regex_match (top, sm, regex))
    {
      double value = std::stod (sm.str (2));
      if (sectionStats.assertions.allOk())
      {
        correct += value;
        passed.push_back (to_string(top));
      }
      else
      {
        failed.push_back (to_string(top));
      }
      points += value;
    }
  }

  virtual void
  testRunEnded (Catch::TestRunStats const&) override
  {
    std::ostringstream oss;
    if (passedFlag)
    {
      oss << "\n\nPASSED:\n\n";
      for (auto const & test : passed)
      {
        oss << test << '\n' << '\n';
      }
    }
    if (failedFlag)
    {
      oss << "\n\nFAILED:\n\n";
      for (auto const & test : failed)
      {
        oss << test << '\n' << '\n';
      }
    }
    oss << "   Points: " << points << '\n'
        << "  Correct: " << correct << '\n'
        << "Incorrect: " << (points - correct) << '\n'
        << '\n'
        << "{ \"scores\": {\"auto\": " << correct << " } }";
    json = oss.str ();
  }
};

bool AutolabListener::passedFlag;
bool AutolabListener::failedFlag;
std::string AutolabListener::json;

CATCH_REGISTER_LISTENER (AutolabListener)

int
main (int argc, char* argv[])
{
  Catch::Session session;
  AutolabListener::passedFlag = false;
  AutolabListener::failedFlag = false;

  using namespace Catch::Clara;
  auto cli = session.cli();
  cli |= Opt (AutolabListener::passedFlag)
      ["-P"]["--passed"]("verbosely print all passed tests");
  cli |= Opt (AutolabListener::failedFlag)
      ["-F"]["--failed"]("verbosely print all failed tests");

  session.cli (cli);

  if (int ret = session.applyCommandLine (argc, argv); ret != 0)
  {
    return ret;
  }

  session.run();
  std::cerr << AutolabListener::json << '\n';
}
