/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 16:31:18 by aapadill          #+#    #+#             */
/*   Updated: 2026/04/11 19:51:39 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

#include <algorithm>
#include <charconv>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <utility>

//begins with 0 1
//0 1 1 3 5 11 21 ..
std::size_t	jacobsthal(std::size_t n)
{
	std::size_t	prev = 0;
	std::size_t	curr = 1;
	std::size_t	i = 2;

	while (i <= n)
	{
		std::size_t next = curr + 2 * prev;
		prev = curr;
		curr = next;
		i++;
	}
	return curr;
}

//order of pend insertion into main
std::vector<std::size_t>	insertionOrder(std::size_t count)
{
	std::vector<std::size_t>	order;
	std::size_t					prevT = 1;
	std::size_t					tIndex = 3;

	while (prevT < count)
	{
		std::size_t currentT = jacobsthal(tIndex);
		if (currentT > count)
			currentT = count;
		std::size_t idx = currentT;
		while (idx > prevT)
		{
			order.push_back(idx);
			--idx;
		}
		if (currentT == count)
			break;
		prevT = currentT;
		tIndex++;
	}
	return order;
}

// aka Ford-Johnson algothe
// pend[k] = {bk, ak} where ak == -1 means no leftover
// pend[0] is a dummy; pend[1].first is the freebie (b1)
template <typename C>
void	mergeInsert(C& seq)
{
	if (seq.size() < 2)
		return;

	//phase 1: pair up and compare
	std::vector<std::pair<int,int>>	aux_pairs;
	C								main;
	int								leftover = -1; //doesnt exist initially
	std::size_t						i = 0;

	while (i + 1 < seq.size())
	{
		int	a = seq[i];
		int	b = seq[i + 1];
		if (a < b)
			std::swap(a, b);
		aux_pairs.push_back({a, b});
		main.push_back(a);
		i += 2;
	}
	if (i < seq.size()) //not enough elements to build another pair, but one element's still left, theory names it bn
		leftover = seq[i];

	//phase 2: recursively sort winners (main chain)
	mergeInsert(main);

	//reorder pairs to match the sorted main chain using a hash map, so repeated a's can preserve their b values
	std::unordered_map<int, std::vector<int>>	b_by_a;
	i = 0;
	while (i < aux_pairs.size())
	{
		int a = aux_pairs[i].first;
		int b = aux_pairs[i].second;
		b_by_a[a].push_back(b);
		i++;
	}

	//build pend: pend[k] = {bk, ak} (1-indexed, 0 index is a dummy just to match algo)
	std::vector<std::pair<int,int>>	pend;
	pend.push_back({-1, -1});
	i = 0;
	while (i < main.size())
	{
		int a = main[i];
		auto& b_values = b_by_a[a];
		int b = b_values.back(); //arbitrary, interchangable //if more than one value it could be front, end result is the same
		pend.push_back({b, a});
		b_values.pop_back();
		i++;
	}
	if (leftover != -1)
		pend.push_back({leftover, -1});
 
	//phase 3: insertion of pend into main
	if (pend.size() <= 1)
	{
		seq = main;
		return;
	}
 
	//algo tells us b1 can be placed first because b1 <= a1 and a1 is the smallest winner
	//remember: b0 is a dummy {-1, -1} we manually added at the zero index of the pend, so the real first element is b1
	int b1 = pend[1].first;
	main.insert(main.begin(), b1);

	//since we already added b1, we skip it in the calculation of the insertion order, so (pend - 1)
	auto	aux_jacobOrder = insertionOrder(pend.size() - 1);
	std::size_t	j = 0;

	while (j < aux_jacobOrder.size())
	{
		std::size_t	t = aux_jacobOrder[j];
		int			b = pend[t].first; //the actual value, the loser because //a >= b
		int			a = pend[t].second; //the one in main
 
		//find insertion upper bound (position of paired winner)
		std::size_t	limit = main.size();
		if (a != -1) //has its 'a pair' in main
		{
			auto it = std::find(main.begin(), main.end(), a); //position in the container
			if (it != main.end())
				limit = (it - main.begin()); //pointer arithmetic //diff between actual position of a and begin returns a number
		}
 
		//binary search of b within [0, limit)
		auto pos = std::lower_bound(main.begin(), main.begin() + limit, b);
		main.insert(pos, b);
		j++;
	} 
	seq = main; //sorted final result
}

template <typename C>
void	sortAndMeasure(const C& input, C& output, double& us)
{
	auto start = std::chrono::steady_clock::now();
	output = input;
	mergeInsert(output);
	auto end = std::chrono::steady_clock::now();
	auto elapsed = end - start;
	us = std::chrono::duration<double, std::micro>(elapsed).count();
}

//OCF stuff
PmergeMe::PmergeMe() : _timeV(0.0), _timeD(0.0) {}

PmergeMe::PmergeMe(const PmergeMe& o) : _vector(o._vector), _vectorO(o._vectorO), _deque(o._deque), _dequeO(o._dequeO), _timeV(o._timeV), _timeD(o._timeD) {}

PmergeMe::~PmergeMe() {}

PmergeMe&	PmergeMe::operator=(const PmergeMe& o)
{
	if (this != &o)
	{
		_vector = o._vector;
		_vectorO = o._vectorO;
		_deque = o._deque;
		_dequeO = o._dequeO;
		_timeV = o._timeV;
		_timeD = o._timeD;
	}
	return *this;
}

//main functions
//from subject: your program must be able to use a positive integer sequence as an argument
int	parsePositiveInt(const char *str)
{
	char	*end;
	long	value;

	if (str[0] == '\0')
		throw std::runtime_error("Error");
	value = std::strtol(str, &end, 10);
	if (*end != '\0' || value <= 0 || value > INT_MAX)
		throw std::runtime_error("Error");
	return static_cast<int>(value);
}

//main parsing
void	PmergeMe::parseInput(int argc, char **argv)
{
	if (argc < 2)
		throw std::runtime_error("Error");
	_vector.clear();
	_deque.clear();
	_timeV = 0.0;
	_timeD = 0.0;
	int i = 1;
	while (i < argc)
	{
		int val = parsePositiveInt(argv[i]);
		_vector.push_back(val);
		_deque.push_back(val);
		i++;
	}
}

void	PmergeMe::printBefore() const
{
	std::cout << "Before:	";
	std::size_t i = 0;
	while (i < _vector.size())
	{
		if (i != 0)
			std::cout << ' ';
		std::cout << _vector[i];
		i++;
	}
	std::cout << std::endl;
}

void	PmergeMe::sortVector()
{
	sortAndMeasure(_vector, _vectorO, _timeV);
}

void	PmergeMe::sortDeque()
{
	sortAndMeasure(_deque, _dequeO, _timeD);
}

void	PmergeMe::printAfter() const
{
	std::cout << "After:	";
	std::size_t i = 0;
	while (i < _vectorO.size())
	//while (i < _dequeO.size())
	{
		if (i != 0)
			std::cout << ' ';
		std::cout << _vectorO[i];
		//std::cout << _dequeO[i];
		i++;
	}
	std::cout << std::endl;
}

//out validation
void	PmergeMe::validateResults() const
{
	std::size_t	i = 0;

	if (!std::is_sorted(_vectorO.begin(), _vectorO.end()))
		throw std::runtime_error("Error");
	if (!std::is_sorted(_dequeO.begin(), _dequeO.end()))
		throw std::runtime_error("Error");
	if (_vectorO.size() != _dequeO.size())
		throw std::runtime_error("Error");
	while (i < _vectorO.size())
	{
		if (_vectorO[i] != _dequeO[i])
			throw std::runtime_error("Error");
		i++;
	}
}

void	PmergeMe::printTimings() const
{
	std::cout << std::fixed << std::setprecision(6)
		<< "Time to process a range of " << _vector.size()
		<< " elements with std::vector : " << _timeV << " us" << std::endl
		<< "Time to process a range of " << _deque.size()
		<< " elements with std::deque : " << _timeD << " us" << std::endl;
}
