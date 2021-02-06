# SSServer

## 警告⚠：此技术仅限用于个人搭建游戏加速器使用！！！若用于其他违法目的，后果自负！！！

CentOS7搭建Shadowsocks并开启原版BBR

直接运行(仅支持CentOS7)：

`yum install -y gcc wget && wget https://cdn.jsdelivr.net/gh/HXHGTS/SSServer/ss.c && chmod +x ss.c && gcc -o ss ss.c && ./ss`

第一次点击安装会升级内核，触发重启，重启后请输入

`./ss`

并选1继续安装!
