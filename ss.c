﻿#include <stdio.h>
#include <stdlib.h>

int run_mode;
int CreateServer(), RunServer(), StopServer(), EditConfig(), RestartServer();
FILE* config;

int main()
{
    printf("请选择操作:\n1.升级系统内核(安装BBR必需)\n2.搭建Shadowsocks服务器\n3.启动Shadowsocks服务器\n4.修改服务器配置\n5.重启Shadowsocks服务器\n6.关闭Shadowsocks服务器\n7.退出\n请输入:");
    scanf("%d", &run_mode);
    if (run_mode == 1) {

    }
    else if (run_mode == 2) {
        CreateServer();
    }
    else if (run_mode == 3) {
        RunServer();
    }
    else if (run_mode == 4) {
        EditConfig();
    }
    else if (run_mode == 5) {
        RestartServer();
    }
    else if (run_mode == 6) {
        StopServer();
    }
    else if (run_mode == 7) {
        exit(0);
    }
    return 0;
}
int CreateServer() {
    system("yum install python python-pip libevent libsodium -y");
    system("pip install --upgrade pip");
    system("pip install greenlet");
    system("pip install gevent");
    system("pip install shadowsocks");
    printf("Shadowsocks服务器搭建完成!\n");
    return 0;
}

int RunServer() {
    char ip[16],passwd[30];
    int port;
    if ((fopen("ss.conf", "r")) != NULL) {
        system("ssserver -c ss.conf -d start");
        printf("Shadowsocks服务器已启动!\n");
    }
    else {
        printf("正在创建配置文件. . .\n");
        printf("请输入服务器ip地址:");
        scanf("%s", ip);
        printf("请输入服务器外部访问端口:");
        scanf("%d",&port);
        printf("请输入密码:");
        scanf("%s", passwd);
        config = fopen("ss.conf", "w");
        fprintf(config, "{\n");
        fprintf(config, "	\"server\":\"my_server_ip\",\n");
        fprintf(config, "	\"local_address\": \"127.0.0.1\",\n");
        fprintf(config, "	\"local_port\":1080,\n");
        fprintf(config, "	\"port_password\":{\n");
        fprintf(config, "	\"%d\":\"%s\"\n",port,passwd);
        fprintf(config, "	},\n");
        fprintf(config, "	\"timeout\":300,");
        fprintf(config, "	\"method\":\"chacha20-ietf-poly1305\",\n");
        fprintf(config, "	\"fast_open\":false\n");
        fprintf(config, "}\n");
        fclose(config);
        printf("已生成默认配置，需要修改请直接在编辑器中修改. . .\n");
        system("vi ss.conf");
        printf("正在重启服务器以应用配置. . .\n");
        system("echo \"net.core.default_qdisc = fq\" >> /etc/sysctl.conf");
        system("echo \"net.ipv4.tcp_congestion_control = bbr\" >> /etc/sysctl.conf");
        system("sysctl -p");
        printf("BBR加速已启动!\n");
        system("echo \"ssserver -c ss.conf -d start\" >> /etc/rc.d/rc.local");
        system("chmod +x /etc/rc.d/rc.local");
        system("reboot");
    }
    return 0;
}

int EditConfig() {
    system("ssserver -c ss.conf -d stop");
    system("vi ss.conf");
    system("ssserver -c ss.conf -d start");
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
    system("yum install -y wget");
    if (system("grep \"151.101.108.133 raw.githubusercontent.com\" /etc/hosts") != 0) {
        system("echo \"151.101.108.133 raw.githubusercontent.com\" >> /etc/hosts");
    }
    if (system("grep  \"52.78.231.108 github.com\" /etc/hosts") != 0) {
        system("echo \"52.78.231.108 github.com\" >> /etc/hosts");
    }
    system("echo \"52.78.231.108 github.com\" >> /etc/hosts");
    system("rm -f preload.sh");
    system("wget https://github.com/HXHGTS/WireGuardServer/raw/master/preload.sh");
    system("chmod +x preload.sh");
    system("bash preload.sh");
    printf("升级完成，正在重启服务器以应用配置. . .\n");
    system("reboot");
    return 0;
}