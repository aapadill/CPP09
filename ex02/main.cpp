/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 16:32:51 by aapadill          #+#    #+#             */
/*   Updated: 2026/04/07 22:14:24 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

#include <exception>
#include <iostream>

int	main(int argc, char **argv)
{
	PmergeMe	pmergeMe;

	try
	{
		pmergeMe.parseInput(argc, argv);
		pmergeMe.printBefore();
		pmergeMe.sortVector();
		pmergeMe.sortDeque();
		pmergeMe.printAfterVector();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}
