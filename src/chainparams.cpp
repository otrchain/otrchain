// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The PIVX developers
// Copyright (c) 2018 The OTRchain developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "libzerocoin/Params.h"
#include "chainparams.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"
#include "net.h"
#include "base58.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
    (0, uint256("00000061dda4910a06c660af9b83eb9c8777a8a0e82ccd3e1c3d5549ee35080b"));
static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1538940515, // * UNIX timestamp of last checkpoint block
    0,    // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    0        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1538940516,
    0,
    0};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1538940517,
    0,
    0};

libzerocoin::ZerocoinParams* CChainParams::Zerocoin_Params() const
{
    assert(this);
    static CBigNum bnTrustedModulus(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParams = libzerocoin::ZerocoinParams(bnTrustedModulus);

    return &ZCParams;
}

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        vTreasuryRewardAddress="oPs2SiaVk6xiXQCrX97xB2ELnnJrbPhpMx";
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x5a;
        pchMessageStart[1] = 0x4c;
        pchMessageStart[2] = 0x3a;
        pchMessageStart[3] = 0x6f;
        vAlertPubKey = ParseHex("04daa244483ebff147f0336235306ab253a30f6af9fdb76e5792347343b0fb3bc5c43d6ea4de8c5983adb40918d903ec6345f3c5cbe6715b57aed8d135f28d923d");
        nDefaultPort = 35566;
        bnProofOfWorkLimit = ~uint256(0) >> 20; // OTRchain starting difficulty is 1 / 2^12
        nSubsidyHalvingInterval = 210000;
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 90; // OTRchain: 1 block
        nTargetSpacing = 1 * 90; //OTRchain: 90 s
        nMaturity = 15;
        nMasternodeCountDrift = 20;
        nMasternodeCollateralLimit = 3000; //MN collateral
        nMaxMoneyOut = 40000000 * COIN; //40 mill
        nMinStakeReserve = 5;
        /** Height or Time Based Activations **/
        nLastPOWBlock = 500; //8.75 Hours POW
        nModifierUpdateBlock = INT_MAX;
        nZerocoinStartHeight = 20000000;
        nAccumulatorStartHeight = 1;
        nZerocoinStartTime = 20000000; // Friday, June 1, 2018 12:00:00 AM - GMT
        nBlockEnforceSerialRange = 1; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = ~1; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = ~1; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = ~1; //Last valid accumulator checkpoint

        /**
         * Build the genesis block. Note that the output of the genesis coinbase cannot
         * be spent as it did not originally exist in the database.
         *
         * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
         *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
         *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
         *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
         *   vMerkleTree: e0028e
         */
    const char* pszTimestamp = "OTRchain genesis hash";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 50 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04daa244483ebff147f0336235306ab253a30f6af9fdb76e5792347343b0fb3bc5c43d6ea4de8c5983adb40918d903ec6345f3c5cbe6715b57aed8d135f28d923d") << OP_CHECKSIG;

        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1538940515;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 927622;
        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x00000061dda4910a06c660af9b83eb9c8777a8a0e82ccd3e1c3d5549ee35080b"));
        assert(genesis.hashMerkleRoot == uint256("0xd58d40f9c61cda9b7698f59db02c4ddc4ab1a2693d0e7d24867ede588869353f"));


        // hashGenesisBlock = genesis.GetHash();
        // assert(hashGenesisBlock == uint256("0000cc75a7c6fa2ce8186e24f872e43acf88b21a1cc02aa11a4ceaee2a562d4c"));
        // assert(genesis.hashMerkleRoot == uint256("3bf54807365f102ff9cdb07cf5f4af411503d5b544835dc96a5beaee140ad419"));

        vSeeds.push_back(CDNSSeedData("seed1", "seed1.otrchain.com"));
        vSeeds.push_back(CDNSSeedData("seed2", "seed2.otrchain.com"));
        vSeeds.push_back(CDNSSeedData("seed3", "seed3.otrchain.com"));
        vSeeds.push_back(CDNSSeedData("seed4", "seed4.otrchain.com"));
        vSeeds.push_back(CDNSSeedData("seed5", "seed5.otrchain.com"));
        vSeeds.push_back(CDNSSeedData("seed6", "seed6.otrchain.com"));
        vSeeds.push_back(CDNSSeedData("seed7", "seed7.otrchain.com"));
        vSeeds.push_back(CDNSSeedData("seed8", "seed8.otrchain.com"));
        vSeeds.push_back(CDNSSeedData("seed9", "seed9.otrchain.com"));
        vSeeds.push_back(CDNSSeedData("seed10", "seed10.otrchain.com"));
        vSeeds.push_back(CDNSSeedData("explorer", "explorer.otrchain.com"));


		base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 115);
		base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);
		base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 122);
		base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x02)(0x2D)(0x25)(0x33).convert_to_container<std::vector<unsigned char> >();
		base58Prefixes[EXT_SECRET_KEY] = list_of(0x02)(0x21)(0x31)(0x2B).convert_to_container<std::vector<unsigned char> >();
        // 	BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0xbc).convert_to_container<std::vector<unsigned char> >();


        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
        strSporkKey = "0236e7b6522c0dffe1530a35ff6439f17ef578289f41e031fb484d8bc327e18f70";
        strObfuscationPoolDummyAddress = "oXXiLcjDKjUtvP37ZWJEuPmGEvbAuUTsQ6";
        nStartMasternodePayments = 1538940515; //Sun, 7 Oct 2018 21:28:35 GMT

        /** Zerocoin */
        zerocoinModulus = "c95577b6dce0049b0a20c779af38079355abadde1a1d80c353f6cb697a7ae5a087bad39caa5798478551d0f9d91e6267716506f32412de1d19d17588765eb9502b85c6a18abdb05791cfd8b734e960281193705eeece210920cc922b3af3ceb178bf12c22eb565d5767fbf19545639be8953c2c38ffad41f3371e4aac750ac2d7bd614b3faabb453081d5d88fdbb803657a980bc93707e4b14233a2358c97763bf28f7c933206071477e8b371f229bc9ce7d6ef0ed7163aa5dfe13bc15f7816348b328fa2c1e69d5c88f7b94cee7829d56d1842d77d7bb8692e9fc7b7db059836500de8d57eb43c345feb58671503b932829112941367996b03871300f25efb5";
        nMaxZerocoinSpendsPerTransaction = 7; // Assume about 20kb each
        nMinZerocoinMintFee = 1 * ZCENT; //high fee required for zerocoin mints
        nMintRequiredConfirmations = 20; //the maximum amount of confirmations until accumulated in 19
        nRequiredAccumulation = 1;
        nDefaultSecurityLevel = 100; //full security level for accumulators
        nZerocoinHeaderVersion = 4; //Block headers must be this version once zerocoin is active
        nBudget_Fee_Confirmations = 6; // Number of confirmations for the finalization fee

    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }


};

std::string CChainParams::GetTreasuryRewardAddressAtHeight(int nHeight) const {
    return vTreasuryRewardAddress;
}

CScript CChainParams::GetTreasuryRewardScriptAtHeight(int nHeight) const {
    CBitcoinAddress address(GetTreasuryRewardAddressAtHeight(nHeight));
    assert(address.IsValid());

    CScript script = GetScriptForDestination(address.Get());
    return script;
}

static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0x8c;
        pchMessageStart[1] = 0x6d;
        pchMessageStart[2] = 0x6a;
        pchMessageStart[3] = 0x7b;
        vAlertPubKey = ParseHex("04daa244483ebff147f0336235306ab253a30f6af9fdb76e5792347343b0fb3bc5c43d6ea4de8c5983adb40918d903ec6345f3c5cbe6715b57aed8d135f28d923d");
        nDefaultPort = 35567;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 90; // OTRchain: 1 block
        nTargetSpacing = 1 * 90;  // OTRchain: 90 s
        nLastPOWBlock = 500;
        nMaturity = 15;
        nMasternodeCountDrift = 4;
        nMasternodeCollateralLimit = 3000; //MN collateral

        nModifierUpdateBlock = 51197; //approx Mon, 17 Apr 2017 04:00:00 GMT
        nMaxMoneyOut = 40000000 * COIN;
        nZerocoinStartHeight = 20000000;
        nZerocoinStartTime = 20000000;
        nBlockEnforceSerialRange = 1; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = 9908000; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = 9891737; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = 9891730; //Last valid accumulator checkpoint

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1538940516;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 666593;

	    hashGenesisBlock = genesis.GetHash();
        //assert(hashGenesisBlock == uint256("0x00000429f38c0dcfca47b861ae17c652293c7db8cabbe98451c3abb6858c3123"));

        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("testnet", "testnet.otrchain.com"));         // Single node address
        vSeeds.push_back(CDNSSeedData("testnet1", "testnet1.otrchain.com"));       // Single node address
        vSeeds.push_back(CDNSSeedData("testnet2", "testnet2.otrchain.com"));       // Single node address


		base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 115);
		base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);
		base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 122);
		base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x02)(0x2D)(0x25)(0x33).convert_to_container<std::vector<unsigned char> >();
		base58Prefixes[EXT_SECRET_KEY] = list_of(0x02)(0x21)(0x31)(0x2B).convert_to_container<std::vector<unsigned char> >();
        // 	BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0xbc).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkKey = "0236e7b6522c0dffe1530a35ff6439f17ef578289f41e031fb484d8bc327e18f70";
        strObfuscationPoolDummyAddress = "oXXiLcjDKjUtvP37ZWJEuPmGEvbAuUTsQ6";
        nStartMasternodePayments = 1538940515; //Sun, 7 Oct 2018 21:28:35 GMT
        nBudget_Fee_Confirmations = 3; // Number of confirmations for the finalization fee. We have to make this very short
                                       // here because we only have a 8 block finalization window on testnet
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        strNetworkID = "regtest";
        pchMessageStart[0] = 0x6a;
        pchMessageStart[1] = 0xcb;
        pchMessageStart[2] = 0x7c;
        pchMessageStart[3] = 0xa3;
        nSubsidyHalvingInterval = 150;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 1 * 90; // OTRchain: 1 block
        nTargetSpacing = 1 * 90;        // OTRchain: 90 s
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1538940517;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 5;

        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 35568;
        //assert(hashGenesisBlock == uint256("0x170183a06a2cb2c104b7bf3a1f4c15d79f0291f4b98b42b333cf236ff5153901"));

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 35569;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval) { nSubsidyHalvingInterval = anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};

static CChainParams* pCurrentParams = 0;

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
