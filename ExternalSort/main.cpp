#include "base.h"
#include "FileStorage.h"
#include "Serializer.h"
#include "ExternalReverse.h"
#include "DataSource.h"
#include "ExternalSort.h"
#include <functional>
#include <ctime>
#include <vld.h>


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

void string_serialization(ostream &stream, const void * data) 
{
    string *str = (string*)data;
    int len = str->length();
    stream.write((char*)(&len), sizeof(size_t)); 
    stream.write(str->c_str(), len);
}

void string_deserialization(istream &stream, void * data) 
{
    string *str = (string*)data;
    size_t len = 0;
    stream.read((char*)(&len), sizeof(size_t));
    char* buf = new char[len];
    stream.read(buf, len);
    *str = string(buf, len);
    delete buf;
}


void test()
{
    {
        Serializer serializer;
        serializer.registerType<string>(string_serialization, string_deserialization);
        
        FileStorage storage("binaryStorage.txt", "", IFile::Write);
        int arr[] = {9, 8, 5, 4, 12};
        fori(i, 5)
            serializer.serialize(storage, arr[i]);

        
        int *f = new int[8];
        fori(i, 8)
            f[i] = i * (-15);
        fori(i, 8)
            serializer.serialize(storage, f[i]);

        double eps = 1e-9;
        serializer.serialize(storage, eps);

        string s = "abacaba";
        serializer.serialize(storage, s);

        delete f;
    }
    {
        Serializer serializer;
        serializer.registerType<string>(string_serialization, string_deserialization);

        FileStorage storage("binaryStorage.txt", "", IFile::Read);

        int arr[5];
        fori(i, 5)
            serializer.deserialize(storage, arr[i]);
        print(arr);


        int *f = new int[8];
        fori(i, 8)
            serializer.deserialize(storage, f[i]);
        print(f, 8);

        double eps;
        serializer.deserialize(storage, eps);
        print(eps);

        string t;
        serializer.deserialize(storage, t);
        print(t);

        delete f;
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
    //Let's do a magic! Don't touch :)
    _setmaxstdio(2048);

    test();

    debugCode(cerr << endl << endl << endl;);

    test_sort();

    debugCode(cerr << endl << endl << endl;);

    test_reverse();

    debugCode(dbg("After tests"));

    system("pause");
    return 0;
}