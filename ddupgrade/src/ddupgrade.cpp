#include <ddupgrade.hpp>
#include <trail.hpp>

#include <eosio/action.hpp>
#include <eosio/permission.hpp>
#include <eosio/crypto.hpp>


// namespace eosio {

void ddupgrade::setconfig (const name& trail_service,
                              const asset& voting_asset) 
{
   require_auth (get_self());
   config_singleton config_s (get_self(), get_self().value);
   config c = config_s.get_or_create(get_self(), config());
   c.trail_service = trail_service;
   c.voting_asset = voting_asset;
   config_s.set (c, get_self());

   // action (
   //    permission_level{get_self(), "active"_n},
   //    trail_service, "newregistry"_n,
   //    std::make_tuple(get_self(), voting_asset, "public"_n))
   // .send();
}

void ddupgrade::remproposal (const name& proposal_name) {
   proposals proptable( get_self(), get_self().value );
   proptable.erase ( proptable.find( proposal_name.value ));
}

void ddupgrade::sayhello () {
   print (" Hello - this is version 2.0 -- upgraded");
}

void ddupgrade::saybye () {
   print (" Bye ");
}

void ddupgrade::reset () 
{
   require_auth(get_self());

   proposals proptable( get_self(), get_self().value );
   auto p_itr = proptable.begin();
   while (p_itr != proptable.end()) {
      p_itr = proptable.erase (p_itr);
   }
}

void ddupgrade::propose( ignore<name> proposer,
                        ignore<name> proposal_name,
                        // ignore<std::vector<permission_level>> requested,
                        ignore<transaction> trx )
{
   name _proposer;
   name _proposal_name;
   // std::vector<permission_level> _requested;
   transaction_header _trx_header;

   _ds >> _proposer >> _proposal_name; // >> _requested;

   const char* trx_pos = _ds.pos();
   size_t size    = _ds.remaining();
   _ds >> _trx_header;

   require_auth( _proposer );
   check( _trx_header.expiration >= eosio::time_point_sec(current_time_point()), "transaction expired" );
   //check( trx_header.actions.size() > 0, "transaction must have at least one action" );

   proposals proptable( get_self(), get_self().value );
   check( proptable.find( _proposal_name.value ) == proptable.end(), "proposal with the same name exists" );

   std::vector<char> pkd_trans;
   pkd_trans.resize(size);
   memcpy((char*)pkd_trans.data(), trx_pos, size);
   proptable.emplace( _proposer, [&]( auto& prop ) {
      prop.proposal_name       = _proposal_name;
      prop.packed_transaction  = pkd_trans;
   });

   config_singleton config_s (get_self(), get_self().value);
   config c = config_s.get ();

   vector<name> options;
   options.push_back ("pass"_n);
   options.push_back ("fail"_n);

   action (
      permission_level{get_self(), "active"_n},
      c.trail_service, "newballot"_n,
      std::make_tuple(_proposal_name, "proposal"_n, get_self(), c.voting_asset.symbol, "1acct1vote"_n, options))
   .send();

   action (
      permission_level{get_self(), "active"_n},
      c.trail_service, "togglebal"_n,
      std::make_tuple(_proposal_name, "votestake"_n))
   .send();

   auto expiration = time_point_sec(current_time_point()) + 65;
   
   action (
      permission_level{get_self(), "active"_n},
      c.trail_service, "readyballot"_n,
      std::make_tuple(_proposal_name, expiration))
   .send();
}


void ddupgrade::exec( name proposer, name proposal_name, name executer ) {
   require_auth( executer );

   config_singleton config_s (get_self(), get_self().value);
   config c = config_s.get ();

   action (
      permission_level{get_self(), "active"_n},
      c.trail_service, "closeballot"_n,
      std::make_tuple(proposal_name, false))
   .send();

   trail::ballots_table b_t (c.trail_service, c.trail_service.value);
   auto b_itr = b_t.find (proposal_name.value);
   check (b_itr != b_t.end(), "Ballot for proposal: " + proposal_name.to_string() + " not found.");

   map<name, asset> votes = b_itr->options;
   asset votes_pass = votes.at("pass"_n);
   asset votes_fail = votes.at("fail"_n);

   if (votes_pass > votes_fail) {
      proposals proptable( get_self(), get_self().value );
      auto& prop = proptable.get( proposal_name.value, "proposal not found" );
      transaction_header trx_header;
      datastream<const char*> ds( prop.packed_transaction.data(), prop.packed_transaction.size() );
      ds >> trx_header;
      check( trx_header.expiration >= eosio::time_point_sec(current_time_point()), "transaction expired" );

      print (" Executing transaction for proposal  : ", proposal_name.to_string(), "\n");
      send_deferred( (uint128_t(proposer.value) << 64) | proposal_name.value, executer,
                     prop.packed_transaction.data(), prop.packed_transaction.size() );

      proptable.erase(prop);
   }
}

