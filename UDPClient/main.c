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


#include <pthread.h>

#define PORT 5000 /* Open Port on Remote Host */
#define MAXDATASIZE 100 /* Max number of bytes of data */

long g_send_num = 0, g_recv_num = 0;
pthread_mutex_t  g_num_mutex;

static void* send_run(void* p)
{

    int fd, numbytes; /* files descriptors */
    char recvbuf[MAXDATASIZE]; /* buf will store received text */
    char sendbuf[MAXDATASIZE];
    struct hostent *he; /* structure that will get information about remote host */
    struct sockaddr_in server,client; /* server's address */

    if ((he=gethostbyname("127.0.0.1"))==NULL){ /* calls gethostbyname() */
        printf("gethostbyname() error\n");
        exit(1);
    }

    
    if ((fd=socket(AF_INET, SOCK_DGRAM, 0))==-1){ /* calls socket() */
        printf("socket() error\n");
        exit(1);
    }
    
    bzero(&server,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT); /* htons() is needed again */
    server.sin_addr = *((struct in_addr *)he->h_addr); /*he->h_addr passes "*he"'s info to "h_addr" */
    

/*	
        client.sin_family = AF_INET;
        client.sin_port = htons(5050);
        client.sin_addr.s_addr = INADDR_ANY;
        bzero(&(client.sin_zero),8);

        if (bind(fd,(struct sockaddr *)&client,
            sizeof(struct sockaddr)) == -1)
        {
            perror("Bind\n");
            exit(1);
        }
*/
	socklen_t len;
    	len=sizeof(struct sockaddr_in);


	//set recv out time
	struct timeval tv_out;
        tv_out.tv_sec = 1;
        tv_out.tv_usec = 0;
	setsockopt(fd,SOL_SOCKET,SO_RCVTIMEO,&tv_out, sizeof(tv_out));
	int brecv = 0;
	
	memset(sendbuf, 0, MAXDATASIZE);    
	sprintf(sendbuf, "001|5358e7db0646f04a820bcb20ebc2e7818a70|1#");

	while (1) {
        sendto(fd,sendbuf,strlen(sendbuf),0,(struct sockaddr *)&server,len);
	

	brecv = 0;
	
	if ((numbytes=recvfrom(fd,recvbuf,MAXDATASIZE,0,(struct sockaddr *)&server,&len)) == -1){ /* calls recvfrom() */
              //printf("recvfrom() error\n");
             // exit(1);
        }else{
		brecv = 1;
	}

	pthread_mutex_lock(&g_num_mutex);
	if(brecv) ++g_recv_num;
	++g_send_num;
	pthread_mutex_unlock(&g_num_mutex);

        recvbuf[numbytes]='\0';
        printf("{%ld : %ld}Server Return Reverse Message: %s\n", g_send_num, g_recv_num, recvbuf); /* it prints server's welcome message */

	}
    
    close(fd); /* close fd */
    return NULL;
}


int threadnum = 50;

int main(int argc, const char * argv[])
{
    pthread_t threads[threadnum];
    int i = 0;
    
	pthread_mutex_init(&g_num_mutex, NULL);
    for (i = 0; i < threadnum; ++i) {
        if(pthread_create(&threads[i],NULL,send_run, NULL)){
            printf("pthread_create error.\n");
        }
    }
    
    while (1) {
        sleep(1000);
    }
    
    
    return 0;
}

















