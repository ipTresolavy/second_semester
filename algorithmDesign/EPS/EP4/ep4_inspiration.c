#include <stdlib.h> 
#include <string.h> 
/* #include <time.h>  */
#include <stdio.h>

struct list 
{
  char  *val;
  struct list *next;
};
struct list *root;    
struct list *head;
FILE *f;
int node_counter=0;
 
int main(char *argv[],int argc)
{
 int i;
  
 i=init_list();
 open_file();
 print_list(head);
 return(0);
  
}  
 
 
int init_list()
{
  node_counter=0;
  root=(struct list *)malloc(sizeof(struct list));
  if(root==NULL )
  {
    printf("Init list failed\n");
    exit(-1);
  }
    root->next=NULL;
     
    return(0);
   
}

int list_add(char *string)
{
  struct list *curr;
  curr = (struct list *)malloc(sizeof(struct list));
  if(curr==NULL)
  {
    printf("Adding to a list failed\n");
    return(1);
  }
   
  if(node_counter==0)
    head=curr;
 
 
  curr->val=string;
  root->next=curr;
  curr->next=NULL;
  root=curr;
  node_counter++;
  return(0);
}

void print_list (struct list * head) 
{
    struct list * ptr =head;
    while(ptr!=NULL)
    {
        printf("%s ", ptr->val);
        ptr=ptr->next;
    }
    printf("\n");
}

int stringtoken (char str[])
{
  char * pch;
  pch = strtok (str," ,.-");
  while (pch != NULL)
  {
    print_list(head);
    list_add(pch);
    pch = strtok (NULL, " ,.-");
  }
  return 0;
}

open_file()
{
  char fname[60];
  char line [ 128 ];
  printf("Give the name of the file\n");
  scanf("%s",fname);
 
 
  f=fopen(fname,"r");
     if ( f!= NULL )
   {
      while ( fgets ( line, sizeof line, f ) != NULL ) /* read a line */
        stringtoken(line);    /* split line to tokens */
      fclose ( f );
   }
   else
   {
      printf("ERROR OPENING THE FILE");
      exit(-1);
   }
   return 0;
}