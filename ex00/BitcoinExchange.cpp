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

BitcoinExchange::BitcoinExchange()
{
}

BitcoinExchange::~BitcoinExchange()
{
}

void	BitcoinExchange::loadDatabase(const std::string& filename)
{
	(void)filename;
}

void	BitcoinExchange::processInputFile(const std::string& filename) const
{
	(void)filename;
}

std::string	BitcoinExchange::trim(const std::string& str)
{
	(void)str;
	return "";
}

bool	BitcoinExchange::isValidDateFormat(const std::string& date)
{
	(void)date;
	return false;
}

bool	BitcoinExchange::isValidDate(const std::string& date)
{
	(void)date;
	return false;
}

bool	BitcoinExchange::isLeapYear(int year)
{
	(void)year;
	return false;
}

bool	BitcoinExchange::parseValue(const std::string& str, double& value)
{
	(void)str;
	(void)value;
	return false;
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
	(void)line;
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
