# Пример File Indexing

```bash
cd examples/file_indexing
```

```bash
clang++ -c ../../src/PlukiPluki.cpp -o ../../bin/PlukiPluki.o -I ../../include
```

```bash
clang++ -c ../../src/core/FileBase.cpp -o ../../bin/FileBase.o
```

```bash
ar rcs ../../bin/PlukiPluki.a ../../bin/PlukiPluki.o ../../bin/FileBase.o 
```

```bash
clang++ main.cpp ../../bin/PlukiPluki.a -I ../../include -o dist/app.out
```

```bash
./dist/app.out
```