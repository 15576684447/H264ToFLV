#ifndef XIAOC_H264TOFLV_H
#define XIAOC_H264TOFLV_H
//����  �����  20160614  xiaoc@pku.edu.cn

typedef struct flv_param
{
    /* ������Ϣ
    -v ���� �磺-v brazil-bq.264
    -a ���� �磺-a brazil-bq.aac
    -t ���� �磺-t brazil-bq.log
    -o ���� �磺-v brazil-bq.flv
    -f ���� �磺-f 25
    **/
    int  b_log;   //�Ƿ��ӡlog��ϸ��Ϣ        -t ���� �磺-t brazil-bq.log
    int  i_fps;   //flv��fps��Ϣ               -f ���� �磺-f 25
    int  b_audio; //�Ƿ�����Ƶ��Ϣ             -a ���� �磺-a brazil-bq.aac
	int  b_video; //�Ƿ�����Ƶ��Ϣ             -a ���� �磺-v brazil-bq.264

}flv_param;

/* ����flv��Ĭ�ϲ��� */
void flv_param_default(flv_param *param);

/* �������ò��� */
bool parse(flv_param *param,int argc, char **argv);

#endif