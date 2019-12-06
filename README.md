# アルゴリズムB

### 方針
- 実験管理はスプレッドシートで行う
https://docs.google.com/spreadsheets/d/1uprDgLpFYpxspH6zLF2VxF3g0QJ7TjyRG4fDPtVUI4A/edit?usp=sharing
- アルゴリズム開発時は， {名前}/featureブランチで作業する


### 実行例
- [ ] data/ディレクトリ以下に入力データdat{0..4}_in, 正解データdat{0..4}_refを保存する。
- [ ] mv example_code/{実行したいアルゴリズム}.c example_code/template.c
- [ ] source evaluate_auto.s
- [ ] 実行結果は標準出力に出力される。
- [ ] 実行後, カレントディレクトリにerror_analysis{0..4}.csvが出力される。
