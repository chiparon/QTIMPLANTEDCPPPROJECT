# my coding process
# - version beta 0.1
complete the finance.class finance and finance manager.
complete the main structure of main.cpp.
i use goto structure and a if (0) to make it more readable at mode switch part.

~~ after suffering from the basic class construction, i take step in readme.md this time.

# - version beta 0.2
finish the reminder class && ReminderManager class.
.cpp .h finished;
iv used TRAE CN to do this task, 'cause u know it is quite repeatable for this pair of class and functions.
especially they will be based on class Record later.
~~TODO:the automatically sorted function has something bad,but ima sleep.so fuck off,and solve it next time.

# - version beta 0.3xx
finish the frame part of class password.
it's quite sophisticated for me, a password 0% known comer, to write an encryption algorithm, but I decided
to do it, idk y.
now I decided to shou cuo AES,it has 4 steps repeating 10 times:
- each char conversion by a template
> take every 16 chars into a 4,4 matrix;and it shall be done before the algorithm
- make each row' char go lefter by distance i (row 0->move 0;row 3 ->left 3)
- make each column do a matrix multiplication, and the relation is in GF(2^8)'s addition&multiplication
  >it's just like : operator + is ^,the XOR
  > *is a
- make current matrix do a XOR with a subkey generating from key
so the logic chain is :
- definite the GF multi & addition;
- definite the GF system's 4,4 matrix algorithm;
- definite the repeatable 4 steps;
- make it into true of the operation of taking a password string into several 4,4 matrix;
how to make its length mol 16===0;
- do the decryption algorithm, refering those 4 steps to give the 4 steps correspondingly(sb ying yu naomale)
- apply these tough shits on our function member ENCRYPTION PASSWORD and DECRYPTION PASSWORD;

then ends the class, functions all the functions, and main part can be modified, then stage1 finished.
no so much talking shit.ima sleep now, cause ima attend tomorrow's oral examination (CET4) even it is not
necessary. i wanna watch stream of IEM DALLAS. this night is MONGOLZ versus TURKISTAN, and falcons vs GL;;
GN;;FUCK I HAVENT COMPLETE YESTDAY'S TODO...


# - version beta 1.0m

lol long time no see, i v been escaping the task for almost 2 weeks? or 10days. ohhh it would be a tough thing
hope i will not tuck it up. QWQ
rebuild the project adding basic record class, in which process abstract those three moduled class and management
whats more, i put them into a unified manager .*, u know, my classmate mentioned the structure
thus learning the more regular, more used construction and applying it.

also i modified main to be more flattened, lol

next step i need to replace the en&de-cryption algo and the sorted of time reminder, which version name 'm' indicates.

but i wanna instantly gonna implant the code into Qt6,take them with ui design.
ima continue watching CS major match lol , i found i have done a almost 100% right bet but changed
referring those fucking silly CS anchor and hosts(even pro Karrigan). fuck you bullshit silly assholes.fuck yall ewe;


# - version alpha 0.3
u will never get that i stay up to 5:53 and complete it:
- learn how to do ui class in creator and make it somelike not a blank of .*
- do a main window
- try finance window
- complete reminder and record window

this is process note::
i found what needs improving lol
such as : when the reminds note got its time, its time for deleting desuwa.
wanjiqi machine ' stream watched too much desuwa.
B8 plays so fucking sick, i am so sad....

[-]unified manager.cpp中编码疑似有些错误，中文变成了乱码
[-]在按月查账模块中输出的总金额一直是.2f：麻了必须要前置规定，为什么输出前不行
[-]reminder的排序逻辑应该是先按时间排序，相同时间优先级排序，或者可以切换不同的排序依据
[-]显示密码时显示出了被加密的密码，要区分加密密码与解密密码呈现
[-]账本管理的 统计分析输出的总支出为%.2f这个字符串而不是两位小数点的数
[-]提醒管理选择日期后按日期查询后没有反应
[-]清理任务后虽然跳出弹窗清理完成，但是还显示有这些reminder记录
[-]显示密码后应该展示原本的密码；按网站名查询后，显示区没有反应，依旧是所有的密码记录
[-]左下角的当月总支出并未修复；
[-]密码的搜索依旧没有反应
[-]更改密码选项，能够更改密码
[-]在排序显示时，分为：按优先级显示 与按 时间显示
[-]按日期查询无响应 
[-]排序显示与按时间排序 按优先级排序存在重复，同时三个按钮全都无响应
[-]如果在reminder模块添加提醒时提醒内容的开头是数字，那么当关闭窗口再次打开的时候，会变成 内容部分为空白，优先级变为提醒内容开头的数字
[-]在搜索网站后无法回到原本的所有网站全部列出来的模式
[-]逆向的密码还是错的，考虑使用替换表
[-]使用静态替换表：
[-]第一次调用初始化
[-]确保每次生成相同的替换表
[-]使用洗牌算法生成随机替换表
[ ]摘掉控制台的输出指令

我靠我桌子上怎么这么多头发呜呜呜呜呜呜我的头发8点多了要不吃个早饭睡大觉去了

# - version alpha 1.0.0
LOOK UPON, THATS WHAT ALL I DID
