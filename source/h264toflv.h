#ifndef XIAOC_H264TOFLV_H
#define XIAOC_H264TOFLV_H
//����  �����  20160614  xiaoc@pku.edu.cn

typedef struct flv_param
{
    /* ������Ϣ
    -i ���� �磺-i brazil-bq.264
    -t ���� �磺-t brazil-bq.log
    -o ���� �磺-v brazil-bq.flv
    -f ���� �磺-f 25
    **/
    int  b_tag;   //�Ƿ��ӡlog��ϸ��Ϣ        -t ���� �磺-t brazil-bq.log
    int  i_fps;   //flv��fps��Ϣ               -f ���� �磺-f 25

}flv_param;

/* ����flv��Ĭ�ϲ��� */
void flv_param_default(flv_param *param);

/* �������ò��� */
bool parse(flv_param *param,int argc, char **argv);

#endif