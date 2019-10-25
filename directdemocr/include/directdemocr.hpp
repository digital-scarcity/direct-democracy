#pragma once

#include <eosio/binary_extension.hpp>
#include <eosio/eosio.hpp>
#include <eosio/ignore.hpp>
#include <eosio/transaction.hpp>
#include <eosio/asset.hpp>
#include <eosio/singleton.hpp>

#include "permissions.hpp"

using namespace eosio;
using std::string;
using std::vector;
using std::map;

class [[eosio::contract("directdemocr")]] directdemocr : public contract {
   public:
      using contract::contract;

      ACTION reset ();

      [[eosio::action]]
      void setconfig (const name& trail_service,
                     const asset& voting_asset,
                     const name& voting_asset_token_contract,
                     const float& quorum);

      [[eosio::action]]
      void addnoncircac (const name& account);

      [[eosio::action]]
      void propose(ignore<name> proposer, 
                     ignore<name> proposal_name,
                     ignore<transaction> trx);

      [[eosio::action]]
      void exec( name proposer, name proposal_name, name executer );

      [[eosio::action]]
      void sayhello (); 

      [[eosio::action]]
      void permtoprime (const name& account); 

      [[eosio::action]]
      void addkey (const name& account, const string& key);

      [[eosio::action]]
      void remproposal (const name& proposal_name);

      using propose_action = eosio::action_wrapper<"propose"_n, &directdemocr::propose>;
      using exec_action = eosio::action_wrapper<"exec"_n, &directdemocr::exec>;

   private:

      struct [[eosio::table ]] config {
         name                 trail_service;
         asset                voting_asset;
         name                 voting_asset_token_contract;
         float                quorum;
         std::vector<name>    noncirculating_balances;
      };

      typedef singleton<"config"_n, config> config_singleton;

      struct [[eosio::table]] proposal {
         name                 proposal_name;
         std::vector<char>    packed_transaction;

         uint64_t primary_key()const { return proposal_name.value; }
      };

      typedef eosio::multi_index< "proposals"_n, proposal > proposals;

      struct [[eosio::table]] currency_stats
      {
         asset supply;
         asset max_supply;
         name issuer;

         uint64_t primary_key() const { return supply.symbol.code().raw(); }
      };

      typedef eosio::multi_index<"stat"_n, currency_stats> stats;

      asset get_circulating_supply () {
         config_singleton config_s (get_self(), get_self().value);
         config c = config_s.get_or_create(get_self(), config());

         stats s_t (c.voting_asset_token_contract, c.voting_asset.symbol.code().raw());
         auto s_itr = s_t.find (c.voting_asset.symbol.code().raw());
         check (s_itr != s_t.end(), "Fatal error. Voting asset symbol not found in voting asset token contract.");
         
         asset supply = s_itr->supply;
         for ( name &acct : c.noncirculating_balances ) {
            accounts a_t (c.voting_asset_token_contract, acct.value);
            auto a_itr = a_t.find (c.voting_asset.symbol.code().raw());
            if (a_itr != a_t.end()) {
               if (a_itr->balance.amount > 0) {
                  supply -= a_itr->balance;
               }
            }
         }

         return supply;
      }

      asset adjust_asset(const asset &original_asset, const float &adjustment)
      {
         return asset{static_cast<int64_t>(original_asset.amount * adjustment), original_asset.symbol};
      }
};
