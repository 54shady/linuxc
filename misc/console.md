# Console

Console指的是控制台,对应的设备名是/dev/console

控制台种类

- 基于键盘+鼠标+显示器的VTConsole(内核启动参数用console=tty0指定)
- 基于串口的SerialConsole(内核启动参数用console=ttyS0,115200指定)
- 基于ip网络的NetConsole(内核启动参数用netconsole=4444@10.0.0.1/eth0)

## VTConsole

- 系统级Console(DummyConsole, VGAConsole),内核在第一时间使用的console
	比如龙芯用的是DummyConsole,所以在启动时最初阶段是没有显示的
- 模块级Console(FBConsole)
	需要等到Frame-Buffer设备注册后才能使用

FBConsole替换系统级Console的过程是内核代码里面的TakeOver

- 通过启动参数可以让/dev/console和/dev/ttyX建立联系
- 通过TakeOver可以让/dev/ttyX再与/dev/fbX建立关联

以上内容参考<用芯探核p428>
