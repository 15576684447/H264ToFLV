//����  �����  20160615  xiaoc@pku.edu.cn

#include <stdio.h>
#include <stdlib.h>
#include "flvfile.h"
#include "h264toflv.h"

/* ��ӡhelp��Ϣ */
static void showHelp()
{
    #define H0 printf
    H0("\n-h ��ӡ����������Ϣ���˳�\n");
    H0("\n-v ����264��       �磺-v brazil-bq.264\n");
	H0("\n-a ����aac��       �磺-a brazil-bq.aac\n");
    H0("\n-t ����log         �磺-t brazil-bq.log\n");
    H0("\n-o �������yuv�ļ� �磺-v brazil-bq.flv\n");
    H0("\n-f ����fps         �磺-f 25\n");
}
/* ����flv��Ĭ�ϲ��� */
void flv_param_default(flv_param *param)
{
    param->b_log   = 1;
    param->b_video = 1;
    param->b_audio = 0;    
    param->i_fps   = 0;
}

/* �������ò��� */
bool parse(flv_param *param,int argc, char **argv)
{
    if (argc <= 1)
    {
        printf("û���κ����ò���\n");
        return false;
    }
    
    for(int i = 1; i < argc; i += 2)
    {
        if(argv[i][0] != '-')
        {
            printf("������������:%s\n",argv[i]);
            return false;
        }
        switch(argv[i][1])
        {
        case 'h':showHelp();system("pause\n");exit(0);          break;
        case 'v':flv_h264_name = argv[i+1]; param->b_video = 1; break;
        case 't':flv_log_name = argv[i+1];  param->b_log   = 1; break;
        case 'a':flv_aac_name = argv[i+1];  param->b_audio = 1; break;
        case 'o':flv_out_name = argv[i+1];  break;
		case 'f':param->i_fps = atoi(argv[i+1]);  break;
        default:printf("������������:%s\n",argv[i]); return false;
        }
    }
    return true;
}
