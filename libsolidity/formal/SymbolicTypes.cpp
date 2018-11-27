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

#include <libsolidity/formal/SymbolicTypes.h>

#include <libsolidity/ast/Types.h>

#include <memory>

using namespace std;
using namespace dev::solidity;

smt::SortPointer dev::solidity::smtSort(Type const& _type)
{
	switch (smtKind(_type.category()))
	{
	case smt::Kind::Int:
		return make_shared<smt::Sort>(smt::Kind::Int);
	case smt::Kind::Bool:
		return make_shared<smt::Sort>(smt::Kind::Bool);
	case smt::Kind::Function:
	{
		auto fType = dynamic_cast<FunctionType const*>(&_type);
		solAssert(fType, "");
		vector<smt::SortPointer> parameterSorts = smtSort(fType->parameterTypes());
		auto returnTypes = fType->returnParameterTypes();
		// TODO remove this when we support tuples.
		solAssert(returnTypes.size() == 1, "");
		smt::SortPointer returnSort = smtSort(*returnTypes.at(0));
		return make_shared<smt::FunctionSort>(parameterSorts, returnSort);
	}
	case smt::Kind::Array:
	{
		if (isMapping(_type.category()))
		{
			auto mapType = dynamic_cast<MappingType const*>(&_type);
			solAssert(mapType, "");
			return make_shared<smt::ArraySort>(smtSort(*mapType->keyType()), smtSort(*mapType->valueType()));
		}
		solAssert(false, "Invalid type.");
	}
	default:
		solAssert(false, "Invalid type.");
	}
}

vector<smt::SortPointer> dev::solidity::smtSort(vector<TypePointer> const& _types)
{
	vector<smt::SortPointer> sorts;
	for (auto const& type: _types)
		sorts.push_back(smtSort(*type));
	return sorts;
}

smt::Kind dev::solidity::smtKind(Type::Category _category)
{
	if (isNumber(_category))
		return smt::Kind::Int;
	else if (isBool(_category))
		return smt::Kind::Bool;
	else if (isMapping(_category))
		return smt::Kind::Array;
	else if (isFunction(_category))
		return smt::Kind::Function;
	solAssert(false, "Invalid type.");
}

bool dev::solidity::isSupportedType(Type::Category _category)
{
	return isNumber(_category) ||
		isBool(_category) ||
		isMapping(_category);
}

bool dev::solidity::isSupportedTypeDeclaration(Type::Category _category)
{
	return isSupportedType(_category) ||
		isFunction(_category);
}

pair<bool, SymbolicVariable> dev::solidity::newSymbolicVariable(
	Type const& _type,
	std::string const& _uniqueName,
	smt::SolverInterface& _solver
)
{
	TypePointer type = convertSolidityType(_type);
	// Function types are a special case: they are supported as declarations
	// and can be called, but they cannot be assigned or used as parameters.
	if (isSupportedTypeDeclaration(type->category()))
		return make_pair(false, SymbolicVariable{type, _uniqueName, _solver});
	return make_pair(true, SymbolicVariable{make_shared<IntegerType>(256), _uniqueName, _solver});
}

TypePointer dev::solidity::convertSolidityType(Type const& _type)
{
	Type::Category category = _type.category();
	if (isAddress(category))
		return make_shared<IntegerType>(160);
	else if (isFixedBytes(category))
	{
		auto const& fixedBytesType = dynamic_cast<FixedBytesType const&>(_type);
		return make_shared<IntegerType>(fixedBytesType.numBytes() * 8);
	}
	else if (isRational(_type.category()))
		return make_shared<IntegerType>(256);

	return _type.shared_from_this();
}

bool dev::solidity::isSupportedType(Type const& _type)
{
	return isSupportedType(_type.category());
}

bool dev::solidity::isInteger(Type::Category _category)
{
	return _category == Type::Category::Integer;
}

bool dev::solidity::isRational(Type::Category _category)
{
	return _category == Type::Category::RationalNumber;
}

bool dev::solidity::isFixedBytes(Type::Category _category)
{
	return _category == Type::Category::FixedBytes;
}

bool dev::solidity::isAddress(Type::Category _category)
{
	return _category == Type::Category::Address;
}

bool dev::solidity::isNumber(Type::Category _category)
{
	return isInteger(_category) ||
		isRational(_category) ||
		isFixedBytes(_category) ||
		isAddress(_category);
}

bool dev::solidity::isBool(Type::Category _category)
{
	return _category == Type::Category::Bool;
}

bool dev::solidity::isFunction(Type::Category _category)
{
	return _category == Type::Category::Function;
}

bool dev::solidity::isMapping(Type::Category _category)
{
	return _category == Type::Category::Mapping;
}

smt::Expression dev::solidity::minValue(IntegerType const& _type)
{
	return smt::Expression(_type.minValue());
}

smt::Expression dev::solidity::maxValue(IntegerType const& _type)
{
	return smt::Expression(_type.maxValue());
}

void dev::solidity::smt::setSymbolicZeroValue(SymbolicVariable const& _variable, smt::SolverInterface& _interface)
{
	setSymbolicZeroValue(_variable.currentValue(), _variable.type(), _interface);
}

void dev::solidity::smt::setSymbolicZeroValue(smt::Expression _expr, TypePointer const& _type, smt::SolverInterface& _interface)
{
	if (isInteger(_type->category()))
		_interface.addAssertion(_expr == 0);
	else if (isBool(_type->category()))
		_interface.addAssertion(_expr == smt::Expression(false));
}

void dev::solidity::smt::setSymbolicUnknownValue(SymbolicVariable const& _variable, smt::SolverInterface& _interface)
{
	setSymbolicUnknownValue(_variable.currentValue(), _variable.type(), _interface);
}

void dev::solidity::smt::setSymbolicUnknownValue(smt::Expression _expr, TypePointer const& _type, smt::SolverInterface& _interface)
{
	if (isInteger(_type->category()))
	{
		auto intType = dynamic_cast<IntegerType const*>(_type.get());
		solAssert(intType, "");
		_interface.addAssertion(_expr >= minValue(*intType));
		_interface.addAssertion(_expr <= maxValue(*intType));
	}
}
