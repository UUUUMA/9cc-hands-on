# Chibi C Compiler
## リファレンス
### chibicc github
https://github.com/rui314/chibicc

### chibicc github first commit
https://github.com/rui314/chibicc/commits/main?after=90d1f7f199cc55b13c7fdb5839d1409806633fdb+314&branch=main

### compiler explorer
https://godbolt.org/

## 開発環境準備
### docker image 作成
```sh
docker build -t compilerbook .
```

### docker プロセス起動
```sh
docker run --rm -v `pwd`/src/:/workspace/src/ -it compilerbook bash
```