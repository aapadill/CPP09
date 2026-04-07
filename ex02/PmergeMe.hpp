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

		//storing
		std::vector<int>	_vectorInput;
		std::vector<int>	_vectorSorted;
		std::deque<int>		_dequeInput;

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
};
