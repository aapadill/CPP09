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

#include <chrono>
#include <charconv>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

int	parsePositiveInt(const std::string& token)
{
	int		value;

	if (token.empty())
		throw std::runtime_error("Error");
	auto	begin = token.c_str();
	auto	end = begin + token.length();
	auto	result = std::from_chars(begin, end, value);
	if (result.ec != std::errc() || result.ptr != end || value <= 0)
		throw std::runtime_error("Error");
	return value;
}

void	printSequence(const std::vector<int>& sequence)
{
	std::size_t	i = 0;

	while (i < sequence.size())
	{
		if (i != 0)
			std::cout << ' ';
		std::cout << sequence[i];
		++i;
	}
	std::cout << std::endl;
}

std::size_t	jacobsthal(std::size_t n)
{
	std::size_t	previous = 0;
	std::size_t	current = 1;
	std::size_t	next = 0;
	std::size_t	index = 2;

	if (n == 0)
		return 0;
	if (n == 1)
		return 1;
	while (index <= n)
	{
		next = current + (2 * previous);
		previous = current;
		current = next;
		++index;
	}
	return current;
}

auto	buildInsertionOrder(std::size_t loserCount)
{
	std::vector<std::size_t>	order;
	std::size_t				previousBoundary = 1;
	std::size_t				currentBoundary = 0;
	std::size_t				jacobsthalIndex = 3;
	std::size_t				index = 0;

	if (loserCount <= 1)
		return order;
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

template <typename IntContainer>
auto	makeItems(const IntContainer& values)
{
	typename TaggedValues<IntContainer>::type	items;
	TaggedValue				item;
	std::size_t				i = 0;

	while (i < values.size())
	{
		item.value = values[i];
		item.id = i;
		items.push_back(item);
		++i;
	}
	return items;
}

template <typename IntContainer>
auto	extractValues(const typename TaggedValues<IntContainer>::type& items)
{
	IntContainer	values;
	std::size_t		i = 0;

	while (i < items.size())
	{
		values.push_back(items[i].value);
		++i;
	}
	return values;
}

template <typename ItemContainer>
void	buildPairs(const ItemContainer& items,
	typename TaggedPairs<ItemContainer>::type& pairs,
	ItemContainer& winners, TaggedValue& straggler, bool& hasStraggler)
{
	TaggedPair	pair;
	std::size_t	i = 0;

	hasStraggler = false;
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

template <typename ItemContainer>
auto	orderPairsByWinners(
	const typename TaggedPairs<ItemContainer>::type& pairs,
	const ItemContainer& sortedWinners)
{
	typename TaggedPairs<ItemContainer>::type	orderedPairs;
	std::size_t				winnerIndex = 0;
	std::size_t				pairIndex = 0;

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

template <typename ItemContainer>
auto	buildInitialChain(
	const typename TaggedPairs<ItemContainer>::type& orderedPairs,
	const ItemContainer& sortedWinners)
{
	ItemContainer	chain;
	std::size_t		i = 0;

	chain.push_back(orderedPairs[0].loser);
	while (i < sortedWinners.size())
	{
		chain.push_back(sortedWinners[i]);
		++i;
	}
	return chain;
}

template <typename ItemContainer>
auto	buildPendingLosers(
	const typename TaggedPairs<ItemContainer>::type& orderedPairs,
	const TaggedValue& straggler, bool hasStraggler)
{
	typename PendingTaggedValues<ItemContainer>::type	pendingLosers;
	std::size_t									i = 0;

	pendingLosers.resize(orderedPairs.size() + (hasStraggler ? 1 : 0) + 1);
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

template <typename ItemContainer>
std::size_t	findWinnerPosition(const ItemContainer& chain, std::size_t winnerId)
{
	std::size_t	i = 0;

	while (i < chain.size())
	{
		if (chain[i].id == winnerId)
			return i;
		++i;
	}
	return chain.size();
}

template <typename ItemContainer>
std::size_t	binaryInsertPosition(const ItemContainer& chain,
	const typename ItemContainer::value_type& item, std::size_t end)
{
	std::size_t	left = 0;
	std::size_t	right = end;
	std::size_t	middle = 0;

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

template <typename ItemContainer>
void	insertPendingLosers(ItemContainer& chain,
	const typename PendingTaggedValues<ItemContainer>::type& pendingLosers)
{
	auto					insertionOrder = buildInsertionOrder(pendingLosers.size() - 1);
	std::size_t				orderIndex = 0;
	std::size_t				pendingIndex = 0;
	std::size_t				limit = 0;
	std::size_t				position = 0;

	while (orderIndex < insertionOrder.size())
	{
		pendingIndex = insertionOrder[orderIndex];
		if (pendingLosers[pendingIndex].hasWinner)
			limit = findWinnerPosition(chain, pendingLosers[pendingIndex].winnerId);
		else
			limit = chain.size();
		position = binaryInsertPosition(chain, pendingLosers[pendingIndex].item,
				limit);
		chain.insert(chain.begin() + static_cast<std::ptrdiff_t>(position),
			pendingLosers[pendingIndex].item);
		++orderIndex;
	}
}

template <typename ItemContainer>
auto	fordJohnson(const ItemContainer& items)
{
	typename TaggedPairs<ItemContainer>::type	pairs;
	ItemContainer	winners;
	TaggedValue		straggler;
	bool			hasStraggler = false;

	if (items.size() <= 1)
		return items;
	buildPairs(items, pairs, winners, straggler, hasStraggler);
	auto	sortedWinners = fordJohnson(winners);
	auto	orderedPairs = orderPairsByWinners(pairs, sortedWinners);
	auto	chain = buildInitialChain(orderedPairs, sortedWinners);
	insertPendingLosers(chain,
		buildPendingLosers<ItemContainer>(orderedPairs, straggler, hasStraggler));
	return chain;
}

template <typename IntContainer>
auto	sortValues(const IntContainer& input)
{
	auto	items = makeItems(input);
	auto	sortedItems = fordJohnson(items);
	auto	sortedValues = extractValues<IntContainer>(sortedItems);

	return sortedValues;
}

template <typename IntContainer>
void	sortAndMeasure(const IntContainer& input, IntContainer& output, double& timeUs)
{
	auto	start = std::chrono::steady_clock::now();

	output = sortValues(input);

	auto	end = std::chrono::steady_clock::now();
	timeUs = std::chrono::duration<double, std::micro>(end - start).count();
}

void	printTimingLine(const char *containerName, std::size_t elementCount, double timeUs)
{
	std::cout << std::fixed << std::setprecision(6)
		<< "Time to process a range of " << elementCount
		<< " elements with " << containerName << " : " << timeUs << " us"
		<< std::endl;
}

PmergeMe::PmergeMe() : _vectorTimeUs(0.0), _dequeTimeUs(0.0)
{
}

PmergeMe::PmergeMe(const PmergeMe& other)
	: _vectorInput(other._vectorInput), _vectorSorted(other._vectorSorted),
		_dequeInput(other._dequeInput), _dequeSorted(other._dequeSorted),
		_vectorTimeUs(other._vectorTimeUs), _dequeTimeUs(other._dequeTimeUs)
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
		_dequeSorted = other._dequeSorted;
		_vectorTimeUs = other._vectorTimeUs;
		_dequeTimeUs = other._dequeTimeUs;
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
	_dequeSorted.clear();
	_vectorTimeUs = 0.0;
	_dequeTimeUs = 0.0;
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

void	PmergeMe::sortVector()
{
	sortAndMeasure(_vectorInput, _vectorSorted, _vectorTimeUs);
}

void	PmergeMe::printAfterVector() const
{
	std::cout << "After:  ";
	printSequence(_vectorSorted);
}

void	PmergeMe::sortDeque()
{
	sortAndMeasure(_dequeInput, _dequeSorted, _dequeTimeUs);
}

void	PmergeMe::printTimings() const
{
	printTimingLine("std::vector", _vectorInput.size(), _vectorTimeUs);
	printTimingLine("std::deque", _dequeInput.size(), _dequeTimeUs);
}
