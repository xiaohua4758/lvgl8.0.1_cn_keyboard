适用于单片机的lvgl简单中文键盘拼音输入法，使用QT5.14创建，直接打开即可运行模拟器
如果QT编译提示有错误,尝试在构建设置中重设目录：QT的左边栏-"项目"->"Desktop QT5.14.2 MinGW 32-bit"->"构建设置"->"概要"->"构建目录",填入一个目录，比如“../output”，参考“如有QT编译错误看这里.jpg”

GUI: lvgl8.0.1
1. 功能
适用于单片机的中文拼音输入法，创建一个拼音输入的键盘，接受拼音输入，并显示拼音对应的候选字，通过触摸点击选择目标候选汉字，gui平台是lvgl8.0.1。

2. 原理
在lvgl原文件"lv_keyboard.c"基础上进行键盘的改写，新文件名为"lv_cn_keyboard.c"，共同存放于"lvgl\src\extra\widgets\keyboard\"目录下，相关的对外函数名称以"lv_cn_xxx"开头。
如创建一个中文拼音输入法的函数名称为
lv_obj_t * lv_cn_keyboard_create(lv_obj_t * parent, const lv_font_t *cn_font)。
原官方文件"lv_keyboard.c"与本中文输入"lv_cn_keyboard.c"可根据实际情况自由选择使用，中文拼音输入法lv_cn_keyboard.c已经包含了官方lv_keyboard.c键盘的所有功能。

3. 涉及文件说明
(1) lvgl\src\extra\widgets\keyboard\lv_cn_keyboard.c，中文拼音输入法键盘控件源文件，可创建一个中文拼音输入法键盘控件
(2) user\tiny_pinyin.c，简单的拼音输入法，来源于网络
(3) myfont\my_cn_font1.c 由字体生成工具 Lvgl Font Tool V0.4 生成中文字库
(4) lvgl\examples\widgets\keyboard\lv_example_keyboard_1.c，使用中文拼音输入法键盘的简单应用例程

4. 使用说明

示例：
(1) 创建键盘
	static lv_obj_t *kb = NULL;
	lv_obj_t * ta;
	
	/*创建一个textarea来接收键盘输入*/
	ta = lv_textarea_create(lv_scr_act());				
	....
	
	if(NULL == kb){
		/*使用lv_cn_keyboard_create创建键盘，注意是lv_cn_前缀，第二参数my_cn_font1是字库*/
		kb = lv_cn_keyboard_create(lv_scr_act(), &my_cn_font1);	
		
		/*可以选择使用lv_cn_keyboard_set_kb_type设置键盘类型，参数等于0时为英文键盘，等于1时为中文键盘*/
		//lv_cn_keyboard_set_kb_type(0);  
	}
	if(NULL != kb){
		/*使用lv_cn_keyboard_set_textarea将键盘与textarea绑定，注意是lv_cn_前缀*/
		lv_cn_keyboard_set_textarea(kb, ta);			
	}

(2) 删除键盘
	完成输入任务后，可删除键盘来释放资源
	lv_obj_del(kb);                 //删除键盘,以便释放键盘资源
    kb = NULL;

(3) 注意事项
	a. 此拼音输入法是在lv_keyboard.c基础上修改，相关函数修改为lv_cn_前缀，键盘上的“En/Cn”键用于切换英文或中文输入法
	b. lv_cn_keyboard_create函数与原lv_keyboard_create函数相比增加一个输入参数。
	lv_obj_t * lv_cn_keyboard_create(lv_obj_t * parent, const lv_font_t *cn_font)
	新增的第二个参数const lv_font_t *cn_font是中文字库，可通过lvgl官网教程制作，或者直接利用字体生成工具 Lvgl Font Tool V0.4 制作
	c. 此拼音输入法仅允许一个实例运行，暂时不支持多个实例运行。
	
5. 有关不足
tiny_pinyin.c只是一个简单、资源占有极低、轻量级拼音输入法，仅能进行单字的候选，未有多字联想候选功能
