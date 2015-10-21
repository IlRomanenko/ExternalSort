#include "base.h"
#include "FileStorage.h"
#include "Serializer.h"
#include "ExternalReverse.h"
#include "DataSource.h"
#include "ExternalSort.h"
#include <functional>
#include <ctime>



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
        FileStorage storage("binaryStorage.txt", "", IFile::Write);
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
        FileStorage storage("binaryStorage.txt", "", IFile::Read);

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

const string input_sort =       "big_input_sort.txt";
const string output_sort =      "big_output_sort.txt";
const string input_reverse =    "big_input_reverse.txt";
const string output_reverse =   "big_output_reverse.txt";
const string directory =        "Temp\\";

void writeFile(string name, int size)
{
    FormatedFileStorage storage(name, directory,  IFile::Write);
    srand((uint)time(NULL));
    for (int i = 0; i < size; i++)
    {
        storage << rand() << ' ';
    }
}

bool check()
{
    StorageInData<int> getter(make_unique(new FormatedFileStorage(output_sort, directory, IFile::Read)));
    int last = -1, cur;
    while (!getter.isEmpty())
    {
        cur = getter.getNext();
        if (last != -1 && cur < last)
        {
            cout << "ERROR" << endl;
            return false;
        }
        swap(cur, last);
    }
    return true;
}

void test_sort()
{
    writeFile(input_sort, 500 * 1000);
    {
        auto dataSourcePtr = 
            shared_ptr<IDataSource<int>>(new StorageInData<int>(
            make_unique(new FormatedFileStorage(input_sort, directory, IFile::Read))));
        auto dataOutSourcePtr = 
            shared_ptr<IDataOutSource<int>>(new StorageOutData<int>(
            make_unique(new FormatedFileStorage(output_sort, directory, IFile::Write))));

        ExternalSort<int> algorithm(
            dataSourcePtr, 
            dataOutSourcePtr,
            directory,
            1 * 1000);
        algorithm.externalWork();    

        debugCode(dbg("AfterSort"));
    }
    if (check())
        cout << "OK" << endl;
}

void test_reverse()
{
    writeFile(input_reverse, 1000);

    {
        auto dataSourcePtr = 
            shared_ptr<IDataSource<int>>(new StorageInData<int>(
            make_unique(new FormatedFileStorage(input_reverse, directory, IFile::Read))));
        auto dataOutSourcePtr = 
            shared_ptr<IDataOutSource<int>>(new StorageOutData<int>(
            make_unique(new FormatedFileStorage(output_reverse, directory, IFile::Write))));

        ExternalReverse<int> algorithm(
            dataSourcePtr, 
            dataOutSourcePtr,
            directory,
            80);
        algorithm.externalWork();    

        debugCode(dbg("AfterReverse"));
    }

}

int main()
{

    test();

    debugCode(cerr << endl << endl << endl;);

    test_sort();

    debugCode(cerr << endl << endl << endl;);

    test_reverse();

    debugCode(dbg("After tests"));

    system("pause");
    return 0;
}