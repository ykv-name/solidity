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

#include <test/TestCase.h>
#include <test/libsolidity/ASTJSONTest.h>
#include <test/libsolidity/SyntaxTest.h>
#include <test/libsolidity/SMTCheckerJSONTest.h>
#include <test/libyul/YulOptimizerTest.h>

#include <boost/filesystem.hpp>

using namespace dev::solidity::test;

/** Container for all information regarding a testsuite */
struct Testsuite
{
	const char* title;
	const boost::filesystem::path path;
	const boost::filesystem::path subpath;
	bool smt;
	bool ipc;
	TestCase::TestCaseCreator testCaseCreator;
};


/// Array of all testsuits
const Testsuite testsuits[] = {
	{"Yul Optimizer",    "libyul",      "yulOptimizerTests",   false, false, &yul::test::YulOptimizerTest::create},
	{"Syntax",           "libsolidity", "syntaxTests",         false, false, &SyntaxTest::create},
	{"JSON AST",         "libsolidity", "ASTJSON",             false, false, &ASTJSONTest::create},
	{"SMT Checker",      "libsolidity", "smtCheckerTests",     true,  false, &SyntaxTest::create},
	{"SMT Checker JSON", "libsolidity", "smtCheckerTestsJSON", true,  false, &SMTCheckerTest::create}
};
