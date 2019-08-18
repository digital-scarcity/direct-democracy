#pragma once

#include <eosio/binary_extension.hpp>
#include <eosio/eosio.hpp>
#include <eosio/ignore.hpp>
#include <eosio/transaction.hpp>
#include <eosio/asset.hpp>
#include <eosio/singleton.hpp>

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
                     const asset& voting_asset);

      [[eosio::action]]
      void propose(ignore<name> proposer, 
                     ignore<name> proposal_name,
                     ignore<transaction> trx);

      [[eosio::action]]
      void exec( name proposer, name proposal_name, name executer );

      [[eosio::action]]
      void sayhello (); 

      [[eosio::action]]
      void remproposal (const name& proposal_name);

      using propose_action = eosio::action_wrapper<"propose"_n, &directdemocr::propose>;
      using exec_action = eosio::action_wrapper<"exec"_n, &directdemocr::exec>;

   private:

      struct [[eosio::table ]] config {
         name                 trail_service;
         asset                voting_asset;
      };

      typedef singleton<"config"_n, config> config_singleton;

      struct [[eosio::table]] proposal {
         name                            proposal_name;
         std::vector<char>               packed_transaction;

         uint64_t primary_key()const { return proposal_name.value; }
      };

      typedef eosio::multi_index< "proposals"_n, proposal > proposals;
};
