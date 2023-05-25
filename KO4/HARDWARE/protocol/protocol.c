/**
  ******************************************************************************
  * @file    protocol.c
  * @version V1.0
  * @date    2020-xx-xx
  * @brief   Ұ��PID��������ͨѶЭ�����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� F407 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

#include "protocol.h"
#include <string.h>
#include "motor.h"
#include "bsp_pid.h"
#include "control.h"


float set_point=0.0;

/*Э��֡�����ṹ��*/
struct prot_frame_parser_t
{
    uint8_t *recv_ptr;         /*���ݽ�������*/
    uint16_t r_oft;            /*��ƫ��*/
    uint16_t w_oft;            /*дƫ��*/
    uint16_t frame_len;        /*֡����*/
    uint16_t found_frame_head;
};

/*����һ��Э��֡�����ṹ��*/
static struct prot_frame_parser_t parser;
/*����һ�����ջ�����*/
static uint8_t recv_buf[PROT_FRAME_LEN_RECV];


/**
 * @brief   ��ʼ������Э��
 * @param   void
 * @return  ��ʼ�����.
 */
int32_t protocol_init(void)
{
	/*ȫ�ֱ���parser���*/
    memset(&parser, 0, sizeof(struct prot_frame_parser_t));
    
    /* ��ʼ���������ݽ��������������*/
    parser.recv_ptr = recv_buf;
  
    return 0;
}


/**
  * @brief ����У���
  * @param ptr����Ҫ���������
  * @param len����Ҫ����ĳ���
  * @retval У���
  */
uint8_t check_sum(uint8_t init, uint8_t *ptr, uint8_t len )
{
	/*У��͵ļ�����*/
	uint8_t sum = init;

	while(len--)
	{
		sum += *ptr;/*�����ۼӸ������ݵ�ֵ*/
		ptr++;
	}

	return sum;
}

/**
 * @brief   ��ȡ֡���ͣ�֡���
 * @param   *buf: ���ݻ�����
 * @param   head_oft: ֡ͷ��ƫ��λ��
 * @return  ֡���ͣ�֡���
 */
static uint8_t get_frame_type(uint8_t *buf, uint16_t head_oft)
{
	/*���㡰֡�����֡�����е�λ��*/
	uint16_t cmdIndex = head_oft + CMD_INDEX_VAL;
	
    return (buf[cmdIndex % PROT_FRAME_LEN_RECV] & 0xFF);
}

/**
 * @brief   ��ȡ֡����
 * @param   *buf: ���ݻ�����
 * @param   head_oft: ֡ͷ��ƫ��λ��
 * @return  ֡����.
 */
static uint16_t get_frame_len(uint8_t *buf, uint16_t head_oft)
{
	/*���㡰֡���ȡ���֡�����е�λ��*/
	uint16_t lenIndex = head_oft + LEN_INDEX_VAL;
	
    return ((buf[(lenIndex + 0) % PROT_FRAME_LEN_RECV] <<  0) |
            (buf[(lenIndex + 1) % PROT_FRAME_LEN_RECV] <<  8) |
            (buf[(lenIndex + 2) % PROT_FRAME_LEN_RECV] << 16) |
            (buf[(lenIndex + 3) % PROT_FRAME_LEN_RECV] << 24));    // �ϳ�֡����
}

/**
 * @brief   ��ȡcrc-16У��ֵ
 * @param   *buf:  ���ݻ�����.
 * @param   head_oft: ֡ͷ��ƫ��λ��
 * @param   frame_len: ֡��
 * @return  У��ֵ
 */
static uint8_t get_frame_checksum(uint8_t *buf, uint16_t head_oft, uint16_t frame_len)
{
	/*���㡰У��͡���֡�����е�λ��*/
	uint16_t crcIndex = head_oft + frame_len - 1;
	
    return (buf[crcIndex % PROT_FRAME_LEN_RECV]);
}

/**
 * @brief   ����֡ͷ
 * @param   *buf:  ���ݻ�����.
 * @param   ring_buf_len: ��������С����������128��
 * @param   start: ��ʼλ�ã���ƫ�ƣ�
 * @param   len: ��Ҫ���ҵĳ���
 * @return  -1��û���ҵ�֡ͷ������ֵ��֡ͷ��λ��.
 */
static int32_t recvbuf_find_header(uint8_t *buf, const uint16_t ring_buf_len, uint16_t start, uint16_t len)
{
    uint16_t i = 0;

	/*֡ͷ��4�ֽڣ���0���ҵ�len-4������ȶ�*/
    for (i = 0; i < (len - 3); i++)
    {
        if (((buf[(start + i + 0) % ring_buf_len] <<  0) |
             (buf[(start + i + 1) % ring_buf_len] <<  8) |
             (buf[(start + i + 2) % ring_buf_len] << 16) |
             (buf[(start + i + 3) % ring_buf_len] << 24)) == FRAME_HEADER) /*0x59485A53*/
        {
            return ((start + i) % ring_buf_len);
        }
    } 
    return -1;
}

/**
 * @brief   ����δ���������ݵĳ���
 * @param   frame_len: ֡���ȣ������м�¼��֡���ȣ�
 * @param   ring_buf_len: ��������С����������128��
 * @param   start: ��ʼλ�ã���ƫ�ƣ�
 * @param   end: ����λ�ã�дƫ�ƣ�
 * @return  δ���������ݳ���
 */
static int32_t recvbuf_get_len_to_parse(uint16_t frame_len, const uint16_t ring_buf_len,uint16_t start, uint16_t end)
{
    uint16_t unparsed_data_len = 0; /*δ���������ݳ���*/

	/*��ƫ��<=дƫ�ƣ�˵�������ڻ��λ��������������洢��*/
    if (start <= end)
	{
        unparsed_data_len = end - start;
	}
	/*�������ݱ��ֳ��������֣�һ�����ڻ�������β��һ�����ڻ�������ͷ*/
    else
	{
		/*��������β���ĳ��� + ��������ͷ�����ĳ���*/
        unparsed_data_len = (ring_buf_len - start) + end;
	}

    if (frame_len > unparsed_data_len)
	{
		/*�����м�¼��֡���� > δ���������ݳ���*/
        return 0;
	}
    else
	{
        return unparsed_data_len;
	}
}

/**
 * @brief   ��������д�뻺����
 * @param   *buf:  ���ݻ�����.
 * @param   ring_buf_len: ��������С����������128��
 * @param   w_oft: дƫ��
 * @param   *data: ��Ҫд�������
 * @param   data_len: ��Ҫд�����ݵĳ���
 * @return  void.
 */
static void recvbuf_put_data(uint8_t *buf, const uint16_t ring_buf_len, uint16_t w_oft, uint8_t *data, uint16_t data_len)
{
	/*Ҫд������ݳ����˻�����β*/
    if ((w_oft + data_len) > ring_buf_len)               
    {
		/*���㻺����ʣ�೤��*/
        uint16_t data_len_part = ring_buf_len - w_oft;     

        /*���ݷ�����д�뻺����*/
        memcpy((buf + w_oft), data, data_len_part); /*�Ƚ�һ����д�뻺����β*/
        memcpy(buf, (data + data_len_part), (data_len - data_len_part));/*�ٽ�ʣ�µĸ���д�뻺����ͷ*/
    }
    else
	{
        memcpy(buf + w_oft, data, data_len);/*ֱ�ӽ���������д�뻺����*/
	}
}

/**
 * @brief   Э��֡����
 * @param   *data: ���ؽ�������֡����
 * @param   *data_len: ����֡���ݵĴ�С
 * @return  ֡���ͣ����
 */
static uint8_t protocol_frame_parse(uint8_t *data, uint16_t *data_len)
{
    uint8_t frame_type = CMD_NONE;  /*֡����*/
    uint16_t need_to_parse_len = 0; /*��Ҫ������ԭʼ���ݵĳ���*/
    uint8_t checksum = 0;           /*У���*/
    
	/*����δ���������ݵĳ���*/
    need_to_parse_len = recvbuf_get_len_to_parse(parser.frame_len, PROT_FRAME_LEN_RECV, parser.r_oft, parser.w_oft);    
    if (need_to_parse_len < 9)     
	{
		/*����̫�٣��϶�������ͬʱ�ҵ�֡ͷ��֡����*/
        return frame_type;
	}

    /*��δ�ҵ�֡ͷ����Ҫ���в���*/
    if (0 == parser.found_frame_head)
    {
		int16_t header_oft = -1; /*֡ͷƫ��*/
		
        /* ͬ��ͷΪ���ֽڣ����ܴ���δ���������������һ���ֽڸպ�Ϊͬ��ͷ��һ���ֽڵ������
           ��˲���ͬ��ͷʱ�����һ���ֽڽ���������Ҳ���ᱻ����*/
        header_oft = recvbuf_find_header(parser.recv_ptr, PROT_FRAME_LEN_RECV, parser.r_oft, need_to_parse_len);
        if (0 <= header_oft)
        {
            /* ���ҵ�֡ͷ*/
            parser.found_frame_head = 1;
            parser.r_oft = header_oft;
          
            /* ȷ���Ƿ���Լ���֡��*/
            if (recvbuf_get_len_to_parse(parser.frame_len, PROT_FRAME_LEN_RECV, parser.r_oft, parser.w_oft) < 9)
			{
                return frame_type;
			}
        }
        else 
        {
            /* δ��������������Ȼδ�ҵ�֡ͷ�������˴ν���������������*/
            parser.r_oft = ((parser.r_oft + need_to_parse_len - 3) % PROT_FRAME_LEN_RECV);
            return frame_type;
        }
    }
    
    /* ����֡������ȷ���Ƿ���Խ������ݽ���*/
    if (0 == parser.frame_len) 
    {
        parser.frame_len = get_frame_len(parser.recv_ptr, parser.r_oft);
        if(need_to_parse_len < parser.frame_len)
		{
            return frame_type;
		}
    }

    /* ֡ͷλ��ȷ�ϣ���δ���������ݳ���֡�������Լ���У���*/
    if ((parser.frame_len + parser.r_oft - PROT_FRAME_LEN_CHECKSUM) > PROT_FRAME_LEN_RECV)
    {
        /* ����֡����Ϊ�����֣�һ�����ڻ�����β��һ�����ڻ�����ͷ */
        checksum = check_sum(checksum, parser.recv_ptr + parser.r_oft, PROT_FRAME_LEN_RECV - parser.r_oft);
        checksum = check_sum(checksum, parser.recv_ptr, parser.frame_len - PROT_FRAME_LEN_CHECKSUM + parser.r_oft - PROT_FRAME_LEN_RECV);
    }
    else 
    {
        /* ����֡����һ����ȡ��*/
        checksum = check_sum(checksum, parser.recv_ptr + parser.r_oft, parser.frame_len - PROT_FRAME_LEN_CHECKSUM);
    }

    if (checksum == get_frame_checksum(parser.recv_ptr, parser.r_oft, parser.frame_len))
    {
        /* У��ɹ���������֡���� */
        if ((parser.r_oft + parser.frame_len) > PROT_FRAME_LEN_RECV) 
        {
            /* ����֡����Ϊ�����֣�һ�����ڻ�����β��һ�����ڻ�����ͷ*/
            uint16_t data_len_part = PROT_FRAME_LEN_RECV - parser.r_oft;
            memcpy(data, parser.recv_ptr + parser.r_oft, data_len_part);
            memcpy(data + data_len_part, parser.recv_ptr, parser.frame_len - data_len_part);
        }
        else 
        {
            /* ����֡����һ����ȡ��*/
            memcpy(data, parser.recv_ptr + parser.r_oft, parser.frame_len);
        }
        *data_len = parser.frame_len;
        frame_type = get_frame_type(parser.recv_ptr, parser.r_oft);

        /* �����������е�����֡*/
        parser.r_oft = (parser.r_oft + parser.frame_len) % PROT_FRAME_LEN_RECV;
    }
    else
    {
        /* У�����˵��֮ǰ�ҵ���֡ͷֻ��żȻ���ֵķ�����*/
        parser.r_oft = (parser.r_oft + 1) % PROT_FRAME_LEN_RECV;
    }
    parser.frame_len = 0;
    parser.found_frame_head = 0;

    return frame_type;
}

/**
 * @brief   ���յ�������д�뻺����
 * @param   *data:  ���յ������ݵ�����.
 * @param   data_len: ���յ������ݵĴ�С
 * @return  void.
 */
void protocol_data_recv(uint8_t *data, uint16_t data_len)
{
	/*����д�뻺����*/
    recvbuf_put_data(parser.recv_ptr, PROT_FRAME_LEN_RECV, parser.w_oft, data, data_len);    
	
	/*����дƫ��*/
    parser.w_oft = (parser.w_oft + data_len) % PROT_FRAME_LEN_RECV;                          
}


/**
 * @brief   ���յ����ݴ���
 * @param   void
 * @return  -1��û���ҵ�һ����ȷ������.
 */
int8_t receiving_process(void)
{
	uint8_t frame_data[128];         // Ҫ�ܷ������֡
	uint16_t frame_len = 0;          // ֡����
	uint8_t cmd_type = CMD_NONE;     // ��������

	while(1)
	{
		/*����ָ������*/
		cmd_type = protocol_frame_parse(frame_data, &frame_len);
		
		switch (cmd_type)
		{
			/*��ָ��*/
			case CMD_NONE:
			{
				return -1;
			}

			/*����PID*/
			case SET_P_I_D_CMD:
			{
				/* ���յ�4bytes��float�͵�PID���ݺϳ�Ϊһ���� */
				uint32_t temp0 = COMPOUND_32BIT(&frame_data[13]);
				uint32_t temp1 = COMPOUND_32BIT(&frame_data[17]);
				uint32_t temp2 = COMPOUND_32BIT(&frame_data[21]);

				/*uint32_tǿ��ת��Ϊfloat*/
				float p_temp, i_temp, d_temp;
				p_temp = *(float *)&temp0;
				i_temp = *(float *)&temp1;
				d_temp = *(float *)&temp2;
				
				/*Ҫ���Ǹ�PID��ȡ���ĸ�ע��*/
        //set_p_i_d(&pid_location2, p_temp, i_temp, d_temp);    // �����Ǹ����Ǹ���ֻ��ͨ��ͨ���۲�
				//set_p_i_d(&pid_location, p_temp, i_temp, d_temp);
				set_p_i_d(&pid_speed, p_temp, i_temp, d_temp); 
				//set_p_i_d(&pid_speed2, p_temp, i_temp, d_temp); 
			}
			break;

			/*����Ŀ��ֵ*/
			case SET_TARGET_CMD:
			{
				int actual_temp = COMPOUND_32BIT(&frame_data[13]);    // �õ�����

				set_point = actual_temp;  
		     
				/*Ҫ���Ǹ�PID��ȡ���ĸ�ע��*/
				//set_pid_target(&pid_speed2, set_point);
				set_pid_target(&pid_speed, set_point);     //
				//set_pid_target(&pid_location2, set_point);     //���set_pointֵӦ����������
				//set_pid_target(&pid_location, set_point);
			}
			break;
			
			case START_CMD:
			{
				/*����pid����*/
//				TIMx_calcPID_start();
			}
			break;

			case STOP_CMD:
			{
				/*ֹͣpid����*/
			}
			break;

			case RESET_CMD:
			{
				NVIC_SystemReset();          // ��λϵͳ
			}
			break;
            /*
			case SET_PERIOD_CMD:
			{
			uint32_t temp = COMPOUND_32BIT(&frame_data[13]);     // ������
			SET_BASIC_TIM_PERIOD(temp);                             // ���ö�ʱ������1~1000ms
			}
			break;
			*/
			default: 
			return -1;
		}
	}
}

/**
  * @brief ������λ����ֵ
  * @param cmd������
  * @param ch: ����ͨ��
  * @param data������ָ��
  * @param num����������
  * @retval ��
  */
void set_computer_value(uint8_t cmd, uint8_t ch, void *data, uint8_t num)
{
	static packet_head_t set_packet;

	uint8_t sum = 0;    // У���
	num *= 4;           // һ������ 4 ���ֽ�

	set_packet.head = FRAME_HEADER;     // ��ͷ 0x59485A53
	set_packet.ch   = ch;              // ����ͨ��
	set_packet.len  = 0x0B + num;      // ����
	set_packet.cmd  = cmd;             // ��������

	sum = check_sum(0, (uint8_t *)&set_packet, sizeof(set_packet));       // �����ͷУ���
	sum = check_sum(sum, (uint8_t *)data, num);                           // �������У���

	usart1_send((uint8_t *)&set_packet, sizeof(set_packet));    // ��������ͷ
	usart1_send((uint8_t *)data, num);                          // ���Ͳ���
	usart1_send((uint8_t *)&sum, sizeof(sum));                  // ����У���
}

/**********************************************************************************************/
