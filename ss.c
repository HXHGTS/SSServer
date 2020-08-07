#include <stdio.h>
#include <stdlib.h>

int run_mode;
int CreateServer(), RunServer(), StopServer(), EditConfig(), RestartServer();
FILE* config;
char server_ip[16];

int main()
{
    printf("当前系统内核版本号:\n");
    system("uname -sr");
    printf("请选择操作:\n1.搭建Shadowsocks服务器\n2.启动Shadowsocks服务器\n3.修改服务器配置\n4.重启Shadowsocks服务器\n5.关闭Shadowsocks服务器\n6.退出\n请输入:");
    scanf("%d", &run_mode);
    if (run_mode == 1) {
        KernelUpdate();
        CreateServer();
    }
    else if (run_mode == 2) {
        RunServer();
    }
    else if (run_mode == 3) {
        EditConfig();
    }
    else if (run_mode == 4) {
        RestartServer();
    }
    else if (run_mode == 5) {
        StopServer();
    }
    else if (run_mode == 6) {
        exit(0);
    }
    return 0;
}
int CreateServer() {
    system("yum -y groupinstall \"Development tools\"");
    system("yum install -y libevent libsodium pwgen");
    system("yum install -y python3-pip python3");
    system("pip3 install --upgrade pip");
    system("pip3 install greenlet");
    system("pip3 install gevent");
    system("pip3 install shadowsocks");
    system("pip3 install https://github.com/shadowsocks/shadowsocks/archive/master.zip -U");
    printf("Shadowsocks服务器搭建完成!\n");
    return 0;
}

int RunServer() {
    char passwd[30];
    int port;
    if ((fopen("ss.conf", "r")) != NULL) {
        system("ssserver -c ss.conf -d start");
        printf("Shadowsocks服务器已启动!\n");
    }
    else {
        printf("正在创建配置文件. . .\n");
        printf("请输入服务器外部访问端口:");
        scanf("%d",&port);
        printf("正在生成随机强密码. . .\n");
        system("pwgen -s 28 1 > passwd.conf");
        config = fopen("passwd.conf", "r");
        fscanf(config, "%s", passwd);
        fclose(config);
        system("yum install curl -y");
        system("curl ifconfig.me > ip.conf");
        config = fopen("ip.conf", "r");
        fscanf(config, "%s", server_ip);
        fclose(config);
        system("rm -rf passwd.conf");
        system("rm -rf ip.conf");
        config = fopen("ss.conf", "w");
        fprintf(config, "{\n");
        fprintf(config, "\"server\":\"0.0.0.0\",\n");
        fprintf(config, "\"local_address\": \"127.0.0.1\",\n");
        fprintf(config, "\"local_port\":1080,\n");
        fprintf(config, "\"port_password\":{\n");
        fprintf(config, "\"%d\":\"%s\"\n",port,passwd);
        fprintf(config, "},\n");
        fprintf(config, "\"timeout\":300,\n");
        fprintf(config, "\"method\":\"chacha20-ietf-poly1305\",\n");
        fprintf(config, "\"fast_open\":false\n");
        fprintf(config, "}\n");
        fclose(config);
        config = fopen("ss-client.conf", "w");
        fprintf(config, "{\n");
        fprintf(config, "\"server\":\"%s\",\n",server_ip);
        fprintf(config, "\"local_address\": \"127.0.0.1\",\n");
        fprintf(config, "\"local_port\":1080,\n");
        fprintf(config, "\"port_password\":{\n");
        fprintf(config, "\"%d\":\"%s\"\n", port, passwd);
        fprintf(config, "},\n");
        fprintf(config, "\"timeout\":300,\n");
        fprintf(config, "\"method\":\"chacha20-ietf-poly1305\",\n");
        fprintf(config, "\"fast_open\":false\n");
        fprintf(config, "}\n");
        fclose(config);
        printf("正在优化服务器数据吞吐量与网络连接性能. . .\n");
        system("echo \"* soft nofile 65535\" > /etc/security/limits.conf");
        system("echo \"* hard nofile 65535\" >> /etc/security/limits.conf");
        system("echo \"ulimit -n 65535\" >> /etc/rc.d/rc.local");
        system("echo \"ulimit -u 65535\" >> /etc/rc.d/rc.local");
        system("modprobe tcp_bbr");
        system("echo \"tcp_bbr\" >> /etc/modules-load.d/modules.conf");
        system("echo \"net.core.default_qdisc = fq\" >> /etc/sysctl.conf");
        system("echo \"net.ipv4.tcp_congestion_control = bbr\" >> /etc/sysctl.conf");
        system("sysctl -p");
        printf("BBR加速已启动!\n");
        printf("正在将Shadowsocks写入开机启动项. . .\n");
        system("echo \"/usr/bin/ssserver -c /root/ss.conf -d start\" >> /etc/rc.d/rc.local");
        system("chmod +x /etc/rc.d/rc.local");
        printf("将服务器DNS修改为Cloudflare DNS + Cisco OpenDNS . . .\n");
        system("echo \"DNS1=1.1.1.1\" >> /etc/sysconfig/network-scripts/ifcfg-eth0");
        system("echo \"DNS2=1.0.0.1\" >> /etc/sysconfig/network-scripts/ifcfg-eth0");
        system("echo \"DNS3=208.67.222.222\" >> /etc/sysconfig/network-scripts/ifcfg-eth0");
        system("echo \"nameserver 1.1.1.1\" > /etc/resolv.conf");
        system("echo \"nameserver 1.0.0.1\" >> /etc/resolv.conf");
        system("echo \"nameserver 208.67.222.222\" >> /etc/resolv.conf");
        printf("生成的客户端端配置如下:\n");
        printf("-----------------------------\n");
        system("cat ss-client.conf");
        printf("-----------------------------\n");
        printf("请参照上述配置修改客户端!\n");
        printf("正在重启服务器以应用配置. . .\n");
        system("reboot");
    }
    return 0;
}

int EditConfig() {
    system("ssserver -c ss.conf -d stop");
    system("vi ss.conf");
    system("ssserver -c ss.conf -d start");
    printf("生成的Server端配置如下:\n");
    printf("-----------------------------\n");
    system("cat ss.conf");
    printf("-----------------------------\n");
    printf("请参照上述配置修改客户端!\n");
    printf("Shadowsocks服务器配置已修改!\n");
    return 0;
}

int StopServer() {
    system("ssserver -c ss.conf -d stop");
    printf("Shadowsocks服务器已停止!\n");
    return 0;
}

int RestartServer() {
    system("ssserver -c ss.conf -d restart");
    printf("Shadowsocks服务器已重启!\n");
    return 0;
}

int KernelUpdate() {
    if ((fopen("preload.sh", "r")) == NULL) {
        system("yum install -y wget deltarpm");
        system("wget https://github.com/HXHGTS/WireGuardServer/raw/master/preload.sh");
        system("chmod +x preload.sh");
        printf("正在升级，将自动触发重启以应用配置. . .\n");
        system("bash preload.sh");
    }
    else {
        system("yum remove -y $(rpm -qa | grep kernel | grep -v $(uname -r))");
    }
    return 0;
}
