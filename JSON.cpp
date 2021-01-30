#include "JSON.h"

JSON::JSON(std::string fileName) {
	std::string text, temp;
	std::ifstream file(fileName);
	if(!file.is_open()) {
		std::cout << "Problem opening \"" << fileName << "\"" << std::endl;
		exit(-1);
	}
	while(std::getline(file, temp))
		text += temp + "\n";
	file.close();
	this->text = text;
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
	// assume it is an obj for now fuck arrays tbh
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

// Implementation for JSON::get<>() inside header because it will cause a linker error