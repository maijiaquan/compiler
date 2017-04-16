#include <fstream>
#include<string>
#include <stdlib.h>
#include <iostream>
using namespace std;

const int WordLen = 20;

char buf[256];   //缓冲区

struct code_val{  //二元式
    char code;               //单词种别
    char val[WordLen + 1];   //单词值
};

/*函数的声明*/
code_val scanner(char buf[], int &i);    //扫描
void concat(char token[], char c);    //拼接字符
char reserve(char token[]);       //查保留字表


//拼接字符函数
//若token = "", 则函数执行之后token = "b"
//若token = "beg", 则函数执行之后token = “begi”
void concat(char token[], char c){
    int index = 0;
    for(int i = 0; token[i]; i++, index = i);   //得到index：token最后一个有效元素的下一个下标
    token[index] = c;
    token[++index] = '\0';
}

//查保留字表
char reserve(char token[]){
    const char *table[] = {"begin", "end", "int", "real"};   //4种保留字
    const char code[] = "{}ac";    //4种保留字对应的代码
    for(int i = 0; i < strlen(code); i++){
        if (strcmp(token, table[i]) == 0) {
            return code[i];   //是保留字，返回对应的代码
        }
    }
    return 'i';     //不是保留字，返回'i'
}

//扫描函数
//拼接完后，i 永远指向下一个 等待读入的字符
code_val scanner(char buf[], int &i){
    code_val t = {'\0',"NUL"};
    char token[WordLen + 1] = "";            //token数组用于拼接单词
    
    if(buf[i] >= 'a' && buf[i] <= 'z'){             //--------------- 1.当该字符是字母
        while((buf[i] >= 'a' && buf[i] <= 'z') || (buf[i] >= '0' && buf[i] <= '9')){    //接下来还是 字母或数字
            concat(token, buf[i++]);              //拼接
        }
        t.code = reserve(token);  //查保留字表
        if (t.code == 'i') {      //不是保留字, code = i
            strcpy(t.val, token);    //直接将token拷贝至t.val
        }
        return t;
    }
    
    
    if (buf[i] >= '0' && buf[i] <= '9'){    //----------------------- 2.当该字符是数字
        while (buf[i] >= '0' && buf[i] <= '9'){   //接下来还是 数字
            concat(token, buf[i++]);       //拼接，得到123
        }
        
        if (buf[i] == '.') {            //遇到小数点
            concat(token, buf[i++]);       //拼接，得到123.
            while (buf[i] >= '0' && buf[i] <= '9'){   //接下来还是 数字
                concat(token, buf[i++]);       //拼接，得到123.4
            }
            t.code = 'y';      //code = y 小数
        }else{
            t.code = 'x';      //整数
        }
        strcpy(t.val, token);    //直接将token拷贝至t.val
        return t;
    }
    
    if (buf[i] == '.'){    //----------------------- 3.当该字符为'.'
        concat(token, buf[i++]);       //拼接，得到.
        if (buf[i] >= '0' && buf[i] <= '9'){    //接下来还是 数字
            while (buf[i] >= '0' && buf[i] <= '9'){   //接下来还是 数字
                concat(token, buf[i++]);       //拼接，得到.123
            }
            t.code = 'y';
            strcpy(t.val, token);    //直接将token拷贝至t.val
            return t;
        }else{ //接下来不是数字，即错误单词
            cout<<token<<"是一个错误的单词"<<endl;
            exit(0);
        }
    }
    
    switch (buf[i]) {     //----------------------- 4.其余单词
        case ',' : t.code = ',';  break;
        case ';' : t.code = ';';  break;
        case '(' : t.code = '(';  break;
        case ')' : t.code = ')';  break;
        case '=' : t.code = '=';  break;
        case '*' : t.code = '*';  break;
        case '#' : t.code = '#';  break;
        case '+' :
            if (buf[++i] == '+') {
                t.code = '$';
            }else{
                t.code = '+';
                i--;
            }
            break;
        
            
        default:
            cout<<buf[i]<<"为非法字符"<<endl;
            exit(0);
            break;
    }
    i++;  //用意：指向下一个字符
    return t;
}


int main(){
    fstream in("/Users/Michael/source.txt");     //读文件, 将文件内容放入buf
    if (!in.is_open()){
        cout << "打开文件出错"; exit (1);
    }
    while (!in.eof()){              //eof（）：当到达文件末尾时返回true
        in.getline (buf,100);
        cout << buf << endl;
    }
    
    fstream coutf("/Users/Michael/out.txt", ios::out);     //写文件
    code_val temp_code_val;   //二元式
    
    int i = 0;
    do{
        while (buf[i] == ' ') { //如果该字符是空格，则去除前导空格
            i++;
        }
        temp_code_val = scanner(buf, i);   //  调用一次scanner，获得一个二元式
        coutf << temp_code_val.code << '\t' << temp_code_val.val <<endl;      //写入out.txt
        cout<<temp_code_val.code<<"   "<<temp_code_val.val<<endl;
    }while (temp_code_val.code != '#');
    
    return 0;
}


