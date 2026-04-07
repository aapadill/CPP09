/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 16:31:18 by aapadill          #+#    #+#             */
/*   Updated: 2026/04/07 22:14:23 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

#include <charconv>
#include <iostream>
#include <stdexcept>

PmergeMe::PmergeMe()
{
}

PmergeMe::PmergeMe(const PmergeMe& other)
	: _vectorInput(other._vectorInput), _dequeInput(other._dequeInput)
{
}

PmergeMe::~PmergeMe()
{
}

PmergeMe&	PmergeMe::operator=(const PmergeMe& other)
{
	if (this != &other)
	{
		_vectorInput = other._vectorInput;
		_dequeInput = other._dequeInput;
	}
	return *this;
}

void	PmergeMe::parseInput(int argc, char **argv)
{
	int	value;
	int	i;

	if (argc < 2)
		throw std::runtime_error("Error");
	_vectorInput.clear();
	_dequeInput.clear();
	i = 1;
	while (i < argc)
	{
		value = parsePositiveInt(argv[i]);
		_vectorInput.push_back(value);
		_dequeInput.push_back(value);
		++i;
	}
}

void	PmergeMe::printBefore() const
{
	std::cout << "Before: ";
	printSequence(_vectorInput);
}

int	PmergeMe::parsePositiveInt(const std::string& token)
{
	int						value;
	const char				*begin;
	const char				*end;
	std::from_chars_result	result;

	if (token.empty())
		throw std::runtime_error("Error");
	begin = token.c_str();
	end = begin + token.length();
	result = std::from_chars(begin, end, value);
	if (result.ec != std::errc() || result.ptr != end)
		throw std::runtime_error("Error");
	if (value <= 0)
		throw std::runtime_error("Error");
	return value;
}

void	PmergeMe::printSequence(const std::vector<int>& sequence)
{
	std::size_t	i;

	i = 0;
	while (i < sequence.size())
	{
		if (i != 0)
			std::cout << ' ';
		std::cout << sequence[i];
		++i;
	}
	std::cout << std::endl;
}
