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
		_vectorSorted = other._vectorSorted;
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
	_vectorSorted.clear();
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

//std::vector version of ford johnson algo
std::vector<PmergeMe::VectorItem>	PmergeMe::fordJohnsonVector(const std::vector<VectorItem>& items)
{
	std::vector<VectorPair>	pairs;
	std::vector<VectorItem>	winners;
	std::vector<VectorItem>	sortedWinners;
	std::vector<VectorPair>	orderedPairs;
	std::vector<VectorItem>	chain;
	VectorItem				straggler;
	bool					hasStraggler;

	if (items.size() <= 1)
		return items;
	buildVectorPairs(items, pairs, winners, straggler, hasStraggler);
	sortedWinners = fordJohnsonVector(winners);
	orderedPairs = orderPairsByWinners(pairs, sortedWinners);
	chain = buildInitialVectorChain(orderedPairs, sortedWinners);
	insertPendingLosers(chain, buildPendingLosers(orderedPairs, straggler, hasStraggler));
	return chain;
}

//helpers for std::vector version of ford johnson algo
void	PmergeMe::sortVector()
{
	_vectorSorted = extractVectorValues(fordJohnsonVector(makeVectorItems(_vectorInput)));
}

void	PmergeMe::printAfterVector() const
{
	std::cout << "After:  ";
	printSequence(_vectorSorted);
}

std::vector<PmergeMe::VectorItem>	PmergeMe::makeVectorItems(const std::vector<int>& values)
{
	std::vector<VectorItem>	items;
	VectorItem				item;
	std::size_t				i;

	i = 0;
	while (i < values.size())
	{
		item.value = values[i];
		item.id = i;
		items.push_back(item);
		++i;
	}
	return items;
}

std::vector<int>	PmergeMe::extractVectorValues(const std::vector<VectorItem>& items)
{
	std::vector<int>	values;
	std::size_t			i;

	i = 0;
	while (i < items.size())
	{
		values.push_back(items[i].value);
		++i;
	}
	return values;
}

void	PmergeMe::buildVectorPairs(const std::vector<VectorItem>& items,
	std::vector<VectorPair>& pairs, std::vector<VectorItem>& winners,
	VectorItem& straggler, bool& hasStraggler)
{
	VectorPair	pair;
	std::size_t	i;

	hasStraggler = false;
	i = 0;
	while (i + 1 < items.size())
	{
		if (items[i].value > items[i + 1].value)
		{
			pair.winner = items[i];
			pair.loser = items[i + 1];
		}
		else
		{
			pair.winner = items[i + 1];
			pair.loser = items[i];
		}
		pairs.push_back(pair);
		winners.push_back(pair.winner);
		i += 2;
	}
	if (i < items.size())
	{
		straggler = items[i];
		hasStraggler = true;
	}
}

std::vector<PmergeMe::VectorPair>	PmergeMe::orderPairsByWinners(const std::vector<VectorPair>& pairs, const std::vector<VectorItem>& sortedWinners)
{
	std::vector<VectorPair>	orderedPairs;
	std::size_t				winnerIndex;
	std::size_t				pairIndex;

	winnerIndex = 0;
	while (winnerIndex < sortedWinners.size())
	{
		pairIndex = 0;
		while (pairIndex < pairs.size())
		{
			if (pairs[pairIndex].winner.id == sortedWinners[winnerIndex].id)
			{
				orderedPairs.push_back(pairs[pairIndex]);
				break ;
			}
			++pairIndex;
		}
		++winnerIndex;
	}
	return orderedPairs;
}

std::vector<PmergeMe::VectorItem>	PmergeMe::buildInitialVectorChain(
	const std::vector<VectorPair>& orderedPairs, const std::vector<VectorItem>& sortedWinners)
{
	std::vector<VectorItem>	chain;
	std::size_t				i;

	chain.push_back(orderedPairs[0].loser);
	i = 0;
	while (i < sortedWinners.size())
	{
		chain.push_back(sortedWinners[i]);
		++i;
	}
	return chain;
}

std::vector<PmergeMe::PendingLoser>	PmergeMe::buildPendingLosers(
	const std::vector<VectorPair>& orderedPairs, const VectorItem& straggler,
	bool hasStraggler)
{
	std::vector<PendingLoser>	pendingLosers;
	std::size_t					i;

	pendingLosers.resize(orderedPairs.size() + (hasStraggler ? 1 : 0) + 1);
	i = 0;
	while (i < orderedPairs.size())
	{
		pendingLosers[i + 1].item = orderedPairs[i].loser;
		pendingLosers[i + 1].winnerId = orderedPairs[i].winner.id;
		pendingLosers[i + 1].hasWinner = true;
		++i;
	}
	if (hasStraggler)
	{
		pendingLosers[orderedPairs.size() + 1].item = straggler;
		pendingLosers[orderedPairs.size() + 1].winnerId = 0;
		pendingLosers[orderedPairs.size() + 1].hasWinner = false;
	}
	return pendingLosers;
}

void	PmergeMe::insertPendingLosers(std::vector<VectorItem>& chain,
	const std::vector<PendingLoser>& pendingLosers)
{
	std::vector<std::size_t>	insertionOrder;
	std::size_t				orderIndex;
	std::size_t				pendingIndex;
	std::size_t				limit;
	std::size_t				position;

	insertionOrder = buildInsertionOrder(pendingLosers.size() - 1);
	orderIndex = 0;
	while (orderIndex < insertionOrder.size())
	{
		pendingIndex = insertionOrder[orderIndex];
		if (pendingLosers[pendingIndex].hasWinner)
			limit = findWinnerPosition(chain, pendingLosers[pendingIndex].winnerId);
		else
			limit = chain.size();
		position = binaryInsertPosition(chain, pendingLosers[pendingIndex].item, limit);
		chain.insert(chain.begin() + static_cast<std::ptrdiff_t>(position),
			pendingLosers[pendingIndex].item);
		++orderIndex;
	}
}

std::size_t	PmergeMe::findWinnerPosition(const std::vector<VectorItem>& chain, std::size_t winnerId)
{
	std::size_t	i;

	i = 0;
	while (i < chain.size())
	{
		if (chain[i].id == winnerId)
			return i;
		++i;
	}
	return chain.size();
}

std::size_t	PmergeMe::binaryInsertPosition(
	const std::vector<VectorItem>& chain, const VectorItem& item, std::size_t end)
{
	std::size_t	left;
	std::size_t	right;
	std::size_t	middle;

	left = 0;
	right = end;
	while (left < right)
	{
		middle = left + ((right - left) / 2);
		if (chain[middle].value < item.value)
			left = middle + 1;
		else
			right = middle;
	}
	return left;
}
