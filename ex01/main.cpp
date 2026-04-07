/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 16:32:44 by aapadill          #+#    #+#             */
/*   Updated: 2026/04/06 09:26:19 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

#include <exception>
#include <iostream>

int	main(int argc, char **argv)
{
	RPN	rpn;

	if (argc != 2)
	{
		std::cerr << "Error" << std::endl;
		return 1;
	}
	try
	{
		std::cout << rpn.evaluate(argv[1]) << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}
