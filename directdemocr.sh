
cleos -u http://blocks.telos.kitchen:8889 push action eosio.token transfer '["hyphadaotst1", "directdemocr",  "250.0000 TLOS", "memo"]' -p hyphadaotst1 
cleos -u http://blocks.telos.kitchen:8889 push action eosio.token transfer '["hyphadaotst1", "directduser2",  "30.0000 TLOS", "memo"]' -p hyphadaotst1 
cleos -u http://blocks.telos.kitchen:8889 push action eosio.token transfer '["hyphadaotst1", "directduser3",  "30.0000 TLOS", "memo"]' -p hyphadaotst1 

# deploy trail
cleos -u http://blocks.telos.kitchen:8889 set contract digscartrail trail-service/contracts/build/trail

cleos -u http://blocks.telos.kitchen:8889 push action digscartrail setconfig '["2.0", true]' -p digscartrail
cleos -u http://blocks.telos.kitchen:8889 push action digscartrail updatetime '["balcooldown", 1]' -p digscartrail
cleos -u http://blocks.telos.kitchen:8889 push action digscartrail updatetime '["minballength", 1]' -p digscartrail

cleos -u http://blocks.telos.kitchen:8889 push action eosio.token transfer '["directdemocr", "digscartrail",  "250.0000 TLOS", "deposit"]' -p directdemocr 


# configure trail registry
cleos -u http://blocks.telos.kitchen:8889 push action digscartrail newregistry '["directdemocr", "10000.00 DD", "public"]' -p directdemocr
cleos -u http://blocks.telos.kitchen:8889 push action digscartrail regvoter '["directdemocr", "2,DD", ""]' -p directdemocr

cleos -u http://blocks.telos.kitchen:8889 push action digscartrail regvoter '["directduser1", "2,DD", ""]' -p directduser1
cleos -u http://blocks.telos.kitchen:8889 push action digscartrail regvoter '["directduser2", "2,DD", ""]' -p directduser2
cleos -u http://blocks.telos.kitchen:8889 push action digscartrail regvoter '["directduser3", "2,DD", ""]' -p directduser3

cleos -u http://blocks.telos.kitchen:8889 push action digscartrail mint '["directduser1", "10.00 DD", "original mint"]' -p directdemocr
cleos -u http://blocks.telos.kitchen:8889 push action digscartrail mint '["directduser2", "10.00 DD", "original mint"]' -p directdemocr
cleos -u http://blocks.telos.kitchen:8889 push action digscartrail mint '["directduser3", "10.00 DD", "original mint"]' -p directdemocr

cleos -u http://blocks.telos.kitchen:8889 set contract directdemocr directdemocr/
cleos -u http://blocks.telos.kitchen:8889 push action directdemocr setconfig '["digscartrail", "10000.00 DD"]' -p directdemocr


cleos -u http://blocks.telos.kitchen:8889 push action directdemocr sayhello '[]' -p directdemocr


cleos -u http://blocks.telos.kitchen:8889 set contract -sjd -x 186400 directdemocr ddupgrade/ddupgrade/ > upgrade.json


cleos -u http://blocks.telos.kitchen:8889 push action directdemocr remproposal '["upgrade"]' -p directduser1

cleos -u http://blocks.telos.kitchen:8889 push action directdemocr propose ./upgrade.json -p directduser1

cleos -u http://blocks.telos.kitchen:8889 get table digscartrail digscartrail registries
cleos -u http://blocks.telos.kitchen:8889 get table digscartrail digscartrail ballots

cleos -u http://blocks.telos.kitchen:8889 push action digscartrail castvote '["directduser1", "upgrade1", ["pass"]]' -p directduser1
cleos -u http://blocks.telos.kitchen:8889 push action digscartrail castvote '["directduser2", "upgrade1", ["pass"]]' -p directduser2
cleos -u http://blocks.telos.kitchen:8889 push action digscartrail castvote '["directduser3", "upgrade1", ["fail"]]' -p directduser3

cleos -u http://blocks.telos.kitchen:8889 push action directdemocr exec '["directduser1", "upgrade1", "directduser1"]' -p directduser1 



