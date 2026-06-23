# require <test>
#require <stdio>

var g = 0;

func main() {
	# Variables and basic arithmetic
	var f = 3.14
	let a = 3
	let b = 4
	let c = 3 + 4
	let d = 10 / 2

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
	let result = (a + b) * 2
	printf("(a + b) * 2 = %d\n", result)
	
	# Negative numbers
	let neg = -5
	let pos = 10 + neg
	printf("pos = %d\n", pos)
	
	# Compound assignments in action
	printf("x *= 2: %d, y /= 4: %d\n", x, y)
}

obj apple() {
	func init() {
		self.a = 1
	}
}

func add(x, y) {
	return x + y
}

func help(a) {return a}

func multiply(x, y) {
	return x*y
}
