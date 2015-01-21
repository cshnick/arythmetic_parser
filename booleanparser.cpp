#include <QtCore>

#include "booleanparser.h"
#include "node.h"

int funct(int a, int b) {}
class functc {
    public:
    int operator ()(int a, int b) {}
};

Node *BooleanParser::parse(const QString &expr)
{
    in = expr;
    in.replace(" ", "");
    pos = 0;

    int (*f)(int, int) = funct;
    f(1, 2);
    Node *node = new Node(Node::Root);
    addChild(node, parseOrExpression());
    return node;
}

Node *BooleanParser::parseOrExpression()
{
    Node *childNode = parseAndExpression();
    if (matchToken("||")) {
        Node *node = new Node(Node::OrExpression);
        addChild(node, childNode);
        while (matchToken("||")) {
            addToken(node, "||", Node::Operator);
            addChild(node, parseAndExpression());
        }
        return node;
    } else {
        return childNode;
    }
}

Node *BooleanParser::parseAndExpression()
{
    Node *childNode = parseAddExpression();
    if (matchToken("&&")) {
        Node *node = new Node(Node::AndExpression);
        addChild(node, childNode);
        while (matchToken("&&")) {
            addToken(node, "&&", Node::Operator);
            addChild(node, parseAddExpression());
        }
        return node;
    } else {
        return childNode;
    }
}

Node *BooleanParser::parseAddExpression() {
    Node *childNode = parseMulExpression();
    QString tok;
    if (matchTokens("+|-", tok)) {
        Node *node = new Node(Node::AddExpression);
        addChild(node, childNode);
        while (matchTokens("+|-", tok)) {
            addToken(node, tok, Node::Operator);
            addChild(node, parseMulExpression());
        }
        return node;
    } else {
        return childNode;
    }
}
Node *BooleanParser::parseMulExpression() {
    Node *childNode = parseNotExpression();
    QString tok;
    if (matchTokens("*|/", tok)) {
        Node *node = new Node(Node::MulExpression);
        addChild(node, childNode);
        while (matchTokens("*|/", tok)) {
            addToken(node, tok, Node::Operator);
            addChild(node, parseNotExpression());
        }
        return node;
    } else {
        return childNode;
    }
}

Node *BooleanParser::parseNotExpression()
{
    if (matchToken("!")) {
        Node *node = new Node(Node::NotExpression);
        while (matchToken("!"))
            addToken(node, "!", Node::Operator);
        addChild(node, parseAtom());
        return node;
    } else {
        return parseAtom();
    }
}

Node *BooleanParser::parseAtom()
{
    if (matchToken("(")) {
        Node *node = new Node(Node::Atom);
        addToken(node, "(", Node::Punctuator);
        addChild(node, parseOrExpression());
        addToken(node, ")", Node::Punctuator);
        return node;
    } else {
        return parseIdentifier();
    }
}

Node *BooleanParser::parseIdentifier()
{
    int startPos = pos;

    auto ndtype = Node::Identifier;
    QString str;
    QVariant res;
    if (in[pos].isLetter()) {
        while (pos < in.length() && in[pos].isLetterOrNumber()) {
            ++pos;
        }
        str = in.mid(startPos, pos - startPos);
        res = str;
    } else if (in[pos].isNumber()) {
        while (pos < in.length() && in[pos].isNumber()) {
            ++pos;
        }
        str = in.mid(startPos, pos - startPos);
        res = str.toInt();
        ndtype = Node::Number;
    }

    if (pos > startPos) {
        return new Node(ndtype, str, res);
    } else {
        return 0;
    }
}

void BooleanParser::addChild(Node *parent, Node *child)
{
    if (child) {
        parent->children += child;
        parent->str += child->str;
        child->parent = parent;
    }
}

void BooleanParser::addToken(Node *parent, const QString &str,
                             Node::Type type)
{
    if (in.mid(pos, str.length()) == str) {
        addChild(parent, new Node(type, str));
        pos += str.length();
    }
}

bool BooleanParser::matchToken(const QString &str) const
{
    return in.mid(pos, str.length()) == str;
}

bool BooleanParser::matchTokens(const QString &pattern, QString &tok) const
{
    QStringList tokens = pattern.split("|", QString::SkipEmptyParts);
    Q_FOREACH(QString ct, tokens) {
        if (matchToken(ct)) {
            tok = ct;
            return true;
        }
    }

    return false;
}
