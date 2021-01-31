# JSON parser

## Usage

Example JSON:

```json
{
	"string": "Hello world!",
	"object": {
		"floatInObject": 3.1415
	}
}
```

Example c++:

```cpp
#include "JSON.h"

int main() {
    JSON json("pathToJSON.json");
    json.generate();
    std::string string = json.get<std::string>("string");
    float floatInObject = json.get<float>("object.floatInObject");
}
```

## How to use my JSON parser

First of all, include the damn header!<br/>
`#include <JSON.h>`<br/>
Next, create a json object like this: <br/>

```cpp
int main() {
    JSON json("pathToJSON.json");
}
```

Now you can work with json data in c++! The function used to extract data from json is the `json.get<type>("key");` function. In the complete example above, it demonstrates extracting a string and float value from json data. It does so by providing the json.get function with the appropiate type, (for number in json it is float in c++, and string in json is std::string in c++) and then a key. A key is what is used to specify which value you'd like in the json. In this example:<br/>

```json
{
	"person1": {
		"full-name": "John Doe",
		"age": 23,
		"friends": ["Mary", "Frank", "John"]
	},
	"person2": {
		"full-name": "Jane Doe",
		"age": 26,
		"friends": ["Joel", "Marie", "Dorothy"]
	}
}
```

To access person1's full name, the key you need to use is this: `"person1.full-name"`. ie: Every time you want to travel 1 object or array deeper, use a `'.'` then add the name of the next object.
<br/>
<br/>
To access person2's first friend, the function would be: `json.get<std::string>("person2.friends.0")` . The 0 at the end of the key is the index inside of the person2's friends array. The value of that index in the array may be an object, and it is valid to tag on extra `.valueName` tags onto the end of that 0.
<br/>
<br/>
If your json starts off with an array (like the example below) then your key will have a slightly different format. Your key must start with the string: `"(ar)"` . This indicates that the json starts with an array. As a result of this, the next chacter in the key after the first `"(ar)"` must be an index inside the array. Here is an example:

```json
[
	{
		"object in root array": 25.235
	}
]
```

The function required to extract the value `"object in root array"` is as follows: `json.get<float>("(ar)0.object in root array")` .

## Current Known Issues

-   Problems with character codes (things such as use of emojis 🙂 and special characters such as 'π')
