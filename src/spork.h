// Copyright (c) 2015 The Metrix developers
// Copyright (c) 2009-2012 The Darkcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_SPORK_H
#define BITCOIN_SPORK_H

#include "base58.h"
#include "darksend.h"
#include "key.h"
#include "main.h"
#include "net.h"
#include "protocol.h"
#include "sync.h"
#include "util.h"
#include "wallet_ismine.h"

#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;

//! Don't ever reuse these IDs for other sporks
#define SPORK_1_MASTERNODE_PAYMENTS_ENFORCEMENT               10000
#define SPORK_2_MAX_VALUE                                     10002
#define SPORK_3_REPLAY_BLOCKS                                 10003
#define SPORK_4_NOTUSED                                       10004


#define SPORK_1_MASTERNODE_PAYMENTS_ENFORCEMENT_DEFAULT       2428537599  //!2046-4-8 1:46:39 GMT
#define SPORK_2_MAX_VALUE_DEFAULT                             112        //!500 Metrix
#define SPORK_3_REPLAY_BLOCKS_DEFAULT                         0
#define SPORK_4_RECONVERGE_DEFAULT                            1420070400  //!2047-1-1

class CSporkMessage;
class CSporkManager;

extern std::map<uint256, CSporkMessage> mapSporks;
extern std::map<int, CSporkMessage> mapSporksActive;
extern CSporkManager sporkManager;

void ProcessSpork(CNode* pfrom, std::string& strCommand, CDataStream& vRecv);
int GetSporkValue(int nSporkID);
bool IsSporkActive(int nSporkID);
void ExecuteSpork(int nSporkID, int nValue);

/**
 * Spork Class
 * Keeps track of all of the network spork settings
 */

class CSporkMessage
{
public:
    std::vector<unsigned char> vchSig;
    int nSporkID;
    int64_t nValue;
    int64_t nTimeSigned;

    uint256 GetHash()
    {
        uint256 n = Hash(BEGIN(nSporkID), END(nTimeSigned));
        return n;
    }

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion)
    {
        READWRITE(nSporkID);
        READWRITE(nValue);
        READWRITE(nTimeSigned);
        READWRITE(vchSig);
    }
};


class CSporkManager
{
private:
    std::vector<unsigned char> vchSig;

    std::string strMasterPrivKey;
    std::string strTestPubKey;
    std::string strMainPubKey;

public:
    CSporkManager()
    {
        strMainPubKey = "0469d959402805bde2f4be0b26db7920d92bddfaa3025e4d1167a3916e6c466f1be4d92d9ea04f1c81ed939a79be9617cde2b51f917d195680c6855c58eb3a5519";
        strTestPubKey = "0469d959402805bde2f4be0b26db7920d92bddfaa3025e4d1167a3916e6c466f1be4d92d9ea04f1c81ed939a79be9617cde2b51f917d195680c6855c58eb3a5519";
    }

    std::string GetSporkNameByID(int id);
    int GetSporkIDByName(std::string strName);
    bool UpdateSpork(int nSporkID, int64_t nValue);
    bool SetPrivKey(std::string strPrivKey);
    bool CheckSignature(CSporkMessage& spork);
    bool Sign(CSporkMessage& spork);
    void Relay(CSporkMessage& msg);
};

#endif // BITCOIN_SPORK_H