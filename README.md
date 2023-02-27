#一个简单的模拟 MIPS 指令运行的小程序
#因为对MIPS有一点了解，所以使用 MIPS 的指令格式和译码方式，后续可以再更改其他指令集

#取指

#可以做到读取 txt 文件中指定行（对应可以进行PC的计算，可轻松添加 J 型指令）的指令，但方法比较笨，开销大，可以优化


#译码

#文件夹中 I.txt 中是二进制 MIPS 机器码，目前只写了 R 型的两个指令，格式是
#OP（6 bit）+ R1 (5 bit)+ R2（5 bit）+R3（结果寄存器，5 bit）
#每种指令的译码方式不同，可能要找标准的MIPS指令集看一下三种指令怎么译码更高效


#执行

#通过 op switch一下，目前仅支持op=000000的ADD和op=000001的SUB，但是我瞎编的op


#访存

#目前不需要


#写回

#也许需要在这个阶段更改PC的值，后期再改吧
#现在是将计算结果写回模拟寄存器


