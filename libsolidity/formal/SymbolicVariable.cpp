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

#include <libsolidity/formal/SymbolicVariable.h>

#include <libsolidity/formal/SymbolicTypes.h>

using namespace std;
using namespace dev::solidity;

SymbolicVariable::SymbolicVariable(
	TypePointer _type,
	std::string const& _uniqueName,
	smt::SolverInterface& _interface
):
	m_type(move(_type)),
	m_uniqueName(_uniqueName),
	m_interface(_interface),
	m_ssa(std::make_shared<SSAVariable>())
{
}

smt::Expression SymbolicVariable::currentValue() const
{
	return valueAtIndex(m_ssa->index());
}

std::string SymbolicVariable::currentName() const
{
	return uniqueSymbol(m_ssa->index());
}

smt::Expression SymbolicVariable::valueAtIndex(int _index) const
{
	return m_interface.newVariable(uniqueSymbol(_index), smtSort(*m_type));
}

smt::Expression SymbolicVariable::operator()(std::vector<smt::Expression> _arguments) const
{
	return currentValue()(std::move(_arguments));
}

smt::Expression SymbolicVariable::increaseIndex()
{
	++(*m_ssa);
	return currentValue();
}

std::string SymbolicVariable::uniqueSymbol(unsigned _index) const
{
	return m_uniqueName + "_" + std::to_string(_index);
}
