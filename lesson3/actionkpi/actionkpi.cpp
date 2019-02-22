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


	void kpi(name user, uint64_t score) {
		work_index works(_self, _self.value);
		works.emplace(_self,[&](auto& work) {
			work.worker = user;
			work.score = score;
			work.token = asset(score * 10 * 100, symbol("EOS",2));
		});

		action(permission_level{_self, name("active")},
        name("eosio.token"), name("transfer"),
        std::make_tuple(_self, from, asset(score * 10 * 100, symbol("EOS", 2)),
        std::string("game send eos")) ).send();
	}

	struct [[eosio::table]] work {
		uint64_t id;
		name worker;
		asset token;
		uint64_t score;
		uint64_t create_time = current_time();

		uint64_t primary_key() const {return id;}

		//序列化
		EOSLIB_SERIALIZE(work, (id)(worker)(token)(score)(create_time))
	};


	//实例化
	typedef eosio::multi_index<"work"_n, work> work_index;
}

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

EOSIO_DISPATCH_CUSTOM(actionkpi, (kpi))