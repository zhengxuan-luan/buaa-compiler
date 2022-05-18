# 挑战实验：函数内联

函数内联是一种将函数体直接展开到调用处的一种优化技术，它可以由手工指定，或者经由编译优化完成。内联是一种重要的优化技术，其好处在于消除函数的调用开销（压栈，保护/恢复），但是内联展开并不一定会导致函数性能的提升，他可能导致生成的代码的体积膨胀，并且影响指令缓存的命中率。

在本个挑战实验中，你应该对输入的源代码中的函数进行内联，策略如下：

- 保留 `main` 函数
- 内联**所有可以内联**的函数
- 保留无法内联的函数（递归函数）

这意味着你不用考虑内联前后的程序性能问题，只需要考虑函数是否能够被内联而不改变程序语义/造成程序错误。

一些 C 语言的例子（当然，你需要输出的是 `.ll` 格式的文件）

例 1

```cpp
int a() {
    return 1;
}

int b() {
    return 2 + a();
}

int c() {
    return 3 + b();
}

int main() {
    return 4 + c();
}
```

在经过内联以后，应该变为

```cpp
int main(){
    return 4 + 3 + 2 + 1;
}
```

例 2

```cpp
int foo(int t) {
    if (t > 0)
        return foo(t - 1) + t;
    return 0;
}

int bar() {
    return foo(10) + 123;
}

int main() {
   putint(bar());
   return 0;
}
```

在经过内联以后，应该变为

```cpp
int main() {
    putint(foo(10) + 123);
    return 0;
}
```

例 3

```cpp
int a = 10;

void foo() {
    a = a + 1;
}

int bar(int c) {
    if (c == 1)
        return 10;
    else
        return 5;
}

int main(){
    int b = 5;
    foo();
    foo();
    b = a + bar(2);
    return b;
}
```

在经过内联以后，应该变为

```cpp
int a = 10;
int main() {
    int b = 5;
    a = a + 1;
    a = a + 1;
    int a_tmp_var;
    if (2 == 1)
        a_tmp_var = 10;
    else
        a_tmp_var = 5;
    b = a + a_tmp_var;
    return b;
}
```

例 4

```cpp
int foo(int t) {
    if (t == 1) {
        return 5;
    } else {
        return 10;
    }
}

int main() {
    return foo(1) + foo(0);
}
```

在经过内联以后，应该变为

```cpp
int main() {
    int tmp_val_in_llvm_ir_a;
    int tmp_val_in_llvm_ir_b;
    if (1 == 1) {
        tmp_val_in_llvm_ir_a = 5;
    } else {
        tmp_val_in_llvm_ir_a = 10;
    }
    if (0 == 1) {
        tmp_val_in_llvm_ir_b = 5;
    } else {
        tmp_val_in_llvm_ir_b = 10;
    }
    return tmp_val_in_llvm_ir_a + tmp_val_in_llvm_ir_b;
}
```

我们会通过你输出的 `.ll` 格式文件中的函数的数量来检查你是否进行了内联，会通过你输出的 `.ll` 格式的文件经过 `lli` 解释执行的结果来验证你内联的正确性。

具体指导参考 [函数内联实验指导](help.md)。

## 注意事项


我们通过检测你导出的`.ll`格式的文件中的函数的数量以及解释执行的结果来判断你的内联是否正确。

你应该在导出的`.ll`格式文件中删除掉被内联以后的函数，否则将不会通过对函数数量的检测。

比如，你具有`foo,main`两个函数定义`define @foo`,`define @main`，在`foo`内联入`main`以后，你应该删去`foo`函数的原定义。

本次实验的评测标识符为 `inline`，即你的 `judge.toml` 第一行应改为 `[jobs.inline]`。



你需要**认真编写**挑战实验的实验报告，详细说明你是如何完成本次挑战实验的，你对你的编译器进行了哪些改动，你参考了哪些资料，并尽可能完整地阐述你的编译器完成挑战实验任务的工作流程。如果实验报告的内容含糊不清，无法证明你独立完成本次实验，违反 [诚信](../../integrity.md) 原则，我们会酌情扣分。当然，你也需要适当地精简语言，我们对实验报告的评定点在于**扣分**，写出特别长的实验报告（如纯文字内容超过 7 页）并不意味着你能得到加分。

- 实验评测截止时间：2022 年 1 月 7 日 23:59
- 实验报告命名格式：`学号_姓名_funcInline.pdf`
- 实验报告提交：[北航云盘](https://bhpan.buaa.edu.cn:443/link/413EA0802B7A7627A6B5112531C40772) `挑战实验/` 对应班级目录中
- 实验报告提交截止时间：2022 年 1 月 9 日 23:59
