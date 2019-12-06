# アルゴリズムB

### 方針
実験管理はここで行う
https://docs.google.com/spreadsheets/d/1uprDgLpFYpxspH6zLF2VxF3g0QJ7TjyRG4fDPtVUI4A/edit?usp=sharing

### 実行例
  cd example_code
  mv (実行したいコード.c) template.c 
  make clean
  make
  ./grpwk ../data/dat?_in ../data/dat?_out (？は自然数1文字)
  cd ..
  python3 evaluate.py -i data/dat?_in -o data/dat?_out -e data/dat?_ref
