/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 16:31:46 by aapadill          #+#    #+#             */
/*   Updated: 2026/04/06 09:26:16 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <list>
#include <string>

class RPN
{
	public:
		RPN();
		RPN(const RPN& other);
		~RPN();
		RPN&	operator=(const RPN& other);

		int	evaluate(const std::string& expression);

	private:
		std::list<int>	_stack;

		static bool	isOperator(const std::string& token);
		static int	applyOperation(int left, int right, char op);
		
		void	pushValue(int value);
		int		popValue();
		void	clear();
};
