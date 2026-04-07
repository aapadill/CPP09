/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 16:31:27 by aapadill          #+#    #+#             */
/*   Updated: 2026/04/07 22:14:21 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <deque>
#include <string>
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

	private:
		std::vector<int>	_vectorInput;
		std::deque<int>		_dequeInput;

		static int	parsePositiveInt(const std::string& token);
		static void	printSequence(const std::vector<int>& sequence);
};
