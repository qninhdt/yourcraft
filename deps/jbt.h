#ifndef JBT_H
#define JBT_H

#include <string>
#include <fstream>
#include <cstring>
#include <map>
#include <iostream>

namespace jbt {

//-----------------------------------------------------------------------------
//|                                 INTERFACE                                 | 
//-----------------------------------------------------------------------------

// forward declaration

class BaseValue;
class BaseTag;
template<typename T> class Value;
template<typename T> class Tag;
class Serializer;

// OBJECT
//-----------------------------------------------------------------------------

class Object {

public:
    
    Object();

    Object(const Object& object);

    template<typename T>
    void set(const std::string& key, const T& value);

    void setBaseValue(const std::string& key, BaseValue* value);

    BaseValue* getBaseValue(const std::string& key) ;

    template<typename T>
    Value<T>* getValue(const std::string& key);

    template<typename T>
    void setValue(const std::string& key, Value<T>* value);

    template<typename T>
    T get(const std::string& key);

    BaseTag* findBaseTag(const std::string& key);

    template<typename T>
    Tag<T>* findTag(const std::string& key);

    void insertBaseTag(BaseTag* tag);

    template<typename T>
    void insertTag(Tag<T>* tag);

    void erase(const std::string& key);

    bool hasTag(const std::string& key) const;

    const std::map<std::string, BaseValue*>* getTagMap() const;

private:
    std::map<std::string, BaseValue*> tags;
};

// DATA TYPE
//-----------------------------------------------------------------------------

enum class DataType {
    NONE = 0,
    STRING,
    OBJECT,
    BYTE,
    UBYTE,
    SHORT,
    USHORT,
    INT,
    UINT,
    LONG,
    ULONG,
};

// some datatype utils
// ----------------------------------------------------------------------------

template<typename T> 
DataType GetDataType() { return DataType::NONE; }

template<> DataType GetDataType< std::string   >() { return DataType::STRING; };
template<> DataType GetDataType< std::int8_t   >() { return DataType::BYTE;   };
template<> DataType GetDataType< std::uint8_t  >() { return DataType::UBYTE;  };
template<> DataType GetDataType< std::int16_t  >() { return DataType::SHORT;  };
template<> DataType GetDataType< std::uint16_t >() { return DataType::USHORT; };
template<> DataType GetDataType< std::int32_t  >() { return DataType::INT;    };
template<> DataType GetDataType< std::uint32_t >() { return DataType::UINT;   };
template<> DataType GetDataType< std::int64_t  >() { return DataType::LONG;   };
template<> DataType GetDataType< std::uint64_t >() { return DataType::ULONG;  };
template<> DataType GetDataType< Object >() { return DataType::OBJECT;  };

BaseValue* CreateValue(DataType dataType);

BaseTag* CreateTag(const std::string& key, DataType dataType);

// BASE VALUE
//-----------------------------------------------------------------------------

class BaseValue {
public:
    BaseValue(const DataType& dataType);

    DataType getDataType() const;

    void* getBaseData() const;

    void setBaseData(void* data);

    virtual void write(Serializer& serializer) const = 0;
    virtual void read(Serializer& serializer) = 0;

protected:
    friend class Serializer;

    DataType dataType;
    void* data;
};

// VALUE
//-----------------------------------------------------------------------------

template<typename T>
class Value: public BaseValue {
public:

    Value();
    Value(const T& value);

    Value(const Value& value);

    ~Value();

    T* getData() const;
    void setData(T* data);
    T get() const;
    void set(const T& value);

    void write(Serializer& serializer) const;
    void read(Serializer& serializer);
};


// BASE TAG
//-----------------------------------------------------------------------------

class BaseTag {
public:

    BaseTag();
    BaseTag(const std::string& key);

    std::string getKey() const;

    DataType getDataType() const;

    BaseValue* getBaseValue() const;

    void setBaseValue(BaseValue* value);
    
protected:
    std::string key;
    BaseValue* value;

private:
    friend class Serializer;

    virtual void writeValue(Serializer& serializer) const = 0;
    virtual void readValue(Serializer& serializer) = 0;

};

// TAG
//-----------------------------------------------------------------------------

template<typename T>
class Tag: public BaseTag {
public:

    Tag(const std::string& key);
    Tag(const std::string& key, const T& value);

    ~Tag();

    Value<T>* getValue() const;
    T get() const;
    void set(const T data);

private:
    friend class Serializer;

    void writeValue(Serializer& serializer) const;
    void readValue(Serializer& serializer);
};

// SERIALIZER
//-----------------------------------------------------------------------------

class Serializer {
public:

    Serializer(std::istream& input, std::ostream& output);

    void write(const int8_t& value) const;

    void write(const uint8_t& value) const;

    void write(const int16_t& value) const;

    void write(const uint16_t& value) const;

    void write(const int32_t& value) const;

    void write(const uint32_t& value) const;

    void write(const int64_t& value) const;

    void write(const uint64_t& value) const;

    void write(const char* const& value) const;
    
    template<size_t N>
    void write(const char (&value)[N]) const;

    void write(const std::string& value) const;

    void write(const DataType& dataType) const;

    void write(BaseTag* value) const;

    void write(const Object& value) const;

    template<typename T>
    void read(T& value);

    void read(std::string& value);

    void read(DataType& value);

    void read(BaseTag*& value);

    void read(Object& value);

private:
    std::istream& reader;
    std::ostream& writer;
};

//-----------------------------------------------------------------------------
//|                              IMPLEMENTATION                               |
//-----------------------------------------------------------------------------

// some datatype utils
//-----------------------------------------------------------------------------
BaseValue* CreateValue(DataType dataType) {
    BaseValue* value;
    switch (dataType) {
        case DataType::BYTE   : value = static_cast<BaseValue*>(new Value<int8_t   >()); break;
        case DataType::UBYTE  : value = static_cast<BaseValue*>(new Value<uint8_t  >()); break;
        case DataType::SHORT  : value = static_cast<BaseValue*>(new Value<int16_t  >()); break;
        case DataType::USHORT : value = static_cast<BaseValue*>(new Value<uint16_t >()); break;
        case DataType::INT    : value = static_cast<BaseValue*>(new Value<int32_t  >()); break;
        case DataType::UINT   : value = static_cast<BaseValue*>(new Value<uint32_t >()); break;
        case DataType::LONG   : value = static_cast<BaseValue*>(new Value<int64_t  >()); break;
        case DataType::ULONG  : value = static_cast<BaseValue*>(new Value<uint64_t >()); break;
        case DataType::STRING : value = static_cast<BaseValue*>(new Value<std::string>()); break;
        case DataType::OBJECT : value = static_cast<BaseValue*>(new Value<Object>  ()); break;
        default: return nullptr;
    }
    return value;
}

BaseTag* CreateTag(const std::string& key, DataType dataType) {
    BaseTag* value;
    switch (dataType) {
        case DataType::BYTE   : value = static_cast<BaseTag*>(new Tag<int8_t   >(key)); break;
        case DataType::UBYTE  : value = static_cast<BaseTag*>(new Tag<uint8_t  >(key)); break;
        case DataType::SHORT  : value = static_cast<BaseTag*>(new Tag<int16_t  >(key)); break;
        case DataType::USHORT : value = static_cast<BaseTag*>(new Tag<uint16_t >(key)); break;
        case DataType::INT    : value = static_cast<BaseTag*>(new Tag<int32_t  >(key)); break;
        case DataType::UINT   : value = static_cast<BaseTag*>(new Tag<uint32_t >(key)); break;
        case DataType::LONG   : value = static_cast<BaseTag*>(new Tag<int64_t  >(key)); break;
        case DataType::ULONG  : value = static_cast<BaseTag*>(new Tag<uint64_t >(key)); break;
        case DataType::STRING : value = static_cast<BaseTag*>(new Tag<std::string>(key)); break;
        case DataType::OBJECT : value = static_cast<BaseTag*>(new Tag<Object>  (key)); break;
        default: return nullptr;
    }
    return value;
}


// OBJECT
//-----------------------------------------------------------------------------

Object::Object() {}

Object::Object(const Object& object) {
    this->tags = object.tags;   
}

void Object::setBaseValue(const std::string& key, BaseValue* value) {
    this->tags[key] = value;
}

template<typename T>
void Object::set(const std::string& key, const T& value) {
    BaseValue* baseValue = new Value<T>(value);
    this->tags[key] = baseValue;
}

BaseValue* Object::getBaseValue(const std::string& key) {
    return this->tags[key];
}

template<typename T>
Value<T>* Object::getValue(const std::string& key) {
    return static_cast<Value<T>*>(getBaseValue(key));
}

template<typename T>
void Object::setValue(const std::string& key, Value<T>* value) {
    setBaseValue(key, static_cast<BaseValue*>(value));
}

template<typename T>
T Object::get(const std::string& key) {
    return getValue<T>(key)->get();
}

BaseTag* Object::findBaseTag(const std::string& key) {
    auto iter = this->tags.find(key);
    if (iter == this->tags.end()) return {};

    auto value = iter->second;
    auto tag = CreateTag(key, value->getDataType());
    tag->setBaseValue(value);

    return tag;
}

template<typename T>
Tag<T>* Object::findTag(const std::string& key) {
    return static_cast<Tag<T>*>(findBaseTag(key));
}

void Object::insertBaseTag(BaseTag* tag) {
    this->tags[tag->getKey()] = tag->getBaseValue();
}

template<typename T>
void Object::insertTag(Tag<T>* tag) {
    insertBaseTag(static_cast<BaseTag*>(tag));
}

void Object::erase(const std::string& key) {
    this->tags.erase(key);
}

bool Object::hasTag(const std::string& key) const {
    return this->tags.find(key) != this->tags.end();
}

const std::map<std::string, BaseValue*>* Object::getTagMap() const {
    return &this->tags;    
}


// BASE VALUE
//-----------------------------------------------------------------------------

BaseValue::BaseValue(const DataType& dataType):
    dataType(dataType) {}

inline DataType BaseValue::getDataType() const {
    return dataType;
}

inline void* BaseValue::getBaseData() const {
    return data;
}

void BaseValue::setBaseData(void* data) {
    this->data = data;
}

// VALUE
//-----------------------------------------------------------------------------

template<typename T>
Value<T>::Value():
    BaseValue(GetDataType<T>()) {
    this->data = new T;
}

template<typename T>
Value<T>::Value(const T& value):
    BaseValue(GetDataType<T>()) {
    
    this->data = new T(value);
}

template<typename T>
Value<T>::Value(const Value<T>& value) {
    this->data = new T(value.get());
    this->dataType = value.dataType;
}

template<typename T>
Value<T>::~Value() {
    delete getData();
}

template<typename T>
inline T Value<T>::get() const { 
    return *getData(); 
}

template<typename T>
void Value<T>::set(const T& value) {
    delete getData();
    this->data = new T(value);
}

template<typename T>
T* Value<T>::getData() const {
    return static_cast<T*>(data);
}

template<typename T>
void Value<T>::setData(T* data) {
    delete getData();
    this->data = static_cast<void*>(data);
}

template<typename T>
inline void Value<T>::write(Serializer& serializer) const {
    serializer.write(*getData());
}

template<typename T>
inline void Value<T>::read(Serializer& serializer) {
    serializer.read(*getData());
}

// BASE TAG
//-----------------------------------------------------------------------------

BaseTag::BaseTag() {}

BaseTag::BaseTag(const std::string& key):
    key(key) {};

inline std::string BaseTag::getKey() const {
    return this->key;
}

DataType BaseTag::getDataType() const {
    return this->value->getDataType();
}

BaseValue* BaseTag::getBaseValue() const {
    return value;
}

void BaseTag::setBaseValue(BaseValue* value) {
    this->value = value;
}

// TAG
//-----------------------------------------------------------------------------

template<typename T>
Tag<T>::Tag(const std::string& key):
    BaseTag(key) {};

template<typename T>
Tag<T>::Tag(const std::string& key, const T& value):
    BaseTag(key) {
    this->value = static_cast<BaseValue*>(new Value<T>(value));
}

template<typename T>
Tag<T>::~Tag() {
    delete getValue();
}

template<typename T>
inline T Tag<T>::get() const {
    return *this->getValue()->getData();
}

template<typename T>
void Tag<T>::set(const T data) {
    getValue()->set(data);
}

template<typename T>
inline Value<T>* Tag<T>::getValue() const {
    return static_cast<Value<T>*>(getBaseValue());
}


template<typename T>
inline void Tag<T>::writeValue(Serializer& serializer) const {
    this->value->write(serializer);
}

template<typename T>
inline void Tag<T>::readValue(Serializer& serializer) {
    this->value->read(serializer);
}

// SERIALIZER
//-----------------------------------------------------------------------------

Serializer::Serializer(std::istream& reader, std::ostream& writer):
    reader(reader),
    writer(writer) {}

void Serializer::write(const int8_t& value) const {
    this->writer.write((char*) &value, sizeof(int8_t));
}

void Serializer::write(const uint8_t& value) const {
    this->writer.write((char*) &value, sizeof(uint8_t));
}

void Serializer::write(const int16_t& value) const {
    this->writer.write((char*) &value, sizeof(int16_t));
}

void Serializer::write(const uint16_t& value) const {
    this->writer.write((char*) &value, sizeof(uint16_t));
}

void Serializer::write(const int32_t& value) const {
    this->writer.write((char*) &value, sizeof(int32_t));
}

void Serializer::write(const uint32_t& value) const {
    this->writer.write((char*) &value, sizeof(uint32_t));
}

void Serializer::write(const int64_t& value) const {
    this->writer.write((char*) &value, sizeof(int64_t));
}

void Serializer::write(const uint64_t& value) const {
    this->writer.write((char*) &value, sizeof(uint64_t));
}

void Serializer::write(const char* const& value) const {
    uint16_t size = strlen(value);
    write(size);
    this->writer.write(value, size);
}

template<size_t N>
void Serializer::write(const char (&value)[N]) const {
    uint16_t size = strlen(&value[0]);
    write(size);
    this->writer.write(&value[0], size);
}

void Serializer::write(const std::string& value) const {
    uint16_t size = value.size();
    write(size);
    this->writer.write(value.data(), size);
}

void Serializer::write(const DataType& dataType) const {
    uint8_t id = static_cast<uint8_t>(dataType);
    write(id);
}

void Serializer::write(BaseTag* value) const {
    write(value->key);
    write(value->getDataType());
    value->writeValue(const_cast<Serializer&>(*this));
}

void Serializer::write(const Object& object) const {
    auto tags = object.getTagMap();

    write(static_cast<uint8_t>(tags->size()));
    for (auto [key, value]: *tags) {
        auto tag = CreateTag(key, value->dataType);
        tag->setBaseValue(value);
        write(tag);
    }
}

template<typename T>
void Serializer::read(T& value) {
    this->reader.read((char*) &value, sizeof(T));
}

void Serializer::read(std::string& value) {
    uint16_t size;
    read(size);
    value.resize(size);
    this->reader.read(value.data(), size);
}

void Serializer::read(DataType& value) {
    uint8_t id;
    read(id);
    value = static_cast<DataType>(id);
}

void Serializer::read(BaseTag*& tag) {
    std::string key;
    DataType dataType;

    read(key);
    read(dataType);

    BaseValue* value = CreateValue(dataType);
    value->read(*this);

    tag = CreateTag(key, dataType);
    tag->setBaseValue(value);
}

void Serializer::read(Object& value) {
    uint8_t size;
    read(size);

    for (int i=0;i<size;++i) {
        BaseTag* tag;
        read(tag);

        value.insertBaseTag(tag);
    }
}

} // namespace jbt

#endif // JBT_H