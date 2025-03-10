#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include<string.h>

//Sの構造体
typedef struct{
    char word[150];
    int w_len;
} S;

//qsortの比較関数
int comp(const S *a, const S *b){
    if(a->w_len>b->w_len) return -1;
    else if(a->w_len<b->w_len) return 1;
    else return 0;
}

//bm法のずらし表を作成
void create_table(S s, int *table){
    int i;
    int word;
    int w_len=s.w_len;
    for(i=0;i<4;i++){
        table[i]=w_len;
    }
    for(i=0;i<w_len;i++){
        word=s.word[i]-97;
        table[word]=w_len-1-i;
    }
}

//Tのpointからsに置き換える
void insert(char *T, S s, int point){
    int i;
    for(i=0;i<s.w_len;i++){
        T[point+i]=s.word[i];
    }
}

//bm法
void bm(char *T, int t_len, S s){
    int table[4];
    create_table(s,table);
    int pt=s.w_len-1;
    int ps;
    int word;
    while (pt<t_len)
    {
        ps=s.w_len-1;
        while (T[pt]==s.word[ps]||T[pt]=='x')
        {
            if(ps==0){
                insert(T,s,pt);
                return;
            }
            pt--;
            ps--;
        }
        word=T[pt]-97;
        pt+=(table[word]>s.w_len-ps) ? table[word] : s.w_len-ps;
    }
    return;
}

int main_prg(int, char**);

int main(int argc, char** argv){

  struct rusage u;
  struct timeval start, end;
  
  getrusage(RUSAGE_SELF, &u);
  start = u.ru_utime;
  
  main_prg(argc, argv);
  
  getrusage(RUSAGE_SELF, &u );
  end = u.ru_utime;
  
  fprintf(stderr, "%lf\n", (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_usec - start.tv_usec)*1e-6);
    
  return(0);
}

int main_prg(int argc, char** argv){
   assert( argc==3 );
   FILE *fp_in = fopen( argv[1], "r" );
   assert( fp_in != NULL );
   FILE *fp_out = fopen( argv[2], "w" );
   assert( fp_out != NULL );

    //T'の用意
    char *T_dash=(char*)malloc(sizeof(char)*500000);
    fscanf(fp_in,"%s\n",T_dash);
    int t_len=strlen(T_dash);

    //Sの用意
    S s[50000];
    int s_cnt=0;
    while(fgets(s[s_cnt].word,150,fp_in)!=NULL){
        s[s_cnt].w_len=strlen(s[s_cnt].word)-1;
        if(s[s_cnt].w_len>1){
            if(s[s_cnt].word[s[s_cnt].w_len]=='\n') s[s_cnt].word[s[s_cnt].w_len]='\0';
            s_cnt++;
        }
    }

    //入力ファイルを開放
    fclose(fp_in);

    //Sを文字列の長い順にソート
    qsort(s,s_cnt,sizeof(S),(int (*)(const void *, const void *))comp);

    //bm法で挿入
    int i;
    for(i=0;i<s_cnt;i++){
        bm(T_dash,t_len,s[i]);
    }

    //置き換えられなかったところをaに置き換える
    for(i=0;i<t_len;i++){
        if(T_dash[i]=='x'){
            T_dash[i]='a';
        }
    }

    //結果を書き込む
    fprintf(fp_out,T_dash);
    
    //出力ファイルを閉じる
    fclose(fp_out);
    return 0;
}