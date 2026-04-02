/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 16:32:00 by aapadill          #+#    #+#             */
/*   Updated: 2026/03/29 13:02:26 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

BitcoinExchange::BitcoinExchange()
{
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) : _rates(other._rates)
{
}

BitcoinExchange::~BitcoinExchange()
{
}

BitcoinExchange&	BitcoinExchange::operator=(const BitcoinExchange& other)
{
	if (this != &other)
		_rates = other._rates;
	return *this;
}

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
		parseDatabaseLine(line);
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
		processInputLine(line);
}

std::string	BitcoinExchange::trim(const std::string& str)
{
	std::string::size_type	start;
	std::string::size_type	end;

	start = str.find_first_not_of(" \t\n\r\f\v");
	if (start == std::string::npos)
		return "";
	end = str.find_last_not_of(" \t\n\r\f\v");
	return str.substr(start, end - start + 1);
}

bool	BitcoinExchange::isValidDateFormat(const std::string& date)
{
	std::size_t	i;

	if (date.length() != 10 || date[4] != '-' || date[7] != '-')
		return false;
	i = 0;
	while (i < date.length())
	{
		if (i != 4 && i != 7 && !std::isdigit(static_cast<unsigned char>(date[i])))
			return false;
		++i;
	}
	return true;
}

bool	BitcoinExchange::isValidDate(const std::string& date)
{
	static const int	daysInMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
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

bool	BitcoinExchange::isLeapYear(int year)
{
	if (year % 400 == 0)
		return true;
	if (year % 100 == 0)
		return false;
	return year % 4 == 0;
}

bool	BitcoinExchange::parseValue(const std::string& str, double& value)
{
	std::istringstream	stream(str);
	std::string::size_type	i;
	int						dotCount;

	if (str.empty())
		return false;
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		++i;
	if (i == str.length())
		return false;
	dotCount = 0;
	while (i < str.length())
	{
		if (str[i] == '.')
			++dotCount;
		else if (!std::isdigit(static_cast<unsigned char>(str[i])))
			return false;
		++i;
	}
	if (dotCount > 1)
		return false;
	stream >> value;
	if (stream.fail())
		return false;
	stream >> std::ws;
	return stream.eof();
}

bool	BitcoinExchange::isPositiveValue(double value)
{
	return value >= 0.0;
}

bool	BitcoinExchange::isTooLargeValue(double value)
{
	return value > 1000.0;
}

void	BitcoinExchange::parseDatabaseLine(const std::string& line)
{
	std::string::size_type	commaPos;
	std::string				date;
	std::string				rateString;
	double					rate;

	commaPos = line.find(',');
	if (commaPos == std::string::npos || line.find(',', commaPos + 1) != std::string::npos)
		throw std::runtime_error("Error: bad database line => " + line);
	date = trim(line.substr(0, commaPos));
	rateString = trim(line.substr(commaPos + 1));
	if (!isValidDate(date))
		throw std::runtime_error("Error: bad database line => " + line);
	if (!parseValue(rateString, rate) || !isPositiveValue(rate))
		throw std::runtime_error("Error: bad database line => " + line);
	if (!_rates.insert(std::make_pair(date, rate)).second)
		throw std::runtime_error("Error: duplicate date in database => " + date);
}

void	BitcoinExchange::processInputLine(const std::string& line) const
{
	std::string::size_type	separatorPos;
	std::string				date;
	std::string				valueString;
	double					value;
	double					rate;

	separatorPos = line.find('|');
	if (separatorPos == std::string::npos || line.find('|', separatorPos + 1) != std::string::npos)
	{
		std::cout << "Error: bad input => " << line << std::endl;
		return ;
	}
	date = trim(line.substr(0, separatorPos));
	valueString = trim(line.substr(separatorPos + 1));
	if (!isValidDate(date) || !parseValue(valueString, value))
	{
		std::cout << "Error: bad input => " << line << std::endl;
		return ;
	}
	if (!isPositiveValue(value))
	{
		std::cout << "Error: not a positive number." << std::endl;
		return ;
	}
	if (isTooLargeValue(value))
	{
		std::cout << "Error: too large a number." << std::endl;
		return ;
	}
	try
	{
		rate = getRateForDate(date);
	}
	catch (const std::out_of_range&)
	{
		std::cout << "Error: bad input => " << line << std::endl;
		return ;
	}
	std::cout << date << " => " << valueString << " = " << value * rate << std::endl;
}

double	BitcoinExchange::getRateForDate(const std::string& date) const
{
	std::map<std::string, double>::const_iterator	it;

	it = _rates.lower_bound(date);
	if (it != _rates.end() && it->first == date)
		return it->second;
	if (it == _rates.begin())
		throw std::out_of_range("No earlier exchange rate found.");
	--it;
	return it->second;
}
