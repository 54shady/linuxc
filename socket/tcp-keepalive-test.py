#!/usr/bin/env python
# coding=utf-8

import socket

# 参考https://hengyun.tech/why-we-need-heartbeat/

# 系统中默认配置在目录/proc/sys/net/ipv4下

# 编程时代码中设置参数对应关系
# TCP_KEEPIDLE(链路空闲间隔时间) 覆盖tcp_keepalive_time, 默认7200秒间隔发送一个keepalive包
# 网络联通正常情况下按照该间隔发送keepalive包

# 如果网络异常(比如断网),则每隔TCP_KEEPINTVL间隔发送发送TCP_KEEPCNT次的keepalive包
# TCP_KEEPINTVL 覆盖tcp_keepalive_intvl,默认75秒
# TCP_KEEPCNT 覆盖tcp_keepalive_probes,默认9次
# TCP_SYNCNT 覆盖tcp_syn_retries

# 1. run nc -l 9999 on server 192.168.1.101
# 2. run wireshark on client with display filter : ip.dst == 192.168.1.101 && !ssh && !vnc && tcp.port == 9999
# 3. run this script on client(192.168.1.100)
def main():
    # 执行3次握手
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    # 使能客户端发送keepalive包
    s.setsockopt(socket.SOL_SOCKET, socket.SO_KEEPALIVE, 1)

    # 设置sync包重传多少次才断开连接, TCP Restransmission次数
    # 基本上能保证是设置的值
    s.setsockopt(socket.SOL_TCP, socket.TCP_SYNCNT, 5)

    # 每隔5秒, 客户端给服务器发送一个keepalive
    s.setsockopt(socket.SOL_TCP, socket.TCP_KEEPIDLE, 5)

    # 发送keepalive probe后对方没有回应(此时可能是网络断开)
    # 后续就按间隔2秒钟开始发送keepalive probe
    # 发送多少次由TCP_KEEPCNT决定
    s.setsockopt(socket.SOL_TCP, socket.TCP_KEEPINTVL, 2)

    # 当对方没有回应后,连续发送的次数
    # 这里是连续发送9 keepalive probe后没有响应的话就发送RST来断开连接
    s.setsockopt(socket.SOL_TCP, socket.TCP_KEEPCNT, 9)

    s.connect(('192.168.1.101', 9999))
    while True:
        None

if __name__ == '__main__':
    main()
