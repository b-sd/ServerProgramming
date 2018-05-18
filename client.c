#include "mug05.h"

#define K 4096

void rand_values();
int login(char *, char *);
int login2(char *, char *);
void LineForServer();
void LineFromServer();
void callSend();
//int got_id=-1;

void rand_values()
{
 char b[K];
 int tt;
 tt=time(NULL);
 srand(tt);
   //rep[id_rep].id=++id_rep;
 rep.x=rand()%10+1;
 rep.y=rand()%10+1;
 rep.z=rand()%10+1;
 rep.h=rand()%10+1;
 //rep.id=++id_rep;
 printf("Initial %d %.2f %.2f %.2f\n", rep.id, rep.x, rep.y, rep.z);
}
void  callSend()
{
  char b[K];
  sprintf(b, "%d %.2f %.2f %.2f\n", rep.id, rep.x++, rep.y++, rep.z++);
  
  sendRep();
  b[0]=0;
  return;
}
int letsetup(char *hostname, int port){
  connected=-1;
  got_id=-1;
  char newBuff[K], *user, *pass;
  unsigned long addr;
  //char *hostname;
  struct hostent *hp;
  setbuf(stdin ,0);
  setbuf(stdout,0);
  name.sin_family = AF_INET;
  //hostname = (argc > 1) ? argv[1] : "cs.indstate.edu";
  user = getenv("GAMEUSER");
  pass = getenv("GAMEPASS");
  if(isdigit(hostname[0])){       /* insufficient! */
    addr = inet_addr(hostname);
    name.sin_addr.s_addr = addr;
  }
  else {
    hp = gethostbyname(hostname);
    if(hp == NULL){
      perror("Getting Host");
      exit(0);
    }
    //fprintf(stderr,"%08x\n", *((unsigned int *) hp->h_addr));
    name.sin_addr.s_addr = *((unsigned int *) hp->h_addr);
  }
  //port=(argc==3) ? atoi(argv[2]) : DEFAULTPORT;
  name.sin_port = htons(port);
  local=socket(AF_INET,SOCK_STREAM,0);
  if(local < 0){
    fprintf(stderr,"Opening local socket\n");
    exit(0);
  }
  if(connect(local,(struct sockaddr *)&name, sizeof(struct sockaddr_in)) < 0){
    fprintf(stderr,"Unable to connect.\n");
    return -1;
  }
  //recv(local, newBuff, 2, 0);
  //printf("ID - %s\n", newBuff);
  //rep.id=(int)newBuff[0];
  //printf("Temp ID-%d\n", rep.id);
  //got_id=0;
  if(user!=NULL && pass!=NULL)
  { 
    printf("Here\n");
    if(login(user, pass)==1){
        rand_values();
            return local;
        }
        else
            return -1;
  }
  else
  { 
  if(login2(user, pass)==1){
      rand_values();
      return local;
    }
    else
    return -1;
  }
  
}

int login(char *user, char *pass){
  char u[K], p[K], newBuff[K];
  strcpy(u, user);
  strcpy(p, pass);
  u[strlen(u)]=0;
  p[strlen(p)]=0;
  printf("%s", u);
  printf("%s", p);
  
  write(local, u, strlen(u));
  usleep(10000);
  write(local, p, strlen(p));
  while(1){
    LineFromServer();
    //LineForServer();
    if((connected==1 && got_id == 1)||connected==0){
       break;
    }
  }
  usleep(50000);
  LineFromServer();
  usleep(50000);
  LineFromServer();
 if(connected==1)
     return 1;
 else
     return -1;
} 

int login2(char *user, char *pass){
  char newBuff[K];
  while(1){
  //usleep(50000);
    LineFromServer();
    usleep(50000);
    LineForServer();
    usleep(50000);
    LineForServer();
    usleep(50000);
    if((connected==1 && got_id == 1)||connected==0){
     break;
   }
   }
  if(connected==1)
    return 1;
  else
    return -1;
}

void sendRep()
{
 //REPORT temp;
 char b[K];
 int n,err,nr;  /* nr == Number of bytes to Read */
 b[0]=0;
 err=ioctl(0,FIONREAD,&nr);
 //if(!nr) return;
 sprintf(b, "%d %.2f %.2f %.2f\n", rep.id, rep.x, rep.y, rep.z);
 //printf("%d %.2f %.2f %.2f\n", rep.id, rep.x, rep.y, rep.z);
 n = strlen(b);
 if(n<=0)
  return;
 b[n]=0;
 write(local, &rep, sizeof(rep));
 //printf("Here I sent\n");
 b[0]='\n';
}

void LineForServer()
{
 char b[K];
 int n,err,nr;  /* nr == Number of bytes to Read */
 b[0]=0;
 err=ioctl(0,FIONREAD,&nr);
 if(!nr) return;
 n=read(0,b,K-1);
 if(n<=0)
  return;
 b[n]=0;
 write(local,b,n);
}

REPORT* revRep()
{
  REPORT *temp;
  char b[K];
  int n,nr,err;
  fd_set iset;
  struct timeval zero;
  zero.tv_sec=0;
  zero.tv_usec=0;
  FD_ZERO(&iset);
  FD_SET(local,&iset);
  select(local+1,&iset,0,0,&zero);
  if(!FD_ISSET(local,&iset))
     return NULL;
  temp=malloc(sizeof(REPORT));
  if((n=read(local,temp,sizeof(REPORT))) < 0) return NULL;
  //printf("Here I received\n");
  if(n == 0){
  fprintf(stderr,"Connect closed by remote host.\n");
     exit(0);
  }
  sprintf(b, "%d %.2f %.2f %.2f\n", temp->id, temp->x, temp->y,temp->z); 
  /*temp->id;
  rep.x=temp->x;
  rep.y=temp->y;
  rep.z=temp->z;
  rep.h=temp->h;*/
  //printf("%d %.2f %.2f %.2f\n", temp->id, temp->x, temp->y,temp->z);
  if(rep.id!=temp->id)
  //b[n]=0;
  //write(0, b, n);
  
  return temp; 
  else
    return NULL;
}

void LineFromServer()
{
  char b[K], c;
  int n,nr,err;
  fd_set iset;
  struct timeval zero;
  zero.tv_sec=0;
  zero.tv_usec=0;
  FD_ZERO(&iset);
  FD_SET(local,&iset);
  select(local+1,&iset,0,0,&zero);
  if(!FD_ISSET(local,&iset))
     return;
  if((n=read(local,b,K-1)) < 0) return;
  if(n == 0){
     fprintf(stderr,"Connect closed by remote host.\n");
     exit(0);
  }
  b[n]=0;
  write(0, b, n);
  if(n<=2){
    rep.id=(int)b[0];
    printf("Got id %d, %s\n", rep.id, b);
    got_id=1;
  }
  if(strncmp(b, "Connection Authenticated and Established", 14)==0)
  {
    connected=1;
   }
  if (strncmp(b, "Invalid", 5)==0)
  {
    printf("Incorrect password and/or username!\n");
    connected=0;
    exit(0);
  }
  return;
}
