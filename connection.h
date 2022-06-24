#pragma once

#ifdef __linux__
int getServerSock(short port);
int acceptSock(int listenfd);
int getClientSock(char* ip, short port);
void sigpipeHandler(int sig);
#endif