class B;

class A {
	B* b;
public:
	static void xd();
};
//class C {};

class B {
	A* a;
	void xd()
	{
		a->xd();
	}
	//C c;
};


//int main() { return 0; }