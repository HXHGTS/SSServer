# SSServer

## 警告⚠：此技术仅限用于个人搭建游戏加速器使用！！！若用于其他违法目的，后果自负！！！

CentOS7搭建Shadowsocks并开启原版BBR

直接运行(仅支持CentOS7)：

sudo yum install -y gcc wget

wget https://github.com/HXHGTS/SSServer/raw/master/ss.c

sudo chmod +x ss.c

gcc -o ss ss.c

sudo ./ss

脚本会自动升级系统内核，重启后需要重新安装gcc，否则将无法识别脚本:

sudo yum install -y gcc

