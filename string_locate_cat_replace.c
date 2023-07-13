#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*用于#if*/
#define func(A, B)              (A | B)

#define NUMBER                  5
#define NUMBER_MAX              100   //一行最大的字符数量
// #define _DEBUG_
#ifdef _DEBUG_
#define DBG_PRINTF(...)             printf(_VA_ARGS_)
#else
#define DBG_PRINTF(...)
#endif

#define TEST_FILE_READ_STATUS  0

/*定位*/
int get_para_from_xml(char* source, char (*result)[32])
{
    /*目标字符数组*/
	char* destination[] = 
	{
		"videoCodecType>",
		"constantBitRate>",
		"maxFrameRate>",
		"videoResolutionWidth>",
		"videoResolutionHeight>"
	};

	int   i                     = 0;
	char* p                     = NULL;
	int   lenth                 = 0; 

	for(i = 0; i < NUMBER; i++)
	{
        /*在 source 中定位到 destination[i] 最后一个字符*/
		p = strstr(source, destination[i]);
        /*p指针定位成功*/
		if (NULL != p)
		{
            /*数一下从 destination[i] 最后一个字符 到下一个 '<' 的字符数量*/
			while (*p  != '<')
			{
				lenth++;
				p++;
			}
            /*起始地址：p - lenth  长度：lenth   内容copy到新数组中*/
			memcpy(result[i], p - lenth, lenth);
            /*新增最后一个字符'\0'*/
			result[i][lenth] = '\0';

			result[i][strlen(destination[i]) - 1] = ':';
            /*将长度归零、指针赋值为空*/
			lenth = 0;
			p = NULL;
		}
        /*将copy的结果打印出来*/
		printf("%s\n", result[i]);
	}

	return 0;
}

/*定位 + 粘贴*/
int modify_para_to_xml(char* recv_buf, char* recv_buf_modify)
{
	char  recv_buf_rear[4096] = {'\0'};

	/*需要修改的定位*/
	char* destination_original  = "videoCodecType>";
	/*修改后的数据*/
	char* destination_modify    = "H.265";

	int   i                     = 0;
	char* p                     = NULL;
	int   lenth                 = 0; 
	/*计算字符串长度*/
	int   string_length         = strlen(recv_buf);

	/*定位目标字符串在文件中起始位置*/
	p = strstr(recv_buf, destination_original);
	/*将前半部分复制给新的modify数组*/
	strncpy(recv_buf_modify, recv_buf, p - recv_buf + strlen(destination_original));
	/*将替换部分复制给modify数组*/
	strcat(recv_buf_modify, destination_modify);
	/*计算替换字符串长度*/
	if (NULL != p)
	{
		while (*p  != '<')
		{
			lenth++;
			p++;
		}
	}

	int j = 0;
	/*将后半部分复制到新数组*/
	for(i = p - recv_buf; i < string_length; i++)
		recv_buf_rear[j++] = recv_buf[i];
	/*将新数组复制给modify数组*/
	strcat(recv_buf_modify, recv_buf_rear);

	return 0;
}

int main()
{
	int ret = 0, i = 0;
	char line_buff[NUMBER_MAX];
	char xml_ori[4096];
	char result[NUMBER][32];

	memset(line_buff, 0, sizeof(line_buff));
	memset(xml_ori, 0, sizeof(xml_ori));

	/*打开文件*/
	FILE *fp = fopen("stream.xml", "r");
	if(NULL == fp)
		printf("open file stream.xml: FAIL\n");
	/*将字符读入line_buff中,再拼接到xml_ori数组内*/
	while(NULL != fgets(line_buff, NUMBER_MAX, fp))
		strcat(xml_ori, line_buff);

#if TEST_FILE_READ_STATUS
	/*另外一种方式把文件读入数组*/
	char xml_read[4096];
	memset(xml_read, 0, sizeof(xml_read));

	fseek(fp, 0, SEEK_SET);//SEEK_SETk开头，SEEK_CUR当前位置，SEEK_END结尾， 0表示向后偏移量
	fread(xml_read, sizeof(char), sizeof(xml_read), fp);

	for(i = 0; i < NUMBER_MAX; i++)
	{
		if(xml_ori[i] != xml_read[i])
			printf("err\n");
	}
#endif
	fclose(fp);

	/*从xml中获取指定参数,结果存入result*/
	get_para_from_xml(xml_ori, result);


	char xml_modify[4096];
	memset(xml_modify, 0, sizeof(xml_modify));
	/*从数组中修改指定参数，结果存入*/
	modify_para_to_xml(xml_ori, xml_modify);
	return 0;
}
