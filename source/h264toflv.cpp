//����  �����  20160614  xiaoc@pku.edu.cn
#include <stdlib.h>
#include "h264toflv.h"
#include "flvfile.h"
#include "encapsulation.h"

int main(int argc, char **argv)
{
    flv_param m_param; //���ò�����Ϣ
    bool ret = false;

    EncaFlv *encflv = new EncaFlv();

    flv_param_default(&m_param);//����Ĭ�ϲ���
    ret = parse(&m_param,argc,argv);  //��������
    if(!ret) //�������ô���
    {
        system("pause");
        return 0;
    }

    openFlvFiles();//����ض�д�ļ�

    encflv->create(&m_param); //�����ڴ�
    encflv->encapsulate();//����flv�ļ�
    encflv->destory();//�ͷ��ڴ�

    delete encflv;
    closeFlvFiles();//�ر���ض�д�ļ�
    system("pause");
    return 0;
}