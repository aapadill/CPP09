/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 16:31:18 by aapadill          #+#    #+#             */
/*   Updated: 2026/04/10 00:00:00 by aapadill         ###   ########.fr       */
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
 
int	parsePositiveInt(const char *s, std::size_t len)
{
	int	value;
 
	if (len == 0)
		throw std::runtime_error("Error");
	auto res = std::from_chars(s, s + len, value);
	if (res.ec != std::errc() || res.ptr != s + len || value <= 0)
		throw std::runtime_error("Error");
	return value;
}
 
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
		++i;
	}
	return curr;
}
 
std::vector<std::size_t>	insertionOrder(std::size_t count)
{
	std::vector<std::size_t>	order;
	std::size_t					prevBound = 1;
	std::size_t					ji = 3;
 
	while (prevBound < count)
	{
		std::size_t bound = jacobsthal(ji);
		if (bound > count)
			bound = count;
		std::size_t idx = bound;
		while (idx > prevBound)
		{
			order.push_back(idx);
			--idx;
		}
		if (bound == count)
			break;
		prevBound = bound;
		++ji;
	}
	return order;
}
 
/*
** losers[k] = {bk, ak} where ak == -1 means no bound (straggler)
** losers[0] is a dummy; losers[1].first is the freebie (b1)
*/
template <typename C>
void	mergeInsert(C& seq)
{
	if (seq.size() < 2)
		return;
 
	//phase 1: pair up and compare
	std::vector<std::pair<int,int>>	aux_pairs;
	C								mainChain;
	int								straggler = -1;
	std::size_t						i = 0;
 
	while (i + 1 < seq.size())
	{
		int winner = seq[i];
		int loser = seq[i + 1];
		if (winner < loser)
			std::swap(winner, loser);
		aux_pairs.push_back({winner, loser});
		mainChain.push_back(winner);
		i += 2;
	}
	if (i < seq.size())
		straggler = seq[i];
 
	//phase 2: recursively sort winners (main chain)
	mergeInsert(mainChain);
 
	//reorder pairs to match sorted main chain using a hash map
	std::unordered_map<int, std::vector<int>>	aux_winToLoser;
	i = 0;
	while (i < aux_pairs.size())
	{
		aux_winToLoser[aux_pairs[i].first].push_back(aux_pairs[i].second);
		++i;
	}
 
	//build losers: losers[k] = {bk, ak} (1-indexed, 0 is dummy)
	std::vector<std::pair<int,int>>	losers;
	losers.push_back({-1, -1});
	i = 0;
	while (i < mainChain.size())
	{
		auto& aux_bucket = aux_winToLoser[mainChain[i]];
		losers.push_back({aux_bucket.back(), mainChain[i]});
		aux_bucket.pop_back();
		++i;
	}
	if (straggler != -1)
		losers.push_back({straggler, -1});
 
	//phase 3: insertion of losers into main chain
	if (losers.size() <= 1)
	{
		seq = mainChain;
		return;
	}
 
	// b1 is free, it's smaller than a1 (the smallest winner)
	mainChain.insert(mainChain.begin(), losers[1].first);
 
	auto	aux_jacobOrder = insertionOrder(losers.size() - 1);
	std::size_t	oi = 0;
 
	while (oi < aux_jacobOrder.size())
	{
		std::size_t	li = aux_jacobOrder[oi];
		int			loser = losers[li].first;
		int			winner = losers[li].second;
 
		//find insertion upper bound (position of paired winner)
		std::size_t	limit = mainChain.size();
		if (winner != -1)
		{
			auto it = std::find(mainChain.begin(), mainChain.end(), winner);
			if (it != mainChain.end())
				limit = static_cast<std::size_t>(it - mainChain.begin()); //we can implictly cast here right?
		}
 
		//binary search within [0, limit)
		auto pos = std::lower_bound(
			mainChain.begin(),
			mainChain.begin() + static_cast<std::ptrdiff_t>(limit),
			loser);
		mainChain.insert(pos, loser);
		++oi;
	}
 
	seq = mainChain;
}
 
template <typename C>
void	sortAndMeasure(const C& input, C& output, double& us)
{
	auto start = std::chrono::steady_clock::now();
	output = input;
	mergeInsert(output);
	auto end = std::chrono::steady_clock::now();
	us = std::chrono::duration<double, std::micro>(end - start).count();
}
 
//OCF stuff
PmergeMe::PmergeMe() : _vecUs(0.0), _deqUs(0.0) {}
 
PmergeMe::PmergeMe(const PmergeMe& o)
	: _vecIn(o._vecIn), _vecOut(o._vecOut),
	  _deqIn(o._deqIn), _deqOut(o._deqOut),
	  _vecUs(o._vecUs), _deqUs(o._deqUs) {}
 
PmergeMe::~PmergeMe() {}
 
PmergeMe&	PmergeMe::operator=(const PmergeMe& o)
{
	if (this != &o)
	{
		_vecIn = o._vecIn;
		_vecOut = o._vecOut;
		_deqIn = o._deqIn;
		_deqOut = o._deqOut;
		_vecUs = o._vecUs;
		_deqUs = o._deqUs;
	}
	return *this;
}
 
//parsing
void	PmergeMe::parseInput(int argc, char **argv)
{
	if (argc < 2)
		throw std::runtime_error("Error");
	_vecIn.clear();
	_deqIn.clear();
	_vecUs = 0.0;
	_deqUs = 0.0;
	int i = 1;
	while (i < argc)
	{
		std::string tok(argv[i]);
		int val = parsePositiveInt(tok.c_str(), tok.size());
		_vecIn.push_back(val);
		_deqIn.push_back(val);
		++i;
	}
}
 
void	PmergeMe::printBefore() const
{
	std::cout << "Before: ";
	std::size_t i = 0;
	while (i < _vecIn.size())
	{
		if (i != 0)
			std::cout << ' ';
		std::cout << _vecIn[i];
		++i;
	}
	std::cout << std::endl;
}
 
void	PmergeMe::sortVector()
{
	sortAndMeasure(_vecIn, _vecOut, _vecUs);
}
 
void	PmergeMe::printAfterVector() const
{
	std::cout << "After:  ";
	std::size_t i = 0;
	while (i < _vecOut.size())
	{
		if (i != 0)
			std::cout << ' ';
		std::cout << _vecOut[i];
		++i;
	}
	std::cout << std::endl;
}
 
void	PmergeMe::sortDeque()
{
	sortAndMeasure(_deqIn, _deqOut, _deqUs);
}
 
void	PmergeMe::printTimings() const
{
	std::cout << std::fixed << std::setprecision(6)
		<< "Time to process a range of " << _vecIn.size()
		<< " elements with std::vector : " << _vecUs << " us" << std::endl
		<< "Time to process a range of " << _deqIn.size()
		<< " elements with std::deque : " << _deqUs << " us" << std::endl;
}