#include <float2byte.h>
//����˵�����������ȸ�������ת��4�ֽ����ݲ�����ָ����ַ 
//����˵�����û�����ֱ�Ӳ����˺��� 
//target:Ŀ�굥��������
//buf:��д������
//beg:ָ��������ڼ���Ԫ�ؿ�ʼд��
//�����޷��� 

/**
  * @brief  �������ȸ�������ת��4�ֽ����ݲ�����ָ����ַ 
  * @param  src:Ŀ�굥��������
            des:��д������
            beg:ָ��������ڼ���Ԫ�ؿ�ʼд��
  * @retval None
  */
void float2byte(float *src, unsigned char *des, unsigned char beg)
{
    unsigned char *point;
    point = (unsigned char*)src;	  //�õ�float�ĵ�ַ
    des[beg]   = point[0];
    des[beg+1] = point[1];
    des[beg+2] = point[2];
    des[beg+3] = point[3];
}

/**
  * @brief  ��4�ֽ�����ת�ɵ����ȸ������ݲ�����ָ����ַ 
  * @param  src:Ŀ��4�ֽ����ݵ�ַ
			beg:ָ��������ڼ���Ԫ�ؿ�ʼд��
            des:��д�뵥���ȸ����ַ
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
