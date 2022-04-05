#define BOOST_TEST_MODULE "ProgramOptionsTest"
#include <boost/test/unit_test.hpp>

#include "ProgramOptions.h"

struct TestProgramOptions {

  Kast::ProgramOptions po;
  
  TestProgramOptions() : po("Description") {
    po.positional<int>("positional", "Positional option, no short form, no default");
    po.positional("positional-default", "Positional option with short form and default", 0);
    po.named<int>("named", "Named option, no short form, no default");
    po.named<int>("named-short,n", "Named option with short form, no default");
    po.named("named-default,d", "Named option with short form and default", 0);
    po.flag("flag", "Flag, no short form");
    po.flag("flag-short,f", "Flag with short form");
  }

};

BOOST_FIXTURE_TEST_SUITE(ProgramOptionsTest, TestProgramOptions);

BOOST_AUTO_TEST_CASE(positional_test) {
  po.parse("exe 1");
  BOOST_TEST(po.as<int>("positional") == 1);
  BOOST_TEST(po.as<int>("positional-default") == 0);
  po.parse("exe 1 2");
  BOOST_TEST(po.as<int>("positional") == 1);
  BOOST_TEST(po.as<int>("positional-default") == 2);
}

BOOST_AUTO_TEST_CASE(named_test) {
  po.parse("exe --named 1 -n2");
  BOOST_TEST(po.as<int>("named") == 1);
  BOOST_TEST(po.as<int>("named-short") == 2);
  BOOST_TEST(po.as<int>("named-default") == 0);
  po.parse("exe -d 3");
  BOOST_TEST(po.as<int>("named-default") == 3);
}

BOOST_AUTO_TEST_CASE(flag_test) {
  po.parse("exe");
  BOOST_TEST(not po.as<bool>("flag"));
  BOOST_TEST(not po.as<bool>("flag-short"));
  po.parse("exe --flag");
  BOOST_TEST(po.as<bool>("flag"));
  po.parse("exe -f");
  BOOST_TEST(po.as<bool>("flag-short"));
}

BOOST_AUTO_TEST_CASE(bad_option_test) {
  BOOST_CHECK_THROW(po.parse("exe --bad 0"), std::exception);
  BOOST_CHECK_THROW(po.parse("exe --named"), std::exception);
}

BOOST_AUTO_TEST_SUITE_END();

