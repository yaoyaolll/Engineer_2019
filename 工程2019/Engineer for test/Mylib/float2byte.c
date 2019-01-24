#include <float2byte.h>
//函数说明：将单精度浮点数据转成4字节数据并存入指定地址 
//附加说明：用户无需直接操作此函数 
//target:目标单精度数据
//buf:待写入数组
//beg:指定从数组第几个元素开始写入
//函数无返回 

/**
  * @brief  将单精度浮点数据转成4字节数据并存入指定地址 
  * @param  src:目标单精度数据
            des:待写入数组
            beg:指定从数组第几个元素开始写入
  * @retval None
  */
void float2byte(float *src, unsigned char *des, unsigned char beg)
{
    unsigned char *point;
    point = (unsigned char*)src;	  //得到float的地址
    des[beg]   = point[0];
    des[beg+1] = point[1];
    des[beg+2] = point[2];
    des[beg+3] = point[3];
}

/**
  * @brief  将4字节数据转成单精度浮点数据并存入指定地址 
  * @param  src:目标4字节数据地址
			beg:指定从数组第几个元素开始写入
            des:待写入单精度浮点地址
  * @retval None
  */
void byte2float(unsigned char *src, unsigned char beg, float *des)
{
	unsigned char *point;
	point = (unsigned char*)des;
	point[0] = src[beg];
	point[1] = src[beg+1];
	point[2] = src[beg+2];
	point[3] = src[beg+3];
}
