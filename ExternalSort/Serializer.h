#pragma once
#include "base.h"
#include "FileStorage.h"
#include <typeinfo>
#include <typeindex>
#include <type_traits>
#include <map>

struct Serialize_wrapper
{
    void (*serialize_function)(ostream &stream, const void* data);
    void (*deserialize_function)(istream &stream, void* data);   

    Serialize_wrapper()
    {
        serialize_function = nullptr;
        deserialize_function = nullptr;
    }

    Serialize_wrapper(void(*ser)(ostream &, const void*), void(*des)(istream&, void*))
    {
        serialize_function = ser;
        deserialize_function = des;
    }

    Serialize_wrapper(const Serialize_wrapper &wrapper)
    {
        serialize_function = wrapper.serialize_function;
        deserialize_function = wrapper.deserialize_function;
    }

    ~Serialize_wrapper()
    {
        serialize_function = nullptr;
        deserialize_function = nullptr;
    }
};

class Serializer
{
    map<string, Serialize_wrapper> registered_types;

    template <typename T> string getTypeName()
    {
        return string(typeid(T).name());
    }

public:
    Serializer() { }

    template <typename T> void registerType(void(*ser_func)(ostream&, const void*), void(*des_func)(istream &, void*))
    {
        if (ser_func == nullptr || des_func == nullptr)
            throw exception("Incorrect serialization/deserialization functions");
        Serialize_wrapper wrapper(ser_func, des_func);
        registered_types.insert(make_pair(getTypeName<T>(), wrapper));
    }

    template <typename T> void unregisterType()
    {
        registered_types.erase(registered_types.find(getTypeName()));
    }

    template <typename T> void serialize(IFileStorage &file, const T &data)
    {
        if (registered_types.find(getTypeName<T>()) != registered_types.end())
            registered_types[getTypeName<T>()].serialize_function(file.stream(), (void*)&data);
        else
        {
            file.write(&data, sizeof(T));
        }
    }

    template <typename T> void serialize(IFileStorage &file, const T *data)
    {
        if (registered_types.find(getTypeName<T*>()) != registered_types.end())
            registered_types[getTypeName<T*>()].serialize_function(file.stream(), (void*)data);
        else
        {
            file.write(data, sizeof(T));
        }
    }

    template <typename T> void deserialize(IFileStorage &file, T *data)
    {
        if (registered_types.find(getTypeName<T*>()) != registered_types.end())
            registered_types[getTypeName<T*>()].deserialize_function(file.stream(), (void*)data);
        else
            file.read(data, sizeof(T));
    }

    template <typename T> void deserialize(IFileStorage &file, T &data)
    {
        if (registered_types.find(getTypeName<T>()) != registered_types.end())
            registered_types[getTypeName<T>()].deserialize_function(file.stream(), (void*)&data);
        else
            file.read(&data, sizeof(T));
    }

    template <typename T> void serialize(IFileStorage &file, const vector<T> &data)
    {
        if (registered_types.find(getTypeName<const vector<T> >()) != registered_types.end())
        {
            registered_types[getTypeName<const vector<T> >()].serialize_function(file.stream(), (void*)&data);
        }
        else
        {
            size_t len = data.size();
            file.write(&len, sizeof(size_t));
            for (T elem : data)
            {
                serialize(file, elem);
            }
        }
    }

    template <typename T> void deserialize(IFileStorage &file, vector<T> &data)
    {
        if (registered_types.find(getTypeName<const vector<T> >()) != registered_types.end())
        {
             registered_types[getTypeName<vector<T> >()].deserialize_function(file.stream(), (void*)&data);
        }
        else
        {
            size_t len = 0;
            file.read(&len, sizeof(size_t));
            data.resize(len, T());
            for (size_t i = 0; i < len; i++)
            {
                deserialize(file, data[i]); 
            }
        }
    }

    ~Serializer() 
    {
        registered_types.clear();
    }
};