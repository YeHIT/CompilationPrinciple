#include <iostream>
#include <string.h>
#include <list>
#include <map>
#include <algorithm> 
#include <stdlib.h>

using namespace std;

//标识符
#define ID 1
//数据类型的关键字
#define INT 2
#define FLOAT 3
#define CHAR 4
#define STRING 5
//对应数据类型的值
#define INT_VALUE 6
#define FLOAT_VALUE 7
#define CHAR_VALUE 8
#define STRING_VALUE 9
//其他关键字
#define IF 10
#define WHILE 11
#define FOR 12
//运算符及括号
//=
#define ASSIGN 20
#define PLUS 21
#define MINUS 22
#define MULTI 23
#define DIVIDE 24
//;
#define SEMIC 25
//,
#define COMMA 26
//>
#define GT 27
//>=
#define GE 28
#define LT 29
#define LE 30
#define EQ 31
//(
#define LR_BRAC 32
#define RR_BRAC 33
//符号表
list<string> symbolTable;

/**
 * 判断当前标识符是否已经存在,若存在则插入符号表
 * @param 输入标识符
*/
void storeIntoSymbelTable(string token){
    list<string>::iterator iter ;
    iter = std::find(symbolTable.begin(),symbolTable.end(), token);
    if(iter == symbolTable.end()){
        symbolTable.push_back(token);
    }
}

/**
 * 用于展示token (x,token)
 * @param type token类型
 * @param token token内容
*/
void showToken(int type,string token){
    cout << "(" << type << "," << token << ")" << endl;
}

/**
 * 根据传入的token判断该token的类型
 * @param token 需要判断的token
 * @return 该token的类型
*/
int getTokenType(string token){
    if(token == "if"){
        return IF;
    }
    else if(token == "while"){
        return WHILE;
    }
    else if(token == "for"){
        return FOR;
    }
    else if(token == "int"){
        return INT;
    }
    else if(token == "float"){
        return FLOAT;
    }
    else if(token == "char"){
        return CHAR;
    }
    else if(token == "string"){
        return STRING;
    }
    else{
        //对于标识符需要存入符号表
        storeIntoSymbelTable(token);
        return ID;
    }
}

/**
 * 读取字符进行词法分析
*/
void tokenScan(char ch){
    //文件读取完毕则停止词法分析
    if(ch == EOF)
        return;
    string token;
    //空字符、换行符、制表符弃用
    while(ch == ' ' || ch == '\t' || ch == '\n'){
        ch = getchar();
    }
    //将第一个有意义的字符放入token
    token.append(1,ch);
    //对于字母
    if(isalpha(ch)){
        ch = getchar();
        while(isalnum(ch)){
            token.append(1,ch);
            ch = getchar();
        }
        showToken(getTokenType(token),token);
        //继续进行词法分析,此时ch是未经判定的,故以此为开始
        return tokenScan(ch);
    }
    //对于数字
    else if(isdigit(ch)){
        ch = getchar();
        while(isdigit(ch)){
            token.append(1,ch);
            ch = getchar();
        }
        //如果有小数点则判定是否为浮点数形式为 (digit)+[.](digit)+
        if(ch == '.'){
            token.append(1,ch);
            ch = getchar();
            if(isdigit(ch)){
                token.append(1,ch);
                ch = getchar();
            }
            else{
                cout<< "浮点数输入有误！" << endl;
                return ;
            }
            while(isdigit(ch)){
                token.append(1,ch);
                ch = getchar();
            }
            showToken(FLOAT_VALUE,token);
            return tokenScan(ch);
        }
        //无小数点
        else{
            showToken(INT_VALUE,token);
            return tokenScan(ch);
        }
    }
    //为单引号判断是否为字符
    else if(ch == '\''){
        ch = getchar();
        token.append(1,ch);
        ch = getchar();
        if(ch == '\''){
            token.append(1,ch);
            showToken(CHAR_VALUE,token);
            return tokenScan(getchar());
        }
        else{
            cout << "输入字符错误" << endl;
            return;
        }
    }
    //为双引号判断是否为字符串
    else if(ch == '\"'){
        ch = getchar();
        while(ch != '\"' && ch != EOF){
            token.append(1,ch);
            ch = getchar();
        }
        //只读取到一个双引号
        if(ch == EOF){
            cout << "输入字符串错误" << endl;
            return;
        }
        token.append(1,ch);
        showToken(STRING_VALUE,token);
        return tokenScan(getchar());
    }
    else{
        //单字符运算符则直接读取新字符再继续分析
        int singleOperator = 1;
        switch (ch){
            case '+': 
            showToken(PLUS,token);
            break;
            case '-': 
            showToken(MINUS,token);
            break;
            case '*': 
            showToken(MULTI,token);
            break;
            case '/': 
            showToken(DIVIDE,token);
            break;
            case ';': 
            showToken(SEMIC,token);
            break;
            case ',': 
            showToken(COMMA,token);
            break;
            case '(': 
            showToken(LR_BRAC,token);
            break;
            case ')': 
            showToken(RR_BRAC,token);
            break;
            //若为多字符判断则设置标志位为0
            default:
            singleOperator = 0;
            break;
        }
        if(singleOperator == 1){
            return tokenScan(getchar());
        }
        //多字符运算符
        switch (ch){
            //若为=号
            case '=': 
            ch = getchar();
            if(ch == '='){
                token.append(1,ch);
                showToken(EQ,token);
                ch = getchar();
            }
            else{
                showToken(ASSIGN,token);
            }
            break;
            case '>': 
            ch = getchar();
            if(ch == '='){
                token.append(1,ch);
                showToken(GE,token);
                ch = getchar();
            }
            else{
                showToken(GT,token);
            }
            break;
            case '<': 
            ch = getchar();
            if(ch == '='){
                token.append(1,ch);
                showToken(LE,token);
                ch = getchar();
            }
            else{
                showToken(LT,token);
            }
            break;
        }
    }
    return tokenScan(ch);
}

/**
 * 展示符号表
*/
void showSymbolTable(){
    while(symbolTable.size() != 0){
        cout << symbolTable.front() << endl;
        symbolTable.pop_front();
    }
}

int main (){
    //将标准输入重定向到program.txt文件
    freopen("C:/Users/SmallYe/Desktop/program.txt", "r", stdin);

    //将标准输出重定向到token.txt文件
    freopen("C:/Users/SmallYe/Desktop/token.txt", "w", stdout); 
    cout << "输出token如下:" << endl;
    //词法分析
    tokenScan(getchar());

    //将标准输出重定向到symbolTable.txt文件
    freopen("C:/Users/SmallYe/Desktop/symbolTable.txt", "w", stdout); 
    cout << "符号表如下:" << endl;
    showSymbolTable();
    return 0;
}
