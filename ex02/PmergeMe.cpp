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
#include <memory>
#include <stdexcept>
#include <string>

struct TaggedValue
{
	int			value;
	std::size_t	id;
};

struct TaggedPair
{
	TaggedValue	winner;
	TaggedValue	loser;
};

struct PendingTaggedValue
{
	TaggedValue	item;
	std::size_t	winnerId;
	bool		hasWinner;
};

template <typename T, typename Container>
struct RebindContainer;

template <typename T, template <typename, typename> class Sequence,
	typename ValueType, typename Allocator>
struct RebindContainer<T, Sequence<ValueType, Allocator> >
{
	typedef Sequence<T, std::allocator<T> >	type;
};

template <typename T, typename Container>
using ReboundContainer = typename RebindContainer<T, Container>::type;

int	parsePositiveInt(const std::string& token)
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
	if (result.ec != std::errc() || result.ptr != end || value <= 0)
		throw std::runtime_error("Error");
	return value;
}

void	printSequence(const std::vector<int>& sequence)
{
	auto	i = std::size_t(0); //std::size_t

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
	auto	previous = std::size_t(0); //std::size_t
	auto	current = std::size_t(1); //std::size_t
	auto	next = std::size_t(0); //std::size_t
	auto	index = std::size_t(2); //std::size_t

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

std::vector<std::size_t>	buildInsertionOrder(std::size_t loserCount)
{
	auto	order = std::vector<std::size_t>(); //std::vector<std::size_t>
	auto	previousBoundary = std::size_t(1); //std::size_t
	auto	currentBoundary = std::size_t(0); //std::size_t
	auto	jacobsthalIndex = std::size_t(3); //std::size_t
	auto	index = std::size_t(0); //std::size_t

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
ReboundContainer<TaggedValue, IntContainer>	makeItems(const IntContainer& values)
{
	auto	items = ReboundContainer<TaggedValue, IntContainer>(); //ReboundContainer<TaggedValue, IntContainer>
	auto	item = TaggedValue(); //TaggedValue
	auto	i = std::size_t(0); //std::size_t

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
IntContainer	extractValues(
	const ReboundContainer<TaggedValue, IntContainer>& items)
{
	auto	values = IntContainer(); //IntContainer
	auto	i = std::size_t(0); //std::size_t

	while (i < items.size())
	{
		values.push_back(items[i].value);
		++i;
	}
	return values;
}

template <typename ItemContainer>
void	buildPairs(const ItemContainer& items,
	ReboundContainer<TaggedPair, ItemContainer>& pairs,
	ItemContainer& winners, TaggedValue& straggler, bool& hasStraggler)
{
	auto	pair = TaggedPair(); //TaggedPair
	auto	i = std::size_t(0); //std::size_t

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
ReboundContainer<TaggedPair, ItemContainer>	orderPairsByWinners(
	const ReboundContainer<TaggedPair, ItemContainer>& pairs,
	const ItemContainer& sortedWinners)
{
	auto	orderedPairs = ReboundContainer<TaggedPair, ItemContainer>(); //ReboundContainer<TaggedPair, ItemContainer>
	auto	winnerIndex = std::size_t(0); //std::size_t
	auto	pairIndex = std::size_t(0); //std::size_t

	while (winnerIndex < sortedWinners.size())
	{
		pairIndex = std::size_t(0);
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
ItemContainer	buildInitialChain(
	const ReboundContainer<TaggedPair, ItemContainer>& orderedPairs,
	const ItemContainer& sortedWinners)
{
	auto	chain = ItemContainer(); //ItemContainer
	auto	i = std::size_t(0); //std::size_t

	chain.push_back(orderedPairs[0].loser);
	while (i < sortedWinners.size())
	{
		chain.push_back(sortedWinners[i]);
		++i;
	}
	return chain;
}

template <typename ItemContainer>
ReboundContainer<PendingTaggedValue, ItemContainer>	buildPendingLosers(
	const ReboundContainer<TaggedPair, ItemContainer>& orderedPairs,
	const TaggedValue& straggler, bool hasStraggler)
{
	auto	pendingLosers = ReboundContainer<PendingTaggedValue, ItemContainer>(); //ReboundContainer<PendingTaggedValue, ItemContainer>
	auto	i = std::size_t(0); //std::size_t

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
std::size_t	findWinnerPosition(const ItemContainer& chain,
	std::size_t winnerId)
{
	auto	i = std::size_t(0); //std::size_t

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
	auto	left = std::size_t(0); //std::size_t
	auto	right = end; //std::size_t
	auto	middle = std::size_t(0); //std::size_t

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
	const ReboundContainer<PendingTaggedValue, ItemContainer>& pendingLosers)
{
	auto	insertionOrder = buildInsertionOrder(pendingLosers.size() - 1); //std::vector<std::size_t>
	auto	orderIndex = std::size_t(0); //std::size_t
	auto	pendingIndex = std::size_t(0); //std::size_t
	auto	limit = std::size_t(0); //std::size_t
	auto	position = std::size_t(0); //std::size_t

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
ItemContainer	fordJohnson(const ItemContainer& items)
{
	auto	pairs = ReboundContainer<TaggedPair, ItemContainer>(); //ReboundContainer<TaggedPair, ItemContainer>
	auto	winners = ItemContainer(); //ItemContainer
	auto	sortedWinners = ItemContainer(); //ItemContainer
	auto	orderedPairs = ReboundContainer<TaggedPair, ItemContainer>(); //ReboundContainer<TaggedPair, ItemContainer>
	auto	chain = ItemContainer(); //ItemContainer
	auto	straggler = TaggedValue(); //TaggedValue
	auto	hasStraggler = false; //bool

	if (items.size() <= 1)
		return items;
	buildPairs(items, pairs, winners, straggler, hasStraggler);
	sortedWinners = fordJohnson(winners);
	orderedPairs = orderPairsByWinners(pairs, sortedWinners);
	chain = buildInitialChain(orderedPairs, sortedWinners);
	insertPendingLosers(chain,
		buildPendingLosers<ItemContainer>(orderedPairs, straggler, hasStraggler));
	return chain;
}

template <typename IntContainer>
IntContainer	sortValues(const IntContainer& input)
{
	return extractValues<IntContainer>(fordJohnson(makeItems(input)));
}

PmergeMe::PmergeMe()
	: _vectorTimeUs(0.0), _dequeTimeUs(0.0)
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
	std::chrono::steady_clock::time_point	start;
	std::chrono::steady_clock::time_point	end;

	start = std::chrono::steady_clock::now();
	_vectorSorted = sortValues(_vectorInput);
	end = std::chrono::steady_clock::now();
	_vectorTimeUs = std::chrono::duration<double, std::micro>(end - start).count();
}

void	PmergeMe::printAfterVector() const
{
	std::cout << "After:  ";
	printSequence(_vectorSorted);
}

void	PmergeMe::sortDeque()
{
	std::chrono::steady_clock::time_point	start;
	std::chrono::steady_clock::time_point	end;

	start = std::chrono::steady_clock::now();
	_dequeSorted = sortValues(_dequeInput);
	end = std::chrono::steady_clock::now();
	_dequeTimeUs = std::chrono::duration<double, std::micro>(end - start).count();
}

void	PmergeMe::printVectorTiming() const
{
	std::cout << std::fixed << std::setprecision(6)
		<< "Time to process a range of " << _vectorInput.size()
		<< " elements with std::vector : " << _vectorTimeUs << " us"
		<< std::endl;
}

void	PmergeMe::printDequeTiming() const
{
	std::cout << std::fixed << std::setprecision(6)
		<< "Time to process a range of " << _dequeInput.size()
		<< " elements with std::deque : " << _dequeTimeUs << " us"
		<< std::endl;
}
