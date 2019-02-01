#include<eosiolib/eosio.hpp>
#include<eosiolib/print.hpp>

using namespace eosio;

class hello : public contract {
public:
	using contract::contract;

	[[eosio::action]]
	void hi( name user ) {
		auto values = user.value;
		print("hello user value :", values)
		print(",: Hello,: ", user)
	}
};

EOSIO_DISPATCH( hello, (hi))