/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 16:31:27 by aapadill          #+#    #+#             */
/*   Updated: 2026/04/07 02:14:21 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstddef>
#include <deque>
#include <memory>
#include <vector>

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
struct Rebound
{
	typedef typename RebindContainer<T, Container>::type	type;
};

template <typename Container>
struct TaggedValues
{
	typedef typename RebindContainer<TaggedValue, Container>::type	type;
};

template <typename Container>
struct TaggedPairs
{
	typedef typename RebindContainer<TaggedPair, Container>::type	type;
};

template <typename Container>
struct PendingTaggedValues
{
	typedef typename RebindContainer<PendingTaggedValue, Container>::type	type;
};

class PmergeMe
{
	public:
		//ocf
		PmergeMe();
		PmergeMe(const PmergeMe& other);
		~PmergeMe();
		PmergeMe&	operator=(const PmergeMe& other);

		//format
		void	parseInput(int argc, char **argv);
		void	printBefore() const;
		void	sortVector();
		void	printAfterVector() const;
		void	sortDeque();
		void	printTimings() const;

	private:
		//storing
		std::vector<int>	_vectorInput;
		std::vector<int>	_vectorSorted;
		std::deque<int>		_dequeInput;
		std::deque<int>		_dequeSorted;
		double				_vectorTimeUs;
		double				_dequeTimeUs;
};
