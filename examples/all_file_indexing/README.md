# Пример File Indexing

```bash
cd examples/all_file_indexing
```

```bash
clang++ -c ../../src/PlukiPluki.cpp -o ../../bin/PlukiPluki.o -I ../../include
```

```bash
clang++ -c ../../src/core/FileBase.cpp -o ../../bin/FileBase.o
```

```bash
ar rcs ../../bin/PlukiPluki.a ../../bin/*.o 
```

```bash
clang++ main.cpp ../../bin/PlukiPluki.a -I ../../include -o dist/app.out
```

```bash
./dist/app.out
```