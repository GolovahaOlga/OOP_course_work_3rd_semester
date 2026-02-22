#pragma once
#include <string>
#include <tuple>
#include <regex>
using namespace std;
class Validator
{
        bool success;
        string norm;
        string error;
        static const regex REGEX_SPACES_ON_THE_SIDES;
        static const regex REGEX_DOUBLE_SPACES;
        static const regex REGEX_DOUBLE_HYPHEN;
        static const regex REGEX_TEXT;
        static const regex REGEX_SPACES;
        static const regex REGEX_AT_ON_THE_SIDES;
        static const regex REGEX_AT;
        static const regex REGEX_NAME_PART;
        static const regex REGEX_DOMAIN_PART;
        static const regex REGEX_THE_FIRST_NUMBER_OPTION;
        static const regex REGEX_THE_SECOND_NUMBER_OPTION;
        static const regex REGEX_ADDRESS;
        static const regex REGEX_REPLACING_DATE_SEPARATORS;
        static const regex REGEX_DATE;

    public:
        Validator();
        void set_norm(string newnorm);
        string get_norm() const;
        void set_error(string newerror);
        string get_error() const;
        
        void set_success(bool newsuccess);
        bool get_success() const;
        static string normalize_text(const string &name);
        static Validator valid_name_surname_patronymic(const string &name);
        static string normalize_email_number(const string &email);
        static Validator valid_email(const string &email);
        static Validator valid_address(const string &address);
        static string normalize_date(const string &date);
        static Validator valid_date(const string &date);
        static Validator valid_number(const string &number);
        static int day_in_month(int month, int year);
        static bool is_leap_year(int year);
        static tuple<int, int, int> date_string_to_int(const string &s);
};
