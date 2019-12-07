#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdbool.h>
#include <string.h>

// 対象の文字種数　a,b,c,dの3つ
#define ALL_CHARS 4

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

// トライ木のノードを表わす構造体
typedef struct trie_ {
    // ノードに対応する文字　a,b,c,dのいずれか
    char value;

    // 単語（刻みデータ）の終端かどうか
    bool word_end;

    // 親ノード
    struct trie_ *parent;

    // 子ノード
    struct trie_ *children[ALL_CHARS];

    // 階層
    int len;

} trie;

// ノードの初期化
trie *trie_create(char value) {
  trie *result = (trie *)malloc(sizeof(trie));

  result->value = value;
  result->word_end = false;
  result->parent = NULL;
  result->len = 0;

  for (size_t i = 0; i < ALL_CHARS; i++) {
    result->children[i] = NULL;
  }

  return result;
}

// ノードを子含め全破棄
void trie_destroy(trie *target) {
  for (size_t i = 0; i < ALL_CHARS; i++) {
    if (target->children[i] != NULL) {
      trie_destroy(target->children[i]);
    }
  }

  free(target);
}

// 文字に対応するインデックスの取得
int chara_to_index(char value) {
  // aを0として差を計算
  int result = value - 'a';

  if (result < 0 || result >= ALL_CHARS) {
    result = 0;
  }

  return result;
}

// 単語の各文字に対応するノードを追加
void trie_add(trie *target, char *str) {
  if (*str == '\0') {
    target->word_end = true;
    return;
  }

  int index = chara_to_index(*str);

  if (target->children[index] == NULL) {
    target->children[index] = trie_create(*str);
    target->children[index]->parent = target;
    target->children[index]->len = target->len + 1;
  }

  // 終端まで再帰的に追加
  trie_add(target->children[index], str + 1);
}

// ノードが葉（子がない）かどうか判定
bool trie_is_leaf(trie *target) {
  for (size_t i = 0; i < ALL_CHARS; i++) {
    if (target->children[i] != NULL) {
      return false;
    }
  }

  return true;
}

// ノードが表す単語を得る
void trie_to_word(trie *target, char *out) {
  int len = target->len;

  out[len] = '\0';

  for (int i = len - 1; i >= 0 && target->parent != NULL; i--)
  {
    out[i] = target->value;
    target = target->parent;
  }
}

// 複数のノードのうち、最も長い単語を表すもの（階層が深いもの）を返す
int trie_max_len_index(trie **targets, size_t targets_len) {
  int len = 0;
  int max_len = -1;
  int result = -1;

  for (size_t i = 0; i < targets_len; i++)
  {
    if (targets[i] != NULL) {
      len = targets[i]->len;

      if (len > max_len) {
        max_len = len;
        result = i;
      }
    }
  }

  return result;
}

// 入力文字に当てはまる（当てはまりそうな）単語のノードを探索する
trie *trie_find_word(trie *target, char *in) {
  // 入力が終端に達した、またはノードが葉の場合は探索終了
  if (*in == '\0' || trie_is_leaf(target)) {
    return target;
  }

  trie *child = NULL;
  char value = *in;

  // 文字が虫食いの場合、子ノードを全て探索して、
  // 適切な単語が見つかればそれを探索結果とする
  if (value == 'x') {
    trie *ret[ALL_CHARS] = { NULL, NULL, NULL, NULL };
    trie *end_ret[ALL_CHARS] = { NULL, NULL, NULL, NULL };

    for (size_t i = 0; i < ALL_CHARS; i++)
    {
      child = target->children[i];
      if (child != NULL) {

        // 入力を一つ進めて再帰的に探索
        ret[i] = trie_find_word(child, in + 1);

        if (ret[i]->word_end) {
          end_ret[i] = ret[i];
        }
      }
    }

    // いずれかの探索が単語終端ノードにたどり着いた場合、
    // その中で最も長い単語のノードを探索結果とする

    int max_len_index = trie_max_len_index(end_ret, ALL_CHARS);
    if (max_len_index >= 0) {
      return end_ret[max_len_index];
    }

    // 子ノードから単語終端ノードを探索できなかった場合、
    // 現在ノードが単語終端であれば探索結果とする
    if (target->word_end) {
      return target;
    }

    // 上記いずれの条件も満たさない場合は、子ノードの探索結果の内
    // 最も語数が長い（階層が深い）ノードを探索結果とする
    max_len_index = trie_max_len_index(ret, ALL_CHARS);
    if (max_len_index >= 0) {
      return ret[max_len_index];
    }
  }
    // 虫食い以外は文字に対応する子ノードを探索
  else {
    child = target->children[chara_to_index(value)];
    if (child != NULL) {
      // 入力を一つ進めて再帰的に探索
      return trie_find_word(child, in + 1);
    }
  }

  // 子ノードの探索で適切な結果が得られなかった場合は探索終了
  return target;
}

int main_prg(int argc, char** argv){
  assert( argc==3 );
  FILE *fp_in = fopen( argv[1], "r" );
  assert( fp_in != NULL );
  FILE *fp_out = fopen( argv[2], "w" );
  assert( fp_out != NULL );

  // 虫食いデータT'を格納する配列
  char *T_dash = (char *)malloc(sizeof(char) * 10000000);
  if(T_dash == NULL) exit(0);

  // 刻みデータSを一時的に格納する配列
  char *S_buf = (char *)malloc(sizeof(char) * 10000000);
  if(S_buf == NULL) exit(0);

  // トライ木のルートノードを作成
  // ルートなので文字に意味はない
  trie *root = trie_create('_');

  // ファイルからT'を取得

  fscanf(fp_in, "%s\n", T_dash);

  size_t S_len = 0;

  // ファイルからSを取得してトライ木を構築していく
  while (fgets(S_buf, 1000, fp_in) != NULL) {
    S_len = strlen(S_buf);

    // 改行文字は削除しておく
    if (S_buf[S_len - 1] == '\n') {
      S_buf[S_len - 1] = '\0';
    }

    // 一行分のSを単語としてノードを追加
    trie_add(root, S_buf);
  }

  fclose(fp_in);

  size_t T_len = strlen(T_dash);

  // トライ木を用いて虫食いデータを再構成
  for (size_t cursor = 0; cursor < T_len; )
  {
    // 入力に当てはまる（当てはまりそうな）単語のノードを探索
    trie *ret = trie_find_word(root, T_dash + cursor);

    // ノードから単語を文字列として取得
    trie_to_word(ret, S_buf);

    // 単語を出力ファイルに書き出す
    fprintf(fp_out, S_buf);

    // 単語の長さ分探索位置を進める
    cursor += strlen(S_buf);
  }

  trie_destroy(root);
  free(T_dash);
  free(S_buf);
  fclose(fp_out);

  return 0 ;
}