#include "JSON.h"

JSON::JSON(std::string fileName) {
	this->fileName = fileName;
	std::string text, temp;
	std::ifstream file(this->fileName);
	if(!file.is_open()) {
		std::cout << "Problem opening \"" << fileName << "\"" << std::endl;
		exit(-1);
	}
	while(std::getline(file, temp))
		text += temp + "\n";
	file.close();
	this->text = text;
	this->generate();
}

// Visiting Nodes
void JSON::visitNode(AST* node, std::map<std::string, std::any> &map) {
	if(node->ASTtype == "assign") {
		Assign* assignPtr = dynamic_cast<Assign*>(node);
		std::string left = assignPtr->left->value;

		std::string rightType = assignPtr->right->type;
		if(rightType == "string") {
			String* stringPtr = dynamic_cast<String*>(assignPtr->right);
			map.insert(std::pair<std::string, std::any>(left, std::string(stringPtr->value)));
		}
		if(rightType == "number") {
			Number* numberPtr = dynamic_cast<Number*>(assignPtr->right);
			map.insert(std::pair<std::string, std::any>(left, numberPtr->value));
		}
		if(rightType == "boolean") {
			Boolean* booleanPtr = dynamic_cast<Boolean*>(assignPtr->right);
			map.insert(std::pair<std::string, std::any>(left, booleanPtr->value));
		}
		if(rightType == "null") {
			map.insert(std::pair<std::string, std::any>(left, std::string("null")));
		}
		if(rightType == "object") {
			Object* objPtr = dynamic_cast<Object*>(assignPtr->right);
			std::map<std::string, std::any> objMap;
			for(auto elem : objPtr->values)
				this->visitNode(elem, objMap);
			map.insert(std::pair<std::string, std::any>(left, objMap));
		}
		if(rightType == "array") {
			std::vector<std::any> values = this->visitArray(assignPtr->right);
			map.insert(std::pair<std::string, std::any>(left, values));
		}
	}
	if(node->ASTtype == "value") {
		Value* valuePtr = dynamic_cast<Value*>(node);
		if(valuePtr->type == "object") {
			for(auto elem : dynamic_cast<Object*>(node)->values)
				this->visitNode(elem, map);
		}
		if(valuePtr->type == "array") {
			this->rootArray = this->visitArray(valuePtr);
		}
	}
}

std::string JSON::visitString(Value* node) {
	return dynamic_cast<String*>(node)->value;
}

float JSON::visitNumber(Value* node) {
	return dynamic_cast<Number*>(node)->value;
}

bool JSON::visitBoolean(Value* node) {
	return dynamic_cast<Boolean*>(node)->value;
}

std::vector<std::any> JSON::visitArray(Value* node) {
	Array* arrayPtr = dynamic_cast<Array*>(node);
	std::vector<std::any> values;
	for(auto elem : arrayPtr->values) {
		if(elem->type == "string") values.push_back(this->visitString(elem));
		if(elem->type == "number") values.push_back(this->visitNumber(elem));
		if(elem->type == "null") values.push_back(std::string("null"));
		if(elem->type == "boolean") values.push_back(this->visitBoolean(elem));
		if(elem->type == "object") {
			Object* objPtr = dynamic_cast<Object*>(elem);
			std::map<std::string, std::any> objMap;
			for(auto elem : objPtr->values)
				this->visitNode(elem, objMap);
			values.push_back(objMap);
		}
		if(elem->type == "array") values.push_back(this->visitArray(elem));
	}
	return values;
}

// visit nodes and fill in variable table
void JSON::generate() {
	Lexer lexer(this->text);
	Parser parser(lexer);
	this->visitNode(parser.ast(), this->variables);
}

void JSON::modNode(AST* node, std::string value) {
	dynamic_cast<String*>(node)->value = value;
}
void JSON::modNode(AST* node, const char* value) {
	dynamic_cast<String*>(node)->value = std::string(value);
}
void JSON::modNode(AST* node, bool value) {
	dynamic_cast<Boolean*>(node)->value = value;
}
void JSON::modNode(AST* node, float value) {
	dynamic_cast<Number*>(node)->value = value;
}

void JSON::rewriteJSON(AST* node) {
	/*
		This function acts like a parser, but instead of generating an AST,
		it generates a string of json
	*/

	// check if the root type is object or array
	if(node->ASTtype == "value" && dynamic_cast<Value*>(node)->type == "object") {
		this->text += "{";
		for(int i = 0; i < dynamic_cast<Object*>(node)->values.size(); i++) {
			this->rewriteJSON(dynamic_cast<Object*>(node)->values[i]);
			if(i < dynamic_cast<Object*>(node)->values.size()-1) this->text += ",";
		}
		this->text += "}";
	} else if(node->ASTtype == "value" && dynamic_cast<Value*>(node)->type == "array") {
		this->text += "[";
		for(int i = 0; i < dynamic_cast<Array*>(node)->values.size(); i++) {
			this->rewriteJSON(dynamic_cast<Array*>(node)->values[i]);
			if(i < dynamic_cast<Array*>(node)->values.size()-1) this->text += ",";
		}
		this->text += "]";
	} else if(node->ASTtype == "assign") {
		auto assignPtr = dynamic_cast<Assign*>(node);
		this->text += "\"" + assignPtr->left->value + "\":";
		this->writeVal(assignPtr->right->type, assignPtr->right);
	} else if(node->ASTtype == "value") {
		auto valPtr = dynamic_cast<Value*>(node);
		std::string valType = valPtr->type;
		this->writeVal(valPtr->type, valPtr);
	}
}

void JSON::writeVal(std::string type, Value* value) {
	if(type == "object" || type == "array")
		this->rewriteJSON(value);
	else if(type == "string" || type == "null")
		this->text += "\"" + dynamic_cast<String*>(value)->value + "\"";
	else if(type == "number")
		this->text += std::to_string(dynamic_cast<Number*>(value)->value);
	else if(type == "boolean")
		this->text += std::to_string(dynamic_cast<Boolean*>(value)->value);
	else
		std::runtime_error("Something went wrong in the JSON::rewriteJSON() function");
}