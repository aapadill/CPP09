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
#include <string>
#include <vector>

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

	private:
		//vector
		struct VectorItem
		{
			int			value;
			std::size_t	id;
		};

		struct VectorPair
		{
			VectorItem	winner;
			VectorItem	loser;
		};

		struct PendingLoser
		{
			VectorItem	item;
			std::size_t	winnerId;
			bool		hasWinner;
		};

		// Deque-side data.
		struct DequeItem
		{
			int			value;
			std::size_t	id;
		};

		struct DequePair
		{
			DequeItem	winner;
			DequeItem	loser;
		};

		struct PendingDequeLoser
		{
			DequeItem	item;
			std::size_t	winnerId;
			bool		hasWinner;
		};

		//storing
		std::vector<int>	_vectorInput;
		std::vector<int>	_vectorSorted;
		std::deque<int>		_dequeInput;
		std::deque<int>		_dequeSorted;

		//helpers
		static int	parsePositiveInt(const std::string& token);
		static void	printSequence(const std::vector<int>& sequence);
		static std::size_t	jacobsthal(std::size_t n);
		static std::vector<std::size_t>	buildInsertionOrder(std::size_t loserCount);

		//helpers for std::vector version of algo
		static std::vector<VectorItem>	fordJohnsonVector(const std::vector<VectorItem>& items);
		static std::vector<VectorItem>	makeVectorItems(const std::vector<int>& values);
		static std::vector<int>			extractVectorValues(const std::vector<VectorItem>& items);
		static void						buildVectorPairs(const std::vector<VectorItem>& items,
											std::vector<VectorPair>& pairs,
											std::vector<VectorItem>& winners,
											VectorItem& straggler,
											bool& hasStraggler);
		static std::vector<VectorPair>	orderPairsByWinners(const std::vector<VectorPair>& pairs,
															const std::vector<VectorItem>& sortedWinners);
		static std::vector<VectorItem>	buildInitialVectorChain(
											const std::vector<VectorPair>& orderedPairs,
											const std::vector<VectorItem>& sortedWinners);
		static std::vector<PendingLoser>	buildPendingLosers(
											const std::vector<VectorPair>& orderedPairs,
											const VectorItem& straggler,
											bool hasStraggler);
		static void						insertPendingLosers(
											std::vector<VectorItem>& chain,
											const std::vector<PendingLoser>& pendingLosers);
		static std::size_t	findWinnerPosition(const std::vector<VectorItem>& chain, 
													 std::size_t winnerId);
		static std::size_t	binaryInsertPosition(const std::vector<VectorItem>& chain,
												 const VectorItem& item,
													   std::size_t end);

		//helpers for std::deque version of algo
		static std::deque<DequeItem>	fordJohnsonDeque(const std::deque<DequeItem>& items);
		static std::deque<DequeItem>	makeDequeItems(const std::deque<int>& values);
		static std::deque<int>			extractDequeValues(const std::deque<DequeItem>& items);
		static void						buildDequePairs(const std::deque<DequeItem>& items,
											std::deque<DequePair>& pairs,
											std::deque<DequeItem>& winners,
											DequeItem& straggler,
											bool& hasStraggler);
		static std::deque<DequePair>	orderDequePairsByWinners(
											const std::deque<DequePair>& pairs,
											const std::deque<DequeItem>& sortedWinners);
		static std::deque<DequeItem>	buildInitialDequeChain(
											const std::deque<DequePair>& orderedPairs,
											const std::deque<DequeItem>& sortedWinners);
		static std::deque<PendingDequeLoser>	buildPendingDequeLosers(
											const std::deque<DequePair>& orderedPairs,
											const DequeItem& straggler,
											bool hasStraggler);
		static void						insertPendingDequeLosers(
											std::deque<DequeItem>& chain,
											const std::deque<PendingDequeLoser>& pendingLosers);
		static std::size_t				findDequeWinnerPosition(
											const std::deque<DequeItem>& chain,
											std::size_t winnerId);
		static std::size_t				binaryDequeInsertPosition(
											const std::deque<DequeItem>& chain,
											const DequeItem& item,
											std::size_t end);
};
