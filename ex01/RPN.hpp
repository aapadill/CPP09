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
	private:
		//using list as a stack (push_back, pop_back, back)
		std::list<int>	_stack;
 
	public:
		//OCF
		RPN();
		RPN(const RPN& other);
		~RPN();
		RPN&	operator=(const RPN& other);
 
		//main flow
		int	evaluate(const std::string& expression);
};