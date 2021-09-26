# 哈希表

![hash table](./ht.png)

- 首先分配一个指针数组,数组的每个元素是一个链表的头指针,每一个链表称为一个槽(slot)
- 哪个数据放入到哪个槽由哈希函数决定,上图中的哈希函数是h(x) = x % 11
- 如果每个槽中至多只有一个数据,那么search,insert,delete操作的时间复杂度就是O(1)
- 如果多个数据被哈希函数映射到同一个槽中,这称为碰撞(Collision),设计一个好的哈希函数可以把数据平均地分布到各个槽中,尽量避免碰撞
- 把n个数据比较均匀的分布到m个槽中,每个槽里约有n/m个数据,则search,insert,delete操作的时间复杂度都是O(n/m)

## hash collision

解决hash冲突的方法

- seprate chaining(缺点:需要通过指针或链表来操作,导致效率不高)
- open addressing(冲突后,寻找下一个slot)

其中open addressing分为下面三种

- linear probing: F(i) = i,缺点(primary clustering problem:寻找slot时产生多次的冲突)
- quadratic probing: F(i) = i*i(消除primary clustering problem), 使用更广
- double hashing: F(i) = i * hash(x) 通过再次hash来找slot

## 关于hash表大小

- hash tablesize一般是大于tablesize的下一个质数
- 在hash表半满时需要扩大hash表(两倍的原hash表)来提高查找速度

再hash的时机(交互式系统中再hash会使得系统变慢)

- 当表操作达到一定负荷时(优先考虑)
- 表半满时
- 插入元素出错时
