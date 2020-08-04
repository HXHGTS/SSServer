# SSServer

CentOS7搭建Shadowsocks并开启BBR

直接运行(仅支持CentOS7)：

sudo yum install -y gcc

wget https://github.com/HXHGTS/SSServer/raw/master/ss.c

sudo chmod +x ss.c

gcc -o ss ss.c

sudo ./ss

Kernel版本低于5的务必先进行内核升级再安装！
