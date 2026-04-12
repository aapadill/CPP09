/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 16:31:40 by aapadill          #+#    #+#             */
/*   Updated: 2026/04/06 09:26:18 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"
 
#include <cctype>
#include <sstream>
#include <stdexcept>
 
//checks if token is one of + - * /
static bool	isOperator(const std::string& token)
{
	if (token.length() != 1)
		return false;
	if (token[0] == '+')
		return true;
	if (token[0] == '-')
		return true;
	if (token[0] == '*')
		return true;
	if (token[0] == '/')
		return true;
	return false;
}

//applies the operation, division by zero throws
static int	applyOperation(int left, int right, char op)
{
	if (op == '+')
		return left + right;
	if (op == '-')
		return left - right;
	if (op == '*')
		return left * right;
	if (right == 0)
		throw std::runtime_error("Error");
	return left / right;
}

//pops the top value from the stack, throws if empty
static int	popValue(std::list<int>& stack)
{
	int	top;
 
	if (stack.empty())
		throw std::runtime_error("Error");
	top = stack.back();
	stack.pop_back();
	return top;
}

//OCF stuff
RPN::RPN() {}

RPN::RPN(const RPN& o) : _stack(o._stack) {}

RPN::~RPN() {}

RPN&	RPN::operator=(const RPN& o)
{
	if (this != &o)
		_stack = o._stack;
	return *this;
}

//tokenizes the expression, pushes digits, applies operators
//at the end the stack must have exactly one value: the result
int	RPN::evaluate(const std::string& expression)
{
	std::istringstream	aux_stream(expression);
	std::string			token;
	int					left;
	int					right;
	int					result;
 
	_stack.clear();
	while (aux_stream >> token)
	{
		if (token.length() == 1 && std::isdigit(static_cast<unsigned char>(token[0])))
		{
			_stack.push_back(token[0] - '0');
		}
		else if (isOperator(token))
		{
			right = popValue(_stack);
			left = popValue(_stack);
			result = applyOperation(left, right, token[0]);
			_stack.push_back(result);
		}
		else
			throw std::runtime_error("Error");
	}
	if (_stack.size() != 1)
		throw std::runtime_error("Error");
	return _stack.back();
}