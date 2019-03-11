#pragma once

#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>

using namespace eosio;

CONTRACT token : public contract
{
  public:
    token(name self) : contract(self){};
    
    ACTION create(name issuer, asset maximum_supply);
    ACTION issue(name to, asset quantity, std::string memo);
    ACTION transfer(name from, name to, asset quantity, std::string memo);

    inline asset get_supply(symbol_code sym) const;
    inline asset get_balance(name owner, symbol_code sym) const;
  
  private:
    TABLE account
    {
        asset balance;

        uint64_t primary_key() const { return balance.symbol.code().raw(); }
    };

    TABLE currency_stats
    {
        asset supply;
        asset max_supply;
        name issuer;

        uint64_t primary_key() const { return supply.symbol.code().raw(); }
    };

    typedef eosio::multi_index<"accounts"_n, account> accounts;
    typedef eosio::multi_index<"stat"_n, currency_stats> stats;

    void sub_balance(name owner, asset value);
    void add_balance(name owner, asset value, name ram_payer);

  public:
    struct transfer_args
    {
        name from;
        name to;
        asset quantity;
        std::string memo;
    };
};

asset love_tattoo::get_supply(symbol_code sym) const
{
    stats statstable(_self, sym.raw());
    const auto &st = statstable.get(sym.raw());
    return st.supply;
}

asset love_tattoo::get_balance(name owner, symbol_code sym) const
{
    accounts accountstable(_self, owner.value);
    const auto &ac = accountstable.get(sym.raw());
    return ac.balance;
}
