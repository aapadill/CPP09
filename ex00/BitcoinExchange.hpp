/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 16:32:12 by aapadill          #+#    #+#             */
/*   Updated: 2026/03/27 18:00:58 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <string>

class BitcoinExchange
{
public:
	BitcoinExchange();
	BitcoinExchange(const BitcoinExchange& other);
	~BitcoinExchange();

	BitcoinExchange&	operator=(const BitcoinExchange& other);

	void	loadDatabase(const std::string& filename);
	void	processInputFile(const std::string& filename) const;

private:
	std::map<std::string, double>	_rates;

private:
	static std::string	trim(const std::string& str);

	static bool	isValidDateFormat(const std::string& date);
	static bool	isValidDate(const std::string& date);
	static bool	isLeapYear(int year);

	static bool	parseValue(const std::string& str, double& value);
	static bool	isPositiveValue(double value);
	static bool	isTooLargeValue(double value);

	void	parseDatabaseLine(const std::string& line);
	void	processInputLine(const std::string& line) const;

	double	getRateForDate(const std::string& date) const;
};
