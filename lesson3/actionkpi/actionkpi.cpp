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

class actionkpi : public contract {
public:
	using contract::contract;

	[[eosio::action]]
	void add(name from, uint64_t phone){
		// 构造函数 multi_index(uint64_t code, uint64_t scope)
		// 参数1 code拥有表的账户，可以对数据库具有读写权限
		// 参数2 scope 范围表示标识符

		work_index works(_self,_self.value);

		works.emplace(_self, [&](auto& work){
			work.worker = from;
			work.phone = phone;
			work.score = asset(1,symbol("EOS",2));
		});
	}


	struct [[eosio::table]] work{
		name worker;//姓名
		asset score;//token数量
		uint64_t phone;//电话

		//主键
		uint64_t primary_key() const { return  worker.value; }
		//二级索引
		uint64_t get_phone() const { return phone; }
		//序列化数据
		EOSLIB_SERIALIZE(work, (worker)(score)(phone))
	};

	typedef eosio::multi_index<"work"_n, work ,
			indexed_by<"phone"_n, const_mem_fun<work, uint64_t, &work:: get_phone>>> work_index;


}

/*
#define EOSIO_DISPATCH_CUSTOM(TYPE, MEMBERS) \
extern "C" { \
   void apply( uint64_t receiver, uint64_t code, uint64_t action ) { \
   auto self = receiver; \
      if(( code == self&&action != name("transfer").value) || code == name("eosio.token").value && action == name("transfer").value) { \
        switch( action ) { \
            EOSIO_DISPATCH_HELPER( TYPE, MEMBERS ) \
         } \
         */
/* does not allow destructor of this contract to run: eosio_exit(0); *//*
 \
      } \
   } \
} \

EOSIO_DISPATCH_CUSTOM(actionkpi, (add))*/
EOSIO_DISPATCH(actionkpi, (add))