/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 16:31:18 by aapadill          #+#    #+#             */
/*   Updated: 2026/04/07 16:14:23 by aapadill         ###   ########.fr       */
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

std::size_t	PmergeMe::jacobsthal(std::size_t n)
{
	std::size_t	previous;
	std::size_t	current;
	std::size_t	next;
	std::size_t	index;

	if (n == 0)
		return 0;
	if (n == 1)
		return 1;
	previous = 0;
	current = 1;
	index = 2;
	while (index <= n)
	{
		next = current + (2 * previous);
		previous = current;
		current = next;
		++index;
	}
	return current;
}

std::vector<std::size_t>	PmergeMe::buildInsertionOrder(std::size_t loserCount)
{
	std::vector<std::size_t>	order;
	std::size_t				previousBoundary;
	std::size_t				currentBoundary;
	std::size_t				jacobsthalIndex;
	std::size_t				index;

	if (loserCount <= 1)
		return order;
	previousBoundary = 1;
	jacobsthalIndex = 3;
	while (previousBoundary < loserCount)
	{
		currentBoundary = jacobsthal(jacobsthalIndex);
		if (currentBoundary > loserCount)
			currentBoundary = loserCount;
		index = currentBoundary;
		while (index > previousBoundary)
		{
			order.push_back(index);
			--index;
		}
		if (currentBoundary == loserCount)
			break ;
		previousBoundary = currentBoundary;
		++jacobsthalIndex;
	}
	return order;
}
