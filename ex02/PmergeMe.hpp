/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 16:31:27 by aapadill          #+#    #+#             */
/*   Updated: 2026/04/10 00:00:00 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
 
#include <deque>
#include <vector>
 
class PmergeMe
{
	public:
		PmergeMe();
		PmergeMe(const PmergeMe& other);
		~PmergeMe();
		PmergeMe&	operator=(const PmergeMe& other);
 
		void	parseInput(int argc, char **argv);
		void	printBefore() const;
		void	sortVector();
		void	printAfterVector() const;
		void	sortDeque();
		void	printTimings() const;
 
	private:
		std::vector<int>	_vecIn;
		std::vector<int>	_vecOut;
		std::deque<int>		_deqIn;
		std::deque<int>		_deqOut;
		double				_vecUs;
		double				_deqUs;
};
