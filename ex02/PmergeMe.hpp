/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 16:31:27 by aapadill          #+#    #+#             */
/*   Updated: 2026/04/11 19:51:54 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <deque>
#include <vector>

class PmergeMe
{
	private:
		std::vector<int>	_vector;
		std::vector<int>	_vectorO;
		std::deque<int>		_deque;
		std::deque<int>		_dequeO;
		double				_timeV;
		double				_timeD;

	public:
		PmergeMe();
		PmergeMe(const PmergeMe& other);
		~PmergeMe();
		PmergeMe&	operator=(const PmergeMe& other);

		void	parseInput(int argc, char **argv);
		void	printBefore() const;
		void	sortVector();
		void	printAfter() const;
		void	sortDeque();
		void	validateResults() const;
		void	printTimings() const;
};
