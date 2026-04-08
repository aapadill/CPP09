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
		void	printVectorTiming() const;
		void	printDequeTiming() const;

	private:
		//storing
		std::vector<int>	_vectorInput;
		std::vector<int>	_vectorSorted;
		std::deque<int>		_dequeInput;
		std::deque<int>		_dequeSorted;
		double				_vectorTimeUs;
		double				_dequeTimeUs;
};
