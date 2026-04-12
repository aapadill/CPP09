/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 16:32:00 by aapadill          #+#    #+#             */
/*   Updated: 2026/04/12 06:53:27 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"
#include <cctype>
#include <climits>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

//trims whitespace from both ends of a string
static std::string	trim(const std::string& str)
{
	std::string::size_type	start;
	std::string::size_type	end;

	start = str.find_first_not_of(" \t\n\r\f\v");
	if (start == std::string::npos)
		return "";
	end = str.find_last_not_of(" \t\n\r\f\v");
	return str.substr(start, end - start + 1);
}

//checks format is exactly YYYY-MM-DD with digits in the right places
static bool	isValidDateFormat(const std::string& date)
{
	std::size_t	i;

	if (date.length() != 10 || date[4] != '-' || date[7] != '-')
		return false;
	i = 0;
	while (i < date.length())
	{
		if (i != 4 && i != 7 && !std::isdigit(static_cast<unsigned char>(date[i])))
			return false;
		i++;
	}
	return true;
}

static bool	isLeapYear(int year)
{
	if (year % 400 == 0)
		return true;
	if (year % 100 == 0)
		return false;
	return year % 4 == 0;
}

//validates format and calendar correctness (leap years, day ranges)
static bool	isValidDate(const std::string& date)
{
	static const int	daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	int					year;
	int					month;
	int					day;
	int					maxDay;

	if (!isValidDateFormat(date))
		return false;
	year = std::atoi(date.substr(0, 4).c_str());
	month = std::atoi(date.substr(5, 2).c_str());
	day = std::atoi(date.substr(8, 2).c_str());
	if (month < 1 || month > 12)
		return false;
	maxDay = daysInMonth[month - 1];
	if (month == 2 && isLeapYear(year))
		maxDay = 29;
	return day >= 1 && day <= maxDay;
}

//parses a num string into a double, rejecting bad input
//allows optional sign, digits, and one dot max
static bool	parseValue(const std::string& str, double& value)
{
	std::istringstream		aux_stream(str);
	std::string::size_type	i;
	int						dotCount;

	if (str.empty())
		return false;
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (i == str.length())
		return false;
	dotCount = 0;
	while (i < str.length())
	{
		if (str[i] == '.')
			dotCount++;
		else if (!std::isdigit(static_cast<unsigned char>(str[i])))
			return false;
		i++;
	}
	if (dotCount > 1)
		return false;
	aux_stream >> value;
	if (aux_stream.fail())
		return false;
	aux_stream >> std::ws;
	return aux_stream.eof();
}

//finds the exchange rate for a date, or the closest earlier date
//throws if no earlier date exists in the database
static double	getRateForDate(const std::map<std::string, double>& rates, const std::string& date)
{
	std::map<std::string, double>::const_iterator	it;

	//lower_bound: first element >= date
	it = rates.lower_bound(date);
	//exact match
	if (it != rates.end() && it->first == date)
		return it->second;
	//no earlier date exists
	if (it == rates.begin())
		throw std::out_of_range("No earlier exchange rate found.");
	//step back to the closest earlier date
	it--;
	return it->second;
}

//parses one line from the csv database (format: "date,rate")
static void	parseDatabaseLine(const std::string& line, std::map<std::string, double>& rates)
{
	std::string::size_type	comma;
	std::string				date;
	std::string				aux_rateText;
	double					rate;

	comma = line.find(',');
	if (comma == std::string::npos || line.find(',', comma + 1) != std::string::npos)
		throw std::runtime_error("Error: bad database line => " + line);
	date = trim(line.substr(0, comma));
	aux_rateText = trim(line.substr(comma + 1));
	if (!isValidDate(date))
		throw std::runtime_error("Error: bad database line => " + line);
	if (!parseValue(aux_rateText, rate) || rate < 0.0)
		throw std::runtime_error("Error: bad database line => " + line);
	if (!rates.insert(std::make_pair(date, rate)).second)
		throw std::runtime_error("Error: duplicate date in database => " + date);
}

//processes one line from the input file (format: "date | value")
//prints result or error to stdout
static void	processInputLine(const std::string& line, const std::map<std::string, double>& rates)
{
	std::string::size_type	separator;
	std::string				date;
	std::string				aux_valueText;
	double					value;
	double					rate;

	separator = line.find('|');
	if (separator == std::string::npos || line.find('|', separator + 1) != std::string::npos)
	{
		std::cout << "Error: bad input => " << line << std::endl;
		return;
	}
	date = trim(line.substr(0, separator));
	aux_valueText = trim(line.substr(separator + 1));
	if (!isValidDate(date) || !parseValue(aux_valueText, value))
	{
		std::cout << "Error: bad input => " << line << std::endl;
		return;
	}
	if (value < 0.0)
	{
		std::cout << "Error: not a positive number." << std::endl;
		return;
	}
	if (value > 1000.0)
	{
		std::cout << "Error: too large a number." << std::endl;
		return;
	}
	try
	{
		rate = getRateForDate(rates, date);
	}
	catch (const std::out_of_range&)
	{
		std::cout << "Error: bad input => " << line << std::endl;
		return;
	}
	std::cout << date << " => " << aux_valueText << " = " << value * rate << std::endl;
}

//OCF stuff
BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& o) : _rates(o._rates) {}

BitcoinExchange::~BitcoinExchange() {}

BitcoinExchange&	BitcoinExchange::operator=(const BitcoinExchange& o)
{
	if (this != &o)
		_rates = o._rates;
	return *this;
}

//main flow
void	BitcoinExchange::loadDatabase(const std::string& filename)
{
	std::ifstream	file(filename.c_str());
	std::string		line;

	if (!file.is_open())
		throw std::runtime_error("Error: could not open database.");
	if (!std::getline(file, line) || line != "date,exchange_rate")
		throw std::runtime_error("Error: invalid database header.");
	_rates.clear();
	while (std::getline(file, line))
		parseDatabaseLine(line, _rates);
}

void	BitcoinExchange::processInputFile(const std::string& filename) const
{
	std::ifstream	file(filename.c_str());
	std::string		line;

	if (!file.is_open())
		throw std::runtime_error("Error: could not open file.");
	if (!std::getline(file, line) || line != "date | value")
		throw std::runtime_error("Error: invalid input header.");
	while (std::getline(file, line))
		processInputLine(line, _rates);
}
