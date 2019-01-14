//level 2 =points 10-20 , level 3=points 20-30, level 4(last)= points30-40 ,win =points40
//multiple poison not included
//walls represented by $
//mines represented by p
//food by F

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <time.h>
#define minnr 1
#define maxxr 18
#define minnc 1
#define maxxc 18
//n =row m=col
using namespace std;
struct node
{
    int x,y;//x=row y=col
    char data;
    node *next,*prev;
}*start;

struct food
{
    int x;
    int y;
    char value;
}f,p,o[2];

struct node *insert_save(struct node *start,int row,int col,char ch)//to insert snake from saved game
{
 struct node *p,*n=new node;
 p=start;
 if(start==NULL)
 {
     n->x=row;
     n->y=col;
     n->data=ch;
     n->next=NULL;
     n->prev=start;
     start=n;
 }
 else
 {
     while(p->next!=NULL)
     {
         p=p->next;
     }
     n->x=row;
     n->y=col;
     n->data=ch;
     p->next=n;
     n->next=NULL;
     n->prev=p;
 }
 return start;
}

struct node* resume_game(char board[20][20],struct node *start,struct food *f,int *scores,struct food *p,int *level,struct food *o)//saved game
{
   FILE *fp,*fp1,*fp2,*fp3,*fp4,*fp5;
   fp=fopen("snake.txt","r");
   fp1=fopen("food.txt","r");
   fp2=fopen("score.txt","r");
   fp3=fopen("poison.txt","r");
   fp4=fopen("level.txt","r");
   fp5=fopen("obstacles.txt","r");
   char ch,s;
   int row,col,i
   ;
   ch=getc(fp);
   while(ch!=EOF)
   {
       if(ch!='\n')
       {   row=ch-'0';
           ch=getc(fp);
          col=ch-'0';
           ch=getc(fp);
           s=ch;
           start=insert_save(start,row,col,s);
       }
       ch=getc(fp);
   }
   char ch1;
   ch1=getc(fp1);
   while(ch1!=EOF)
   {
       f->x=ch1-'0';
       ch1=getc(fp1);
       f->y=ch1-'0';
       ch1=getc(fp1);
       f->value=ch1;
       board[f->x][f->y]=f->value;
       ch1=getc(fp1);
   }
   char ch2;
   ch2=getc(fp3);
   while(ch2!=EOF)
   {
       p->x=ch2-'0';
       ch2=getc(fp3);
       p->y=ch2-'0';
       ch2=getc(fp3);
       p->value=ch2;
       board[p->x][p->y]=p->value;
       ch2=getc(fp3);
   }
   *scores=getc(fp2)-'0';
   *level=getc(fp4)-'0';
   char ch3=getc(fp5);
   i=0;
   while(ch3!=EOF)
   {
       if(ch3!='\n'){
       o[i].x=ch3-'0';
       ch3=getc(fp5);
       o[i].y=ch3-'0';
       ch3=getc(fp5);
       o[i].value=ch3;
       board[o[i].x][o[i].y]=o[i].value;
       }
       i++;
       ch3=getc(fp5);
   }
   fclose(fp);
   fclose(fp1);
   fclose(fp2);
   fclose(fp3);
   fclose(fp4);
   fclose(fp5);
   return start;
}

void save_game(struct node *start,struct food f,int scores,struct food p,int level,struct food *o)//save nodes in file and save all data of game to files
{
   FILE *fp,*fp1,*fp2,*fp3,*fp4,*fp5;
   fp=fopen("snake.txt","w");//to save snake link list
   fp1=fopen("food.txt","w");//ro save food value and coordinates
   fp2=fopen("score.txt","w");//to save score
   fp3=fopen("poison.txt","w");//to save poison values
   fp4=fopen("level.txt","w");//to save level of game
   fp5=fopen("obstacles.txt","w");//to save obstacles in game
   struct node *p1=start;
   int i=0;
   while(p1!=NULL)
   {
       putc(p1->x+'0',fp);
       putc(p1->y+'0',fp);
       putc(p1->data,fp);
       putc('\n',fp);
       p1=p1->next;
   }
   putc(f.x+'0',fp1);
   putc(f.y+'0',fp1);
   putc(f.value,fp1);
   putc(scores+'0',fp2);
   putc(p.x+'0',fp3);
   putc(p.y+'0',fp3);
   putc(p.value,fp3);
   putc(level+'0',fp4);
   while(i<2)
   {
      putc(o[i].x+'0',fp5);
      putc(o[i].y+'0',fp5);
      putc(o[i].value,fp5);
      putc('\n',fp5);
      i++;
   }
   fclose(fp);
   fclose(fp1);
   fclose(fp2);
   fclose(fp3);
   fclose(fp4);
   fclose(fp5);
}

struct node *insertend(struct node *start,int i,int j,char value)//if user starts new game
{
    node *temp;
    temp=new node;

    if(start==NULL)
    {
        temp->x=i;
        temp->y=j;
        temp->data=value;
        temp->prev=NULL;
        temp->next=NULL;
        start=temp;
    }
    else
    {
        node *temp1;
        temp1=start;
        while(temp1->next!=NULL)
        {
            temp1=temp1->next;
        }
        node *temp;
        temp=new node;
        temp->x=i;
        temp->y=j;
        temp->data=value;
        temp->next=NULL;
        temp->prev=temp1;
        temp1->next=temp;

    }

    return start;
}
void length(struct node *start,char x)//increment length of snake after eating food
{
        node *temp1;
        temp1=start;
        while(temp1->next!=NULL)
        {
            temp1=temp1->next;
        }
        node *temp;
        temp=new node;
        temp->x=temp1->x;
        temp->y=temp1->y-1;
        temp->data=x;
        temp->next=NULL;
        temp->prev=temp1;
        temp1->next=temp;
}

void make_food(char board[20][20])//generate random food
{
    f.x=(rand()%(maxxr-minnr+1))+minnr;
    f.y=(rand()%(maxxc-minnc+1))+minnc;
    f.value='F';
    board[f.x][f.y]=f.value;
}

void make_poison(char board[20][20])//generates random poison
{
    p.x=(rand()%(maxxr-minnr+1))+minnr;
    p.y=(rand()%(maxxc-minnc+1))+minnc;
    p.value='P';
    board[p.x][p.y]=p.value;
}

void make_obstacle(char board[20][20],int i)//generates random obstacles
{
    o[i].x=(rand()%(maxxr-minnr+1))+minnr;
    o[i].y=(rand()%(maxxc-minnc+1))+minnc;
    o[i].value='$';
    board[o[i].x][o[i].y]=o[i].value;
}

void display(char board[20][20],struct node *start,int points,int level)//display board
{
   int i,j;
   struct node *ptr=start;
   cout<<"points :"<<points<<"    ";
while(ptr!=NULL)
{
board[ptr->x][ptr->y]=ptr->data;
ptr=ptr->next;
}

cout<<"level :"<<level<<endl;
   for(i=0;i<20;i++)
    {
        for(j=0;j<20;j++)
        {
            cout<<board[i][j]<<" ";
        }
        cout<<endl;
    }
    if(points==40){cout<<"CONGRATS !! YOU WON !!"<<endl;}
}


void swap(struct node *start,int row,int col)// changing snake position with swapping nodes data
{
    int x1,x2,y1,y2;
   struct node *ptr=start;
   x1=ptr->x;
   y1=ptr->y;
while(ptr->next!=NULL)
{
   ptr=ptr->next;
   x2=ptr->x;
   y2=ptr->y;
    ptr->x=x1;
 ptr->y=y1;
 x1=x2;
 y1=y2;
}
 start->x=row;
 start->y=col;
}

void clear_board(char board[20][20],int level)//clearing previous board values to save current snake,food,poison,obstacles values
{
    int i,j;
    if(level==2 || level==3)
   {
      for(i=0;i<20;i++)
    {
        for(j=0;j<20;j++)
        {
            if(i==0 ||i==19 ||j==0 ||j==19){board[i][j]='+';}
             else if(board[i][j]!=f.value && board[i][j]!=p.value) board[i][j]=' ';
        }
    }
    }
    else if(level==1)
   {
        for(i=0;i<20;i++)
    {
        for(j=0;j<20;j++)
        {
            if(i==0 ||i==19 ||j==0 ||j==19){board[i][j]='+';}
             else if(board[i][j]!=f.value) board[i][j]=' ';
        }
    }
    }

  else if(level==4)
   {
      for(i=0;i<20;i++)
    {
        for(j=0;j<20;j++)
        {
            if(i==0 ||i==19 ||j==0 ||j==19){board[i][j]='+';}
             else if(board[i][j]!=f.value && board[i][j]!=p.value && board[i][j]!='$') board[i][j]=' ';
        }
    }
    }

}

bool check(struct node *start,struct food f,struct food p,struct food *o)//to check if food,poison,obstacles,snake never get on same place when generated
{
    struct node *pt=start;//o = obstacles(array of obstacles) p = poison f = food
    int fl=0;
    while(pt!=NULL)
    {
        if((f.x==pt->x && f.y==pt->y) || (p.x==pt->x && p.y==pt->y) || (o[0].x==pt->x && o[0].y==pt->y) || (o[1].x==pt->x && o[1].y==pt->y))
        {
            fl=1;
            break;
        }
        else if(f.x==p.x && f.y==p.y)
        {
            fl=1;
            break;
        }
        else if((f.x==o[0].x && f.y==o[0].y) || (f.x==o[1].x && f.y==o[1].y))
        {
            fl=1;
            break;
        }
         else if((p.x==o[0].x && p.y==o[0].y) || (p.x==o[1].x && p.y==o[1].y))
        {
            fl=1;
            break;
        }

        pt=pt->next;
    }
    if(fl==0){return 0;}
    else {return 1;}
}

bool game_over(struct node *start)//to check if snake head don't touch body and any other game over condition is checked here
{
    struct node *pt=start->next;
    int fl=0;
    while(pt!=NULL)
    {
        if(start->x==pt->x && start->y==pt->y)
        {
            fl=1;
            break;
        }
        pt=pt->next;
    }
    if(fl==1)
    {
    FILE *fp,*fp1,*fp2,*fp3;
    fp=fopen("snake.txt","w");
    fp1=fopen("food.txt","w");
    fp2=fopen("score.txt","w");
    fp3=fopen("poison.txt","w");
    return 1;
    }
    else{return 0;}
}

void snake(char board[20][20],int op)//actual movement of snake (main function for movement of snake and every other activity in game is controlled with this function)
{
    struct node *ptr;
    start=NULL;
    int points=0,flag=0,level=1,i=0;
    if(op==2)
{
    int t=1,arr[3]={2,2,2},arr1[3]={3,2,1};
    char arr2[3]={'x','#','#'};
    make_food(board);
    make_poison(board);
    while(check(start,f,p,o))
    {
        make_food(board);
        make_poison(board);
    }
    start=insertend(start,arr[0],arr1[0],arr2[0]);
    ptr=start;
    while(t<3)
     {
        ptr=insertend(ptr,arr[t],arr1[t],arr2[t]);
        t++;
     }
     display(board,start,points,level);
}

else if(op==1)
{
     FILE *fp;
     fp=fopen("snake.txt","r");
     char ch=getc(fp);
     if(ch==EOF)
     {
         cout<<"no saved game"<<endl;
         flag=1;
     }
     else
     {
     start=resume_game(board,start,&f,&points,&p,&level,o);
     display(board,start,points,level);
     }
}

if(flag==0){
        char ch;
        cout<<"Press up(w) down(s) left(a) right(d) quit(q): ";
        cin>>ch;
    while(1)
    {
        cout<<"Press up(w) down(s) left(a) right(d) quit(q): ";
        cin>>ch;
        int n,m;

if(ch=='d')
{
  ptr=start;
 if(ptr->y==maxxc && level<3)
  {
  n=ptr->x;m=1;
 }

 else if(ptr->y==maxxc && level>=3)
 {
     clear_board(board,level);
      system("cls");
      display(board,start,points,level);
    FILE *fp,*fp1,*fp2,*fp3;
    fp=fopen("snake.txt","w");
    fp1=fopen("food.txt","w");
    fp2=fopen("score.txt","w");
    fp3=fopen("poison.txt","w");
    cout<<endl<<"!! GAME OVER !!"<<endl;
    break;
 }

 else
 {
    n=ptr->x;
 m=ptr->y+1;}
 if(m!=start->next->y)
 {
 swap(start,n,m);
 }

 if(game_over(start)){cout<<endl<<"!! GAME OVER !!"<<endl;break;}
}
else if(ch=='s')
{
 ptr=start;
 if(ptr->x==maxxr && level<3)
 {n=1;
 m=ptr->y;}

 else if(ptr->x==maxxr && level>=3)
 {
     clear_board(board,level);
      system("cls");
      display(board,start,points,level);
    FILE *fp,*fp1,*fp2,*fp3;
    fp=fopen("snake.txt","w");
    fp1=fopen("food.txt","w");
    fp2=fopen("score.txt","w");
    fp3=fopen("poison.txt","w");
    cout<<endl<<"!! GAME OVER !!"<<endl;
    break;
 }

 else
 {
 n=ptr->x+1;
 m=ptr->y;}
  if(n!=start->next->x )
 {
 swap(start,n,m);

 }

 if(game_over(start)){cout<<endl<<"!! GAME OVER !!"<<endl;break;}
}
else if(ch=='a')
{
 ptr=start;
 if(ptr->y==1 && level<3)
 {n=ptr->x;
 m=maxxc;
 }

 else if(ptr->y==1 && level>=3)
 {
     clear_board(board,level);
      system("cls");
      display(board,start,points,level);
    FILE *fp,*fp1,*fp2,*fp3;
    fp=fopen("snake.txt","w");
    fp1=fopen("food.txt","w");
    fp2=fopen("score.txt","w");
    fp3=fopen("poison.txt","w");
    cout<<endl<<"!! GAME OVER !!"<<endl;
    break;
 }

 else
 {n=ptr->x;
 m=ptr->y-1;}
  if(m!=start->next->y)
 {
 swap(start,n,m);
 }

 if(game_over(start)){cout<<endl<<"!! GAME OVER !!"<<endl;break;}
}
else if(ch=='w')
{
 ptr=start;
 if(ptr->x==1 && level<3)
 {n=maxxr;
 m=ptr->y;
 }

 else if(ptr->x==1 && level>=3)
 {
     clear_board(board,level);
      system("cls");
      display(board,start,points,level);
    FILE *fp,*fp1,*fp2,*fp3;
    fp=fopen("snake.txt","w");
    fp1=fopen("food.txt","w");
    fp2=fopen("score.txt","w");
    fp3=fopen("poison.txt","w");
    cout<<endl<<"!! GAME OVER !!"<<endl;
    break;
 }

 else
 {n=ptr->x-1;
 m=ptr->y;}

 if(n!=start->next->x)
 {
 swap(start,n,m);
 }

 if(game_over(start)){cout<<endl<<"!! GAME OVER !!"<<endl;break;}
}

if(start->x==f.x && start->y==f.y)
{
    length(start,'#');
    make_food(board);
    while(check(start,f,p,o))
    {
        make_food(board);
    }
points++;
if(points>=10 && points<20 && level==1)
{
  level=2;
  make_poison(board);
  while(check(start,f,p,o))
    {
        make_food(board);
    }
}
else if(points>=20 && points<30 && level==2)
{
    level=3;
}

else if(points>=30 && points<40 && level==3)
{
    level=4;
    while(i<2)
    {
        make_obstacle(board,i);
        while(check(start,f,p,o))
        {
            make_obstacle(board,i);
        }
        i++;
    }

}

else if(points==40)
{
      clear_board(board,level);
      system("cls");
      display(board,start,points,level);
    FILE *fp,*fp1,*fp2,*fp3,*fp4;
    fp=fopen("snake.txt","w");
    fp1=fopen("food.txt","w");
    fp2=fopen("score.txt","w");
    fp3=fopen("poison.txt","w");
    fp4=fopen("level.txt","w");
    break;
}

}

else if(points>=10 && start->x==p.x && start->y==p.y)
{
      clear_board(board,level);
      system("cls");
      display(board,start,points,level);
    FILE *fp,*fp1,*fp2,*fp3,*fp4;
    fp=fopen("snake.txt","w");
    fp1=fopen("food.txt","w");
    fp2=fopen("score.txt","w");
    fp3=fopen("poison.txt","w");
    fp4=fopen("level.txt","w");
    cout<<endl<<"!! GAME OVER !!"<<endl;
    break;
}

else if(points>=30 && board[start->x][start->y]=='$')
{
      clear_board(board,level);
      system("cls");
      display(board,start,points,level);
    FILE *fp,*fp1,*fp2,*fp3,*fp4;
    fp=fopen("snake.txt","w");
    fp1=fopen("food.txt","w");
    fp2=fopen("score.txt","w");
    fp3=fopen("poison.txt","w");
    fp4=fopen("level.txt","w");
    cout<<endl<<"!! GAME OVER !!"<<endl;
    break;
}

if(ch=='q')
{
    save_game(start,f,points,p,level,o);
    break;
}

clear_board(board,level);
system("cls");
display(board,start,points,level);

}
}

}

int main()
{
    srand(time(0));
    cout<<"@@---SNAKE GAME---@@"<<endl;
    char board[20][20];

    for(int i=0;i<20;i++)
    {
        for(int j=0;j<20;j++)
        {
            if(i==0 || i==19 || j==0 || j==19)
            {
                board[i][j]='+';
            }
            else
            {
            board[i][j]=' ';
        }
        }
    }
    int op;
    cout<<"enter option 1 to resume or option 2 to start new game :";
    cin>>op;
    snake(board,op);

}
