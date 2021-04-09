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
void JSON::visitNode(std::shared_ptr<AST> node, std::map<std::string, std::any> &map) {
	if(node->ASTtype == "assign") {
		auto assignPtr = std::dynamic_pointer_cast<Assign>(node);
		std::string left = assignPtr->left->value;

		std::string rightType = assignPtr->right->type;
		if(rightType == "string") {
			auto stringPtr = std::dynamic_pointer_cast<String>(assignPtr->right);
			map.insert(std::pair<std::string, std::any>(left, std::string(stringPtr->value)));
		}
		if(rightType == "number") {
			auto numberPtr = std::dynamic_pointer_cast<Number>(assignPtr->right);
			map.insert(std::pair<std::string, std::any>(left, numberPtr->value));
		}
		if(rightType == "boolean") {
			auto booleanPtr = std::dynamic_pointer_cast<Boolean>(assignPtr->right);
			map.insert(std::pair<std::string, std::any>(left, booleanPtr->value));
		}
		if(rightType == "null") {
			map.insert(std::pair<std::string, std::any>(left, std::string("null")));
		}
		if(rightType == "object") {
			auto objPtr = std::dynamic_pointer_cast<Object>(assignPtr->right);
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
		auto valuePtr = std::dynamic_pointer_cast<Value>(node);
		if(valuePtr->type == "object") {
			for(auto elem : std::dynamic_pointer_cast<Object>(node)->values)
				this->visitNode(elem, map);
		}
		if(valuePtr->type == "array") {
			this->rootArray = this->visitArray(valuePtr);
		}
	}
}

std::string JSON::visitString(std::shared_ptr<Value> node) {
	return std::dynamic_pointer_cast<String>(node)->value;
}

float JSON::visitNumber(std::shared_ptr<Value> node) {
	return std::dynamic_pointer_cast<Number>(node)->value;
}

bool JSON::visitBoolean(std::shared_ptr<Value> node) {
	return std::dynamic_pointer_cast<Boolean>(node)->value;
}

std::vector<std::any> JSON::visitArray(std::shared_ptr<Value> node) {
	auto arrayPtr = std::dynamic_pointer_cast<Array>(node);
	std::vector<std::any> values;
	for(auto elem : arrayPtr->values) {
		if(elem->type == "string") values.push_back(this->visitString(elem));
		if(elem->type == "number") values.push_back(this->visitNumber(elem));
		if(elem->type == "null") values.push_back(std::string("null"));
		if(elem->type == "boolean") values.push_back(this->visitBoolean(elem));
		if(elem->type == "object") {
			auto objPtr = std::dynamic_pointer_cast<Object>(elem);
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

std::shared_ptr<Value> JSON::createNode(std::string value) {
	auto node = std::make_shared<String>(String(value));
	return std::dynamic_pointer_cast<Value>(node);
}
std::shared_ptr<Value> JSON::createNode(const char* value) {
	auto node = std::make_shared<String>(String(value));
	return std::dynamic_pointer_cast<Value>(node);
}
std::shared_ptr<Value> JSON::createNode(bool value) {
	auto node = std::make_shared<Boolean>(Boolean(value));
	return std::dynamic_pointer_cast<Value>(node);
}
std::shared_ptr<Value> JSON::createNode(float value) {
	auto node = std::make_shared<Number>(Number(value));
	return std::dynamic_pointer_cast<Value>(node);
}
std::shared_ptr<Value> JSON::createNode(long long int value) {
	auto node = std::make_shared<Null>(Null());
	return std::dynamic_pointer_cast<Value>(node);
}

void JSON::rewriteJSON(std::shared_ptr<AST> node) {
	/*
		This function acts like a parser, but instead of generating an AST,
		it generates a string of json
	*/

	// check if the root type is object or array
	if(node->ASTtype == "value" && std::dynamic_pointer_cast<Value>(node)->type == "object") {
		this->text += "{ ";
		for(int i = 0; i < std::dynamic_pointer_cast<Object>(node)->values.size(); i++) {
			this->rewriteJSON(std::dynamic_pointer_cast<Object>(node)->values[i]);
			if(i < std::dynamic_pointer_cast<Object>(node)->values.size()-1) this->text += ", ";
		}
		this->text += " }";
	} else if(node->ASTtype == "value" && std::dynamic_pointer_cast<Value>(node)->type == "array") {
		this->text += "[ ";
		for(int i = 0; i < std::dynamic_pointer_cast<Array>(node)->values.size(); i++) {
			this->rewriteJSON(std::dynamic_pointer_cast<Array>(node)->values[i]);
			if(i < std::dynamic_pointer_cast<Array>(node)->values.size()-1) this->text += ", ";
		}
		this->text += " ]";
	} else if(node->ASTtype == "assign") {
		auto assignPtr = std::dynamic_pointer_cast<Assign>(node);
		this->text += "\"" + assignPtr->left->value + "\": ";
		this->writeVal(assignPtr->right->type, assignPtr->right);
	} else if(node->ASTtype == "value") {
		auto valPtr = std::dynamic_pointer_cast<Value>(node);
		this->writeVal(valPtr->type, valPtr);
	}
}

void JSON::writeVal(std::string type, std::shared_ptr<Value> value) {
	if(type == "object" || type == "array")
		this->rewriteJSON(value);
	else if(type == "string")
		this->text += "\"" + std::dynamic_pointer_cast<String>(value)->value + "\"";
	else if(type == "null")
		this->text += "null";
	else if(type == "number")
		this->text += std::to_string(std::dynamic_pointer_cast<Number>(value)->value);
	else if(type == "boolean")
		this->text += std::dynamic_pointer_cast<Boolean>(value)->value ? "true" : "false";
	else
		std::runtime_error("Something went wrong in the JSON::rewriteJSON() function");
}