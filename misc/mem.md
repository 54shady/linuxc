参考:Linux Device Drivers Development

PAGE_SIZE=4KB

CONFIG_PAGE_OFFSET

下图是典型的进程虚拟地址空间布局

+------------+ <-------- 0xFFFFFFFF (4GB)
|            |
| 内核地址   |
|            |
-------------- <-------- CONFIG_PAGE_OFFSET (x86: 0xC0000000, ARM: 0x8000000)
|            |
|            |
|            |
|            |
|用户空间地址|
|            |
|            |
|            |
+------------+ <-------- 0x0

其中内核地址空间又拆分为HIGHMEM(顶部的128MB)和LOWMEM(第一个896MB)

+----------------+ <--------- 0xFFFFFFFF (4GB) --
|                |                              ^
|                | 用于临时映射1GB以上的物理内存|
| ZONE_HIGHMEM   | 64位系统上不存在高端内存这一  128MB (HIGHMEM)
|                | 概念,因为2的64次方范围大,    |
|                | 拆分3G/1G没意义              V
+----------------+ <-----------------------------
|                |                              ^
|                |                              |
|                |                              |
|                |                              |
|                |                              |
| ZONE_NORMAL    |                              896MB (LOWMEM)
| 供常规使用     |                              |
|                |                              |
|                |                              |
|                |                              |
+----------------+ 3G+16MB                      |
|                |                              |
|                | LOWMEM的第一个16MB内存       |
|                | 保留为DMA使用                |
| ZONE_DMA(16MB) | 16MB                         |
|                |                              V
+----------------+ <-------- 0xC0000000 (3GB) ---

启动早期,内核永久映射这896MB内存

------


+-------------------------------+
|                               |
|        内核模块               |
|                               |
+----+------------------------+-+
     |          |             |
     |          |             |
     V          |             V
+----------+    |           +----------+
|  kmalloc |    |           | vmalloc  |
|  分配器  |    |           | 分配器   |
+----------+    |           +-|--------+
     |          |             |
     |          |             |
     V          V             |
+------------------+          |
|                  |          |
|   slab分配器     |          |
|                  |          |
+------------------+          |
     |                        |
     |                        |
     V                        V
+-------------------------------+
|                               |
|       页面分配器              |
| 物理内存按4KB,8KB等块大小分配 |
+-------------------------------+
              |
              V
+-------------------------------+
|                               |
|           主内存(物理内存)    |
|                               |
+-------------------------------+

- 页面分配器API
    alloc_page(mask, order)分配2的order次方个页面,用__free_pages释放
    mask选项如下:
    1. GFP_KERNEL: 用于内核分配,不能在中断处理程序中使用,因为可能会睡眠
    2. GFP_USER: 用于用户内存分配
    3. GFP_HIGHMEM: 从HIGH_MEM区域分配
    4. GFP_ATOMIC: 以禁止休眠的原子方式分配,用于中断上下文中使用


	alloc_pages(gfp_t gfp_mask, unsigned int order)
		alloc_pages_current(gfp_t gfp, unsigned order)
			alloc_page_interleave(gfp_t gfp, unsigned order,
				__alloc_pages(gfp_t gfp_mask, unsigned int order, int preferred_nid)
					__alloc_pages_nodemask(gfp_t gfp_mask, unsigned int order, int preferred_nid,
						if (unlikely(order >= MAX_ORDER)) { //这里限制了分配内存的最大order
							WARN_ON_ONCE(!(gfp_mask & __GFP_NOWARN));
							return NULL;
						}

在代码~/src/linux/include/linux/mmzone.h中如果配置了CONFIG_FORCE_MAX_ZONEORDER,默认值11

     /* Free memory management - zoned buddy allocator.  */
     #ifndef CONFIG_FORCE_MAX_ZONEORDER
     #define MAX_ORDER 11
     #else
     #define MAX_ORDER CONFIG_FORCE_MAX_ZONEORDER
     #endif
     #define MAX_ORDER_NR_PAGES (1 << (MAX_ORDER - 1)) // 2的10次方 = 1024, 最大分配1024个页面

如果页大小是4KB的话, 最大能分配的连续物理内存就是4KB * 1024 = 4MB
因为kmalloc在底层是基于页分配器的,所以最大也是分配4MB连续的物理内存

- slab分配器(是kmalloc依赖的分配器)
	其目的主要是消除小内存分配情况下由伙伴系统引起的内存分配和释放造成的碎片

内核里有不同种类的slab分配器

	1. SLOB : 尽可能紧凑
	2. SLAB : 尽可能缓存友好
	3. SLUB : 很简单,需要较少的指令成本

- kmalloc分配器

	1. 返回的内存在物理内存和虚拟内存中是连续的
	2. 没有指定的情况下是从LOW_MEM区域分配
	3. 每一次分配的最大尺寸是4MB,总的分配大小为128MB

为什么要使用kzalloc

	kmalloc返回的内存保留了其以前的内容,因此如果将它提供给用户空间,则可能会有安全风险
	需要将kmalloc分配的内容初始化为零,应该使用kzalloc

为数组分配内存使用kcalloc

- vmalloc分配器

分配的内存在物理上不是连续的,返回的内存始终来自HIGH_MEM
返回的地址不能像使用kmalloc分配的调用virt_tophys或virt_to_bus来转换

可以使用/proc/vmallocinfo 显示系统上vmalloc分配的所有内存
