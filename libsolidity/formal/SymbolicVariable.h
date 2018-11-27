/*
	This file is part of solidity.

	solidity is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	solidity is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with solidity.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <libsolidity/formal/SSAVariable.h>

#include <libsolidity/formal/SolverInterface.h>

#include <libsolidity/ast/Types.h>

#include <memory>

namespace dev
{
namespace solidity
{

class Type;

/**
 * This class represents the symbolic version of a program variable.
 */
class SymbolicVariable
{
public:
	SymbolicVariable(
		TypePointer _type,
		std::string const& _uniqueName,
		smt::SolverInterface& _interface
	);

	smt::Expression currentValue() const;
	std::string currentName() const;
	smt::Expression valueAtIndex(int _index) const;
	smt::Expression operator()(std::vector<smt::Expression> _arguments) const;
	smt::Expression increaseIndex();

	unsigned index() const { return m_ssa->index(); }
	unsigned& index() { return m_ssa->index(); }

	TypePointer const& type() const { return m_type; }

protected:
	std::string uniqueSymbol(unsigned _index) const;

	TypePointer m_type;
	std::string m_uniqueName;
	smt::SolverInterface& m_interface;
	std::shared_ptr<SSAVariable> m_ssa;
};

}
}
