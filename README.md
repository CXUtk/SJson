# SJson
A tiny, header-only C++ JSON library in C++ 17.

## Delivery
There is only one header file for this library, you can locate it at `Ver2/SJson.hpp`. This is the only file you need to include in your project.

You can find the test cases and testing program at `Ver2/test.cpp`

## Features

### Simple construct
You can create a JSON object from the following code:
```c++
auto node = SJson::object({
    {"Name", "CXUtk"},
    {"Age", 21},
    {"Company", "Microsoft"},
    {"Object", {1, 2, 3}},
    {"Test", {1, "3", false}},
});
/*
Output: {
  "Age": 21,
  "Company": "Microsoft",
  "Name": "CXUtk",
  "Object": [
    1,
    2,
    3
  ],
  "Test": [
    1,
    "3",
    false
  ]
}
*/

SJson::JsonNode json;
json["name"]["s"] = "CXUtk";
/*
Output: {
  "name": {
    "s": "CXUtk"
  }
}
*/
```

### Parsing
You can parse JSON string by using
```cpp
auto node = SJson::JsonConvert::Parse(text);
```
This will return the JSON node.

### Serialize
You can serialize a value type by using
```cpp
auto str = SJson::JsonConvert::Serialize(object, SJson::DefaultOption);

// Turn vector to JSON array
std::vector<int> vec;
auto str = SJson::JsonConvert::Serialize(vec, SJson::DefaultOption);

// Turn map to JSON array of key, value pairs
std::map<int, std::string> vec;
auto str = SJson::JsonConvert::Serialize(vec, SJson::DefaultOption);
```
Look at the definition of `struct JsonFormatOption` for more information about formatting.

### Deserialize
You can turn JSON string to a C++ object.
```cpp
auto v1 = SJson::JsonConvert::Deserialize<std::vector<int>>(R"([1, 2, 3, 4])");
/*
v1 = std::vector<int>({1, 2, 3, 4});
*/
```
Serialization and deserialization reqiures manually set the meta information. 
```cpp
class TestObject
{
public:
    int                                     Age;
    float                                   Weight;
    bool                                    Male;
    std::string                             Name;
    std::vector<int>                        List;
    std::map<int, std::vector<std::string>> Mapp;

    // Set fields, capture by field name
    PROPERTIES(TestObject, 
        PROPERTY(Age), 
        PROPERTY(Weight), 
        PROPERTY(Male),
        PROPERTY(Name),
        PROPERTY(List),
        PROPERTY(Mapp),
        PROPERTY(InternalData),
        PROPERTY(EnumValue)
        );
};
```

