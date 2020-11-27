#include <iostream>
#include <string.h>
#include <list>
#include <map>
#include <algorithm> 
#include <stdlib.h>
#include <stack>
using namespace std;

#define ID 1
#define INT 2
#define FLOAT 3
#define INT_VALUE 6
#define FLOAT_VALUE 7
#define ASSIGN 20
#define PLUS 21
#define MINUS 22
#define MULTI 23
#define DIVIDE 24
#define SEMIC 25
#define NOACTION "NOACTION"
//动作表
string actionTable[23][10];
//转移表
string gotoTable[23][7];

//状态栈
stack<int> stateStack;
//输入栈
stack<string> inputStack;
//当前输入
string inputbuf;

/**
 * 根据输入的字符串得到对应的状态
 */
int getInputNumber(string s){
    int begin = s.find_first_of('(');
    int end = s.find_first_of(',');
    string numberStr = s.substr(begin + 1 ,end - 1); 
    int number = atoi(numberStr.c_str());
    switch (number){
    case ID:
        return 0;
        break;
    case INT_VALUE:
        return 1;
        break;
    case FLOAT_VALUE:
        return 1;
        break;
    case PLUS:
        return 2;
        break;
    case MINUS:
        return 3;
        break;
    case MULTI:
        return 4;
        break;
    case DIVIDE:
        return 5;
        break;
    case INT:
        return 6;
        break;
    case FLOAT:
        return 7;
        break;
    case ASSIGN:
        return 8;
        break;
    case SEMIC:
        return 9;
        break;
    default:
        return -1;
        break;
    }
}

/**
 * 根据归约到的字符串获取当前需要跳转的状态
 */
int getGotoNumber(string s){
    int end = s.find_first_of(' ');
    string gotoStr = s.substr(0,end);
    if(gotoStr.compare("EXP") == 0){
        return 0;
    }
    if(gotoStr.compare("ITEM") == 0){
        return 1;
    }
    if(gotoStr.compare("FACTOR") == 0){
        return 2;
    }
    if(gotoStr.compare("DEFINE") == 0){
        return 3;
    }
    if(gotoStr.compare("ASSIGN") == 0){
        return 4;
    }
    if(gotoStr.compare("KEYWORD") == 0){
        return 5;
    }
    if(gotoStr.compare("S") == 0){
        return 6;
    }
}

/**
 * 根据执行的归约语句(A->B)，计算|B|,即B所含元素的个数
 */
int getRightNumber(string s){
    int begin = s.find_first_of("-> ");
    string rightStr = s.substr(begin + 4);
    int number = 1;
    while(rightStr.find_first_of(" ") != rightStr.npos){
        rightStr = rightStr.substr(rightStr.find_first_of(" ") + 1 );
        number++;
    }
    return number;
}

/**
 * 词法分析
 */
void analyse(int readFlag){
    if(readFlag == 0){
        getline(cin,inputbuf);
    }
    //如果是状态转移(开始处为S)
    if(actionTable[stateStack.top()][getInputNumber(inputbuf)].find("S") == 0){
        //符号a入栈
        inputStack.push(inputbuf);
        //状态入栈
        string stateStr = actionTable[stateStack.top()][getInputNumber(inputbuf)].substr(1);
        stateStack.push(atoi(stateStr.c_str()));
        //继续分析
        analyse(0);
    }
    //如果是归约(开始处为R)
    else if(actionTable[stateStack.top()][getInputNumber(inputbuf)].find("R") == 0){
        //打印当前结果
        string stateStr = actionTable[stateStack.top()][getInputNumber(inputbuf)].substr(1); 
        cout << stateStr << endl;
        int number = getRightNumber(stateStr); 
        while(number != 0){
            //输入栈和状态栈弹出一个符号
            stateStack.pop();
            inputStack.pop();
            number--;
        }
        //符号入栈
        inputStack.push(inputbuf);
        //状态入栈 
        stateStr = gotoTable[stateStack.top()][getGotoNumber(stateStr)];
        stateStack.push(atoi(stateStr.c_str()));
        //继续分析
        analyse(1);
    }
    //如果accept
    else if(actionTable[stateStack.top()][getInputNumber(inputbuf)].compare("accept") == 0){
        //清空栈
        while(stateStack.size() != 0){
            stateStack.pop();
        }
        while(inputStack.size() != 0){
            inputStack.pop();
        }
        //初始化栈 
        stateStack.push(0);
        inputStack.push("#");
        cout << "accept!!!!" << endl;
        return;
    }
    //出错则报错
    else{
        cout << "ERROR!!!!" << endl;
        return;
    }
}

/**
 * 初始化动作表和状态表
 */
void initTables(){
    for(int i = 0 ; i < 23; i++){
        getline(cin,inputbuf);
        inputbuf = inputbuf.substr(inputbuf.find_first_of(',') + 1);
        for(int j = 0 ; j < 17 ; j++){
            string tempStr = inputbuf.substr(0,inputbuf.find_first_of(','));
            inputbuf = inputbuf.substr(inputbuf.find_first_of(',') + 1);
            if(j <= 9){
                actionTable[i][j] = (tempStr.size() != 0) ? tempStr : NOACTION;
            }
            else{
                gotoTable[i][j - 10] = (tempStr.size() != 0) ? tempStr : NOACTION;
            }
        }
    }
}

int main(){
    //将标准输入重定向到table.csv文件
    freopen("C:/Users/SmallYe/Desktop/tables.csv", "r", stdin);
    initTables();
    //将标准输入重定向到token.txt文件
    freopen("C:/Users/SmallYe/Desktop/token.txt", "r", stdin);
    //将标准输出重定向到analyseResult.txt文件
    freopen("C:/Users/SmallYe/Desktop/analyseResult.txt", "w", stdout);
    //初始化栈 
    stateStack.push(0);
    inputStack.push("#");
    getline(cin,inputbuf);
    while(inputbuf.size() != 0){
        analyse(1);
        getline(cin,inputbuf);
    }
    return 0;
}