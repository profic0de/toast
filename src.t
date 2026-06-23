# require <test>
#require <stdio>

int g = 0;

int main() {
	# Variables and basic arithmetic
	var f = 3.14
	var a = 3
	var b = 4
	var c = 3 + 4
	var d = 10 / 2

	# Bitwise operations
	var e = 3 >> 4
	var f = 8 << 2
	var g = 12 & 5
	var h = 8 | 2
	var i = 5 ^ 3
	
	# Compound assignment operators
	var x = 5
	x *= 2
	var y = 20
	y /= 4
	
	# String literals with printf
	printf("%d\n", c)
	printf("%d\n", d)
	
	# Function calls with multiple arguments
	printf("Result: %d, Division: %d\n", c, d)
	
	# More complex expressions
	var result = (a + b) * 2
	printf("(a + b) * 2 = %d\n", result)
	
	# Negative numbers
	var neg = -5
	var pos = 10 + neg
	printf("pos = %d\n", pos)
	
	# Compound assignments in action
	printf("x *= 2: %d, y /= 4: %d\n", x, y)
}

obj apple() {
	int init() {
		self.a = 1
	}
}

int add(x, y) {
	return x + y
}

int help(a) {return a}

int multiply(x, y) {
	return x*y
}
