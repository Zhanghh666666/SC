#模拟 RISC-V 指令运行的小程序

#将 MIPS 的指令集更改成 RISC-V指令集

#取指

#可以做到读取 txt 文件中指定行（对应可以进行PC的计算，可轻松添加 J 型指令）的指令（实际上在 MPC上需要指令 ORAM 访问，同时需要安全计算 PC）


#译码/执行

#文件夹中 I.txt 中是二进制 RISC-V 机器码，目前实现了逻辑指令
#每种指令的译码方式不同，也许需要更高效的译码方式，比如产生一串二进制吗，用于后续的操作
#译码顺序是 op -> fun3 -> fun7


#访存

#目前不需要


#写回

#也许需要在这个阶段更改 PC 的值，后期再改
#现在是将计算结果写回模拟寄存器


