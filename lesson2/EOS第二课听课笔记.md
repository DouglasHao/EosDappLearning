---
typora-root-url: ./assets
---

# EOS第二课听课笔记

## 0 课前准备

```
第二课作业
1.创建一个叫game的用户。
2.写一个游戏合约把第一课自己创建的token通过游戏合约转账给game用户。
3.游戏合约只能由特定的用户调用转账。
```

下载EOS源码

```shell
git clone https://github.com/EOSIO/eos --recursive
```

## 1 EOS源码解析

### 1.1 EOS项目组成

1. ```
   1. 编译模块
   2. 主程序模块
   3. 依赖库模块
   4. 插件模块
   5. 智能合约模块
   6. 测试模块
   7. Docker工具模块
   8. 文档说明模块
   ```

### 1.2 EOS项目模块简介

#### 1.2.1编译模块

eos项目的整体编译、依赖库下载、代码编译等功能，依托于一个完整的脚本体系，主要包括以下三部分：

```
eos/eosio_build.sh
eos/scripts
eos/CMakeModules
```

#####  1.2.1.1 eos/eosio_build.sh

eos/eosio_build.sh是eos项目的主编译脚本，通过运行这个脚本，就可以实现对eos项目的一键编译，脚本运行过程中主要测试了编译所需的硬件与软件环境是否满足要求以及显示编译结果。

##### 1.2 .1.2 eos/scripts

eos/scripts目录中包含了项目编译所需的其他脚本文件，其中：

```
abi_to_rc目录、abigen.sh和abi_is_json.py共同负责将C++语言编写的智能合约编译成.abi文件，再将.abi文件编译成可执行文件;
eosio_build_前缀的脚本文件分别是针对不同系统的编译子脚本;
eosio-tn_前缀的脚本负责自动化运行，关闭节点。
```

##### 1.2.1.3 eos/CMakeModules

由于eos项目是基于CMake等工具编译的，所以eos/CMakeModules中主要包含CMake编译所需要使用的一些配置信息。

#### 1.2.2 主程序模块

 eos项目的主程序源代码位于eos/program目录下，包含6个基本功能组件：

- ```
  eos/program/nodeos
  eos/program/cleos
  eos/program/keosd
  eos/program/eosio-abigen
  eos/program/eosio-applesedemo
  eos/program/eosio-launcher
  ```

##### 1.2.2.1 nodeos

eos/program/nodeos是eos项目的核心程序源代码，可以配置不同插件来运行不同类型节点。该进程主要负责提供区块生产，封装API接口和本地开发的功能;

##### 1.2.2.3 cleos

eos/program/cleos是eos项目中与nodeos之间交互信息的命令行工具cleos的源代码，编译后与nodeos公开的REST API进行信息交互。

##### 1.2.2.4 keosd

eos/program/keosd是eos项目中的内置钱包程序，配合钱包相关插件通过HTTP接口或RPC API与cleos进行通信，完成账户创建，转账等基本钱包功能。

##### 1.2.2.5 eosio-abigen

eos/program/eosio-abigen是eos项目的智能合约编译器源代码（用于生成智能合约的.abi文件）

##### 1.2.2.6 eosio-applesedemo

eos/program/eosio-applesedemo是eos项目中公私钥生成与数字签名生成组件的源代码，主要为eos项目的提供密码学（采用ECDSA算法中的secp256r1）支持，其中:

```
main.cpp 为该工具的核心主函数;
r1_signature_compactor为数字签名功能描述文件
sign.sh是生成应用程序包applesedemo.app并对其进行签名的自动化脚本。
```

#####  1.2.2.7 eosio-launcher

eos/program/eosio-launcher是eos项目的P2P网络组成启动器源代码，该部分简化了nodeos节点组网的流程。

#### 1.2.3 依赖库模块 

eos项目的运行依托于许多依赖库，这些依赖库的基本信息位于eos/libraries目录中，包括：

```
eos/libraries/abi_generator
eos/libraries/appbase
eos/libraries/builtins
eos/libraries/chain
eos/libraries/appbase/fc
eos/libraries/chainbase
eos/libraries/egenesis
eos/libraries/utilitiles
```

#####  1.2.3.1 abi_generator

eos/libraries/abi_generator包含了智能合约编译器所需的依赖文件，可以说编译器的主要代码就是位于这个目录中.

##### 1.2.3.2 appbase

eos/libraries/appbase提供了一个用于从一组插件构建应用程序的基本框架。该模块负责管理插件的生命周期，并确保所有插件按正确的顺序配置，初始化，启动和关闭。

 该依赖库包含5个主要特征：

```
动态指定要加载的插件
自动加载依赖插件
插件可以指定命令行参数和配置文件选项
程序正常退出SIGINT和SIGTERM
最小依赖（Boost 1.60，c ++ 14）
```

##### 1.2.3.3 builtins

eos/libraries/builtins中包含了eos项目编译过程中所需要的compiler-RT编译器（libgcc的替换库）的源代码，包括编译器本身以及相关功能函数的代码描述。

#####  1.2.3.4 chain

可以说eos/libraries/chain包含有eos项目的核心内容，包括区块，区块链，merkle树等数据结构以及初始区块，控制器等关键算法，这一部分将在后续的教程中展开介绍。

##### 1.2.3.5 chainbase

eos/libraries/appbase/chainbase中定义了保存eos区块链数据的数据库结构，可以说该数据库旨在满足区块链应用程序的苛刻要求，但适用于任何需要强大的事务数据库的应用程序。

#####  1.2.3.6 fc

eos/libraries/fc是eos项目的细胞级模块，定义了eos项目中的基本变量数据结构，包括String，Time，Base系列编码等，在后需的具体代码分析中，我们将会遇到很多fc::前缀的引用，届时具体回顾。

在eos/libraries/fc/src目录中我们可以看到具体的代码实现

##### 1.2.3.7 softfloat

eos/libraries/softfloat包含了一个Berkeley SoftFloat，即符合IEEE浮点运算标准二进制浮点的软件实现。

##### 1.2.3.8 testing

eos/libraries/testing包含了几个测试实例，包括对区块链数据库的链接测试，P2P网络的链接测试等。

##### 1.2.3.9 utilities

eos/libraries/utilities主要包含了一些通用的标准函数

##### 1.2.3.10 wasn-jit

eos/libraries/wasn-jit中包含了一个WebAssembly的独立VM。 它可以加载标准的二进制格式，也可以加载WebAssembly参考解释器定义的文本格式。 对于文本格式，它可以加载标准堆栈机器语法和参考解释器使用的老式AST语法以及所有测试命令。

##  2 合约构建

钱包私钥

```
PW5JJY3FmQaPgC7Jd8qfyQ5cbsjs6iwRVJH8CPrD8gMwGysy94uPV
```

创建测试账户

```shell
test1
[hadoop@iz8vb9mhpnn0ub0s7clmjez hello]$ cleos create key --to-console
Private key: 5JxCyXAtC3BrJQ51fM2dv2BTPcdBf6HegwDLdpkMoTnyJ8LmcB1
Public key: EOS53SDdXnC8GTEzPHcYXog5yogE7DuhzPTSZcT41unraWqhbdTpB
```

创建测试账户

```shell
cleos system newaccount eosio test1 EOS53SDdXnC8GTEzPHcYXog5yogE7DuhzPTSZcT41unraWqhbdTpB EOS53SDdXnC8GTEzPHcYXog5yogE7DuhzPTSZcT41unraWqhbdTpB --stake-net "1000.00 SYS" --stake-cpu "1000.00 SYS" --buy-ram-kbytes 5000000
```

编译合约

```shell
eosio-cpp -I include -o hello.wasm hello.cpp --abigen
```

部署hello合约到测试账户test1

```shell
cleos set contract test1 /home/hadoop/contracts/EOSLearning/EosDappLearning/lesson2/hello --abi hello.abi -p test1@active
```

测试调用hello合约

```shell
cleos push action test1 hi '["test"]' -p test
```

设置权限

```shell
cleos set account permission test1 active '{"threshold": 1,"keys": [{"key":"EOS53SDdXnC8GTEzPHcYXog5yogE7DuhzPTSZcT41unraWqhbdTpB", "weight":1}],"accounts":[{"permission":{"actor":"test1","permission":"eosio.code"},"weight":1}]}' owner -p test1
```

创建两个公私钥

```shell
公私钥1（用于game账户）
[hadoop@iz8vb9mhpnn0ub0s7clmjez EosDappLearning]$ cleos create key --to-console
Private key: 5JTxHzdahL28Zbmv3SFAsM4gdj2bkWxcfzpnomxX1sq3DR1kAWE
Public key: EOS7LiuhAo3GRYYy3JnPQSYKfcwum2RJzVfnLPF5kQjQ6YeNmc5aj
公私钥2（用于playgame）
[hadoop@iz8vb9mhpnn0ub0s7clmjez playgame]$ cleos create key --to-console
Private key: 5JaQgPWYCuoRiVy4RTmiWD94Y8M7SLkTGLoHtGgKc8tv3PyHihZ
Public key: EOS7554Dh1bdsom16ZwFiyDmwTRkwkmU6Rtag6igdKetKRBub9fjf
```

创建playgame账户和 game账户

```shell
playgame账户：
cleos system newaccount eosio playgame EOS7554Dh1bdsom16ZwFiyDmwTRkwkmU6Rtag6igdKetKRBub9fjf EOS7554Dh1bdsom16ZwFiyDmwTRkwkmU6Rtag6igdKetKRBub9fjf --stake-net "1000.00 SYS" --stake-cpu "1000.00 SYS" --buy-ram-kbytes 5000000
game账户：
cleos system newaccount eosio game EOS7LiuhAo3GRYYy3JnPQSYKfcwum2RJzVfnLPF5kQjQ6YeNmc5aj EOS7LiuhAo3GRYYy3JnPQSYKfcwum2RJzVfnLPF5kQjQ6YeNmc5aj --stake-net "1000.00 SYS" --stake-cpu "1000.00 SYS" --buy-ram-kbytes 5000000
```

![25.59](/25.59.png)

编译playgame合约

```shell
eosio-cpp -I include -o playgame.wasm playgame.cpp --abigen
```

![887.29.11](/887.29.11.png)

部署playgame合约

```shell
cleos set contract playgame /home/hadoop/contracts/EOSLearning/EosDappLearning/lesson2/playgame --abi playgame.abi -p playgame@active
```

![887.31.05](/887.31.05.png)

设置palygame账户权限

```
cleos set account permission playgame active '{"threshold": 1,"keys": [{"key":"EOS7554Dh1bdsom16ZwFiyDmwTRkwkmU6Rtag6igdKetKRBub9fjf", "weight":1}],"accounts":[{"permission":{"actor":"playgame","permission":"eosio.code"},"weight":1}]}' owner -p playgame
```

![887.35.15](/887.35.15.png)

转账playgame账户HELLO币

```shell
cleos action eosio.token issue '["playgame","100.00 HELLO"]' -p hello
```

![887.36.49](/887.36.49.png)

使用内部调用向game账户转账

```shell
 cleos push action playgame gametransfer '["game"]' -p playgame
```

![887.43.18](/887.43.18.png)

使用延时调用向game账户转账

```shell
cleos push action playgame delaygame '["game"]' -p playgame
```

![887.44.56](/887.44.56.png)

