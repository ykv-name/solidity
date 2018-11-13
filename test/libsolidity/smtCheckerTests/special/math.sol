pragma experimental SMTChecker;

contract C
{
	function f(uint x, uint y, uint z) public pure {
		uint r = addmod(x, y, z);
		assert(r > 0);
		uint q = mulmod(x, y, z);
		assert(q > 0);
	}
	function g(bytes memory data) public pure {
		bytes32 k = keccak256(data);
		assert(k > 0);
		bytes32 s = sha256(data);
		assert(s > 0);
		bytes20 r = ripemd160(data);
		assert(r > 0);
	}
	function h(address b, bytes32 _hash, uint8 _v, bytes32 _r, bytes32 _s) public pure returns (address) {
		address a = ecrecover(_hash, _v, _r, _s);
		assert(a != b);
		return a;
	}
}
// ----
// Warning: (126-139): Assertion violation happens here
// Warning: (171-184): Assertion violation happens here
// Warning: (201-218): Assertion checker does not yet support the type of this variable.
// Warning: (258-262): Internal error: Expression undefined for SMT solver.
// Warning: (258-262): Assertion checker does not yet implement this type.
// Warning: (267-280): Assertion violation happens here
// Warning: (303-307): Internal error: Expression undefined for SMT solver.
// Warning: (303-307): Assertion checker does not yet implement this type.
// Warning: (312-325): Assertion violation happens here
// Warning: (351-355): Internal error: Expression undefined for SMT solver.
// Warning: (351-355): Assertion checker does not yet implement this type.
// Warning: (360-373): Assertion violation happens here
// Warning: (528-542): Assertion violation happens here
