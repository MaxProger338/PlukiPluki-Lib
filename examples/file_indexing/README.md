# Пример File Indexing

```bash
cd examples/file_indexing
```

```bash
clang++ -c ../../src/PlukiPluki.cpp -o ../../bin/PlukiPluki.o -I ../../include
```

```bash
ar rcs ../../bin/PlukiPluki.a ../../bin/PlukiPluki.o
```

```bash
clang++ main.cpp ../../bin/PlukiPluki.a -I ../../include -o dist/app.out
```

```bash
./dist/app.out
```