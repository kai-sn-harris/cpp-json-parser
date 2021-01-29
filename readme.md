# JSON parser

This is a JSON parser for c++. While this is not the most elegant solution, I wanted to create a JSON parser, so I did. Not sure why anyone would use this parser over other <em>better</em> options but here it is nonetheless.

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

Then, you must generate the json to work with it in c++. Do this like so:<br/>

```cpp
int main() {
    JSON json("pathToJSON.json");
    json.generate();
}
```

Now you can work with json data in c++! The most important function to get started is the `json.get<type>("key");` function. In the complete example above, it demonstrates retrieving a string and float value from json data. It does so by providing the json.get function with the appropiate type, (for number in json it is float in c++, and string in json is std::string) and then a key. A key is what is used to specify which value you'd like in the json. In this example:<br/>

```json
{
	"person1": {
		"full-name": "John Doe",
		"age": 23
	},
	"person2": {
		"full-name": "Jane Doe",
		"age": 26
	}
}
```

To access person1's full name, the key you need to use is this: `"person1.full-name"`. ie: Every time you want to travel 1 object deeper, use a `'.'` then add the name of the next object. You must also provide a starting point for the function to read from. The full function would be: `json.get<std::string>("person1.full-name", json.variables)` . json.variables is of the type `std::map<std::string, std::any>` and is where all the json data is stored.
<br/>
<br/>
To access person2's age, the function would be: `json.get<float>("person2.age", json.variables)` .

## Current Limitations

-   Cannot read json that starts with an array, eg:

```json
[
	{
		"this will": "not work"
	}
]
```

-   Reading from arrays inside objects takes more than I'd like to type out. This will be improved in the future, but it is still possible to read from arrays inside objects.
