#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/action.hpp>
#include <eosiolib/symbol.hpp>
#include <eosiolib/singleton.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/transaction.hpp>
#include <eosiolib/crypto.h>
#include <eosiolib/dispatcher.hpp>

using namespace eosio;
using namespace std;

class actionkpi :public contract {
public:
    using contract::contract;

    //记录kpi，发放奖励
    [[eosio::action]]
    void recordkpi(name user, uint64_t kpi) {
        require_auth(_self);

        //记录用户的数据信息
        work_index works(_self,_self);

        works.emplace(_self,[&](auto& work) {
            work.worker = user;
            work.token = asset(10 * kpi * 100, symbol("EOS", 2);
            work.score = kpi;
        });

        //给用户转账
        //使用内联接调用
        action(permission_level{_self, name("active")},
                name("eosio.token"), name("transfer"),
                std::make_tuple(_self, user, asset(10 * kpi * 100, symbol("EOS",2)),
                                std::string("record kpi and send EOStoken"))).send();
    }

    //添加数据
    [[eosio::action]]
    void addrecord(name user) {
        require_auth(_self);

        //增加数据
        work_index works(_self,_self);
        works.emplace(_self,[&](auto& work) {
            work.worker = user;
            work.token = asset(0, symbol("EOS", 2));
            work.score = 0;
        });
    }

    //删除数据
    [[eosio::action]]
    void delrecord(uint64_t id) {
        work_index works(_self, _self.value);
        auto iterator = works.find(id);
        works.erase(iterator);
    }

    //定义表结构
    struct [[eosio::table]] work {
        uint64_t id;
        name worker;
        asset token;
        uint64_t score;
        uint64_t create_time = current_time();

        //主键
        uint64_t primary_key() const { return id;}


        //序列化数据
        EOSLIB_SERIALIZE( work, (id)(worker)(token)(score))
    };
    //参数1 表名称 最长 12 个字符，只能由小写字母，数字 1-5、"."组成。
    //参数2 表对象 表中 行的定义
    //参数3 可变参数index 最多支持16个 必须返回结构中定义的常量类型，返回二级索引或者引用

    typedef eosio::multi_index<"work"_n ,work> work_index;

};
#define EOSIO_DISPATCH_CUSTOM(TYPE, MEMBERS) \
extern "C" { \
   void apply( uint64_t receiver, uint64_t code, uint64_t action ) { \
   auto self = receiver; \
      if(( code == self&&action != name("transfer").value) ) { \
        switch( action ) { \
            EOSIO_DISPATCH_HELPER( TYPE, MEMBERS ) \
         } \
         /* does not allow destructor of this contract to run: eosio_exit(0); */ \
      } \
   } \
} \

EOSIO_DISPATCH_CUSTOM(actionkpi, (recordkpi)(addrecord)(delrecord))