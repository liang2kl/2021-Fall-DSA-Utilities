# 2021-Fall-DSA-Utilities

邓俊辉老师[数据结构](https://dsa.cs.tsinghua.edu.cn/~deng/ds/dsacpp/index.htm)课程部分数据结构和算法的实现及 [OJ](https://dsa.cs.tsinghua.edu.cn/oj/) 提交实用工具。使用代码需要遵守[课程诚信守则](https://dsa.cs.tsinghua.edu.cn/oj/static/pa-book.html#2.-%E8%AF%84%E5%88%86%E6%96%B9%E5%BC%8F)。

## 数据结构及算法实现

部分实现参考了[课程示例代码](https://dsa.cs.tsinghua.edu.cn/~deng/ds/src_link/index.htm)，修改了部分在 g++ 上会出现问题的 undefined behaviour。

实现的数据结构包括：

- [`Vector`](./ds/vector.h)：向量
- [`Stack`](./ds/stack.h)：栈
- [`Queue`](./ds/queue.h)：队列
- [`List`](./ds/list.h)：列表
- [`BitMap`](./ds/bitmap.h)：位图
- [`BinTree`](./ds/bintree.h)：二叉树
- [`BST`](./ds/bst.h)：二叉搜索树
- [`AVL`](./ds/avl.h)：AVL 树
- [`RBTree`](./ds/rbtree.h)：红黑树
- [`PQ_CBH`](./ds/pq_cbh.h)：以完全二叉堆作为优先级队列
- [`PQ_LH`](./ds/pq_lh.h)：以左式堆作为优先级队列
- [`UFSet`](./ds/ufset.h)：并查集

## OJ 提交工具

`convert.py` 可以递归地将源文件所依赖的 `.h` 文件内容添加到源文件中，生成一个用于提交的版本，从而写代码时可以直接 `#include` 相应的数据结构头文件而无需手动复制。

使用方法：在**源文件所在目录**下，执行

```bash
python3 <convert.py 路径> <源文件(.cpp)路径>

# or
python <convert.py 路径> <源文件(.cpp)路径>
```

在相同目录下将生成 `文件名_submit.cpp` 文件，可直接提交到 OJ。

### 高级功能

**保留某些 `#include` 声明而不进行替换**

在 `#include` 后加上 `/* EXCLUDE */`：

```cpp
#include "<file_name>.h" /* EXCLUDE */
```

当题目提供了不可更改的头文件时可以使用此方法。

**移除 doc string**

使用如下格式为函数声明添加文档而不会出现在最终的提交版本中：

```cpp
/**
 <任意内容>
 */
```

**移除 `#ifndef _OJ_` 等预编译指令**

头文件和源文件中的 `#ifndef _OJ_`、`#if !defined(_OJ_)` 和 `#ifdef DEBUG` 及其作用范围内的所有内容将被移除，避免残留代码影响白盒评分。
