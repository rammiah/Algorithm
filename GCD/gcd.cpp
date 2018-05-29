int gcd(int a, int b) {
	while (b != 0) {
		// a, b = b, a % b
		int t = a % b;
		a = b;
		b = t;
	}
	return a;
}