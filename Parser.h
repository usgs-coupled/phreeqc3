#if !defined(PARSER_H_INCLUDED)
#define PARSER_H_INCLUDED

#include <string>   // std::string
#include <map>      // std::map
#include <vector>   // std::vector
#include <sstream>  // std::istringstream std::ostringstream
#include <ostream>  // std::ostream
#include <istream>  // std::istream
#include "char_star.h"

class CParser
{
public:
        CParser(std::istream& input);
        CParser(std::istream& input, std::ostream& output);
        CParser(std::istream& input, std::ostream& output, std::ostream& error);

        virtual ~CParser();

        enum LINE_TYPE {
                LT_EOF          = -1,
                LT_OK           = 1,
                LT_EMPTY        = 2,
                LT_KEYWORD      = 3,
                LT_OPTION       = 8
        };

        enum TOKEN_TYPE {
                TT_EMPTY        = 2,
                TT_UPPER        = 4,
                TT_LOWER        = 5,
                TT_DIGIT        = 6,
                TT_UNKNOWN      = 7
        };

        enum FIND_TYPE {
                FT_OK           = 0,
                FT_ERROR        = 1
        };

        enum KEY_TYPE {
                KT_NONE         = -1,
                KT_END          =  0,
                KT_EOF          =  1,
                KT_SOLUTION     =  4,
                KT_SOLUTION_RAW     =  5
        };

        enum OPT_TYPE {
                OPT_DEFAULT  = -4,
                OPT_ERROR    = -3,
                OPT_KEYWORD  = -2,
                OPT_EOF      = -1
        };

        enum ONERROR_TYPE {
                OT_CONTINUE     = 0,
                OT_STOP         = 1
        };

        enum STATUS_TYPE {
                PARSER_ERROR    = 0,
                PARSER_OK       = 1
        };

        /**
                Function gets a new line and checks for empty, eof, and keywords.

                Arguments:
                        string        Input, character string used in printing error message
                        allow_empty   Input, True or false, if a blank line is accepable
                                      if false, another line is read
                        allow_eof     Input, True or false, if EOF is acceptable
                        allow_keyword Input, True or false, if a keyword is acceptable

                Returns:
                        LT_EMPTY         if empty line read and allow_empty == true
                        LT_KEYWORD       if line begins with keyword
                        LT_EOF           if eof and allow_eof == true
                        LT_OK            otherwise
                        LT_OPTION        if line begins with -[alpha]

           Terminates       if EOF and allow_eof == false.
        */
        LINE_TYPE check_line(const std::string& str, bool allow_empty, bool allow_eof, bool allow_keyword, bool print);

        /**
                Read a line from input file put in "line".
                Copy of input line is stored in "line_save".
                Characters after # are discarded in line but retained in "line_save"

                Arguments:
                        None
                Returns:
                        LT_EMPTY,
                        LT_EOF,
                        LT_KEYWORD,
                        LT_OK,
                        LT_OPTION
        */
        LINE_TYPE get_line();

        // bool check_key(const std::string::iterator ptr);
        bool check_key(std::string::iterator begin, std::string::iterator end);

        STATUS_TYPE check_units(std::string& tot_units, bool alkalinity, bool check_compatibility,
                const std::string& default_units, bool print);


        KEY_TYPE next_keyword()const { return m_next_keyword; }
        int get_option(const std::vector<std::string>& opt_list, std::string::iterator& next_char);
        int get_option(const std::vector<std::string>& opt_list, std::istream::pos_type& next_pos);
        int getOptionFromLastLine(const std::vector<std::string>& opt_list, std::string::iterator& next_char);
        int getOptionFromLastLine(const std::vector<std::string>& opt_list, std::istream::pos_type& next_pos);


        std::string& line()            {return m_line;}
        std::istringstream& get_iss()  {return m_line_iss;}
        int incr_input_error()         {return ++m_input_error;}
        std::ostream& get_output()     {return m_output_stream;}
        int get_input_error()          {return m_input_error;}


        /**
                Copies from begin to token until first space is encountered.

                Arguments:
                        token    output, the token
                        begin    input, begin iterator
                        end      input, end iterator

                Returns:
                        TT_EMPTY
                        TT_UPPER
                        TT_LOWER
                        TT_DIGIT
                        TT_UNKNOWN
        */
        static TOKEN_TYPE copy_token(std::string& token, std::string::iterator& begin, std::string::iterator& end);
        static TOKEN_TYPE token_type(const std::string& token);
        static TOKEN_TYPE copy_token(std::string& token, std::istream& is);
        TOKEN_TYPE copy_token(std::string& token, std::istream::pos_type& pos);
        CParser::TOKEN_TYPE peek_token();

        /**
                Function reads an element name out of the equation string.
                An element name is composed of a capital letter followed by any number
                of lower case characters.

                Arguments:
                        begin    input, points to position in the equation to begin
                                        output, points to next character of equation after
                                        element name.
                        end      input, points to last position in the equation
                        element  input pointer to place to return element character string
        */
        STATUS_TYPE get_elt(std::string::iterator& begin, const std::string::iterator end, std::string& element);


        /**
                Compares a string value to match beginning letters of a list of options

                Arguments:
                        item    entry: pointer to string to compare
                        n       exit:  item in list that was matched
                        list    entry: pointer to list of character values, assumed to
                                be lower case
                        count_list entry: number of character values in list

                Returns:
                        OK      item matched
                        ERROR   item not matched
                        n       -1      item not matched
                                i       position of match in list
        */
        static FIND_TYPE find_option(const std::string& item, int *n, const std::vector<std::string>& list, bool exact);


        int error_msg(const std::ostringstream& err_str, ONERROR_TYPE stop)    {return error_msg(err_str.str().c_str(), stop);}
        int error_msg(const char *err_str, ONERROR_TYPE stop);
        int warning_msg(const char *err_str);


        STATUS_TYPE parse_couple(std::string& token);

        STATUS_TYPE addPair(std::map<char *, double, CHARSTAR_LESS> &totals, std::istream::pos_type& pos);
        STATUS_TYPE addPair(std::map<char *, double> &totals, std::istream::pos_type& pos);

protected:
        LINE_TYPE get_logical_line();

private:
        std::istream&        m_input_stream;
        std::ostream&        m_output_stream;
        std::ostream&        m_error_stream;
        int                  m_input_error;
        KEY_TYPE             m_next_keyword;
        std::string          m_line;
        std::string          m_line_save;
        std::istringstream   m_line_iss;
        LINE_TYPE            m_line_type;
};

#endif // PARSER_H_INCLUDED
