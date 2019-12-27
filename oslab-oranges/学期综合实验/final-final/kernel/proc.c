
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                               proc.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "string.h"
#include "proc.h"
#include "global.h"

/*======================================================================*
                              schedule
 *======================================================================*/
PUBLIC void schedule()
{
	PROCESS* p;
	int	 greatest_ticks = 0;
	int  smallest_tflag = 5;
	int  stat=0;
	int  complish=0; /*统计完成个数，若全部完成则重新赋值，重新开始*/
	for (p = proc_table; p < proc_table+NR_TASKS; p++) {
			if (p->tflag==3) {
				stat++;
			}
		}
	if(stat==NR_TASKS){    /*全部进入第三队列，时间片轮转*/
	do{
	complish++;
	if(complish==6){
		disp_str("Finished !");
		break;
	}
	p_proc_ready++;
	if(p_proc_ready>=proc_table+NR_TASKS)
			p_proc_ready=proc_table;
	}while(p_proc_ready->ticks<1);
	}
	else{
	int jk=0;
	p=p_proc_ready;				//从当前ready的下一位开始遍历数组;
	do{							// 这样是为了实现单个队列的先来先服务
	jk++;
	p++;
	if(p>=proc_table+NR_TASKS)
			p=proc_table;
	if (p->tflag < smallest_tflag) {                
				smallest_tflag = p->tflag;
				p_proc_ready = p;
			}
	
	}while(jk<=5);
	}
	
	if(complish==6)  /*全部完成，重新初始化*/
	{
		for (p = proc_table; p < proc_table+NR_TASKS; p++) {
			p->tflag=1;
			p->ticks=p->priority;
			}
		p_proc_ready=proc_table;
	}

}

/*======================================================================*
                           sys_get_ticks
 *======================================================================*/
PUBLIC int sys_get_ticks()
{
	return ticks;
}

PUBLIC int sys_get_crc()  
{  
	PROCESS* tp=p_proc_ready; //当前项指针
	int start;//进程表一项的开始地址
	int end; //;/进程表一项的结束地址
	int size; //进程表一项的大小 
	start=&proc_table[1];
	end=&proc_table[0];
	size=start-end;
	u16 crc=0;
	u8* addr=tp;   //addr当前字节的地址指针 *addr++ *addr addr++
	int i;
    for (; size > 0; size--)              /* Step through bytes in memory */  
    {  
        crc = crc ^ (*addr++ << 8);     /* Fetch byte from memory, XOR into CRC top byte*/  
        for (i = 0; i < 8; i++)             /* Prepare to rotate 8 bits */  
        {  
            if (crc & 0x8000)            /* b15 is set... */  
                crc = (crc << 1) ^ 0x1021;    /* rotate and XOR with polynomic */  
            else                          /* b15 is clear... */  
                crc <<= 1;                  /* just rotate */  
        }                             /* Loop for 8 bits */  
        crc &= 0xFFFF;                  /* Ensure CRC remains 16-bit value */  
    }                               /* Loop until size=0 */  
    return(crc);                    /* Return updated CRC */  
}  
