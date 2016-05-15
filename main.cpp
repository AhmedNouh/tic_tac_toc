#include <stdio.h>
#include <string.h.>
#include <vector>

using namespace std;

#define WIN_STATES 8
#define check(t,i) t&(1<<i)
#define mark(t,i) t | (1<<i)
#define xy2i(x,y) y+x*3
typedef pair<int,int> ii;
bool psym ; //indicates which sym the player has if x -> 1 o -> 0
int win_states[WIN_STATES] = {7,56,448,73,146,292,84,273};

bool win(int x);
int score(int p,int c);
void print_board(int player,int com);
ii play_min(int p,int c ,int r ,int alpha ,int beta);
ii play_max(int p,int c ,int r ,int alpha ,int beta);

int main(int argc,char **argv)
{
    if(argc!= 2){
        fprintf(stderr,"invalid arguments");
        return 1;
    }

    psym = (argv[1][0]=='x');
    int turn = 1 ,p=0,c=0,u ,v ,round = 0,winner;
    ii next_move;
    bool game = true  ;

    while(game){
        if(turn == psym)
        {
            while(1)
            {
                scanf("%d %d",&u,&v);
                int idx = xy2i(u,v);
                if(idx > 8 || check(p,idx) || check(c,idx))
                {
                    printf("invalid position\n");
                    continue;
                }
                p = mark(p,idx);
                break;
            }

        }
        else{
            next_move = play_max(p,c,round,-10,10);
            c  = mark(c,next_move.first);
            print_board(p,c);
        }
        turn = (turn+1)%2;
        round ++;
        winner = score(p,c);
        if(winner!=0 || round ==9){
            if(winner == -1)printf("You win!!!\nTHAT CANNOT HAPPEN\n");
            else if(winner == 1)printf("You lost\nYOU WILL NEVER WIN\n");
            else printf("DRAW!!\nYOU CANNOT DO BETTER\n");
            game = false;
        }
    }
    return 0;
}
bool win(int x)
{
    for(int i=0;i<WIN_STATES;i++)
    {
        if( (x & win_states[i] )== win_states[i] )return true;
    }
    return false;
}
int score(int p,int c)
{
    if(win(p))return -1;
    if(win(c))return 1;
    return 0 ;
}
void print_board(int player,int com)
{
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            int idx = xy2i(i,j);
            if(check(player,idx))
                printf("%c",psym?'X':'O');
            else if(check(com,idx))
                printf("%c",psym?'O':'X');
            else
                printf("-");
        }
        printf("\n");
    }
    printf("*******\n");
}
ii play_max(int p,int c ,int r ,int alpha ,int beta)
{
    int sc = score(p,c) , mx = - 10 , nc  , idx;
    ii next_move;
    if(r==9 || sc!=0)
    {
        // reached a leaf
        next_move.first=  -1 ;
        next_move.second = sc ;
        return next_move;
    }
    for(int i=0;i<9;i++)
    {
        if(check(p,i) || check(c,i) ) continue;
        nc = mark(c,i);
        next_move = play_min(p,nc,r+1,alpha,beta);
        if(mx <  next_move.second)
        {
            mx = next_move.second;
            idx = i;
            alpha = (alpha > mx)?alpha:mx;
            if(beta <= alpha)break;
        }
    }
    next_move.first =  idx ;
    next_move.second = mx;
    return next_move;
}
ii play_min(int p,int c ,int r ,int alpha ,int beta)
{
    int sc = score(p,c) , mn =  10 , np  , idx;
    ii next_move;
    if(r==9 || sc!=0)
    {
        next_move.first =  -1 ;
        next_move.second = sc ;
        return next_move;
    }
    for(int i=0;i<9;i++)
    {
        if(check(p,i) || check(c,i)) continue;
        np = mark(p,i);
        next_move = play_max(np,c,r+1,alpha,beta);
        if(mn > next_move.second)
        {
            mn = next_move.second;
            idx = i;
            beta = (beta<mn)?beta:mn;
            if(beta <=alpha)break;
        }
    }
    next_move.first =  idx ;
    next_move.second = mn;
    return next_move;
}
