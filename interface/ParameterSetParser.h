#ifndef MessageFacility_ParameterSet_ParameterSetParser_h
#define MessageFacility_ParameterSet_ParameterSetParser_h

///////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "ParameterSet.h"

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/any.hpp>

#include <string>
#include <list>

namespace fhicl 
{

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phoenix = boost::phoenix;

typedef BOOST_TYPEOF(ascii::space 
    | qi::lit('#') >>*(qi::char_ - boost::spirit::eol) >> boost::spirit::eol 
    | qi::lit("//")>>*(qi::char_ - boost::spirit::eol) >> boost::spirit::eol
) skipper_type;


template <typename Iterator>
struct PSetParser 
: qi::grammar<Iterator, skipper_type>
{

  PSetParser();

  ParameterSet getPSet(std::string const & name = "");
  std::vector<std::string> getErrorMsgs() { return errs; };

private:

  boost::any * findPrimaryPtr(std::string const &, int ver=0, bool bInsert=false);
  boost::any * findPrimaryPtr(std::pair<std::string, int> const &, bool Insert=false); 
  boost::any * findPSetPtr(boost::any *, std::string const &, bool);
  boost::any * findArrayElementPtr(boost::any *, int, bool);

  boost::any getObjFromName(std::string & name);
  void       setObjFromName(std::string & name, boost::any & obj);
  boost::any * parseRef(std::string &, bool, bool bTopLvl=true, ParameterSet * pset=NULL);

  void insertPSetEntry(ParameterSet &, std::pair<std::string, boost::any> const&);

  // Qi parser rules
  qi::rule<Iterator, skipper_type> doc;
  qi::rule<Iterator, ParameterSet(), skipper_type> pset;
  qi::rule<Iterator, std::string() , skipper_type> key;
  qi::rule<Iterator, std::string() , skipper_type> valid_key;
  qi::rule<Iterator, void()        , skipper_type> keywords;
  qi::rule<Iterator, std::string() , skipper_type> str;
  qi::rule<Iterator, boost::any()  , skipper_type> expr;
  qi::rule<Iterator, boost::any()  , skipper_type> nil;
  qi::rule<Iterator, boost::any(), qi::locals<std::string>, skipper_type> reference;
  qi::rule<Iterator, std::string() , skipper_type> ref_literal;
  qi::rule<Iterator, std::string() , skipper_type> refver_literal;
  qi::rule<Iterator, std::vector<boost::any>(), skipper_type> array;
  qi::rule<Iterator, std::pair<std::string, boost::any>(), skipper_type> assign;
  qi::rule<Iterator, std::pair<std::string, boost::any>(), skipper_type> unnamed_assign;
  qi::rule<Iterator, void(), qi::locals<std::string>, skipper_type> re_assign;
  qi::rule<Iterator, std::string() , skipper_type> primary_key;
  qi::rule<Iterator, std::string() , skipper_type> double_literal;
  qi::rule<Iterator, std::string() , skipper_type> int_literal;
  qi::rule<Iterator, int() , skipper_type> last_literal;
  qi::rule<Iterator> space;

  // data structure for primary entries
  std::vector<std::pair<std::string, boost::any> > PrimaryValues;

  // nil object initialized from ParameterSet::nil_obj
  boost::any nilObj;

  // vector of string to hold parsing errors
  std::vector<std::string> errs;

};

class Parser
{
private:
  Parser() {}

public:
  static bool Parse(std::string const &, ParameterSet &);
  static bool ParseString(std::string &, ParameterSet &);

private:
  static bool PreProcess(std::string const &, std::string &);
  static void TrimSpace(std::string &);
};

}// namespace fhicl 

#endif
