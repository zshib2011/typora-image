/****************************************Copyright (c)**************************************************
**                               Honestar Technology Co.,LTD.
**
**                                 http://www.honestar.com
**
**--------------File Info-------------------------------------------------------------------------------
** File Name:           at_utils.c
** Last modified Date:  2020/10/03
** Last Version:        V1.0
** Description:         AT command utils
**
**------------------------------------------------------------------------------------------------------
** Created By:          Alan Lan
** Created date:        2020/10/03
** Version:             V1.0
** Descriptions:        Initial Version
**
**------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
********************************************************************************************************/
#include "at_utils.h"
#include "at.h"
#include <stdlib.h>
#include <stdio.h>


/*********************************************************************************************************
** Global variable
*********************************************************************************************************/



/**
 * dump hex format data to console device
 *
 * @param name name for hex object, it will show on log header
 * @param buf hex buffer
 * @param size buffer size+
 */
void at_print_raw_cmd(const char *name, const char *buf, rt_size_t size)
{
/* if you will use this function,that you can achieve it*/
#if 0    
#define __is_print(ch)       ((unsigned int)((ch) - ' ') < 127u - ' ')
#define WIDTH_SIZE           32

    rt_size_t i, j;
    
    for (i = 0; i < size; i += WIDTH_SIZE)
    {
        NRF_LOG_INFO("[D/AT] %s: %04X-%04X: ", name, i, i + WIDTH_SIZE);
			
        for (j = 0; j < WIDTH_SIZE; j++)
        {
            if (i + j < size)
            {
                NRF_LOG_INFO("%02X ", buf[i + j]);
            }
            else
            {
                NRF_LOG_INFO("   ");
            }
            if ((j + 1) % 8 == 0)
            {
                NRF_LOG_INFO(" ");
            }
        }
        NRF_LOG_INFO("  ");
				
        for (j = 0; j < WIDTH_SIZE; j++)
        {
            if (i + j < size)
            {
                NRF_LOG_INFO("%c", __is_print(buf[i + j]) ? buf[i + j] : '.');
            }
        }
        NRF_LOG_INFO("\n");
    }
#endif
}

//const char *at_get_last_cmd(rt_size_t *cmd_size)
//{
//    *cmd_size = last_cmd_len;
//    return send_buf;
//}

rt_size_t at_vprintf(rt_device_t device, const char *format, va_list args)
{
     char send_buf[256];
     rt_size_t last_cmd_len = 0;
    
    last_cmd_len = vsnprintf(send_buf, sizeof(send_buf), format, args);

#ifdef AT_PRINT_RAW_CMD
    at_print_raw_cmd("send", send_buf, last_cmd_len);
#endif

    return at_device_write(device, 0, send_buf, last_cmd_len);
}

rt_size_t at_vprintfln(rt_device_t device, const char *format, va_list args)
{
    rt_size_t len;

    len = at_vprintf(device, format, args);

    at_device_write(device, 0, "\r\n", 2);
	
    return len + 2;
}


uint8_t ucCHAR2BIN( uint8_t ucCharacter )
{
    if( ( ucCharacter >= '0' ) && ( ucCharacter <= '9' ) )
    {
        return ( uint8_t )( ucCharacter - '0' );
    }
    else if( ( ucCharacter >= 'A' ) && ( ucCharacter <= 'F' ) )
    {
        return ( uint8_t )( ucCharacter - 'A' + 0x0A );
    }
    else if( ( ucCharacter >= 'a' ) && ( ucCharacter <= 'f' ) )
    {
        return ( uint8_t )( ucCharacter - 'a' + 0x0a );
    }    
    else
    {
        return 0x00;
    }
}



at_result_t at_get_uuid_type(const char *args, const uint8_t *base_uuid, uint8_t *uuid, uint32_t *uuid_type)
{
#if 0    
    int argc;
    uint32_t type=0;

    uint8_t gig_base_uuid[]=SIG_BASE_UUID;

    char uuid_str[16 * 2 + 1];
    uint8_t uuid_int[16 + 1];
    
    const char *req_expr = "=%32[0123456789abcdefABCDEF],%d";

    memset(uuid_str, 0, sizeof(uuid_str));
    memset(uuid_int, 0, sizeof(uuid_int));    
 
    argc = at_req_parse_args(args, req_expr, &uuid_str, &type);



    for(uint32_t i = 0; i < strlen(uuid_str); i += 2)
    {
        uuid_int[i / 2] = (ucCHAR2BIN(uuid_str[i]) << 4 ) | (ucCHAR2BIN(uuid_str[i + 1]));
    }
        

    uint16_t short_uuid = uuid_int[0]<<8|uuid_int[1];
    
    if(argc == 1 || (argc == 2 && type == UUID_TYPE_16BIT_BASE_SERVICE))
    {
        if(strlen(uuid_str)!=4)
        {
            return AT_RESULT_PARSE_FAILE;
        }
        
        *uuid_type = UUID_TYPE_16BIT_BASE_SERVICE;
        memcpy(uuid, base_uuid, 16); 
        memcpy(&uuid[12], &short_uuid, 2); 
    }
    else if(argc == 2 && type == UUID_TYPE_16BIT_BASE_SIG)
    {
        if(strlen(uuid_str)!=4)
        {
            return AT_RESULT_PARSE_FAILE;
        } 
        
        *uuid_type = type;
        memcpy(uuid, gig_base_uuid, 16); 
        memcpy(&uuid[12], &short_uuid, 2);                
    }
    else if(argc == 2 && type == UUID_TYPE_128BIT)
    {
        if(strlen(uuid_str)!=16*2)
        {
            return AT_RESULT_PARSE_FAILE;
        } 
        
        *uuid_type = type;
        memcpy(uuid, uuid_int, 16);    
    }
    else         
    {
        return AT_RESULT_PARSE_FAILE;
    }

#endif
    
    return AT_RESULT_OK;    
}


/*********************************************************************************************************
** End Of File
*********************************************************************************************************/
