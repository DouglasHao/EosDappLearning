#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/action.hpp>
#include <eosiolib/symbl.hpp>
#include <eosiolib/singleton.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/transaction.hpp>
#include <eosiolib/crypto.h> 

using namespace eosio;

class hello : public contract {
public:
	using contract::contract;

	[[eosio::action]]
	void hi( name user ) {
		auto values = user.value;
		print("hello user value :", values);
		print(", Hello,: ", user);

		//内联调用 语法说明
		//action(
		//permission_level, //权限
		//code, 合约
		//action, 合约方法
		//data); 调用数据

		action(permission_level{_self, name("active")},
              	name("eosio.token"), name("transfer"),
                std::make_tuple(_self, user, asset(1, symbol("EOS", 2)),
                                std::string("game send  eos")) ).send();
	}
};

EOSIO_DISPATCH( hello, (hi))