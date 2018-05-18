#include "cli.c"

int main(int argc, char *argv[])
{
  int i;
  char *hostname;
  REPORT *temp;
  hostname = (argc > 1) ? argv[1] : "cs.indstate.edu";
  port=(argc==3) ? atoi(argv[2]) : DEFAULTPORT;
  local=letsetup(hostname, 7305);
  if(local==-1){
    printf("Could not connect, try again later!\n");
    exit(0);
  }
  else {
    //rand_values();
    while(1)
    {
      //revRep();
      
      callSend();
      usleep(500000);
      //for(i=0; i<10; i++)
      //{
      //  usleep(10000);
      temp=revRep();
      if (temp!=NULL)
        printf("%d %.2f %.2f %.2f %.2f\n", temp->id, temp->x, temp->y,temp->z, temp->h);
             free(temp);
     // }
      usleep(500000);
    }
  }
  return 0;
}
