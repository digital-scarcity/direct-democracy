
Private key: 5JckCei8yfNXspj9Unffy6cKseKYM4dpgM5wrrPV8pJMit5a3Fn
Public key: EOS5nMxgaQPwMy1MAQ8s2ovzK96jbGXwkLSNQJSsBnrWnNKBoZvZj

dratstrail11        RAM: 
dratstokens1        RAM:  

dratsrimpers1       RAM: 
dratsuser111
dratsuser112
dratsuser113
dratsuser114

cleos -u http://test.telos.kitchen set contract dratstrail11 ~/dev/trail-service/contracts/build/trail
cleos -u http://test.telos.kitchen set contract dratstokens ~/dev/tokensale/token/token
cleos -u http://test.telos.kitchen set contract dratsrimpers1 directdemocr/directdemocr

# change permissions

# deploy trail
cleos -u http://test.telos.kitchen push action dratstrail11 setconfig '["2.0", true]' -p dratstrail11
cleos -u http://test.telos.kitchen push action dratstrail11 updatetime '["balcooldown", 1]' -p dratstrail11
cleos -u http://test.telos.kitchen push action dratstrail11 updatetime '["minballength", 1]' -p dratstrail11
cleos -u http://test.telos.kitchen push action dratstrail11 updatefee '["registry", "1.0000 TLOS"]' -p dratstrail11
cleos -u http://test.telos.kitchen push action dratstrail11 updatefee '["ballot", "1.0000 TLOS"]' -p dratstrail11
cleos -u http://test.telos.kitchen push action dratstrail11 updatefee '["archive", "1.0000 TLOS"]' -p dratstrail11

cleos -u http://test.telos.kitchen push action eosio.token transfer '["hyphadaotst1", "dratsrimpers1",  "250.0000 TLOS", "memo"]' -p hyphadaotst1 
cleos -u http://test.telos.kitchen push action eosio.token transfer '["hyphadaotst1", "dratsuser112",  "30.0000 TLOS", "memo"]' -p hyphadaotst1 
cleos -u http://test.telos.kitchen push action eosio.token transfer '["hyphadaotst1", "dratsuser113",  "30.0000 TLOS", "memo"]' -p hyphadaotst1 

cleos -u http://test.telos.kitchen push action eosio.token transfer '["dratsrimpers1", "dratstrail11",  "250.0000 TLOS", "deposit"]' -p dratsrimpers1 


# configure trail registry
cleos -u http://test.telos.kitchen push action dratstrail11 regvoter '["dratsrimpers1", "2,DD", ""]' -p dratsrimpers1
cleos -u http://test.telos.kitchen push action dratstrail11 regvoter '["dratsuser111", "2,DD", ""]' -p dratsuser111
cleos -u http://test.telos.kitchen push action dratstrail11 regvoter '["dratsuser112", "2,DD", ""]' -p dratsuser112
cleos -u http://test.telos.kitchen push action dratstrail11 regvoter '["dratsuser113", "2,DD", ""]' -p dratsuser113

cleos -u http://test.telos.kitchen push action dratstrail11 mint '["dratsrimpers1", "1.00 DD", "original mint"]' -p dratsrimpers1
cleos -u http://test.telos.kitchen push action dratstrail11 mint '["dratsuser111", "10.00 DD", "original mint"]' -p dratsrimpers1
cleos -u http://test.telos.kitchen push action dratstrail11 mint '["dratsuser112", "10.00 DD", "original mint"]' -p dratsrimpers1
cleos -u http://test.telos.kitchen push action dratstrail11 mint '["dratsuser113", "10.00 DD", "original mint"]' -p dratsrimpers1

cleos -u http://test.telos.kitchen set contract dratsrimpers1 dratsrimpers1/
cleos -u http://test.telos.kitchen push action dratsrimpers1 setconfig '["dratstrail11", "10000.00 DD"]' -p dratsrimpers1


cleos -u http://test.telos.kitchen push action dratsrimpers1 sayhello '[]' -p dratsrimpers1


cleos -u http://test.telos.kitchen set contract -sjd -x 186400 dratsrimpers1 ddupgrade/ddupgrade/ > upgrade.json


cleos -u http://test.telos.kitchen push action dratsrimpers1 remproposal '["upgrade"]' -p dratsuser111

cleos -u http://test.telos.kitchen push action dratsrimpers1 propose ./upgrade.json -p dratsuser111

cleos -u http://test.telos.kitchen get table dratstrail11 dratstrail11 registries
cleos -u http://test.telos.kitchen get table dratstrail11 dratstrail11 ballots

cleos -u http://test.telos.kitchen push action dratstrail11 castvote '["dratsuser111", "upgrade1", ["pass"]]' -p dratsuser111
cleos -u http://test.telos.kitchen push action dratstrail11 castvote '["dratsuser112", "upgrade1", ["pass"]]' -p dratsuser112
cleos -u http://test.telos.kitchen push action dratstrail11 castvote '["dratsuser113", "upgrade1", ["fail"]]' -p dratsuser113

cleos -u http://test.telos.kitchen push action dratsrimpers1 exec '["dratsuser111", "upgrade1", "dratsuser111"]' -p dratsuser111 



