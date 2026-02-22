#include "libvalidator.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <regex>
#include <tuple>
#include <ctime>
#include <sstream>
#include <algorithm>
using namespace std;
const int MAX_AGE = 200;
const regex Validator::REGEX_SPACES_ON_THE_SIDES("^\\s+|\\s+$");
const regex Validator::REGEX_DOUBLE_SPACES("\\s{2,}");
const regex Validator::REGEX_DOUBLE_HYPHEN("--");
const regex Validator::REGEX_TEXT("^[A-Za-zА-Яа-я][A-Za-zА-Яа-я0-9\\s\\-]*[A-Za-zА-Яа-я0-9]$");
const regex Validator::REGEX_SPACES("\\s+");
const regex Validator::REGEX_AT_ON_THE_SIDES("^@|@$");
const regex Validator::REGEX_AT("@");
const regex Validator::REGEX_NAME_PART("^[A-Za-z0-9]+$");
const regex Validator::REGEX_DOMAIN_PART("^[A-Za-z0-9]+(\\.[A-Za-z0-9]+)+$");
const regex Validator::REGEX_THE_FIRST_NUMBER_OPTION("^((\\+7)|8)(\\(\\d{3}\\)|\\d{3})\\d{7}$");
const regex Validator::REGEX_THE_SECOND_NUMBER_OPTION("^((\\+7)|8)\\(\\d{3}\\)\\d{3}-\\d{2}-\\d{2}$");
const regex Validator::REGEX_ADDRESS("[A-Za-zА-Яа-я]+");
const regex Validator::REGEX_REPLACING_DATE_SEPARATORS("[-/]");
const regex Validator::REGEX_DATE("^\\d{2}\\.\\d{2}\\.\\d{4}$");
Validator::Validator()
{
    success = false;
    norm = "";
    error = "";
}
void Validator::set_norm(string newnorm)
{
    norm = newnorm;
}
string Validator::get_norm() const
{
    return norm;
}
void Validator::set_error(string newerror)
{
    error = newerror;
}
string Validator::get_error() const
{
    return error;
}
void Validator::set_success(bool newsuccess)
{
    success = newsuccess;
}
bool Validator::get_success() const
{
    return success;
}
string Validator::normalize_text(const string &name)
{
    string norm = regex_replace(name, REGEX_SPACES_ON_THE_SIDES, "");
    norm = regex_replace(norm, REGEX_DOUBLE_SPACES, " ");
    return norm;
}
Validator Validator::valid_name_surname_patronymic(const string &name)
{
    Validator result;
    result.set_norm(normalize_text(name));
    if (result.get_norm().empty())
    {
        result.set_error("Error! You have written an empty line or a line consisting only of spaces.\n");
        return result;
    }
    if ((regex_search(result.get_norm(), REGEX_DOUBLE_HYPHEN)))
    {
        result.set_error("Error! You have entered a name that contains two hyphens in a row.");
        return result;
    }
    if (!regex_match(result.get_norm(), REGEX_TEXT))
    {
        result.set_error("Error! Make sure that you have entered at least one letter, the word does not start with a number or a hyphen, the word does not end with a hyphen.");
        return result;
    }
    result.set_success(true);
    return result;
}
string Validator::normalize_email_number(const string &email)
{
    string norm = regex_replace(email, REGEX_SPACES, "");
    return norm;
}
Validator Validator::valid_email(const string &email)
{
    Validator result;
    result.set_norm(normalize_email_number(email));
    if (result.get_norm().empty())
    {
        result.set_error("Error! You have written an empty line or a line consisting only of spaces.\n");
        return result;
    }
    if (!regex_search(result.get_norm(), REGEX_AT))
    {
        result.set_error("Error! The email must contain the '@' symbol.\n");
        return result;
    }
    if (regex_search(result.get_norm(), REGEX_AT_ON_THE_SIDES))
    {
        result.set_error("Error! An email must not begin or end with the '@' symbol.\n");
        return result;
    }
    string copynorm=result.get_norm();
    if (count(copynorm.begin(), copynorm.end(), '@') > 1)
    {
        result.set_error("Error! The email must contain a single '@' character.\n");
        return result;
    }
    string name_part = result.get_norm().substr(0, result.get_norm().find('@'));
    string domain_part = result.get_norm().substr(result.get_norm().find('@') + 1);
    if (!regex_match(name_part, REGEX_NAME_PART))
    {
        result.set_error("Error!\n- The user's name (before '@') must consist of only Latin letters and numbers.\n");
        return result;
    }
    if (!regex_match(domain_part, REGEX_DOMAIN_PART))
    {
        result.set_error("Error!\n- The domain (after the '@') must consist of only Latin letters, numbers, and at least one dot.\n");
        return result;
    }
    result.set_success(true);
    return result;
}
Validator Validator::valid_number(const string &number)
{
    Validator result;
    result.set_norm(normalize_email_number(number));
    if (result.get_norm().empty())
    {
        result.set_error("Error! You have written an empty line or a line consisting only of spaces.\n");
        return result;
    }
    if (!(regex_match(result.get_norm(), REGEX_THE_FIRST_NUMBER_OPTION)) && !(regex_match(result.get_norm(), REGEX_THE_SECOND_NUMBER_OPTION)))
    {
        result.set_error("Error! The number format is incorrect. Make sure you specify the number as shown in the examples. Examples of assigning numbers: +78121234567; +78121234567; 88121234567; +7(812)1234567; 8(812)1234567; +7(812)123-45-67; 8(812)123-45-67\n");
        return result;
    }
    result.set_success(true);
    return result;
}
Validator Validator::valid_address(const string &address)
{
    Validator result;
    result.set_norm(normalize_text(address));
    if (result.get_norm().empty())
    {
        result.set_error("Error! You have written an empty line or a line consisting only of spaces.\n");
        return result;
    }
    if (!regex_search(result.get_norm(), REGEX_ADDRESS))
    {
        result.set_error("Error! The address must contain at least one letter of the English or Russian alphabet.");
        return result;
    }
    result.set_success(true);
    return result;
}
string Validator::normalize_date(const string &date)
{
    string norm = regex_replace(date, REGEX_SPACES_ON_THE_SIDES, "");
    norm = regex_replace(norm, REGEX_DOUBLE_SPACES, "");
    norm = regex_replace(norm, REGEX_REPLACING_DATE_SEPARATORS, ".");
    return norm;
}
Validator Validator::valid_date(const string &date)
{
    Validator result;
    result.set_norm(normalize_date(date));
    if (result.get_norm().empty())
    {
        result.set_error("Error! You have written an empty line or a line consisting only of spaces.\n");
        return result;
    }
    if (!(regex_match(result.get_norm(), REGEX_DATE)))
    {
        result.set_error("Error! The date was entered incorrectly. Allowed characters for entering the date: digits, dot, /, - .Enter the date in one of the provided options: day(two digits).month(two digits).year(full year, i.e. four digits) or day(two digits)/month(two digits)/year(full year, i.e. four digits) or day(two digits)-month(two digits)-year(full year, i.e. four digits)");
        return result;
    }
    int day, month, year;
    tie(day, month, year) = date_string_to_int(result.get_norm());
    if (day < 1)
    {
        result.set_error("Error! The day can't be negative");
        return result;
    }
    if (month < 1 || month > 12)
    {
        result.set_error("Error! The month can't be negative or more than 12");
        return result;
    }
    time_t t = time(0);
    tm *now = localtime(&t);
    ostringstream os;
    os << put_time(now, "%d.%m.%Y");
    string real_date = os.str();
    int real_day, real_month, real_year;
    tie(real_day, real_month, real_year) = date_string_to_int(real_date);
    if (year < real_year - MAX_AGE)
    {
        result.set_error("Error! A person's age cannot be more than 200 years old");
        return result;
    }
    if (day > day_in_month(month, year))
    {
        result.set_error("Error! There can be a maximum of"+ to_string(day_in_month(month, year)) + "days in" + to_string(month) + "month in" + to_string(year) + "year");
        return result;
    }
    if (real_year < year || (real_year == year && real_month < month) || (real_year == year && real_month == month && real_day <= day))
    {
        result.set_error("Error! A person's date of birth cannot be the same as or greater than today's date.");
        return result;
    }
    result.set_success(true);
    return result;
}
tuple<int, int, int> Validator::date_string_to_int(const string &s)
{
    size_t first_dot = s.find('.');
    size_t second_dot = s.find('.', first_dot + 1);
    int day = stoi(s.substr(0, first_dot));
    int month = stoi(s.substr(first_dot + 1, second_dot - first_dot - 1));
    int year = stoi(s.substr(second_dot + 1));
    return make_tuple(day, month, year);
}
int Validator::day_in_month(int month, int year)
{
    switch (month)
    {
        case 1:case 3:case 5:case 7:case 8:case 10:case 12: return 31; break;
        case 4:case 6:case 9:case 11: return 30; break;
        case 2:return is_leap_year(year) ? 29 : 28; break;
        default: return 0; break;
    }
}
bool Validator::is_leap_year(int year)
{
    if ((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0))
    {
        return true;
    }
    else
    {
        return false;
    }
}