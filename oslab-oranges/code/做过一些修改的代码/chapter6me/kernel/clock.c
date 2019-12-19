
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                               clock.c
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
                           clock_handler
 *======================================================================*/
PUBLIC void clock_handler(int irq)
{
	ticks++;
	p_proc_ready->ticks--;
	p_proc_ready->cnt++;

	if (k_reenter != 0) {
		return;
	}
/*
	if (p_proc_ready->ticks > 0) {
		return;
	}
*/
	if(p_proc_ready->ticks<1){   /*程序执行完毕，设置为第三队列执行完毕。这样方便后续处理。*/
		p_proc_ready->tflag=3;	/*这里用<1不用=0是因为可能因为中断重入0--
		p_proc_ready->cnt=15;
	}
	if(p_proc_ready->tflag==1)
		if(p_proc_ready->cnt<5)
			return;
	if(p_proc_ready->tflag==2)
		if(p_proc_ready->cnt<10)
			return;
	if(p_proc_ready->tflag==3)
		if(p_proc_ready->cnt<15)
			return;
	p_proc_ready->cnt=0;
	if(p_proc_ready->tflag!=3)
		p_proc_ready->tflag++;
	schedule();

}

/*======================================================================*
                              milli_delay
 *======================================================================*/
PUBLIC void milli_delay(int milli_sec)
{
        int t = get_ticks();

        while(((get_ticks() - t) * 1000 / HZ) < milli_sec) {}
}

