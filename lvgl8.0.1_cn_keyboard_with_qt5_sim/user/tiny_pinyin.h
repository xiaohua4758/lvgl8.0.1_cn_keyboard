#ifndef _TINY_PINYIN_H_
#define _TINY_PINYIN_H_

typedef struct
{
	const char *PYList;  	//拼音检索字符串
	int			num;		//总汉字数
	const char *HZGB;		//以所定义的拼音组合的汉字GB码的指针
}PinYinDef;


//拼音组合的汉字字库
//拼音检索的结构体
struct PinYinPCB
{
	char *PYSerList;  		//拼音检索字符
	const unsigned char *PYRankOneList;  	//以所定义的拼音组合的汉字字库的指针
};

char *PYSearch(unsigned char *msg , int *num);

#endif //_TINY_PINYIN_H_
