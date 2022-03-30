# Workqueue and Tasklet

[参考文章:tasklet和workqueue ](http://blog.csdn.net/houxn22/article/details/45720247)

[参考文章:软中断,tasklet和工作队列详解](http://blog.csdn.net/godleading/article/details/52971179)

## softirq

内核中定义的softirq

	enum
	{
		HI_SOFTIRQ=0,
		TIMER_SOFTIRQ,
		NET_TX_SOFTIRQ,
		NET_RX_SOFTIRQ,
		BLOCK_SOFTIRQ,
		IRQ_POLL_SOFTIRQ,
		TASKLET_SOFTIRQ,
		SCHED_SOFTIRQ,
		HRTIMER_SOFTIRQ,
		RCU_SOFTIRQ,    /* Preferable RCU should always be the last softirq */

		NR_SOFTIRQS
	};

softirq特性

- 一个softirq不会抢占另一个softirq
- 中断才可以抢占softirq
- 同一个softirq可以在不同的cpu上同时执行(可并行执行)
- softirq必须是可重入
- 在编译期间就已经静态分配好

## tasklet(属于softirq中的特殊例子)

- 可以动态注册和清除
- 相同的tasklet不能同时执行(无法并行执行)
- tasklet始终运行在被初始提交的同一个处理器上

softirq和tasklet相同的特性

- softirq和tasklet都运行在中断上下文,因此所有的tasklet代码都必须是原子的
- 不可以阻塞,不能休眠
- 不可以被重新调度

## workqueue

- 工作队列函数在一个特殊内核进程的上下文中运行,因此它们具有更好的灵活性,尤其是工作队列函数可以休眠
- workqueue可以通过调度在不同cpu上进行切换执行(可以重新调度), 内核代码可以请求工作队列函数的执行延迟给定的时间间隔

![区别如图](./irq_buttom.png)
