#include "base.h"
#include "FileStorage.h"
#include "Serializer.h"
#include "ExternalReverse.h"
#include "DataSource.h"
#include "ExternalSort.h"
#include <functional>

#define fori(i, n) for(int i = 0; i < (int)(n); i++)

template <typename T> void print(T x)
{
    cout << "template <typename T> void print(T x)" << endl;
    cout << x << endl;
}
template <typename T> void print(vector<T> v)
{
    cout << "template <typename T> void print(vector<T> v)" << endl;
    fori(i, v.size())
        cout << v[i] << ' ';
    cout << endl;
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
void test()
{
    {
        FileStorage storage("temp.txt", "", IFile::Write);
        int arr[] = {9, 8, 5, 4, 12};
        Serializer::Serialize(storage, arr);

        int *f = new int[8];
        fori(i, 8)
            f[i] = i * (-15);
        Serializer::Serialize(storage, f, 8);

        double eps = 1e-9;
        Serializer::Serialize(storage, eps);

        string s = "abacaba";
        Serializer::Serialize(storage, s);
    }
    {
        FileStorage storage("temp.txt", "", IFile::Read);

        int arr[5];
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
    }
    {
        FormatedFileStorage storage("FormatedFileStorage.txt", "", IFile::Write);
        fori (i, 10)
            Serializer::Serialize(storage, i);

        vector<double> v;
        fori(i, 7)
            v.push_back(rand() + (rand() / (double)RAND_MAX));
        Serializer::Serialize(storage, v);

        string s = "abacaba";

        Serializer::Serialize(storage, s);
    }
    {
        print('\n');
        FormatedFileStorage storage("FormatedFileStorage.txt", "", IFile::Read);
        int f;
        fori (i, 10)
        {
            Serializer::Deserialize(storage, f);
            print(f);
        }

        vector<double> vt;
        Serializer::Deserialize(storage, vt);
        print(vt);

        string st = "";
        Serializer::Deserialize(storage, st);
        print(st);
    }
}

int main()
{
    test();

    {
        ExternalReverse<double> algorithm(
            new StorageInData<double>(new FormatedFileStorage("big_file.txt", "", IFile::Read)), 
            new StorageOutData<double>(new FormatedFileStorage("out_big_file.txt", "", IFile::Write)),
            "",
            5);
        algorithm.externalWork();
    }

    {  
        
        ExternalSort<int, less<int>, equal_to<int> > algorithm(
            new StorageInData<int>(new FormatedFileStorage("big_file_int.txt", "", IFile::Read)),
            new StorageOutData<int>(new FormatedFileStorage("out_big_file_int.txt", "", IFile::Write)),
            "",
            5,
            less<int>(),
            equal_to<int>());
        algorithm.externalWork();
    }

    system("pause");
    return 0;
}