#pragma once
#include <memory>
#include <vector>
#include <stdexcept>
#include <iostream>
#include "Lexer.h"
struct ASTNode {
    virtual ~ASTNode() = default;
    virtual void print(int indent = 0) const = 0;
};

struct NumberNode : public ASTNode {
    std::string value;
    
    NumberNode(const std::string& v) : value(v) {}
    
    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "NUMBER: " << value << "\n";
    }
};

struct IdentifierNode : public ASTNode {
    std::string name;
    
    IdentifierNode(const std::string& n) : name(n) {}
    
    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "IDENTIFIER: " << name << "\n";
    }
};

struct BinaryOpNode : public ASTNode {
    std::shared_ptr<ASTNode> left;
    std::string op;
    std::shared_ptr<ASTNode> right;
    
    BinaryOpNode(std::shared_ptr<ASTNode> l, const std::string& o, std::shared_ptr<ASTNode> r)
        : left(l), op(o), right(r) {}
    
    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "BINARY OP: " << op << "\n";
        std::cout << std::string(indent, ' ') << "  LEFT:\n";
        left->print(indent + 4);
        std::cout << std::string(indent, ' ') << "  RIGHT:\n";
        right->print(indent + 4);
    }
};

struct UnaryOpNode : public ASTNode {
    std::string op;
    std::shared_ptr<ASTNode> operand;
    
    UnaryOpNode(const std::string& o, std::shared_ptr<ASTNode> opnd)
        : op(o), operand(opnd) {}
    
    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "UNARY OP: " << op << "\n";
        operand->print(indent + 2);
    }
};

struct ExpressionStatementNode : public ASTNode {
    std::shared_ptr<ASTNode> expression;
    
    ExpressionStatementNode(std::shared_ptr<ASTNode> expr)
        : expression(expr) {}
    
    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "EXPRESSION STATEMENT:\n";
        expression->print(indent + 2);
    }
};
struct ProgramNode : public ASTNode {
    std::vector<std::shared_ptr<ASTNode>> statements;
    
    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "PROGRAM:\n";
        for (const auto& stmt : statements) {
            stmt->print(indent + 2);
        }
    }
};

class Parser {
private:
    std::vector<Token> tokens;
    size_t position;
    
    const Token& currentToken() const {
        return position >= tokens.size() ? tokens.back() : tokens[position];
    }
    
    void advance() { 
        if (position < tokens.size()) position++; 
    }
    
    bool match(TokenType type) const {
        return currentToken().type == type;
    }
    
    bool consume(TokenType type) {
        if (match(type)) { advance(); return true; }
        return false;
    }
    
    std::shared_ptr<ASTNode> parseExpression() {
        auto left = parseTerm();
        
        while (match(TokenType::PLUS) || match(TokenType::MINUS)) {
            std::string op = currentToken().value;
            advance();
            left = std::make_shared<BinaryOpNode>(left, op, parseTerm());
        }
        return left;
    }
    
    std::shared_ptr<ASTNode> parseTerm() {
        auto left = parseFactor();
        
        while (match(TokenType::STAR) || match(TokenType::SLASH)) {
            std::string op = currentToken().value;
            advance();
            left = std::make_shared<BinaryOpNode>(left, op, parseFactor());
        }
        return left;
    }
    
    std::shared_ptr<ASTNode> parseFactor() {
        if (match(TokenType::MINUS)) {
            advance();
            return std::make_shared<UnaryOpNode>("-", parseFactor());
        }
        return parsePrimary();
    }
    
    std::shared_ptr<ASTNode> parsePrimary() {
        if (match(TokenType::NUMBER)) {
            auto value = currentToken().value;
            advance();
            return std::make_shared<NumberNode>(value);
        }
        
        if (match(TokenType::IDENTIFIER)) {
            auto name = currentToken().value;
            advance();
            return std::make_shared<IdentifierNode>(name);
        }
        
        if (consume(TokenType::LPAREN)) {
            auto expr = parseExpression();
            consume(TokenType::RPAREN);
            return expr;
        }
        
        throw std::runtime_error("Unexpected token: " + currentToken().value);
    }
    
    std::shared_ptr<ASTNode> parseStatement() {
        auto expr = parseExpression();
        consume(TokenType::SEMICOLON);
        return std::make_shared<ExpressionStatementNode>(expr);
    }
    
public:
    Parser(const std::vector<Token>& toks) : tokens(toks), position(0) {}
    
    std::shared_ptr<ProgramNode> parse() {
        auto program = std::make_shared<ProgramNode>();
        
        while (!match(TokenType::END_OF_FILE)) {
            try {
                program->statements.push_back(parseStatement());
            } catch (const std::exception& e) {
                std::cerr << "Parse error: " << e.what() << "\n";
                break;
            }
        }
        
        return program;
    }
};