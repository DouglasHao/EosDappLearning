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
	void add(name from){
		// 构造函数 multi_index(uint64_t code, uint64_t scope)
		// 参数1 code拥有表的账户，可以对数据库具有读写权限
		// 参数2 scope 范围表示标识符

		work_index works(_self,_self.value);

		works.emplace(_self, [&](auto& work){
			work.id = works.available_primary_key();
			work.worker = from;
			work.score = 0;
			work.token = asset(0,symbol("EOS",2));
		});
	}

	[[eosio::action]]
	void kpi(name from, uint64_t recordkpi){

		action(permission_level{_self, name("active")},
			   name("eosio.token"), name("transfer"),
			   std::make_tuple(_self, from, asset(recordkpi * 10 * 100, symbol("EOS", 2)),
							   std::string("start to record kpi and send token")) ).send();

		work_index works(_self,_self.value);
		works.emplace(_self, [&](auto& work){
			work.worker = from;
			work.score = recordkpi;
			work.token = asset(recordkpi * 10 * 100, symbol("EOS",2));
		});
	}


	struct [[eosio::table]] work{
		uint64_t id;//用户ID
		name worker;//用户名
		asset token;//token数量
		uint64_t score;//记录kpi
		uint64_t create_time = current_time();
		//主键
		uint64_t primary_key() const { return  id; }

		//序列化数据
		EOSLIB_SERIALIZE(work, (id)(worker)(token)(score)(create_time))
	};

	typedef eosio::multi_index<"work"_n, work> work_index;


};



#define EOSIO_DISPATCH_CUSTOM(TYPE, MEMBERS) \
extern "C" { \
   void apply( uint64_t receiver, uint64_t code, uint64_t action ) { \
   auto self = receiver; \
      if(( code == self&&action != name("transfer").value) || code == name("eosio.token").value && action == name("transfer").value) { \
        switch( action ) { \
            EOSIO_DISPATCH_HELPER( TYPE, MEMBERS ) \
         } \
         /* does not allow destructor of this contract to run: eosio_exit(0); */ \
      } \
   } \
} \

EOSIO_DISPATCH_CUSTOM(actionkpi, (add)(kpi))





