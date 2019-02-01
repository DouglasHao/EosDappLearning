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

class playgame : public contract {
public:
	using contract::contract;
	

	[[eosio::action]]
	void gametransfer( name user ) {
		require_auth(_self);
		auto values = user.value;
		print("game user value :", values);
		print(", gameuser: ", user);

		//内联调用 语法说明
		//action(
		//permission_level, //权限
		//code, 合约
		//action, 合约方法
		//data); 调用数据

		action(permission_level{_self, name("active")},
              	name("eosio.token"), name("transfer"),
                std::make_tuple(_self, user, asset(1, symbol("HELLO", 2)),
                                std::string("game send  hello")) ).send();
	}

	//延时调用
	[[eosio::action]]
	void delaygame(string memo) {
		eosio::transaction t{};
		t.actions.emplace_back(eosio::permission_level(_self,name("active")),
								name("playgame"),name("gametransfer"),
								std::make_tuple(name("game")));
		t.delay_sec = 1;
		t.send(1, _self, false);
		print("delay end");
	}

	[[eosio::action]]
	void transfer(name from, name to, asset quantity, string memo) {
		print("transfer from", from);
		eosio_assert(0," transfer error");
	}		
};

#define EOSIO_DISPATCH_CUSTOM(TYPE, MEMBERS) \
extern "C" { \
   void apply( uint64_t receiver, uint64_t code, uint64_t action ) { \
   auto self = receiver; \
      if(( code == self&&action != name("transfer").value) ||code == name("hello").value && action == name("transfer").value ) { \
        switch( action ) { \
            EOSIO_DISPATCH_HELPER( TYPE, MEMBERS ) \
         } \
         /* does not allow destructor of this contract to run: eosio_exit(0); */ \
      } \
   } \
} \

EOSIO_DISPATCH_CUSTOM(playgame, (gametransfer)(delaygame)(transfer))
