//����  �����  20160617  xiaoc@pku.edu.cn
#include "encapsulation.h"

EncaFlv::EncaFlv()
{
	m_param          = NULL;
}

void EncaFlv::create(flv_param *param)/* �����ڴ� */
{
	bool ok = true;
	m_param = param;
}
void EncaFlv::destory()/* �ͷ��ڴ� */
{

}

/* ��װflv */
void EncaFlv::encapsulate()
{

}