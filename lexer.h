#pragma once
#include <string>
#include <vector>
#include <cctype>
using namespace std;

enum class TokenType {
    NUMBER,IDENTIFIER, PLUS, MINUS, STAR, SLASH, 
    LPAREN, RPAREN, SEMICOLON, END_OF_FILE, UNKNOWN
};

struct Token 
{   TokenType type;
    string cont;//cont=content
    
    Token(TokenType t, const string& v) : type(t), cont(v) {}
};

class Lexer {
private:
    string source;
    size_t ind;//ind=index
    
    char currentChar() const {
        return  ind>= source.length() ? '\0' : source[ind];
    }
    
    void advance() { ind++; }
    
    void skipspace() {
        while (isspace(currentChar())) advance();
    }
    
    Token readnumber() 
    {
        string number;
        while (isdigit(currentChar())) 
        {
            number += currentChar();
            advance();
        }
        return Token(TokenType::NUMBER, number);
    }
    
    Token readidentifier() 
    {
        string identifier;
        while (isalnum(currentChar()) || currentChar() == '_') 
        {
            identifier += currentChar();
            advance();
        }
        return Token(TokenType::IDENTIFIER, identifier);
    }
    
public:
    Lexer(const string& src) : source(src), ind(0) {}
    
    std::vector<Token> tokenize() {
        vector<Token> tokens;
        
        while (currentChar() != '\0') 
        {
            skipspace();
            if (currentChar() == '\0') break;
            
            char ch = currentChar();
            
            if (isdigit(ch)) {
                tokens.push_back(readnumber());
            } 
            else if (isalpha(ch) || ch == '_') {
                tokens.push_back(readidentifier());
            } 
            else if (ch == '+') {
                tokens.push_back(Token(TokenType::PLUS, "+"));
                advance();
            } 
            else if (ch == '-') {
                tokens.push_back(Token(TokenType::MINUS, "-"));
                advance();
            } 
            else if (ch == '*') {
                tokens.push_back(Token(TokenType::STAR, "*"));
                advance();
            } 
            else if (ch == '/') {
                tokens.push_back(Token(TokenType::SLASH, "/"));
                advance();
            } 
            else if (ch == '(') {
                tokens.push_back(Token(TokenType::LPAREN, "("));
                advance();
            } 
            else if (ch == ')') {
                tokens.push_back(Token(TokenType::RPAREN, ")"));
                advance();
            } 
            else if (ch == ';') {
                tokens.push_back(Token(TokenType::SEMICOLON, ";"));
                advance();
            } 
            else {
                tokens.push_back(Token(TokenType::UNKNOWN, std::string(1, ch)));
                advance();
            }
        }
        
        tokens.push_back(Token(TokenType::END_OF_FILE, ""));
        return tokens;
    }
};
