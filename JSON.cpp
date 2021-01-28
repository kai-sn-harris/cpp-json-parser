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
	}
	// assume it is an obj for now fuck arrays tbh
	if(node->ASTtype == "value") {
		for(auto elem : dynamic_cast<Object*>(node)->values)
			this->visitNode(elem, map);
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

// visit nodes and fill in variable table
void JSON::generate() {
	Lexer lexer(this->text);
	Parser parser(lexer);
	this->visitNode(parser.ast(), this->variables);
}

// std::string JSON::get(std::string key) {
// 	std::vector<std::string> keys;
// 	std::string word;
// 	for(int i = 0; i < key.size(); i++) {
// 		if(key[i] == ' ') {
// 			keys.push_back(word);
// 			word = "";
// 		}
// 		word += key[i];
// 	}
// 	// last word
// 	keys.push_back(word);

// 	Val value;
// 	for(int i = 0; i < keys.size(); i++) {
// 		std::cout << keys[i] << std::endl;
// 		if(i == 0) value = this->variables[keys[0]];
// 		else if(i == keys.size()-2) value = value;
// 		else {
// 			if(value.type == "string") return value.stringVal;
// 			if(value.type == "bool") return std::to_string(value.boolVal);
// 			if(value.type == "number") return std::to_string(value.numberVal);
// 			if(value.type == "null") return value.nullVal;
// 		}
// 		std::cout << "did we get here" << std::endl;
// 	}
// 	return std::any_cast<std::string>(value);
// }