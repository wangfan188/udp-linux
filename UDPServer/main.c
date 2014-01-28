#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> /* netbd.h is needed for struct hostent =) */
#include <locale.h>
#include <arpa/inet.h>
#include <string.h>
#include <asm/ioctl.h>


#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/time.h>  
#include <sys/stat.h> 
#include <fcntl.h> 

#include <pthread.h>

#include "xlist.h"


#define PORT 8668 /* Open Port on Remote Host */
#define MAXDATASIZE 100 /* Max number of bytes of data */


xlist* g_clientlist; /*client list.*/
int g_fdlisten; /*listen socket.*/
typedef struct stru_clientinfo{
	char ip[64];
	int port;
}stru_clientinfo;  


static void* send_run(void* p)
{

    	int numbytes; /* files descriptors */
    	char sendbuf[MAXDATASIZE];
    	struct hostent *he; /* structure that will get information about remote host */
    	struct sockaddr_in client; /* server's address */

	sprintf(sendbuf, "ok.");
	printf("send thread start.");
    	while (1) {
		
		if(xlist_getcout(g_clientlist)<=0){
			sleep(1);
			continue;
		}

		xnode* node = xlist_pop(g_clientlist);
		if(!node){ continue; }

	        bzero(&client,sizeof(client));
        	client.sin_family = AF_INET;
        	client.sin_port = ((stru_clientinfo*)node->data)->port; /* htons() is needed again */
        	client.sin_addr.s_addr = inet_addr(((stru_clientinfo*)node->data)->ip); /*he->h_addr passes "*he"'s info to "h_addr" */
        	socklen_t len;
        	len=sizeof(struct sockaddr_in);

		printf("sendto %s:%d\n", ((stru_clientinfo*)node->data)->ip, ((stru_clientinfo*)node->data)->port);		

        	sendto(g_fdlisten,sendbuf,strlen(sendbuf),0,(struct sockaddr *)&client,len);
		
		xlist_deletenode(node);
	}
    
    
   	 return NULL;
}

long g_num = 0;

void *recv_run(void* p)
{
        int addr_len, bytes_read;
        char recv_data[2048];
        struct sockaddr_in server_addr , client_addr;
	fd_set fds;
	struct timeval timeout={3,0};
	int nret;	

        if ((g_fdlisten = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
            perror("Socket\n");
            exit(1);
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(5000);
        server_addr.sin_addr.s_addr = INADDR_ANY;
        bzero(&(server_addr.sin_zero),8);
/*
	int flags = fcntl(g_fdlisten, F_GETFL, 0);
	fcntl(g_fdlisten, F_SETFL, flags | O_NONBLOCK);
*/
        if (bind(g_fdlisten,(struct sockaddr *)&server_addr,
            sizeof(struct sockaddr)) == -1)
        {
            perror("Bind\n");
            exit(1);
        }

        addr_len = sizeof(struct sockaddr);
  
	printf("UDPServer Waiting for client on port 5000\n");
        /*fflush(stdout);*/

	while (1)
 	{
/*
		FD_ZERO(&fds);  
          	FD_SET(g_fdlisten,   &fds);  
          	timeout.tv_sec   =   0;  
          	timeout.tv_usec   =	1;   

		nret = select(g_fdlisten+1, &fds, NULL, NULL, &timeout);

		if(nret == -1){
			printf("select error...\n");
			continue;
		}else if(nret < 0){
			printf("outtime.\n");
			continue;	
		}

		if(FD_ISSET(g_fdlisten,&fds) <= 0){printf("nothing..\n");continue;}
*/
        	bytes_read = recvfrom(g_fdlisten,recv_data,2048,0,
                     (struct sockaddr *)&client_addr, &addr_len);
  
		if(bytes_read <=0){
			continue;
		}
/*
		xnode* node = xlist_createnode();
		char* str_ip = inet_ntoa(client_addr.sin_addr);
		if(node){
			stru_clientinfo* clientinfo = (stru_clientinfo*)malloc(sizeof(stru_clientinfo));
			char*	netdata = (char*)malloc(2048);
			if(clientinfo && netdata){
				memcpy(netdata, recv_data, bytes_read);
				sprintf(clientinfo->ip, "%s", str_ip);
				clientinfo->port = client_addr.sin_port;
				node->data = clientinfo;
				node->expand = (void*)netdata;
				xlist_push(g_clientlist, node);
				
				sendto(g_fdlisten,"ok.",strlen("ok."),0,(struct sockaddr *)&client_addr,addr_len);
			}else{
				if(clientinfo) free(clientinfo);
				if(netdata) free(netdata);
				xlist_deletenode(node);
			}			
		}		
*/
		++g_num;
		char* str_ip = inet_ntoa(client_addr.sin_addr);	
		sendto(g_fdlisten,"ok.",strlen("ok."),0,(struct sockaddr *)&client_addr,addr_len);
          	printf("(%s , %d) said : %s  : %ld\n",str_ip, client_addr.sin_port, recv_data, g_num);
   		/*fflush(stdout);*/
        }	
}


int threadnum = 1;

int main(int argc, const char * argv[])
{
	/*create client list.*/
	g_clientlist = xlist_create();
	if(!g_clientlist){
		printf("create client list error.");
		return;
	}
	
	/*setup recv thread*/
	pthread_t recv_thread;
	if(pthread_create(&recv_thread, NULL, recv_run, NULL)){
		printf("create recv thread error.\n");
	}
/*
	pthread_t threads[threadnum];
    	int i = 0;
    
    	for (i = 0; i < threadnum; ++i) {
        	if(pthread_create(&threads[i],NULL,send_run, NULL)){
        	    	printf("pthread_create error.\n");
        	}
    	}
  */  
   	while (1) {
        	sleep(1000);
    	}
    
    
    	return 0;
}

















