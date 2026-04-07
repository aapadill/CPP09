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

RPN::RPN()
{
}

RPN::RPN(const RPN& other) : _stack(other._stack)
{
}

RPN::~RPN()
{
}

RPN&	RPN::operator=(const RPN& other)
{
	if (this != &other)
		_stack = other._stack;
	return *this;
}

int	RPN::evaluate(const std::string& expression)
{
	std::istringstream	stream(expression);
	std::string			token;
	int					left;
	int					right;
	int					result;

	clear();
	while (stream >> token)
	{
		if (token.length() == 1 && std::isdigit(static_cast<unsigned char>(token[0])))
			pushValue(token[0] - '0');
		else if (isOperator(token))
		{
			right = popValue();
			left = popValue();
			result = applyOperation(left, right, token[0]);
			pushValue(result);
		}
		else
			throw std::runtime_error("Error");
	}
	if (_stack.size() != 1)
		throw std::runtime_error("Error");
	return _stack.back();
}

bool	RPN::isOperator(const std::string& token)
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

int	RPN::applyOperation(int left, int right, char op)
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

void	RPN::pushValue(int value)
{
	_stack.push_back(value);
}

int	RPN::popValue()
{
	int	value;

	if (_stack.empty())
		throw std::runtime_error("Error");
	value = _stack.back();
	_stack.pop_back();
	return value;
}

void	RPN::clear()
{
	_stack.clear();
}
