
struct A {
	int B;

	A() = default;
	explicit A(const int b) : B{b} {}
};

int main() {
	A a, b;
	a = {};
	b = a;
}