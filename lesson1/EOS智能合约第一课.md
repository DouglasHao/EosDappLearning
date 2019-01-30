# EOS智能合约第一课

## 0 开始之前

### 0.1 第一课作业内容

```
第一课作业
1.搭建自己的节点
如果没有条件的可以使用EOS 的麒麟测试网。
测试网地址：https://api-kylin.eosasia.one
2.创建自己的账户
搭建测试网络的同学请创建系统账户，创建系统账户之前请创建系统token：SYS、EOS到eosio.token下
系统账户：eosio.stake、eosio.saving、eosio.ramfee、eosio.ram、eosio.names
eosio.msig、eosio.bpay
使用EOS的system合约创建账户，并且分配RAM，CPU资源。
3.创建自己的token，token名称、数量可以自己定义，并且尝试向其他账户转账。
作业提交请提交截图或者节点地址和账户名称验证
```

### 0.2 开发环境

```
阿里云服务器：47.92.80.175
系统：CentOS 7.4
配置：CPU 4核	内存 8G
```

## 1 搭建测试环境

### 1.1 下载并应用程序

下载安装EOS应用程序

```shell
wget https://github.com/eosio/eos/releases/download/v1.5.0/eosio-1.5.0-1.el7.x86_64.rpm
sudo yum install ./eosio-1.5.0-1.el7.x86_64.rpm
```

下载安装CDT

```shell
wget https://github.com/EOSIO/eosio.cdt/releases/download/v1.4.1/eosio.cdt-centos-1.4.1.x86_64-0.x86_64.rpm
sudo yum install ./eosio.cdt-centos-1.4.1.x86_64-0.x86_64.rpm
```

注：不同的系统，安装命令不同，可详见如下网站：

[1]: https://developers.eos.io/eosio-home/docs/setting-up-your-environment	"developers"

### 1.2 启动node

```shell
1.创建目录
  mkdir contracts
2.进入contracts
  cd contracts
3.创建目录
  mkdir 8888
4.启动节点
  nodeos -e -p eosio \
  --plugin eosio::producer_plugin \
  --plugin eosio::chain_api_plugin \
  --plugin eosio::http_plugin \
  --plugin eosio::history_plugin \
  --plugin eosio::history_api_plugin \
  --data-dir /home/hadoop/contracts/8888/eosio/data \
  --config-dir /home/hadoop/contracts/8888/eosio/config \
  --access-control-allow-origin='*' \
  --contracts-console \
  --http-validate-host=false \
  --verbose-http-errors \
  --filter-on='*' >> /home/hadoop/contracts/8888/nodeos.log 2>&1 &
```

注：启动节点后，会在目录8888中生成eosio和和nodes.log

### 1.3 设置测试网络能够被外网访问

```shell
需要更改config.ini文件同时要设置云服务器的安全组（云服务器的安全组设置完之后要重启云服务器）
修改完成之后要重启节点
1.关闭节点
  pkill nodeos
2.修改/home/hadoop/contracts/8888/eosio/config目录下的config.ini文件
  http-server-address监听地址 http-server-address = 127.0.0.1:8888 为 http-server-address = 0.0.0.0:8888
3.重新启动节点
  启动命令和1.2中保持一致
```

注：1.节点初次启动时，http-server-address监听地址为127.0.0.1:8888只能被服务器本地访问，要使外界也可以反问，则需要修改监听地址0.0.0.0:8888同时设置云服务器的安全组。2.config.ini文件修改后，需要重启节点才能生效。

### 1.4 测试外界api访问

使用浏览器或者postman测试节点能否被外界访问

```
http://47.92.80.175:8888/v1/chain/get_info
```

## 2 创建钱包

### 2.1 创建default钱包

```shell
cleos wallet create --to-console
```

default钱包的私钥

```
PW5JJY3FmQaPgC7Jd8qfyQ5cbsjs6iwRVJH8CPrD8gMwGysy94uPV
```

解锁钱包命令

```shell
cleos wallet unlock
```

输入钱包的私钥

```
私钥:PW5JJY3FmQaPgC7Jd8qfyQ5cbsjs6iwRVJH8CPrD8gMwGysy94uPV
```

### 2.2 导入eosio账户私钥到钱包

eosio公私钥

```
公钥：EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
私钥：5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3
```

导入eosio私钥到钱包

```
cleos wallet import
```

## 3 创建账户

### 3.1 创建账户的公私钥

```shell
cleos create key --to-console
```

```shell
公私钥1（用于测试账户hello）
[hadoop@iz8vb9mhpnn0ub0s7clmjez config]$ cleos create key --to-console
Private key: 5HzuRc2LBM7vkDq4GhFAWQSPYpwJ9xwtuScath6pNW59iXRhuwQ
Public key: EOS8cXtxTBPwN8P3ymhoCJAY46HiQP7b8j6H2FsFKGYC7ZQz17gdX
公私钥2(用于测试用户bob)
[hadoop@iz8vb9mhpnn0ub0s7clmjez config]$ cleos create key --to-console
Private key: 5JkkeBbojS1ZbMpGmbrbvoQzFFWUHfZJWLw9Fc27pBAyWsdUoQj
Public key: EOS6RSkh2xvMhjrSc1v9Bh1KRwCPaHZRYQwPT22gnEk8ynqv1a385
公私钥3（用于eosio.token）
[hadoop@iz8vb9mhpnn0ub0s7clmjez config]$ cleos create key --to-console
Private key: 5JBCVxABizJXtqStcPhDehhdLHug8AnUpvB4AP3o8Ei5PYxYbHe
Public key: EOS7BWP2kaezo4Tr5PwNjABFdPAAExMfHkGvwdKiNeZy2w9uPVUkG
公私钥4(用于测试账户hhb)
[hadoop@iz8vb9mhpnn0ub0s7clmjez config]$ cleos create key --to-console
Private key: 5JYzX49JepkeAwWi2r1nSg3QPvPRQJVJX4FnjYpXYdu23HnCeHv
Public key: EOS8Nxfy7PWTfVq1x22Y7EHKd8jKECZgn3gC7HnGVDuaEoSM7CE2k
公私钥5（用于系统账号eosio.stake）
[hadoop@iz8vb9mhpnn0ub0s7clmjez config]$ cleos create key --to-console
Private key: 5KTQ3GHZ2pdbwL6Wpvq1rY1Qytjq6LDWY83HAcNWKjCBuJRa4bZ
Public key: EOS8gfdt4wsW5saqDu8T9hfLDt2xJFyGzTnJGvU7t93bcgRGsS3RC
公私钥匙6（用于系统账号eosio.saving）
[hadoop@iz8vb9mhpnn0ub0s7clmjez config]$ cleos create key --to-console
Private key: 5KBHkMH5c9ZJvWyC1ZqT3jq5wFjucc7DsuPgGNLUVYsVeS94bMy
Public key: EOS7fArdRWmyNGcrJXtGQeEXgzCjU5TeyzNCiZsKLAezvmtKgiBym
公私钥7（用于系统账户eosio.ramfee）
[hadoop@iz8vb9mhpnn0ub0s7clmjez ~]$ cleos create key --to-console
Private key: 5J2ZN8viQ6yiX7ksVGCWDmqLbx1PXeMGHRMZmbLMvVsuLdxmBmW
Public key: EOS8KsuKV6iM3J4n67BoLmsVeWF2Nf2H1BsBRvL6qRSrvTvcZDbs5
公私钥8（用于系统账户eosio.ram）
[hadoop@iz8vb9mhpnn0ub0s7clmjez ~]$ cleos create key --to-console
Private key: 5K5dVDyXzhbRniG4kkmTWQQo2tCEBHJ1cuvgfSRdNrtn5Fh9aUC
Public key: EOS8j2Cf5st88fUU3kgT1qWLbSsgRsY7WBfHp4n4dtjpDVymLWGM1
公私钥9（用于系统账户eosio.names）
[hadoop@iz8vb9mhpnn0ub0s7clmjez ~]$ cleos create key --to-console
Private key: 5KdK5EqLNrtpPFfHcN74fa4XUatAg8TwVGBYYQH8pCCSsMNPD8R
Public key: EOS7vMwUcF5gjCXXH8KnF6j4zSakiCWnk2jsBhHS92YdHSQEdMyoX
公私钥10（用于系统账户eosio.msig）
[hadoop@iz8vb9mhpnn0ub0s7clmjez ~]$ cleos create key --to-console
Private key: 5JpCEcGKoUn278ttx5LfXzXbWP5oSRFbRb3bHRAQ1yzyDY6cYtM
Public key: EOS6MifPkQp8au7LupWNNxjUVJL4H6WyucctDdJE3LCYiC2qzYMum
公私钥11（用于系统账户eosio.bpay）
[hadoop@iz8vb9mhpnn0ub0s7clmjez ~]$ cleos create key --to-console
Private key: 5K7sX3oQP84Fw6hG9WsJeo7oWxx6xVQEba1TNDVxaHawjMpKJnA
Public key: EOS6wCSRinNTCe3xGoNfupNKJ4m1YgtBYVhsE3Ha2h763FHCUmro9
公私钥12（用于系统账户eosio.system）
[hadoop@iz8vb9mhpnn0ub0s7clmjez ~]$ cleos create key --to-console
Private key: 5JwfMvhytM1fgVVsvYUJZY7d9nrpPRXovYkGuYv1ELSGhGZ7VsK
Public key: EOS87RPCKY7qaVdJjjbcWCGq5hEftNBR4Uk6NKwH64tCnCJeuq7G8
公私钥13（用于系统创建账户test）
[hadoop@iz8vb9mhpnn0ub0s7clmjez eosio.system]$ cleos create key --to-console
Private key: 5JBvttojtmBFbRoPv1RAeEAAgEsPwE2h6jav1t2vzmGBNyKKmqw
Public key: EOS5ytXLgcpCDkpr6qbjzBCsbzRd6eV3gBktdt3VkXeGHXxbFAUns
```

### 3.2 创建测试账户

```
cleos create account eosio 账户名 公钥 私钥
cleos create account eosio hhb EOS64hqGVB7LBGSFFQB89Dt17YsFq4UxmCVFihs45Sw6CmLRU8NWc EOS64hqGVB7LBGSFFQB89Dt17YsFq4UxmCVFihs45Sw6CmLRU8NWc
创建game账户
Private key: 5K4gj99mfS87mvqWdDFZL2mmz2f3Gg2kRVY72poA31pbsT4K56b
Public key: EOS65vL3ybs8pouXeTMUkoNkv8AHurhWXx2gAWKeP5N5ZPi5HkgHH
```

查看账户状态

```
cleos get account 用户名
cleos get account hhb
```

### 3.3 创建eosio.token账户

```shell
cleos create account eosio eosio.token EOS7BWP2kaezo4Tr5PwNjABFdPAAExMfHkGvwdKiNeZy2w9uPVUkG EOS7BWP2kaezo4Tr5PwNjABFdPAAExMfHkGvwdKiNeZy2w9uPVUkG
```

## 4 编译部署eosio.token合约

下载代币合约

```shell
git clone https://github.com/EOSIO/eosio.contracts --branch v1.4.0 --single-branch
```

编译eosio.token合约

```shell
eosio-cpp -I include -o eosio.token.wasm src/eosio.token.cpp --abigen
```

部属eosio.token合约

```shell
cleos set contract eosio.token /home/hadoop/contracts/8888/eosio.contracts/eosio.token --abi eosio.token.abi -p eosio.token@active
```

![屏幕快照 2019-01-30 上午9.45.13](/Users/mac/MyDocument/EOSLearning/EosDappLearning/lesson1/picture/屏幕快照 2019-01-30 上午9.45.13.png)

## 5 创建系统token

创建系统token：SYS 和 EOS

```
cleos push action eosio.token create '["eosio","1000000.00 SYS"]' -p eosio.token
```

![屏幕快照 2019-01-30 上午11.07.34](/Users/mac/MyDocument/EOSLearning/EosDappLearning/lesson1/picture/屏幕快照 2019-01-30 上午11.07.34.png)

```
cleos push action eosio.token create '["eosio","1000000.00 EOS"]' -p eosio.token
```

![屏幕快照 2019-01-30 上午11.10.47](/Users/mac/MyDocument/EOSLearning/EosDappLearning/lesson1/picture/屏幕快照 2019-01-30 上午11.10.47.png)

## 6 发行自己的token

发行hello币给hello账户

```
cleos push action eosio.token create '["hello","1000000.00 HELLO"]' -p eosio.token
```

![屏幕快照 2019-01-30 上午11.15.11](/Users/mac/MyDocument/EOSLearning/EosDappLearning/lesson1/picture/屏幕快照 2019-01-30 上午11.32.07.png)

发行HELLO token即向测试账户bob转账HELLO token

```
cleos push action eosio.token issue '["bob", "100.00 HELLO", "memo"]' -p hello
cleos push action eosio.token issue '[ "alice", "100.0000 SYS", "memo" ]' -p eosio@active
```

![屏幕快照 2019-01-30 上午11.32.07](/Users/mac/MyDocument/EOSLearning/EosDappLearning/lesson1/picture/屏幕快照 2019-01-30 上午11.32.07.png)

查询token发行信息

```shell
cleos get table eosio.token HELLO stat
```

![屏幕快照 2019-01-30 上午11.34.26](/Users/mac/MyDocument/EOSLearning/EosDappLearning/lesson1/picture/屏幕快照 2019-01-30 上午11.34.26.png)

测试从bob账户向hhb账户转帐

```shell
cleos push action eosio.token transfer '["bob","hhb","10.00 HELLO","memotest"]' -p bob
```

![屏幕快照 2019-01-30 上午11.50.31](/Users/mac/MyDocument/EOSLearning/EosDappLearning/lesson1/picture/屏幕快照 2019-01-30 上午11.50.31.png)

注：转账的token小数点后的数字个数要和发行时的一致。

转账完成之后查看双方账户

```shell
cleos get currency balance eosio.token bob
cleos get currency balance eosio.token hhb
```

![屏幕快照 2019-01-30 上午11.53.55](/Users/mac/MyDocument/EOSLearning/EosDappLearning/lesson1/picture/屏幕快照 2019-01-30 上午11.53.55.png)

## 7 编译部署eosio.system合约

编译eosio.system合约

```shell
eosio-cpp -I include -o eosio.system.wasm src/eosio.system.cpp --abigen
```

部署eosio.system合约

```shell
cleos set contract eosio /home/hadoop/contracts/8888/eosio.contracts/eosio.system -p eosio@active
```

![屏幕快照 2019-01-30 下午7.43.32](/Users/mac/MyDocument/EOSLearning/EosDappLearning/lesson1/picture/屏幕快照 2019-01-30 下午7.43.32.png)

初始化eosio.system合约

```shell
cleos push action eosio init '["0", "2,SYS"]' -p eosio@active
注意：这里使用的2是创建token时小数点的个数。
```

![WechatIMG34](/Users/mac/MyDocument/EOSLearning/EosDappLearning/lesson1/picture/WechatIMG34.png)

创建test账户并分配资源

```shell
cleos system newaccount eosio test EOS5ytXLgcpCDkpr6qbjzBCsbzRd6eV3gBktdt3VkXeGHXxbFAUns EOS5ytXLgcpCDkpr6qbjzBCsbzRd6eV3gBktdt3VkXeGHXxbFAUns --stake-net "1000.00 SYS" --stake-cpu "1000.00 SYS" --buy-ram-kbytes 5000000
```

![屏幕快照 2019-01-30 下午4.51.50](/Users/mac/MyDocument/EOSLearning/EosDappLearning/lesson1/picture/屏幕快照 2019-01-30 下午4.51.50.png)

