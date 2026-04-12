/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 16:32:12 by aapadill          #+#    #+#             */
/*   Updated: 2026/04/12 06:53:37 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once 
#include <map>
#include <string>
 
class BitcoinExchange
{
	private:
		//date -> rate, sorted by date (std::map)
		std::map<std::string, double>	_rates;
 
	public:
		//OCF
		BitcoinExchange();
		BitcoinExchange(const BitcoinExchange& other);
		~BitcoinExchange();
		BitcoinExchange&	operator=(const BitcoinExchange& other);
 
		//main flow
		void	loadDatabase(const std::string& filename);
		void	processInputFile(const std::string& filename) const;
};
