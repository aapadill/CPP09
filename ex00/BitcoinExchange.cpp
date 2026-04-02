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
#include <sstream>
#include <stdexcept>

BitcoinExchange::BitcoinExchange()
{
}

BitcoinExchange::~BitcoinExchange()
{
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
	(void)filename;
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
	static const int	daysInMonth[12] = {
		31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
	};
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

	stream >> value;
	if (stream.fail())
		return false;
	stream >> std::ws;
	return stream.eof();
}

bool	BitcoinExchange::isPositiveValue(double value)
{
	(void)value;
	return false;
}

bool	BitcoinExchange::isTooLargeValue(double value)
{
	(void)value;
	return false;
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
	(void)line;
}

double	BitcoinExchange::getRateForDate(const std::string& date) const
{
	(void)date;
	return 0.0;
}
