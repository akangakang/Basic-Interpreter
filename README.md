# Basic-Interpreter
BASIC 解释器说明文档——PRO2
 518021910789  刘书畅
一、	基本组成
1.	basicwindow.h   basicwindow.cpp
用于
①	带行号程序的存储
②	变量的存储
③	程序的运行函数

2.	calc.h  calc.cpp.
用于计算表达式（表达式树）

3.	console.h  console.cpp
用于操纵台的输入输出

4.	tokenizer.h   tokenizer.cpp
     用于一些取符号的函数


二、	完成功能
1.	Bonus 完成了幂乘运算
2.	带括号计算
3.	得到中缀表达式后建树
4.	变量表

三、	各类介绍
一、	basicwindow
1.	带行号程序存储 
使用Qmap program存储   
关键字为 行号 值为去掉行号的语句
当输入行号关键字不合法时 该行程序不会被存入program

2.	变量的存储
使用Qmap variable 存储
 
Key 为变量名  value为变量的值
Value之所以被存储为QString 是因为basic的let语句定义变量的时候不区分变量类型，而C++区分，为了支持除int 外的变量类型的定义和输入，将所有变量存储为字符串（但因为时间有限　，没有完成支持非int类型的变量的存储）

3.	GUI存储
　①  inputwindow 
				用于和用户交互 为用户的输入界面和 help print 等函数的输出界面
①	 outputwindow 
用以ERROR警告 当用户输入非法时 在outputwindow输入错误提示
②	QPushButton name
解释器的名字显示
  
4.	关键字和程序输入处理函数（阐述重点函数像if等简单关键字处理不阐述）
①	处理输入
 
当inputWindow输入一行时 发送信号 该类进行接受
该函数检查输入的行号和关键字是否合法
如果合法就将关键字和语句分离并存入program 不合法就报错
如果输入的是不需要行号立即执行的关键字 如run help则立即处理相应函数
②	处理异常抛出
  
③	处理run关键字
 
若检测到关键字为run 则遍历 program存储的程序 分别检测关键字和相应函数
④	处理input函数
 
若检测到input关键字则在屏幕打印“？“
并等待用户输入
此处设计了一个循环
 
                若用户成功输入则跳出循环 反正一直循环等待用户输入
                检测用户是否输入的做法在console类里详细阐述
⑤	处理打印print关键字
 
此处分类讨论
若输入形式为 print x=1+1等类型 则先存储x进变量表值为2 并打印2（x的值）
若输入为print x/x+1/1+1 则见计算再打印


二、calc 
表达式树的计算
关键点为计算x+1等含变量的表达式时 先通过信号与槽得出变量的值 再把变量替换成相应大小的int类型的数值来初始化表达式树

三、consol
交互框
关键点为区分普通程序输入和等待input时的输入
 
输入回车时给basicwindow类发送信号 有新程序输入
若该行包含“？“（因为REM PRINT等关键词是可能也会输入问号　所以排出此类情况）则判断为是等待ｉｎｐｕｔ时的输入，反之是普通程序输入

四、tokenizer
行使一些取符号的功能
 


             
             
四、	异常处理
一、	输入过程中检测到错误
1.	line number error
不合法行号输入 
行号和其他输入不能相连必须间隔空格
必须由数字组成
不能为负数
2.	key word error
①	 行号后未输入关键字
②	关键字不合法
③	未输行号时 输入了goto if end 关键字 Key word error! (line number needed)
④	输入行号时 输入了run等关键字 Key word error! (no line number needed)
⑤	输入if 未输入 then key word error! (if...then)

输入过程中就检测到错误并且报错，不会把错误输入写进内存，并且可以让用户立即重新输入。

二、	运行过程中检测到错误
1.	ERROR: LINE n: invalid line number after THEN
运行到if语句时 检查 then 后面的行号是否合法
2.	ERROR: LINE n: invalid line number after GOTO
检查goto后的行号是否合理
  
3.	ERROR: LINE n: no valid operator
比较符必须时’<’ ‘=’ ‘>’ 否则报错结束程序
 
4.	ERROR: invalid variable
①	访问的变量不存在时报错
比如 1 let x=y+1  
Y未定义则报错
②	输入不合法变量名
1、	必须以字母开头；
2、	不可以包含嵌入的句号或者类型声明字符
3、	不能和受到限制的关键字同名；
  4、由字母数字下划线组成

5.	ERROR: There must be a euqal sign after variable


