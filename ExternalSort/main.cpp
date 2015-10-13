#include "base.h"
#include "FileStorage.h"
#include "Serializer.h"
#include <sstream>


#define fori(i, n) for(int i = 0; i < (int)(n); i++)

template <typename T> void print(T x)
{
	cout << "template <typename T> void print(T x)" << endl;
	cout << x << endl;
}
template <typename T> void print(T *x, int k)
{
	cout << "template <typename T> void print(T *x, int k)" << endl;
	fori(i, k)
		cout << x[i] << ' ';
	cout << endl;
}
template <typename T, size_t N> void print(T (&x)[N])
{
	cout << "template <typename T, size_t N> void print(T (&x)[N])" << endl;
	fori(i, N)
		cout << x[i] << ' ';
	cout << endl;
}

class A
{
public:
	vector<int> p;
	A()
	{
		fori(i, 10)
			p.push_back(i);
	}
	A(int _p, int _d) 
	{
		fori(i, _p)
			p.push_back(_d * 12);
		p.push_back(_p);
	}
};

ostream& operator<<(ostream &stream, A& obj)
{
	stream << "A" << endl;
	for (int val : obj.p)
		stream << val << ' ' ;
	stream << endl;
	return stream;
}

int main()
{
	{
		FileStorage storage("temp.txt", "", true, false);
		int arr[] = {9, 8, 5, 4};
		Serializer::Serialize(storage, arr);
		
		int *f = new int[8];
		fori(i, 8)
			f[i] = i * (-15);
		Serializer::Serialize(storage, f, 8);

		double eps = 1e-9;
		Serializer::Serialize(storage, eps);
		
		string s = "abacaba";
		Serializer::Serialize(storage, s);

		//Something strange...
		A **arA = new A*[10];
		fori(i, 10)
			arA[i] = new A(i, i);
		Serializer::Serialize(storage, arA, 10);
	}
	{
		FileStorage storage("temp.txt", "", true, true);
		
		int arr[4];
		Serializer::Deserialize(storage, arr);
		print(arr);
		
		
		int *f = nullptr;
		uint count = Serializer::Deserialize(storage, f);
		print(f, count);

		double eps;
		Serializer::Deserialize(storage, eps);
		print(eps);
		
		string t;
		Serializer::Deserialize(storage, t);
		print(t);

		//WaT?! Why is it working?
		A **arA = nullptr;
		Serializer::Deserialize(storage, arA);
		fori(i, 10)
			print(*arA[i]);
	}
	system("pause");
	return 0;
}