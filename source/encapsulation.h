#ifndef XIAOC_ENCAPSULATION_H
#define XIAOC_ENCAPSULATION_H
//����  �����  20160615  xiaoc@pku.edu.cn
#include "h264toflv.h"
#include "flvfile.h"

class EncaFlv
{
public:
	flv_param* m_param;         //���ò�����Ϣ

	EncaFlv();
	void create(flv_param *param);/* �����ڴ� */
	void destory();/* �ͷ��ڴ� */

	/* ��װflv */
	void encapsulate();

};

#endif