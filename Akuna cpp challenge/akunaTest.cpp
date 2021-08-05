#include <vector>
#include <functional>
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>

/// Parser for the Akuna Lego Unit input file
//
// When the process() method is called, this class read from 
// stdin and call callbacks for each unit, input, connection, and
// value.
class input_parser {
  public:
      /// Register the callback for each UNIT in the input file.
      /// The first parameter is the "unit-name", the second parameter is the "unit-type".
      /// E.g:
      ///    a := sum
      /// We would call unit_callback_("a", "sum")
      ///
      void register_unit_callback(std::function<void(std::string, std::string)> cb)
      {
          unit_callback_ = cb;
      }

      /// Register the callback for the INPUTS line in the input file
      ///
      /// The parameter tells how many input ports there are.
      /// E.g:
      ///    INPUTS: 2
      /// We would call input_callback_(2)
      void register_input_callback(std::function<void(int)> cb)
      {
          input_callback_ = cb;
      }

      /// Register the callback for each CONNECTION in the input file
      ///
      /// The first 3 parameters tell you the "from" side,
      /// the last 3 parameters tell you the "to" side.
      ///
      /// For each three parameter group:
      ///  - The first is the unit name, "input" or "result"
      ///  - The second is "in", "out" or "" (empty)
      ///  - The third is port number (could be empty string), e.g. "0", "1", or "".
      /// Example 1:
      ///   input/0 -- a/in/0
      ///   =>   connection_callback_("input", "", "0", "a", "in", "0")
      /// Example 2:
      ///   a/out/0 -- result
      ///   =>   connection_callback_("a", "out", 0, "result", "", "")
      /// Example 3:
      ///   a/out/3 -- b/in/1
      ///   =>   connection_callback_("a", "out", "3", "b", "in", "1")
      ///
      void register_connection_callback(std::function<void(std::string, std::string, std::string, std::string, std::string, std::string)> cb)
      {
          connection_callback_ = cb;
      }

      /// Register the callback for each VALUE in the input file
      ///
      /// The first parameter is always "input" :)
      /// The second parameter is the input port number in string. e.g. "0", "1"
      /// The third parameter is the value number in string. e.g. "999"
      /// E.g:
      ///    input/0 := 3
      /// We would call value_callback_("input", "0", "3")
      void register_value_callback(std::function<void(std::string, std::string, std::string)> cb)
      {
          value_callback_ = cb;
      }

      /// Process lines from stdin and call the appropriate callbacks.  Note that
      /// this is very intolerant of syntax errors, and will assert in any unexpected
      /// input
      void process()
      {
          // process the UNITS line
          int units_num = get_section_title_line();

          // handle the expected number of units
          for (int i = 0; i < units_num; i++)
          {
              std::string name, type;
              get_section_internal_line(name, type);
              if (unit_callback_)
                  unit_callback_(name, type);
          }

          // process the INPUTS line
          int inputs_num = get_section_title_line();
          if (input_callback_)
              input_callback_(inputs_num);

          // process the CONNECTIONS line
          int connections_num = get_section_title_line();


          // handle the expected number of connections
          for (int i = 0; i < connections_num; i++)
          {
              std::string from, to;
              get_section_internal_line(from, to);
              assert(from != "result");
              std::vector<std::string> from_strs = split(from, '/');
              assert(from_strs.size() >= 2);
              std::vector<std::string> to_strs = split(to, '/');
              assert(from_strs.size() >= 1);

              // the first parameter is from_strs[0]
              std::string second = from_strs[0] == "input" ? "" : from_strs[1];
              std::string third = from_strs[0] == "input" ? from_strs[1] : from_strs[2];

              std::string fifth = "";
              std::string sixth = "";
              if (to_strs[0] == "input")
              {
                  sixth = to_strs[1];
              }
              else if (to_strs[0] != "result")
              {
                  fifth = to_strs[1];
                  sixth = to_strs[2];
              }

              if (connection_callback_)
                  connection_callback_(from_strs[0], second, third,
                                       to_strs[0], fifth, sixth);
          }

          // process the VALUES line
          int values_num = get_section_title_line();

          // handle the expected number of values
          for (int i = 0; i < values_num; i++)
          {
              std::string input, value;
              get_section_internal_line(input, value);
              std::vector<std::string> strs = split(input, '/');
              assert(strs.size() == 2);
              if (value_callback_)
                  value_callback_(strs[0], strs[1], value);
          }
      }

  private:
      // hackerrank doesn't support boost files, so use our own utilities...

      // split a string on a delimiter and append each item to result.
      template<typename Out>
      void split(const std::string &s, char delim, Out result)
      {
          std::stringstream ss;
          ss.str(s);
          std::string item;
          while (std::getline(ss, item, delim))
          {
              *(result++) = item;
          }
      }

      // split a string an a delimiter and return a vector of the results
      std::vector<std::string> split(const std::string &s, char delim)
      {
          std::vector<std::string> elems;
          split(s, delim, std::back_inserter(elems));
          return elems;
      }

      // read a line from stdin and return the number in the second token
      int get_section_title_line()
      {
          std::string line;
          std::getline(std::cin, line);
          std::vector<std::string> words = split(line, ' ');
          assert(words.size() == 2);
          return atoi(words[1].c_str());
      }

      // read a line from stdin and return the first and third token
      void get_section_internal_line(std::string& a, std::string& b)
      {
          std::string line;
          std::getline(std::cin, line);
          std::vector<std::string> words = split(line, ' ');
          assert(words.size() == 3);
          a = words[0];
          b = words[2];
      }

  private:
      std::function<void(std::string, std::string)> unit_callback_;

      std::function<void(int n)> input_callback_;

      std::function<void(std::string, std::string, std::string, std::string, std::string, std::string)> connection_callback_;

      std::function<void(std::string, std::string, std::string)> value_callback_;
};

using std::cout;
int main()
{
    input_parser data;
    return 0;
}
