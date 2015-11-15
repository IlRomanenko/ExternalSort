#pragma once
#include "base.h"
#include "FileStorage.h"
#include <typeinfo>
#include <typeindex>
#include <type_traits>
#include <map>

template <typename T> class Serialize_wrapper
{
public:

    void (*serialize_function)(ostream &stream, const T data);
    void (*deserialize_function)(istream &stream, T& data);   
    
    Serialize_wrapper()
    {
        serialize_function = nullptr;
        deserialize_function = nullptr;
    }

    Serialize_wrapper(void(*ser)(ostream &, const T), void(*des)(istream&, T&))
    {
        serialize_function = ser;
        deserialize_function = des;
    }

    Serialize_wrapper(const Serialize_wrapper<T> &wrapper)
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
    map<string, void*> registered_types;

    template <typename T> string getTypeName()
    {
        return string(typeid(T).name());
    }

    template <typename T> Serialize_wrapper<T> getSerializeWrapper(string type_name)
    {
        return *(Serialize_wrapper<T>*)registered_types[type_name];
    }

public:
    Serializer() { }

    template <typename T> void registerType(void(*ser_func)(ostream&, const T), void(*des_func)(istream &, T&))
    {
        if (ser_func == nullptr || des_func == nullptr)
            throw exception("Incorrect serialization/deserialization functions");
        registered_types.insert(make_pair(getTypeName<T>(), new Serialize_wrapper<T>(ser_func, des_func)));
    }

    template <typename T> void unregisterType()
    {
        auto it = registered_types.find(getTypeName<T>());
        if (it == registered_types.end())
            throw exception("Can't unregister type.");
        delete *it;
        registered_types.erase(registered_types.find(getTypeName()));
    }

    template <typename T> void serialize(IFileStorage &file, const T &data)
    {
        if (registered_types.find(getTypeName<T>()) != registered_types.end())
            getSerializeWrapper<T>(getTypeName<T>()).serialize_function(file.stream(), data);
        else
        {
            file.write(&data, sizeof(T));
        }
    }

    template <typename T> void serialize(IFileStorage &file, const T *data)
    {
        if (registered_types.find(getTypeName<T*>()) != registered_types.end())
            getSerializeWrapper<T>(getTypeName<T>()).serialize_function(file.stream(), data);
        else
        {
            file.write(data, sizeof(T));
        }
    }

    template <typename T> void deserialize(IFileStorage &file, T *data)
    {
        if (registered_types.find(getTypeName<T*>()) != registered_types.end())
            getSerializeWrapper<T>(getTypeName<T>()).deserialize_function(file.stream(), data);
        else
            file.read(data, sizeof(T));
    }

    template <typename T> void deserialize(IFileStorage &file, T &data)
    {
        if (registered_types.find(getTypeName<T>()) != registered_types.end())
            getSerializeWrapper<T>(getTypeName<T>()).deserialize_function(file.stream(), data);
        else
            file.read(&data, sizeof(T));
    }

    template <typename T> void serialize(IFileStorage &file, const vector<T> &data)
    {
        if (registered_types.find(getTypeName<const vector<T> >()) != registered_types.end())
        {
            getSerializeWrapper<const vector<T> >(getTypeName<const vector<T> >()).serialize_function(file.stream(), data);
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
             getSerializeWrapper<const vector<T> >(getTypeName<const vector<T> >()).deserialize_function(file.stream(), data);
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
        for (auto it = registered_types.begin(); it != registered_types.end(); it++)
            delete it->second;
        registered_types.clear();
    }
};