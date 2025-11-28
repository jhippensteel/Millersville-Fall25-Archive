#include <cstdlib>
#include <regex>
#include <vector>

#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_DISABLE_MATCHERS
#define CATCH_CONFIG_CPP11_TO_STRING
#define CATCH_CONFIG_CPP17_UNCAUGHT_EXCEPTIONS
#define CATCH_CONFIG_CPP17_STRING_VIEW
#define CATCH_CONFIG_RUNNER
#include  <catch2/catch_all.hpp>

#include "./support/TestList.hpp"

static std::regex const regex{
  "[ \t]*(When|Then|Scenario|Given|And):[ \t]*\\[([0-9]*)\\].*",
  std::regex::optimize};

struct AutolabListener : Catch::EventListenerBase
{

  static std::string json;
  std::vector<std::string> testStack;
  int incorrect = 0;
  int points = 0;

  using EventListenerBase::EventListenerBase; // inherit constructor

  virtual void
  sectionStarting (Catch::SectionInfo const& sectionInfo) override
  {
    testStack.push_back (sectionInfo.name);
  }

  virtual void
  sectionEnded (Catch::SectionStats const& sectionStats) override
  {
    std::string top = testStack.back ();
    std::smatch sm;
    if (std::regex_match (top, sm, regex))
    {
      int value = std::atoi (sm.str (2).c_str ());
      if (!sectionStats.assertions.allOk())
      {
        incorrect += value;
      }
      points += value;
    }
    testStack.pop_back ();
  }

  virtual void
  testRunEnded (Catch::TestRunStats const& testRunStats) override
  {
    std::ostringstream oss;
    oss << "Points: " << points << '\n';
    oss << "Incorrect: " << incorrect << '\n';
    oss << '\n';
    oss << "{ \"scores\": {\"auto\": " << (points - incorrect) << " } }";
    json = oss.str ();
  }
};

std::string AutolabListener::json;

CATCH_REGISTER_LISTENER (AutolabListener)

int
main (int argc, char* argv[])
{
  Catch::Session ().run (argc, argv);
  std::cerr << AutolabListener::json << '\n';
}
