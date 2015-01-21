#ifndef BOOLEANPARSER_H
#define BOOLEANPARSER_H

#include "node.h"


enum {
    add,
    sub,
};
template<typename T>
class Summ {

};

class BooleanParser
{
public:
    Node *parse(const QString &expr);

private:
    Node *parseOrExpression();
    Node *parseAndExpression();
    Node *parseAddExpression();
    Node *parseMulExpression();
    Node *parseNotExpression();
    Node *parseAtom();
    Node *parseIdentifier();
    void addChild(Node *parent, Node *child);
    void addToken(Node *parent, const QString &str, Node::Type type);
    bool matchToken(const QString &str) const;
    bool matchTokens(const QString &str, QString &tok) const;

    QString in;
    int pos;
};

#endif
